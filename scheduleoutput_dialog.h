#ifndef SCHEDULEOUTPUT_DIALOG_H
#define SCHEDULEOUTPUT_DIALOG_H

#include <QDialog>

namespace Ui {
class ScheduleOutput_Dialog;
}

class ScheduleOutput_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScheduleOutput_Dialog(QWidget *parent = nullptr);
    void setXML(QString xmlOutput);
    ~ScheduleOutput_Dialog();

private:
    Ui::ScheduleOutput_Dialog *ui;
};

#endif // SCHEDULEOUTPUT_DIALOG_H
