//events with gui
//mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "device.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_5_clicked();

    void on_pushButton_reset_clicked();
    void on_pushButton_readADD_clicked();
    void on_pushButton_writeADD_clicked();

private:
    Ui::MainWindow *ui;
    device *dev;
    void updatePortList();
};
#endif
