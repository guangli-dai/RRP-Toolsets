#include "partition.h"

Partition::Partition()
{

}

Partition::Partition(QString name, long long w, long long p)
{
    id = name;
    wcet = w;
    period = p;
}
