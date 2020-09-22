#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <pcg.h>
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

    void on_actionOpen_triggered();

    void on_actionSave_as_triggered();

    void on_pcgButton_clicked();

    void on_ascButton_clicked();

    void on_schedulabilityButton_clicked();

    void on_actionPCG_triggered();

    void on_actionASG_triggered();

    void on_actionST_triggered();

    void on_actionRRP_model_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
