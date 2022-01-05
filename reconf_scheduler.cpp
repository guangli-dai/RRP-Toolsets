#include "reconf_scheduler.h"
#include "ui_reconf_scheduler.h"
#include "QFile"
#include "QFileDialog"
#include "QJsonObject"
#include "QJsonDocument"
#include "QJsonArray"
#include "QJsonParseError"
#include "QMessageBox"
#include "QDebug"
#include "partition.h"
#include "reconf_partition_dialog.h"
#include "scheduleoutput_dialog.h"
#include <QtXml>

reconf_scheduler::reconf_scheduler(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::reconf_scheduler)
{

    ui->setupUi(this);
    QStringList title_col;
    ui->tableWidget->setColumnCount(5);
    title_col << "Partition ID" << "WCET(ms)" << "Period(ms)" << "Scheduling Deadline(ms)" << "Availablity Factor";
    //ui->tableWidget->setColumnCount(4);
    //title_col << "Task ID" << "WCET(ms)" << "Period(ms)" << "Availablity Factor";
    ui->tableWidget->setHorizontalHeaderLabels(title_col);
    ui->tableWidget->setColumnWidth(3, 200);
}

reconf_scheduler::~reconf_scheduler()
{
    delete ui;
}

void reconf_scheduler::on_reconf_schedulerButton_clicked()
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    int temp = ui->tableWidget->rowCount() - 1;
    ui->tableWidget->setItem(temp, Name, new QTableWidgetItem());
    ui->tableWidget->setItem(temp, WCET, new QTableWidgetItem());
    ui->tableWidget->setItem(temp, Period, new QTableWidgetItem());
    ui->tableWidget->setItem(temp, Deadline, new QTableWidgetItem());
    ui->tableWidget->setItem(temp, Availability_Factor, new QTableWidgetItem());
}


void reconf_scheduler::on_loadButton_clicked()
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

void reconf_scheduler::loadFile()
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
    QString item_name;

    QJsonObject rootObj = jsonDoc.object();

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
        deadline = rootObj.value(QString::number(i + 1)).toObject()["Deadline"].toString();

        //adding to the tablewidge
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        int temp = ui->tableWidget->rowCount() - 1;

        //set the table column by column
        ui->tableWidget->setItem(temp, Name, new QTableWidgetItem(name));
        ui->tableWidget->setItem(temp, WCET, new QTableWidgetItem(wcet));
        ui->tableWidget->setItem(temp, Period, new QTableWidgetItem(period));
        ui->tableWidget->setItem(temp, Deadline, new QTableWidgetItem(deadline));
        ui->tableWidget->setItem(temp, Availability_Factor, new QTableWidgetItem(QString::number(wcet.toDouble()/period.toDouble())));
    }
    ui->timeSliceLengthEdit->setPlainText(rootObj.value("timeSliceLength").toString());
}

void reconf_scheduler::on_saveButton_clicked()
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

void reconf_scheduler::saveFile()
{
    //get number of rows in the table
    item_counter = ui->tableWidget->rowCount();

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
        jsonObject.insert("Deadline",ui->tableWidget->item(i,Deadline)->text());

        mainObj.insert(QString::number(i + 1),jsonObject);
    }

    mainObj.insert("timeSliceLength", ui->timeSliceLengthEdit->toPlainText());

    QJsonObject jsonObject;
    jsonObject.insert("item_counter", item_counter);
    mainObj.insert("number",jsonObject);


    QJsonDocument jsonDoc;
    jsonDoc.setObject(mainObj);

    file.write(jsonDoc.toJson());
    file.close();
}

void reconf_scheduler::on_AddedButton_clicked()
{
    int res;
    Reconf_Partition_Dialog pd;
    res = pd.exec();
    if(res == QDialog::Rejected)
    {
        return;
    }

    name = pd.Name();
    wcet = pd.WCET();
    period = pd.Period();
    deadline = pd.Deadline();

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    int temp = ui->tableWidget->rowCount() - 1;
    ui->tableWidget->setItem(temp, Name, new QTableWidgetItem(name));
    ui->tableWidget->setItem(temp, WCET, new QTableWidgetItem(wcet));
    ui->tableWidget->setItem(temp, Period, new QTableWidgetItem(period));
    ui->tableWidget->setItem(temp, Availability_Factor, new QTableWidgetItem(QString::number(wcet.toDouble()/period.toDouble())));
    ui->tableWidget->setItem(temp, Deadline, new QTableWidgetItem(deadline));
}

