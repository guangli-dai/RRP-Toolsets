#include "partition_dialog.h"
#include "ui_partition_dialog.h"

Partition_Dialog::Partition_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Partition_Dialog)
{
    ui->setupUi(this);
}

Partition_Dialog::~Partition_Dialog()
{
    delete ui;
}

void Partition_Dialog::on_buttonBox_accepted()
{
    //TODO: do a parameter check here
    accept();
}

void Partition_Dialog::on_buttonBox_rejected()
{
    reject();
}

QString Partition_Dialog::Name() const
{
    return ui->idLineEdit->text();
}

QString Partition_Dialog::WCET() const
{
    return ui->wcetLineEdit->text();
}

QString Partition_Dialog::Period() const
{
    return ui->periodLineEdit->text();
}

