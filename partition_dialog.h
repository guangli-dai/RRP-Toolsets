#ifndef PARTITION_DIALOG_H
#define PARTITION_DIALOG_H

#include <QDialog>

namespace Ui {
class Partition_Dialog;
}

class Partition_Dialog : public QDialog
{
    Q_OBJECT

public:

    QString Name() const;
    QString WCET() const;
    QString Period() const;

    explicit Partition_Dialog(QWidget *parent = nullptr);
    ~Partition_Dialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::Partition_Dialog *ui;
};

#endif // PARTITION_DIALOG_H
