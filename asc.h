#ifndef ASC_H
#define ASC_H

#include <QDialog>

namespace Ui {
class ASC;
}

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
    QString curSaveFile;
    QString curOpenFile;

    QString name;
    QString wcet, period;

    enum Column
    {
        Name, WCET, Period, Availability_Factor
    };

    int item_counter = 0;

    int id = 1;
};

#endif // ASC_H
