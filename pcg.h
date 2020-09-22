#ifndef PCG_H
#define PCG_H

#include <QDialog>
#include <task.h>
#include <QMap>
#include <task_dialog.h>
#include <QListWidgetItem>

namespace Ui {
class PCG;
}

class PCG : public QDialog
{
    Q_OBJECT

public:
    explicit PCG(QWidget *parent = nullptr);
    ~PCG();

public slots:
    void on_taskEnterButton_clicked();

    void on_taskRemoveButton_clicked();

private slots:
    void on_saveButton_clicked();

    void on_loadButton_clicked();

private:
    Ui::PCG *ui;

    QString name;
    double wcet, deadline, period, jitter, offset;
    bool isperiodic;

    enum Column
    {
        Name, WCEP, Deadline, Period, Jitter, Offset, Periodic_or_sporadic,
    };

    QVector<QString> m_data;

    int id = 1;
};

#endif // PCG_H
