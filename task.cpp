#include "task.h"

Task::Task()
{


}

Task::Task(QString name, double a, double b, double c, double d, double e, bool f)
{
    id = name;
    wcet =  a;
    deadline = b;
    period = c;
    jitter = d;
    offset = e;
    isperiodic = f;
}


Task::Task(QString info)
{
    QStringList temp_list = info.split('@');
    int counter = temp_list.size();
    while(counter > 0)
    {
        switch (counter)
        {
            case 1:
                wcet = temp_list[counter - 1].toDouble();
                break;
            case 2:
                deadline = temp_list[counter - 1].toDouble();
                break;
            case 3:
                period  = temp_list[counter - 1].toDouble();
                break;
            case 4:
                jitter = temp_list[counter - 1].toDouble();
                break;
            case 5:
                offset = temp_list[counter - 1].toDouble();
                break;
            case 6:
                if(temp_list[counter - 1] == 1)
                {
                    isperiodic = true;
                }
                else
                {
                    isperiodic = false;
                }
                break;
            default:
                break;
        }
        counter--;
    }
}

QString Task::wrap_info()
{
    QString temp;

    temp = QString::number(wcet) + "@" + QString::number(deadline) + "@" + QString::number(period) + "@" + QString::number(jitter) + "@" + QString::number(offset) + "@" + isperiodic + "@" + id;

    return temp;
}

double Task::get_wcet()
{
    return wcet;
}

double Task::get_deadline()
{
    return deadline;
}

double Task::get_period()
{
    return period;
}

double Task::get_jitter()
{
    return jitter;
}

double Task::get_offset()
{
    return offset;
}

bool Task::get_isperiodic()
{
    return isperiodic;
}