void reconf_scheduler::on_removeButton_clicked()
{
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}

void reconf_scheduler::on_generateButton_clicked()
{
    //  1. Collect the data inputs
    //  2. Collect the old schedule (convert from XML back to QVector<QString> schedules)
    //  3. Implement the OHR-OPT algorithm

    // Collect reconf_partitions;
    QVector<Reconf_Partition> partitions;
    for(int i=0; i<ui->tableWidget->rowCount(); i++)
    {
        QString id = ui->tableWidget->item(i, Name)->text();
        int wcet = ui->tableWidget->item(i, WCET)->text().toInt();
        int period = ui->tableWidget->item(i, Period)->text().toInt();
        double deadline = ui->tableWidget->item(i, Deadline)->text().toDouble();
        Reconf_Partition temp_par(id, wcet, period, deadline);
        partitions.push_back(temp_par);
    }
    int time_slice_size = ui->timeSliceLengthEdit->toPlainText().toInt();
    QVector<QString> schedule = OHR_OPT(partitions, time_slice_size);
    QVector<QVector<QString>> schedules;
    schedules.push_back(schedule);
    QString xml_outputs = getXMLFormat(schedules, time_slice_size);
    ScheduleOutput_Dialog s;
    s.setXML(xml_outputs);
    s.exec();

}

QVector<QString> reconf_scheduler::OHR_OPT(QVector<Reconf_Partition>& partitions, int time_slice_size)
{
    // approximate each partition using MulZ, update the partitions accordingly
    int factors[4] = {3,4,5,7};
    bool success = false;
    for(int factor:factors)
    {
        if(test_approx(partitions, factor))
        {
            success = true;
            break;
        }
    }
    if(!success)
    {
        QMessageBox::warning(
                    this,
                    "TextEditor",
                    tr("Cannot schedule the partitions on a single core. Please modify the parameters.")
                    );
        return QVector<QString>();
    }

    // separate U and V
    // also retrieve the largest period
    QVector<int> U;
    QSet<int> V;
    int hyper_period = 0;
    for(int i=0; i<partitions.size(); i++)
    {
        //qDebug()<<i<<"(" << partitions[i].getId() << ")" <<":" << partitions[i].getWCET() << "/" << partitions[i].getPeriod()<<endl;
        if(partitions[i].getWCET()==1)
        {
            V.insert(i);
        }
        else
        {
            U.push_back(i);
        }

        if(hyper_period == 0)
        {
            hyper_period = partitions[i].getPeriod();
        }
        else
        {
            hyper_period = lcm(hyper_period, partitions[i].getPeriod());
        }

    }

    QVector<QString> schedule(hyper_period, "-1");
    // schedule U, needs to enumerate all partitions in
    // pregenerate all schedules for different delta for all partitions, then use dfs to do the search
    QVector<QVector<QSet<int>>> taus; // tau[i][j] represents the set of time slices beloning to partitions[i] with delta = j
    for(int i=0; i<U.size(); i++)
    {
        int index = U[i];
        QVector<QSet<int>> taus_now;
        int p = partitions[index].getPeriod();
        for(int j=0; j<p; j++)
        {
            auto t = tau(p, partitions[index].getWCET(), j);
            // check whether the earliest time slice in t is no larger than the deadline
            bool fits = false;
            for(int s:t)
            {
                if(s * time_slice_size <= partitions[index].getDeadline())
                {
                    //qDebug() << partitions[index].getId() << ": " << s*time_slice_size << "," << partitions[index].getDeadline();
                    fits = true;
                    break;
                }
            }
            if(fits)
            {
                taus_now.push_back(t);
            }
        }
        taus.push_back(taus_now);
    }
    QVector<int> result;
    QSet<int> now;
    bool schedU = search(taus, result, now);
    if(!schedU)
    {
        QMessageBox::warning(
                    this,
                    "TextEditor",
                    tr("Cannot schedule the partitions with q larger than 1. Please modify the parameters.")
                    );
        return schedule;
    }

    // collect the result by updating schedule
    for(int i=0; i<result.size(); i++)
    {
        int j = result[i];
        int index = U[i];
        //qDebug() << i << " --> " << U[i];
        for(int k=0; k<hyper_period / partitions[index].getPeriod(); k++)
        {

            for(auto t:taus[i][j])
            {
                schedule[t + k * partitions[index].getPeriod()] = partitions[index].getId();
            }
        }
    }

    /*
    // for debugging purposes
    for(int i=0; i<schedule.size(); i++)
    {
        qDebug() << i << ":" << schedule[i];
    }
    */

    QMap<int, int> avails;
    for(int i=0; i<schedule.size(); i++)
    {
        if(schedule[i]=="-1")
        {
            avails.insert(i, 0); // the value 0 does not matter at all since we are only using the key
        }
    }

    // schedule V using EDF
    while(!V.empty())
    {
        // enumerate to find the smallest deadline in V
        double smallestDDL = 1000000, next = -1;
        for(auto index:V)
        {
            if(partitions[index].getDeadline() < smallestDDL)
            {
                smallestDDL = partitions[index].getDeadline();
                next = index;
            }
        }
        // schedule next partition
        int timeNow = avails.lowerBound(0).key(); // timeNow in the unit of time slice
        if(timeNow * time_slice_size > smallestDDL)
        {
            QMessageBox::warning(
                        this,
                        "TextEditor",
                        tr("Cannot schedule the partitions with q equal to 1. Please modify the parameters.")
                        );
            return schedule;
        }
        for(int k=0; k<hyper_period/partitions[next].getPeriod(); k++)
        {
            int temp = timeNow + k*partitions[next].getPeriod();
            schedule[temp] = partitions[next].getId();
            avails.remove(temp);
        }

        V.remove(next);
    }


    return schedule;

}

