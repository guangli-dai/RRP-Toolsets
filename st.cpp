#include "st.h"
#include "ui_st.h"
#include "QFile"
#include "QDebug"
#include "QMessageBox"
#include "QFileDialog"

ST::ST(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ST)
{
    ui->setupUi(this);
    QStringList title_col;
    ui->tableWidget->setColumnCount(7);
    title_col << "Task #" << "WCET(ms)" << "Deadline(ms)" << "Period(ms)" << "Jitter(ms)" << "Offset(ms)" << "Periodic/sporadic";
    ui->tableWidget->setHorizontalHeaderLabels(title_col);
}

ST::~ST()
{
    delete ui;
}

void ST::on_taskEnterButton_clicked()
{
    int res;
    St_Dialog td;
    res = td.exec();
    if(res == QDialog::Rejected)
    {
        return;
    }

    name = td.Name();
    wcet = td.WCET();
    deadline = td.Deadline();
    period = td.Period();
    jitter = td.Jitter();
    offset = td.Offset();
    isperiodic = td.IsPeriodic();

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    int temp = ui->tableWidget->rowCount() - 1;

    QComboBox *dropdown = new QComboBox();
    dropdown->addItem("Periodic");
    dropdown->addItem("Sporadic");

    //逐行输入
    ui->tableWidget->setItem(temp, Name, new QTableWidgetItem(name));
    ui->tableWidget->setItem(temp, WCET, new QTableWidgetItem(wcet));
    ui->tableWidget->setItem(temp, Deadline, new QTableWidgetItem(deadline));
    ui->tableWidget->setItem(temp, Period, new QTableWidgetItem(period));
    ui->tableWidget->setItem(temp, Jitter, new QTableWidgetItem(jitter));
    ui->tableWidget->setItem(temp, Offset, new QTableWidgetItem(offset));

    //下拉选单
    ui->tableWidget->setCellWidget(temp, Periodic_or_sporadic, dropdown);
    if(isperiodic == "true")
    {
        dropdown->setCurrentIndex(0);
    }
    else
    {
        dropdown->setCurrentIndex(1);
    }

}

void ST::on_taskRemoveButton_clicked()
{
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}

void ST::on_saveButton_clicked()
{
    QString filename1 = QFileDialog::getSaveFileName(
                this,
                "TextEditor - save as",
                "",
                "Text File (*.txt);;All Files (*.*)");
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

void ST::saveFile()
{
    //统计行数
    item_counter = ui->tableWidget->rowCount();
    qDebug() << item_counter;

    QFile file("data2.txt");
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Can not open file";
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_7);

    for(int i = 0; i < item_counter; i++)
    {
        out << ui->tableWidget->item(i,Name)->text()
            << ui->tableWidget->item(i,WCET)->text()
            << ui->tableWidget->item(i,Deadline)->text()
            << ui->tableWidget->item(i,Period)->text()
            << ui->tableWidget->item(i,Jitter)->text()
            << ui->tableWidget->item(i,Offset)->text();
        //下拉选单
    }


    file.flush();
    file.close();
}

void ST::on_loadButton_clicked()
{
    QString filename2 = QFileDialog::getOpenFileName(
                this,
                tr("Open Address Book"),
                "",
                tr("Address Book (*.txt,,All Files (*))"));
    curOpenFile = filename2;
    if(!filename2.isEmpty())
    {
        loadFile();
    }
    else
    {
        return;
    }
}

void ST::loadFile()
{
    QFile file(curOpenFile);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(
                    this,
                    "TextEditor",
                    tr("Cannot open file %1./nError: %2")
                    .arg(curOpenFile)
                    .arg(file.errorString()));
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_7);

    qDebug() << file;
    QString temp_counter;
    in >> temp_counter;
    item_counter = temp_counter.toInt();

    ui->tableWidget->setRowCount(0);
    for(int i = 0; i < item_counter; i++)
    {
        in >> name
           >> wcet
           >> deadline
           >> period
           >> jitter
           >> offset;
           //下拉选单
           //>> isperiodic;

        qDebug() << name;

        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        int temp = ui->tableWidget->rowCount() - 1;

        QComboBox *dropdown = new QComboBox();
        dropdown->addItem("Periodic");
        dropdown->addItem("Sporadic");

        //逐行输入
        ui->tableWidget->setItem(temp, Name, new QTableWidgetItem(name));
        ui->tableWidget->setItem(temp, WCET, new QTableWidgetItem(wcet));
        ui->tableWidget->setItem(temp, Deadline, new QTableWidgetItem(deadline));
        ui->tableWidget->setItem(temp, Period, new QTableWidgetItem(period));
        ui->tableWidget->setItem(temp, Jitter, new QTableWidgetItem(jitter));
        ui->tableWidget->setItem(temp, Offset, new QTableWidgetItem(offset));
        //下拉选单
        ui->tableWidget->setCellWidget(temp, Periodic_or_sporadic, dropdown);
        if(isperiodic == "true")
        {
            dropdown->setCurrentIndex(0);
        }
        else
        {
            dropdown->setCurrentIndex(1);
        }
    }

    file.close();
}
