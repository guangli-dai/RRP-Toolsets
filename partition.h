#ifndef PARTITION_H
#define PARTITION_H
#include <QString>

class Partition
{
public:
    Partition();
    Partition(QString name, int w, int p);
    QString getId(){return id;}
    int getWCET(){return wcet;}
    int getPeriod(){return period;}
    double getAF(){return af;}
    void reset(int w, int p){wcet = w; period=p;}
private:
    QString id;
    int wcet;
    int period;
    double af;//short for availability factor
};

#endif // PARTITION_H
