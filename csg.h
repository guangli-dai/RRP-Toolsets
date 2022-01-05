#ifndef ASC_H
#define ASC_H

#include <QDialog>
#include <QSet>
#include "partition.h"

namespace Ui {
class ASC;
}

struct dp_return
{
    QSet<QString> ids;
    double af_sum;
};

struct sched_entry
{
    int start;
    int duration;
    QString id;
};

class ASC : public QDialog
{
    Q_OBJECT

public:
    explicit ASC(QWidget *parent = nullptr);
    ~ASC();

private slots:
    void on_addPartitionButton_clicked();

    void on_removePartitionButton_clicked();

    void on_generateButton_clicked();

    void on_loadButton_clicked();

    void on_saveButton_clicked();

private:
    Ui::ASC *ui;

    void saveFile();
    void loadFile();
    double approximate_value(double input);
    approximates z_approx(double aaf, int factor);
    dp_return dp_single(QVector<Partition> partition_list, int factor);
    QVector<QVector<QString>> MulZ_ILO(QVector<Partition> partitions, int numCPU);
    QVector<QString> CSG_AZ(QVector<Partition>, int factor);
    bool check_delta(QSet<int> avail_set, QVector<int> standard_p, int delta, int p);
    int find_delta(QSet<int> avail_timeslices, int p, int q, int q_left);
    int lcm(int a, int b);
    QString getXMLFormat(QVector<QVector<QString>> schedules, int time_slice_size);

    QString curSaveFile;
    QString curOpenFile;

    QString name;
    QString wcet, period;

    enum Column
    {
        Name, WCET, Period, Availability_Factor
    };

    QVector<QString> m_data;

    int item_counter = 0;

    int id = 1;
};

#endif // ASC_H
