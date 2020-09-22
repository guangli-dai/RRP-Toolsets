#include "st.h"
#include "ui_st.h"

ST::ST(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ST)
{
    ui->setupUi(this);
}

ST::~ST()
{
    delete ui;
}
