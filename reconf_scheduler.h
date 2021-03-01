#ifndef RECONF_SCHEDULER_H
#define RECONF_SCHEDULER_H

#include <QDialog>

namespace Ui {
class reconf_scheduler;
}

class reconf_scheduler : public QDialog
{
    Q_OBJECT

public:
    explicit reconf_scheduler(QWidget *parent = nullptr);
    ~reconf_scheduler();

private slots:
    void on_reconf_schedulerButton_clicked();

    void on_ScheduleButton_clicked();

    void on_saveButton_clicked();

    void on_AddedButton_clicked();

    void on_removeButton_clicked();

    void on_loadButton_clicked();

private:
    Ui::reconf_scheduler *ui;

    void saveFile();
    void loadFile();

    QString curSaveFile;
    QString curOpenFile;

    QString name;
    QString wcet;
    QString period;
    QString deadline;

    enum Column
    {
        Name, WCET, Period, Availability_Factor
    };

    int item_counter = 0;

    int id = 1;
};

#endif // RECONF_SCHEDULER_H
