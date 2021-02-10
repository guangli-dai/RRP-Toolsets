#include "csg.h"
#include "ui_asc.h"
#include "partition_dialog.h"
#include "QFile"
#include "QDebug"
#include "QMessageBox"
#include "QFileDialog"
#include "QJsonObject"
#include "QJsonDocument"
#include "QJsonArray"
#include "QJsonParseError"
#include "scheduleoutput_dialog.h"
#include <QtMath>
#include <QtXml>

ASC::ASC(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ASC)
{
    ui->setupUi(this);
    QStringList title_col;
    ui->tableWidget->setColumnCount(4);
    title_col << "Task ID" << "WCET(ms)" << "Period(ms)" << "Availablity Factor";
    ui->tableWidget->setHorizontalHeaderLabels(title_col);
}

ASC::~ASC()
{
    delete ui;
}

void ASC::on_addPartitionButton_clicked()
{
    int res;
    Partition_Dialog pd;
    res = pd.exec();
    if(res == QDialog::Rejected)
    {
        return;
    }

    name = pd.Name();
    wcet = pd.WCET();
    period = pd.Period();

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    int temp = ui->tableWidget->rowCount() - 1;
    ui->tableWidget->setItem(temp, Name, new QTableWidgetItem(name));
    ui->tableWidget->setItem(temp, WCET, new QTableWidgetItem(wcet));
    ui->tableWidget->setItem(temp, Period, new QTableWidgetItem(period));
    ui->tableWidget->setItem(temp, Availability_Factor, new QTableWidgetItem(QString::number(wcet.toDouble()/period.toDouble())));
}

void ASC::on_removePartitionButton_clicked()
{
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}

void ASC::on_generateButton_clicked()
{
        QVector<Partition> partitions;
        for(int i=0; i<ui->tableWidget->rowCount(); i++)
        {
            QString id = ui->tableWidget->item(i, Name)->text();
            long long wcet = ui->tableWidget->item(i, WCET)->text().toLongLong();
            long long period = ui->tableWidget->item(i, Period)->text().toLongLong();
            Partition temp_par(id, wcet, period);
            partitions.push_back(temp_par);
        }
        //print to check
        //for(int i=0; i<partitions.size(); i++)
        //{
        //    qDebug()<<partitions[i].getId()<<": "<<partitions[i].getWCET()<<" / "<<partitions[i].getPeriod()<<" = "<<partitions[i].getAF()<<endl;
        //}
        //implement the algorithm here
        int time_slice_length = ui->timeSliceLengthEdit->toPlainText().toInt(); //in milliseconds, need to be input by the user in the later version
        int numCPU = ui->numCPUTextEdit->toPlainText().toInt();
        auto schedules = MulZ_ILO(partitions, numCPU);
        QString xml_outputs = getXMLFormat(schedules, time_slice_length);
        //qDebug()<<xml_outputs;
        ScheduleOutput_Dialog s;
        s.setXML(xml_outputs);
        s.exec();
}

void ASC::on_saveButton_clicked()
{
    QString filename1 = QFileDialog::getSaveFileName(
                this,
                "TextEditor - save as",
                "",
                "Json File (*.json);;All Files (*.*)");
    curSaveFile = filename1;
    if(!filename1.isEmpty())
    {
        saveFile();
    }
    else
    {
        return;
    }
}

void ASC::saveFile()
{
    //get number of rows in the table
    item_counter = ui->tableWidget->rowCount();
    qDebug() << item_counter;

    QFile file(curSaveFile);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(
                    this,
                    "TextEditor",
                    tr("Cannot write file %1.\nError: %2")
                    .arg(curSaveFile)
                    .arg(file.errorString()));

    }

    QJsonObject mainObj;
    //save the table
    for(int i = 0; i < item_counter; i++)
    {
        QJsonObject jsonObject;
        jsonObject.insert("RSID",ui->tableWidget->item(i,Name)->text());
        jsonObject.insert("WCET",ui->tableWidget->item(i,WCET)->text());
        jsonObject.insert("Period",ui->tableWidget->item(i,Period)->text());

        mainObj.insert(QString::number(i + 1),jsonObject);
    }

    //save CPU #
    mainObj.insert("CPU#", ui->numCPUTextEdit->toPlainText());
    mainObj.insert("timeSliceLength", ui->timeSliceLengthEdit->toPlainText());

    QJsonObject jsonObject;
    jsonObject.insert("item_counter", item_counter);
    mainObj.insert("number",jsonObject);

    QJsonDocument jsonDoc;
    jsonDoc.setObject(mainObj);

    file.write(jsonDoc.toJson());
    file.close();
}

