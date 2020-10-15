#ifndef PCG_H
#define PCG_H

#include <QDialog>
#include <task.h>
#include <QMap>
#include <QVector>
#include <QComboBox>
#include <task_dialog.h>
#include <QListWidgetItem>
#include <QtMath>

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

    void on_getconfigButton_clicked();

    //calcualtes a_deno/a_no += b_deno/b_no
    void fraction_add(long& a_deno, long& a_no, long b_deno, long b_no);

    //reduce a_deno/a_no
    void reduce(long& a_deno, long& a_no);
private:
    Ui::PCG *ui;

    QString name;
    QString wcet, deadline, period, jitter, offset;
    QString isperiodic;

    enum Column
    {
        Name, WCET, Deadline, Period, Jitter, Offset, Periodic_or_sporadic,
    };

    QVector<QString> m_data;

    int item_counter = 0;
    int id = 1;
};

#endif // PCG_H
