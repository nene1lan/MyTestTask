//mainwindow.cpp
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QtSerialPort/QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , dev(new device(this))
{
    ui->setupUi(this);
    updatePortList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updatePortList()
{
    ui->comboBox->clear();
    QStringList ports = dev->getAvailablePorts();
    ui->comboBox->addItems(ports);
}

void MainWindow::on_pushButton_clicked()
{
    QString selectedPort = ui->comboBox->currentText();

    if (dev->connectToPort(selectedPort)) {
        ui->label_2->setText("Connected to " + selectedPort);
        ui->label_2->setStyleSheet("color: green;");
        ui->plainTextEdit_2->insertPlainText("Connected to " + selectedPort + "\n");
    } else {
        ui->label_2->setText("Connection failed!");
        ui->label_2->setStyleSheet("color: red;");
        ui->plainTextEdit_2->insertPlainText("Connection to " + selectedPort + " failed.\n");
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    updatePortList();
    ui->plainTextEdit_2->insertPlainText("Port list refreshed.\n");
}

void MainWindow::on_pushButton_reset_clicked(){
    if(dev->resetDevice()){
        ui->plainTextEdit_2->insertPlainText((dev->readData().toHex()) + "\n");
    } else ui->plainTextEdit_2->insertPlainText("An error occured.\n");
}

void MainWindow::on_pushButton_readADD_clicked()
{
    QByteArray response = dev->readADD();

    if (!response.isEmpty()) {
        ui->plainTextEdit_2->insertPlainText (response.toHex(' ') + "\n");
    } else {
        ui->plainTextEdit_2->insertPlainText("An error occured.\n");
    }
}

void MainWindow::on_pushButton_writeADD_clicked(){
    QString text = ui->plainTextEdit->toPlainText();
    QByteArray addr = QByteArray::fromHex(text.toUtf8().trimmed());

    if (dev->writeADD(addr)) {
        ui->plainTextEdit_2->insertPlainText("Sucsess.\n");
    } else {
       ui->plainTextEdit_2->insertPlainText("Write failed.\n");
    }
}