void ASC::on_loadButton_clicked()
{
    QString filename1 = QFileDialog::getOpenFileName(
                this,
                "TextEditor - Open",
                "",
                "Json File (*.json);;All Files (*.*)");
    curOpenFile = filename1;
    if(!filename1.isEmpty())
    {
        loadFile();
    }
    else
    {
        return;
    }
}

void ASC::loadFile()
{
    QFile file(curOpenFile);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Can not open file";
        return;
    }

    QByteArray allData = file.readAll();
    file.close();

    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    if(json_error.error != QJsonParseError::NoError)
    {
        qDebug() << "error";
        return;
    }

    int item_size = 0;

    QJsonObject rootObj = jsonDoc.object();
    //getting item size
    if(rootObj.contains("number"))
    {
        item_size = rootObj.value("number").toObject()["item_counter"].toInt();
    }

    ui->tableWidget->setRowCount(0);
    //reading items
    for(int i = 0; i <item_size; i++)
    {
        name = rootObj.value(QString::number(i + 1)).toObject()["RSID"].toString();
        wcet = rootObj.value(QString::number(i + 1)).toObject()["WCET"].toString();
        period = rootObj.value(QString::number(i + 1)).toObject()["Period"].toString();

        //adding to the tablewidge
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        int temp = ui->tableWidget->rowCount() - 1;

        //set the table column by column
        ui->tableWidget->setItem(temp, Name, new QTableWidgetItem(name));
        ui->tableWidget->setItem(temp, WCET, new QTableWidgetItem(wcet));
        ui->tableWidget->setItem(temp, Period, new QTableWidgetItem(period));
        ui->tableWidget->setItem(temp, Availability_Factor, new QTableWidgetItem(QString::number(wcet.toDouble()/period.toDouble())));
    }
    //read CPU#
    ui->numCPUTextEdit->setPlainText(rootObj.value("CPU#").toString());
    ui->timeSliceLengthEdit->setPlainText(rootObj.value("timeSliceLength").toString());
}

double ASC::approximate_value(double value)
{
    double result = qFloor(value);
    if(value - result > 0.9999)
        return result + 1;
    else if(value - result > 0.49999 && value - result < 0.5)
        return result + 0.5;
    else if (value - result > 0 && value - result < 0.00001)
        return result;
    return value;

}

approximates ASC::z_approx(double af, int factor)
{
    double factor_double = static_cast<double>(factor);
    approximates result;
    result.aaf = 1;
    result.WCET = 1;
    result.period = 1;

    if(af==0)
    {
        return result;
    }
    else if (af > 0 && af < 1.0 / factor_double)
    {
        int n = qFloor(approximate_value(qLn(factor_double*af)/qLn(0.5)));
        result.WCET = 1;
        result.period = factor * qPow(2,n);
        result.aaf = static_cast<double>(result.WCET)/ static_cast<double>(result.period);
        return result;
    }
    else if (af >= 1.0 / factor_double && af <= (factor_double - 1) / factor_double)
    {
        result.period = factor;
        result.WCET = qCeil(approximate_value(factor * af));
        result.aaf = static_cast<double>(result.WCET) / static_cast<double>(result.period);
        return result;
    }
    else if (af > (factor_double - 1)/ factor_double && af < 1)
    {
        int n = qCeil(approximate_value(qLn(factor * (1 - af)) / qLn(0.5)));
        result.WCET = factor * qPow(2, n) - 1;
        result.period = factor * qPow(2, n);
        result.aaf = static_cast<double>(result.WCET) / static_cast<double>(result.period);
        return result;
    }
    else
    {
        result.aaf = 1;
        result.WCET = 1;
        result.period = 1;
        return result;
    }
}


