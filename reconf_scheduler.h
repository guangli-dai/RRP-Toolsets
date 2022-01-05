#ifndef RECONF_SCHEDULER_H
#define RECONF_SCHEDULER_H

#include "partition.h"
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

    void on_saveButton_clicked();

    void on_AddedButton_clicked();

    void on_removeButton_clicked();

    void on_loadButton_clicked();

    void on_generateButton_clicked();

private:
    Ui::reconf_scheduler *ui;

    void saveFile();
    void loadFile();
    QString getXMLFormat(QVector<QVector<QString> > schedules, int time_slice_size);
    QVector<QString> OHR_OPT(QVector<Reconf_Partition>& partitions);
    approximates z_approx(double af, int factor);
    double approximate_value(double value);
    bool test_approx(QVector<Reconf_Partition>& partitions, int factor);
    int lcm(int a, int b);
    QSet<int> tau(int p, int q, int delta);
    bool search(QVector<QVector<QSet<int>>> &taus, QVector<int> &result, QSet<int> &now);

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

#endif // RECONF_SCHEDULER_H
