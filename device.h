//connect device to port that has been chosen in mainwindow in list reset, write, read
//device.h
#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>

class device : public QObject
{
    Q_OBJECT

public:
    explicit device(QObject *parent = nullptr);
    ~device();

    QStringList getAvailablePorts();
    bool connectToPort(const QString &portName);

    bool sendData(const QByteArray &data);
    QByteArray readData();

    bool resetDevice();
    QByteArray readADD();
    bool writeADD(const QByteArray &addr);

private:
    QSerialPort *serial;
};

#endif
