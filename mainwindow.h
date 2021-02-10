#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
//#include <rsg.h>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:

    //void on_actionOpen_triggered();

    //void on_actionSave_as_triggered();

    void on_ascButton_clicked();

    void on_schedulabilityButton_clicked();

    void on_actionRSG_triggered();

    void on_actionASG_triggered();

    void on_actionST_triggered();

    void on_actionRRP_model_triggered();

    void on_rsgButton_clicked();

    void on_AddedBuuton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
