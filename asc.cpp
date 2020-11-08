#include "asc.h"
#include "ui_asc.h"
#include "partition_dialog.h"
#include "QFile"
#include "QDebug"
#include "QMessageBox"
#include "QFileDialog"

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
    //QString name;
    //double wcet, period;
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

}

void ASC::on_saveButton_clicked()
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

void ASC::saveFile()
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

    for(int i = 0; i < item_counter; i++)
    {
        out << ui->tableWidget->item(i,Name)->text()
            << ui->tableWidget->item(i,WCET)->text()
            << ui->tableWidget->item(i,Period)->text();
    }


    file1.flush();
    file1.close();
}

void ASC::on_loadButton_clicked()
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

void ASC::loadFile()
{
    QFile file("data3.txt");
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Can not open file";
        return;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_7);

    for(int i = 0; i < item_counter; i++)
    {
        in >> name
           >> wcet
           >> period;

        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        int temp = ui->tableWidget->rowCount() - 1;

        //逐行输入
        ui->tableWidget->setItem(temp, Name, new QTableWidgetItem(name));
        ui->tableWidget->setItem(temp, WCET, new QTableWidgetItem(wcet));
        ui->tableWidget->setItem(temp, Period, new QTableWidgetItem(period));
    }
    file.close();
}