dp_return ASC::dp_single(QVector<Partition> partition_list, int factor)
{
    //approximate all partitions using the factor, note that af stays the same while WCET and period are updated so that WCET/period = aaf
    int largest_period = 1;
    for(int i=0; i<partition_list.size(); i++)
    {
        approximates app_result = z_approx(partition_list[i].getAF(), factor);
        partition_list[i].reset(app_result.WCET, app_result.period);
        if(app_result.period > largest_period)
            largest_period = app_result.period;
    }
    //convert all wcet and period according to the largest_period
    for(int i=0; i<partition_list.size(); i++)
    {
        long long current_period = partition_list[i].getPeriod();
        long long current_WCET = partition_list[i].getWCET();
        partition_list[i].reset(current_WCET * largest_period / current_period, largest_period);
    }

    int n = partition_list.size()+1;
    int m = largest_period + 1;
    //use 2d array dp[i][j] to represent the total availability factor accommodated by partition i-1 with j* 1/largest_period space used
    QVector<QVector<double>> dp;
    //dp_result stores the set of ids of partitions chosen to reach dp[i][j]
    QVector<QVector<QSet<QString>>> dp_result;
    //initialize the two 2D arrays
    for(int i=0; i<n; i++)
    {
        QVector<double> temp_dp;
        QVector<QSet<QString>> temp_result;
        for(int j=0; j<m; j++)
        {
            temp_dp.push_back(0);
            QSet<QString> temp;
            temp_result.push_back(temp);
        }
        dp.push_back(temp_dp);
        dp_result.push_back(temp_result);
    }

    for(int i=0; i<n; i++)
    {
       if(i==0)
            continue;
       //the index of the partition exploring now has an index of i-1
       int WCET_now = partition_list[i-1].getWCET();
       double af_now = partition_list[i-1].getAF();
       QString id_now = partition_list[i-1].getId();
        for(int j=0; j<m; j++)
        {
            if(j==0)
                continue;
            double temp1 = dp[i-1][j];
            double temp2 = 0;
            if(j >= WCET_now)
            {
                temp2 = dp[i-1][j-WCET_now] + af_now;
            }

            if(temp1 >= temp2)
            {
                dp[i][j] = temp1;
                dp_result[i][j] = dp_result[i-1][j];
            }
            else
            {
                dp[i][j] = temp2;
                dp_result[i][j] = dp_result[i-1][j - WCET_now];
                dp_result[i][j].insert(id_now);
            }
        }
    }
    dp_return result;
    result.ids = dp_result[n-1][m-1];
    result.af_sum = dp[n-1][m-1];
    return result;
}

QVector<QVector<QString>> ASC::MulZ_ILO(QVector<Partition> partitions, int numCPU)
{
    int factors[4]= {3,4,5,7};
    QVector<QVector<QString>> results;
    for(int i=0; i<numCPU; i++)
    {
        //push in empty schedules if no partitions left
        if(partitions.size()==0)
        {
            QVector<QString> result;
            results.push_back(result);
            continue;
        }
        //choose a set of partitions for a certain CPU
        double max_s = 0;
        int factor_used = 0;
        QSet<QString> chosen_ps;
        for(int factor : factors)
        {
            dp_return temp_result = dp_single(partitions, factor);
            if(temp_result.af_sum > max_s)
            {
                max_s = temp_result.af_sum;
                chosen_ps = temp_result.ids;
                factor_used = factor;
            }
        }

        //use partitions with id in chosen_ps to generate a schedule
        //filter used partition out from the partitions
        QVector<Partition> temp_partitions;
        QVector<Partition> partitions_now;
        //qDebug()<<"Chosen partitions for CPU #"<<i<<" with factor "<<factor_used<<endl;
        for(auto p : partitions)
        {
            if(chosen_ps.find(p.getId())==chosen_ps.constEnd())
                temp_partitions.push_back(p);
            else
            {
                //qDebug()<<p.getId()<<", ";
                partitions_now.push_back(p);
            }
        }
        partitions = temp_partitions;
        QVector<QString> result = CSG_AZ(partitions_now, factor_used);
        results.push_back(result);
    }
    return results;
}

