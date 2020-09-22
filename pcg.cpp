#include "pcg.h"
#include "ui_pcg.h"
#include "QComboBox"
#include "QFile"

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
    /*
    QString name;
    double wcet, deadline, period, jitter, offset;
    bool isperiodic;
    */
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

    QComboBox *dropdown = new QComboBox();
    dropdown->addItem("Periodic");
    dropdown->addItem("Sporadic");

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    int temp = ui->tableWidget->rowCount() - 1;
    ui->tableWidget->setItem(temp, Name, new QTableWidgetItem(name));
    ui->tableWidget->setItem(temp, WCEP, new QTableWidgetItem(QString::number(wcet)));
    ui->tableWidget->setItem(temp, Deadline, new QTableWidgetItem(QString::number(deadline)));
    ui->tableWidget->setItem(temp, Period, new QTableWidgetItem(QString::number(period)));
    ui->tableWidget->setItem(temp, Jitter, new QTableWidgetItem(QString::number(jitter)));
    ui->tableWidget->setItem(temp, Offset, new QTableWidgetItem(QString::number(offset)));

    ui->tableWidget->setCellWidget(temp, Periodic_or_sporadic, dropdown);
    if(isperiodic == true)
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

}

void PCG::on_loadButton_clicked()
{

}
