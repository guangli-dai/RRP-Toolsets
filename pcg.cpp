#include "pcg.h"
#include "ui_pcg.h"
#include "QComboBox"
#include "QFile"
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
    //统计行数 创建数组
    item_counter = ui->tableWidget->rowCount();
    qDebug() << item_counter;
    QVector<QVector<QString>> vec;
    //存入vector
    for(int i = 0; i < item_counter; i++)
    {
        QVector<QString> temp;
        temp.push_back(ui->tableWidget->item(i,Name)->text());
        temp.push_back(ui->tableWidget->item(i,WCET)->text());
        temp.push_back(ui->tableWidget->item(i,Deadline)->text());
        temp.push_back(ui->tableWidget->item(i,Period)->text());
        temp.push_back(ui->tableWidget->item(i,Jitter)->text());
        temp.push_back(ui->tableWidget->item(i,Offset)->text());
        //下拉菜单
        //temp.push_back(ui->tableWidget->cellWidget(i, Periodic_or_sporadic));

        vec.push_back(temp);
    }

    QFile file("save.txt");
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug()<<"can not open to write";
        return;
    }

}

void PCG::on_loadButton_clicked()
{

}
