#ifndef TASK_H
#define TASK_H
#include <QString>
#include <QStringList>

class Task
{
public:
    Task();
    Task(QString, double, double, double, double, double, bool);
    Task(QString info);

    QString wrap_info();

    double get_wcet();
    double get_deadline();
    double get_period();
    double get_jitter();
    double get_offset();
    bool get_isperiodic();

private:
    double wcet;
    double deadline;
    double period;
    double jitter;
    double offset;
    bool isperiodic;

    QString id;
};

#endif // TASK_H
