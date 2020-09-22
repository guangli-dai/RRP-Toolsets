#ifndef PARTITION_H
#define PARTITION_H
#include <QString>

class Partition
{
public:
    Partition();
    Partition(QString name, long long w, long long p);

private:
    QString id;
    long long wcet;
    long long period;
};

#endif // PARTITION_H
