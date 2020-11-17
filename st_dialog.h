#ifndef ST_DIALOG_H
#define ST_DIALOG_H

#include <QDialog>
#include <QDebug>
#include <QMap>

namespace Ui {
class St_Dialog;
}

class St_Dialog : public QDialog
{
    Q_OBJECT

public:

    void addTask();

    QString Name() const;

    QString WCET() const;

    QString Deadline() const;

    QString Period() const;

    QString Jitter() const;

    QString Offset() const;

    QString IsPeriodic() const;

    QString execution(double, double, double, double, double, bool);

    QString get_info();

    explicit St_Dialog(QWidget *parent = nullptr);
    ~St_Dialog();

private slots:
    void on_OkbuttonBox_accepted();

    void on_OkbuttonBox_rejected();

private:
    Ui::St_Dialog *ui;

    QString text;

    QMap<QString, QString> maps;
};

#endif // TASK_DIALOG_H
