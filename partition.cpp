#include "partition.h"

Partition::Partition()
{

}

Partition::Partition(QString name, int w, int p)
{
    id = name;
    wcet = w;
    period = p;
    af = static_cast<double>(wcet)/static_cast<double>(period);
}


