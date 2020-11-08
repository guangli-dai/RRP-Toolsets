#ifndef ST_H
#define ST_H

#include <QDialog>
#include <st_dialog.h>

namespace Ui {
class ST;
}

class ST : public QDialog
{
    Q_OBJECT

public:
    explicit ST(QWidget *parent = nullptr);
    ~ST();

public slots:
    void on_taskEnterButton_clicked();

    void on_taskRemoveButton_clicked();

private slots:
    void on_saveButton_clicked();

    void on_loadButton_clicked();

private:
    Ui::ST *ui;

    void saveFile();
    void loadFile();
    QString curSaveFile;
    QString curOpenFile;

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
#endif // ST_H
