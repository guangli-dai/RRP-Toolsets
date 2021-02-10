#ifndef ADDED_H
#define ADDED_H

#include <QDialog>

namespace Ui {
class added;
}

class added : public QDialog
{
    Q_OBJECT

public:
    explicit added(QWidget *parent = nullptr);
    ~added();

private slots:
    void on_AddedButton_clicked();

    void on_ScheduleButton_clicked();

    void on_saveButton_clicked();

private:
    Ui::added *ui;

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
        Name, WCET, Period, Deadline, Availability_Factor
    };

    int item_counter = 0;

    int id = 1;
};

#endif // ADDED_H
