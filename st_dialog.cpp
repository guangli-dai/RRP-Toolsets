#include "st_dialog.h"
#include "ui_st_dialog.h"

St_Dialog::St_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::St_Dialog)
{
    ui->setupUi(this);
}

St_Dialog::~St_Dialog()
{
    delete ui;
}

void St_Dialog::addTask()
{
    exec();
}

QString St_Dialog::execution(double a, double b, double c, double d, double e, bool f)
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

QString St_Dialog::get_info()
{
    return text;
}


void St_Dialog::on_OkbuttonBox_rejected()
{
    reject();
}

void St_Dialog::on_OkbuttonBox_accepted()
{
    accept();
}

QString St_Dialog::Name() const
{
    return ui->name->text();
}

QString St_Dialog::WCET() const
{
    return ui->wcetEdit->text();
}

QString St_Dialog::Deadline() const
{
    return ui->deadtimeEdit->text();
}

QString St_Dialog::Period() const
{
    return  ui->periodEdit->text();
}

QString St_Dialog::Jitter() const
{
    return ui->jitterEdit->text();
}

QString St_Dialog::Offset() const
{
    return ui->offsetEdit->text();
}

QString St_Dialog::IsPeriodic() const
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
