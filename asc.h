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

private:
    Ui::ASC *ui;

    enum Column
    {
        Name, WCEP, Period, Availability_Factor
    };

    QVector<QString> m_data;

    int id = 1;
};

#endif // ASC_H
