#ifndef RECONF_PARTITION_DIALOG_H
#define RECONF_PARTITION_DIALOG_H

#include <QDialog>

namespace Ui {
class Reconf_Partition_Dialog;
}

class Reconf_Partition_Dialog : public QDialog
{
    Q_OBJECT

public:

    QString Name() const;
    QString WCET() const;
    QString Period() const;
    QString Deadline() const;

    explicit Reconf_Partition_Dialog(QWidget *parent = nullptr);
    ~Reconf_Partition_Dialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::Reconf_Partition_Dialog *ui;
};

#endif // PARTITION_DIALOG_H
