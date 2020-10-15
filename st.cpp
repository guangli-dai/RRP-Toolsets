#include "st.h"
#include "ui_st.h"

ST::ST(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ST)
{
    ui->setupUi(this);
    QStringList title_col;
    ui->tableWidget->setColumnCount(7);
    title_col << "Task #" << "WCET(ms)" << "Deadline(ms)" << "Period/Minimum Separation(ms)" << "Jitter(ms)" << "Offset(ms)" << "Periodic/sporadic";
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

}

void ST::on_saveButton_clicked()
{

}

void ST::on_loadButton_clicked()
{

}

void ST::on_getSTButton_clicked()
{
    item_counter = ui->tableWidget->rowCount();
    double af = 0;
    //calculate the smallest availability factor required to make it schedulable
    for(int i = 0; i < item_counter; i++)
    {
        double wcet = ui->tableWidget->item(i, WCET)->text().toDouble();
        double period = ui->tableWidget->item(i, Period)->text().toDouble();
        double deadline = ui->tableWidget -> item(i, Deadline)->text().toDouble();
        double temp = qCeil(wcet) / (double)qFloor(qMin(period, deadline));
        af += temp;
    }
    double inputAf = ui->wcetTextEdit->toPlainText().toDouble()/ui->periodTextEdit->toPlainText().toDouble();
    //set schedulable or not accordingly
    qDebug()<<af<<endl;
    qDebug()<<inputAf<<endl;
    if(af <= inputAf)
    {
        ui->schedulableComboBox->setCurrentIndex(0);
    }
    else
    {
        ui->schedulableComboBox->setCurrentIndex(1);
    }
}
