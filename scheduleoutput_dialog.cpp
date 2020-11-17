#include "scheduleoutput_dialog.h"
#include "ui_scheduleoutput_dialog.h"

ScheduleOutput_Dialog::ScheduleOutput_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScheduleOutput_Dialog)
{
    ui->setupUi(this);
}

ScheduleOutput_Dialog::~ScheduleOutput_Dialog()
{
    delete ui;
}

void ScheduleOutput_Dialog::setXML(QString xmlOutput)
{
    ui->xmlResultTextEdit->setPlainText(xmlOutput);
}
