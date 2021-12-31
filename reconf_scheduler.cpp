﻿#include "reconf_scheduler.h"
#include "ui_reconf_scheduler.h"
#include "QFile"
#include "QFileDialog"
#include "QJsonObject"
#include "QJsonDocument"
#include "QJsonArray"
#include "QJsonParseError"
#include "QMessageBox"
#include "QDebug"
#include "reconf_partition_dialog.h"

reconf_scheduler::reconf_scheduler(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::reconf_scheduler)
{

    ui->setupUi(this);
    QStringList title_col;
    ui->tableWidget->setColumnCount(5);
    title_col << "Task ID" << "WCET(ms)" << "Period(ms)" << "Scheduling Deadline(ms)" << "Availablity Factor";
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

void reconf_scheduler::on_ScheduleButton_clicked()
{
    QString filename1 = QFileDialog::getOpenFileName(
                this,
                "TextEditor - Open",
                "",
                "Json File (*.json);;All Files (*.*)");
    curOpenFile = filename1;
    if(!filename1.isEmpty())
    {
        //read xml file here
        //loadFile();
    }
    else
    {
        return;
    }
    ui->scheduleNameLabel->setText(curOpenFile);
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
    //getting item size
    if(rootObj.contains("Schedule File"))
    {
        item_name = rootObj.value("Schedule File").toObject()["Schedule name"].toString();
        ui->scheduleNameLabel->setText(item_name);
    }

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
        jsonObject.insert("Deadline",ui->tableWidget->item(i,Deadline)->text());

        mainObj.insert(QString::number(i + 1),jsonObject);
    }

    QJsonObject jsonObject;
    jsonObject.insert("item_counter", item_counter);
    mainObj.insert("number",jsonObject);

    QJsonObject specification;
    specification.insert("Schedule name", ui->scheduleNameLabel->text());
    mainObj.insert("Schedule File", specification);

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
    // TODO:
    //  1. Collect the data inputs
    //  2. Implement the OHR-OPT algorithm
}
