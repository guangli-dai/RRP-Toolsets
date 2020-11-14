#include "asc.h"
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
    //统计行数
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

    for(int i = 0; i < item_counter; i++)
    {
        QJsonObject jsonObject;
        jsonObject.insert("Name",ui->tableWidget->item(i,Name)->text());
        jsonObject.insert("WCET",ui->tableWidget->item(i,WCET)->text());
        jsonObject.insert("Period",ui->tableWidget->item(i,Period)->text());

        mainObj.insert(QString::number(i + 1),jsonObject);
    }

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
    qDebug() << "start loading";
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
        name = rootObj.value(QString::number(i + 1)).toObject()["Name"].toString();
        wcet = rootObj.value(QString::number(i + 1)).toObject()["WCET"].toString();
        period = rootObj.value(QString::number(i + 1)).toObject()["Period"].toString();

        //adding to the tablewidge
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        int temp = ui->tableWidget->rowCount() - 1;

        //逐行输入
        ui->tableWidget->setItem(temp, Name, new QTableWidgetItem(name));
        ui->tableWidget->setItem(temp, WCET, new QTableWidgetItem(wcet));
        ui->tableWidget->setItem(temp, Period, new QTableWidgetItem(period));
        ui->tableWidget->setItem(temp, Availability_Factor, new QTableWidgetItem(QString::number(wcet.toDouble()/period.toDouble())));
    }
}