/*
 * Print the elements in a QSet
 * For debugging usage only
*/
/*
void printSet(QSet<int> s)
{
    for(auto n:s)
        qDebug() << n << ",";
    qDebug() <<endl;
}
*/

bool reconf_scheduler::search(QVector<QVector<QSet<int>>> &taus, QVector<int> &result, QSet<int> &now)
{
    int iNow = result.size();
    if(iNow == taus.size())
    {
        return true;
    }
    for(int j=0; j<taus[iNow].size(); j++)
    {
        if(!taus[iNow][j].intersects(now))
        {
            // can add this one in
            now += taus[iNow][j];
            result.push_back(j);
            if(search(taus, result, now))
                return true;
            result.pop_back();
            now -= taus[iNow][j];
        }
    }
    return false;
}

QSet<int> reconf_scheduler::tau(int p, int q, int delta)
{
    QSet<int> time_slices;
    for(int k=0; k<q; k++)
    {
        int t_now = (qFloor(k*p/q) + delta) % p;
        time_slices.insert(t_now);
    }
    return time_slices;
}

int reconf_scheduler::lcm(int a, int b)
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

bool reconf_scheduler::test_approx(QVector<Reconf_Partition> &partitions, int factor)
{
    QVector<approximates> temp;
    double sum = 0;
    for(int i=0; i<partitions.size(); i++)
    {
        temp.push_back(z_approx(partitions[i].getAF(), factor));
        sum += temp[i].aaf;
    }
    if(sum <= 1)
    {
        for(int i=0; i<partitions.size(); i++)
        {
            partitions[i].reset(temp[i].WCET, temp[i].period);
        }
        return true;
    }
    return false;
}

approximates reconf_scheduler::z_approx(double af, int factor)
{
    double factor_double = static_cast<double>(factor);
    approximates result;
    result.aaf = 1;
    result.WCET = 1;
    result.period = 1;

    if(af<=0)
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

double reconf_scheduler::approximate_value(double value)
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

QString reconf_scheduler::getXMLFormat(QVector<QVector<QString> > schedules, int time_slice_size)
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
            time_now += duration;
            counter ++;
            plan.appendChild(slot);
        }
        plan.setAttribute("majorFrame", QString::number(schedules[i].size() * time_slice_size)+"ms");
        root.appendChild(cpu_now);
    }
    return doc.toString();
}
