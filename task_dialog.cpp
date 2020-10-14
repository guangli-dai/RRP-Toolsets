#include "task_dialog.h"
#include "ui_task_dialog.h"


Task_Dialog::Task_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Task_Dialog)
{
    ui->setupUi(this);
}

Task_Dialog::~Task_Dialog()
{
    delete ui;
}

void Task_Dialog::addTask()
{
    exec();
}


QString Task_Dialog::execution(double a, double b, double c, double d, double e, bool f)
{
    ui->wcetEdit->setText(QString::number(a));
    ui->deadtimeEdit->setText(QString::number(b));
    ui->periodEdit->setText(QString::number(c));
    ui->jitterEdit->setText(QString::number(d));
    ui->offsetEdit->setText(QString::number(e));
    ui->checkBox->setChecked(f);
    exec();

    return text;
}


QString Task_Dialog::get_info()
{
    return text;
}

void Task_Dialog::on_OkbuttonBox_rejected()
{
    reject();
}

void Task_Dialog::on_OkbuttonBox_accepted()
{
    accept();
}

QString Task_Dialog::Name() const
{
    return ui->name->text();
}

QString Task_Dialog::WCET() const
{
    return ui->wcetEdit->text();
}

QString Task_Dialog::Deadline() const
{
    return ui->deadtimeEdit->text();
}

QString Task_Dialog::Period() const
{
    return  ui->periodEdit->text();
}

QString Task_Dialog::Jitter() const
{
    return ui->jitterEdit->text();
}

QString Task_Dialog::Offset() const
{
    return ui->offsetEdit->text();
}

QString Task_Dialog::IsPeriodic() const
{
    if(ui->checkBox->checkState())
    {
        return "true";
    }
    else
    {
        return "false";
    }
}