bool ASC::check_delta(QSet<int> avail_set, QVector<int> standard_p, int delta, int p)
{
    for(int time : standard_p)
    {
        int time_now = (time + delta)%p;
        if(!avail_set.contains(time_now))
            return false;
    }
    return true;
}


int ASC::find_delta(QSet<int> avail_timeslices, int p, int q, int q_left)
{
    QVector<int> standard_p1, standard_p2;
    for(int k=0; k<q; k++)
    {
        int t_now = qFloor(k*p/q)%p;
        standard_p1.push_back(t_now);
    }
    for(int k=0; k<q_left; k++)
    {
        int t_now = qFloor(k*p/q_left)%p;
        standard_p2.push_back(t_now);
    }
    //find potential delta1 (delta is always smaller than p)
    for(int delta1 = 0; delta1<p; delta1++)
    {
        if(check_delta(avail_timeslices, standard_p1, delta1, p))
        {
            if(standard_p2.size()==0)
                return delta1;
            //check delta2, if compatible, return delta1
            QSet<int> temp_avail = avail_timeslices;
            QSet<int> partition1_set;
            for(int time : standard_p1)
            {
                partition1_set.insert((time + delta1)%p);
            }
            temp_avail.subtract(partition1_set);
            for(int delta2 = 0; delta2<p; delta2++)
            {
                if(check_delta(temp_avail, standard_p2, delta2, p))
                    return delta1;
            }
        }
    }
    qDebug()<<"Cannot find delta!";
    for(auto time : avail_timeslices)
        qDebug()<<time;
    qDebug()<<p<<","<<q<<","<<q_left<<standard_p2.size();
    return -1;
}

QVector<QString> ASC::CSG_AZ(QVector<Partition> partition_list, int factor)
{
    //to be finished
    QVector<QString> result;
    QVector<QString> schedule;
    QSet<int> avail_timeslices;
    //approximate partitions using the factor, calculate the hyper-period meanwhile
    int hyper_period = 0;
    for(int i=0; i<partition_list.size(); i++)
    {
        approximates app_result = z_approx(partition_list[i].getAF(), factor);
        partition_list[i].reset(app_result.WCET, app_result.period);
        if(hyper_period==0)
            hyper_period = app_result.period;
        else
        {
            hyper_period = lcm(hyper_period, app_result.period);
        }
    }

    //use CSG-AZ algorithm to generate a schedule
    //initialize the schedule with empty time slices
    for(int i=0; i<hyper_period; i++)
    {
        schedule.push_back("-1");
        avail_timeslices.insert(i);
    }
    //sort the partition by their aaf!
    std::sort(partition_list.begin(), partition_list.end(),
              [](Partition &a, Partition &b)
                {
                    double a_aaf = static_cast<double>(a.getWCET())/ static_cast<double>(a.getPeriod());
                    double b_aaf = static_cast<double>(b.getWCET())/ static_cast<double>(b.getPeriod());
                    return a_aaf > b_aaf;

                });

    for(Partition p : partition_list)
    {
        //qDebug()<<p.getId()<<endl;
        QSet<int> occupied_time;
        if(p.getWCET()!=1)
        {

            int delta1 = find_delta(avail_timeslices, p.getPeriod(), p.getWCET(), avail_timeslices.size()*p.getPeriod()/hyper_period - p.getWCET());
            if(delta1==-1)
            {
                qDebug()<<"Unschedulable Resource Specifications!";
                return result;
            }
            //allocate time slices according to delta1
            for(int l=0; l<hyper_period/p.getPeriod(); l++)
            {
                for(int k=0; k<p.getWCET(); k++)
                {
                    int index_now = qFloor(k*p.getPeriod()/p.getWCET() + delta1) % p.getPeriod() + l * p.getPeriod();
                    if(!avail_timeslices.contains(index_now))
                    {
                        qDebug()<<"Something wrong with time slice "<<index_now;
                        return result;
                    }
                    schedule[index_now] = p.getId();
                    occupied_time.insert(index_now);
                }
            }
        }
        else
        {
            int index = -1;
            for(int time : avail_timeslices)
            {
                if(index==-1 || time < index)
                    index = time;
            }
            if(index >= p.getPeriod())
            {
                qDebug()<<"Unschedulable Resource Specifications!";
                return result;
            }
            for(int l=0; l<hyper_period/p.getPeriod(); l++)
            {
                int index_now = index + l*p.getPeriod();
                schedule[index_now] = p.getId();
                occupied_time.insert(index_now);
            }

        }
        avail_timeslices.subtract(occupied_time);
    }
    return schedule;
    //convert schedule array into a list of sched_entries
    /*int time_now = 0, duration = 0;
    QString last_id = schedule[0];
    for(QString entry : schedule)
    {
        if(entry==last_id)
        {
            duration += time_slice_size;
            continue;
        }
        if(last_id!="-1")
        {
            sched_entry temp;
            temp.id = last_id;
            temp.start = time_now;
            temp.duration  = duration;
            result.push_back(temp);
        }
        time_now += duration;
        duration = time_slice_size;
        last_id = entry;

    }
    if(last_id!="-1")
    {
        sched_entry temp;
        temp.id = last_id;
        temp.start = time_now;
        temp.duration = duration;
    }
    return result;
    */
}

