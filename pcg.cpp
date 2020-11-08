#include "pcg.h"
#include "ui_pcg.h"
#include "QComboBox"
#include "QFile"
#include "QMessageBox"
#include "QFileDialog"
#include "QDebug"

PCG::PCG(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PCG)
{
    ui->setupUi(this);
    QStringList title_col;
    ui->tableWidget->setColumnCount(7);
    title_col << "Task #" << "WCET(ms)" << "Deadline(ms)" << "Period(ms)" << "Jitter(ms)" << "Offset(ms)" << "Periodic/sporadic";
    ui->tableWidget->setHorizontalHeaderLabels(title_col);
}

PCG::~PCG()
{
    delete ui;
}

void PCG::on_taskEnterButton_clicked()
{
    int res;
    Task_Dialog td;
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

void PCG::on_taskRemoveButton_clicked()
{
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}

void PCG::on_saveButton_clicked()
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

void PCG::saveFile()
{
    //统计行数
    item_counter = ui->tableWidget->rowCount();
    qDebug() << item_counter;

    QFile file1(curSaveFile);
    if(!file1.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(
                    this,
                    "TextEditor",
                    tr("Cannot write file %1./nError: %2")
                    .arg(curSaveFile)
                    .arg(file1.errorString()));

    }

    QDataStream out(&file1);
    out.setVersion(QDataStream::Qt_4_7);

    out << QString::number(item_counter);
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

    file1.flush();
    file1.close();
}

void PCG::on_loadButton_clicked()
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

void PCG::loadFile()
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

void PCG::fraction_add(long& a_deno, long& a_no, long b_deno, long b_no)
{
    long temp_deno = a_deno*b_no + a_no*b_deno;
    long temp_no = a_no * b_no;
    a_deno = temp_deno;
    a_no = temp_no;
    reduce(a_deno, a_no);
}

void PCG::reduce(long& a_deno, long& a_no)
{
    long temp_deno = a_deno;
    long temp_no = a_no;
    //temp_no should always be no smaller than temp_deno
    //get the gcd, store it in temp_no
    while(temp_deno!=0)
    {
        long temp = temp_no%temp_deno;
        temp_no = temp_deno;
        temp_deno = temp;
    }

    a_deno /= temp_no;
    a_no /= temp_no;

}

void PCG::on_getconfigButton_clicked()
{
    //get the number of rows
    item_counter = ui->tableWidget->rowCount();
    long af_upper = 0, af_lower = 1;
    //calculate the sum of ceil(wcet)/floor(min(period, deadline))
    for(int i = 0; i < item_counter; i++)
    {
        double wcet = ui->tableWidget->item(i, WCET)->text().toDouble();
        double period = ui->tableWidget->item(i, Period)->text().toDouble();
        double deadline = ui->tableWidget -> item(i, Deadline)->text().toDouble();
        long temp_upper = qCeil(wcet);
        long temp_lower = qFloor(qMin(period, deadline));
        fraction_add(af_upper, af_lower, temp_upper, temp_lower);
    }
    //set the availability factor in the UI
    //ui->afUpperEdit->
    ui->afUpperEdit->setText(QString::number(af_upper));
    ui->afLowerEdit->setText(QString::number(af_lower));
}
