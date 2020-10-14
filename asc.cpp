#include "asc.h"
#include "ui_asc.h"
#include "partition_dialog.h"

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
    QString name;
    double wcet, period;
    int res;
    Partition_Dialog pd;
    pd.exec();
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
    ui->tableWidget->setItem(temp, WCEP, new QTableWidgetItem(QString::number(wcet)));
    ui->tableWidget->setItem(temp, Period, new QTableWidgetItem(QString::number(period)));
    ui->tableWidget->setItem(temp, Availability_Factor, new QTableWidgetItem(QString::number(wcet/period)));

}

void ASC::on_removePartitionButton_clicked()
{
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}

void ASC::on_generateButton_clicked()
{

}