int ASC::lcm(int a, int b)
{
    int mul = a*b;
    int larger = qMax(a, b);
    int smaller = qMin(a, b);
    while(smaller != 0)
    {
        int temp = larger % smaller;
        larger = smaller;
        smaller = temp;
    }
    mul /= larger;
    return mul;
}

QString ASC::getXMLFormat(QVector<QVector<QString> > schedules, int time_slice_size)
{
    QDomDocument doc;
    QDomElement root = doc.createElement("ProcessorTable");
    doc.appendChild(root);
    for(int i=0; i<schedules.size();i++)
    {
        //write in the CPU #i's schedule
        QDomElement cpu_now = doc.createElement("Processor");
        cpu_now.setAttribute("id", QString::number(i));
        if(schedules[i].size()==0)
        {
            root.appendChild(cpu_now);
            continue;
        }
        QDomElement plan = doc.createElement("Plan");
        plan.setAttribute("id", "0");
        plan.setAttribute("majorFrame", QString::number(schedules[i].size()));
        cpu_now.appendChild(plan);
        int time_now = 0, counter = 0, duration = 0;
        QString last_id = schedules[i][0];
        for(QString id_now : schedules[i])
        {
            if(id_now == last_id)
            {
                duration += time_slice_size;
                continue;
            }
            if(last_id!="-1")
            {
                QDomElement slot = doc.createElement("Slot");
                slot.setAttribute("id", QString::number(counter));
                slot.setAttribute("start", QString::number(time_now)+"ms");
                slot.setAttribute("duration", QString::number(duration)+"ms");
                slot.setAttribute("partitionId", last_id);
                counter ++;
                plan.appendChild(slot);
            }
            time_now += duration;
            duration = time_slice_size;
            last_id = id_now;
        }
        if(last_id!="-1")
        {
            QDomElement slot = doc.createElement("Slot");
            slot.setAttribute("id", QString::number(counter));
            slot.setAttribute("start", QString::number(time_now)+"ms");
            slot.setAttribute("duration", QString::number(duration)+"ms");
            slot.setAttribute("partitionId", last_id);
            counter ++;
            plan.appendChild(slot);
        }
        root.appendChild(cpu_now);
    }
    return doc.toString();
}
