// device.cpp
#include "device.h"
#include <QByteArray>

#include <QDebug>

device::device(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
    // connect(serial, &QSerialPort::readyRead, this, &device::readData); //attempt to use multithreading
}

device::~device()
{
    disconnect();
}

QStringList device::getAvailablePorts()
{
    QStringList ports;
    const auto portList = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : portList) {
        ports << info.portName();
    }
    return ports;
}

bool device::connectToPort(const QString &portName)
{
    if (serial->isOpen()) {
        serial->close();
    }

    serial->setPortName(portName);
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::HardwareControl);

    if (serial->open(QIODevice::ReadWrite)) {
        serial->setDataTerminalReady(true);
        serial->setRequestToSend(true);
        // qDebug() << "Connected to" << serial->portName();
        return true;
    } else {
        // qDebug() << "Failed to open port:" << serial->errorString();
        return false;
    };
}

bool device::sendData(const QByteArray &data)
{
    if (!serial->isOpen()) return false;
    qint64 bytesWritten = serial->write(data);
    return (bytesWritten == data.size());
}

QByteArray device::readData()
{
    QByteArray data;

    if (!serial->isOpen()) return data;

    if (serial->waitForReadyRead(200)) {
        data.append(serial->readAll());
        while (serial->waitForReadyRead(20)) {
            data.append(serial->readAll());
        }
    }

    // qDebug() << "Read bytes:" << data.toHex(' ');

    return data;
}

// QByteArray device::readData() //attempt to use multithreading
// {
//     QByteArray data;

//     data.append(serial->readAll());
//     while (serial->waitForReadyRead(20)) {
//                     data.append(serial->readAll());
//                 }

//     qDebug() << "Received:" << data.toHex(' ');

//     return data;
// }

bool device::resetDevice()
{
    QByteArray command = QByteArray::fromRawData("\x01\x03\x0C\x00", 4);
    return sendData(command);
}

QByteArray device::readADD()
{
    QByteArray command = QByteArray::fromRawData("\x01\x09\x10\x00", 4);
    if (!sendData(command)) return {};

    QByteArray response = readData();

    return response;
}

bool device::writeADD(const QByteArray &addr)
{
    if (addr.size() != 6) return false;

    QByteArray command = QByteArray::fromRawData("\x01\x01\xFC\x06", 4);
    command.append(addr);
    return sendData(command);
}
