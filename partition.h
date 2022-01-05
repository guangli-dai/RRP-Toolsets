#ifndef PARTITION_H
#define PARTITION_H
#include <QString>

struct approximates
{
    double aaf;
    int WCET;
    int period;
};

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

class Reconf_Partition
{
public:
    Reconf_Partition();
    Reconf_Partition(QString name, int w, int p, double d);
    QString getId(){return id;}
    int getWCET(){return wcet;}
    int getPeriod(){return period;}
    double getAF(){return af;}
    double getDeadline(){return deadline;}
    void reset(int w, int p){wcet = w; period=p;}

private:
    QString id;
    int wcet;
    int period;
    double af;
    double deadline;

};

#endif // PARTITION_H
