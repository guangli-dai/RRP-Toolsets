#include "reconf_partition_dialog.h"
#include "ui_reconf_partition_dialog.h"

Reconf_Partition_Dialog::Reconf_Partition_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Reconf_Partition_Dialog)
{
    ui->setupUi(this);
}

Reconf_Partition_Dialog::~Reconf_Partition_Dialog()
{
    delete ui;
}

void Reconf_Partition_Dialog::on_buttonBox_accepted()
{
    //TODO: do a parameter check here
    accept();
}

void Reconf_Partition_Dialog::on_buttonBox_rejected()
{
    reject();
}

QString Reconf_Partition_Dialog::Name() const
{
    return ui->idLineEdit->text();
}

QString Reconf_Partition_Dialog::WCET() const
{
    return ui->wcetLineEdit->text();
}

QString Reconf_Partition_Dialog::Period() const
{
    return ui->periodLineEdit->text();
}

QString Reconf_Partition_Dialog::Deadline() const
{
    return ui->deadlineLineEdit->text();
}

