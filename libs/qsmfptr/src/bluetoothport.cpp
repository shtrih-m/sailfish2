#include <QBluetoothSocket>
#include <QBluetoothUuid>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QObject>

#include "bluetoothport.h"
#include "utils.h"

BluetoothPort::BluetoothPort(QObject* parent)
    : QObject(parent)
{
    connected = false;
    this->address = "";
    readTimeout = 1000;
    writeTimeout = 1000;
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);

    QObject::connect(socket, &QBluetoothSocket::connected, this,
        &BluetoothPort::socketConnected);

    QObject::connect(socket, &QBluetoothSocket::disconnected, this,
        &BluetoothPort::socketDisconnected);

    QObject::connect(socket, static_cast<void (QBluetoothSocket::*)(QBluetoothSocket::SocketError)>(&QBluetoothSocket::error), this, &BluetoothPort::socketError);

    QObject::connect(socket, &QBluetoothSocket::stateChanged, this, &BluetoothPort::socketStateChanged);
}

void BluetoothPort::setAddress(QString value)
{
    address = value;
}

void BluetoothPort::socketConnected()
{
    qDebug() << "connected";
    connected = true;
}

void BluetoothPort::socketDisconnected()
{
    qDebug() << "disconnected";
    connected = false;
}

void BluetoothPort::socketError(QBluetoothSocket::SocketError error)
{
    qDebug() << "error: " << error;
}

void BluetoothPort::socketStateChanged(QBluetoothSocket::SocketState state)
{
    qDebug() << "stateChanged: " << state;
}

bool BluetoothPort::connectToDevice()
{
    qDebug() << "connectToDevice";

    if (connected) return true;
    //QString suuid = "{00000000-deca-fade-deca-deafdecacaff}";
    QString suuid = "{00001101-0000-1000-8000-00805F9B34FB}";

    socket->connectToService(QBluetoothAddress(address),
        QBluetoothUuid(suuid));
    //socket->connectToService(QBluetoothAddress(address), 3);

    QElapsedTimer timer;
    timer.start();
    while (!connected)
    {
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
        if (timer.elapsed() > connectTimeout) {
            qCritical() << "Connect timeout error";
            return false;
        }
    }
    qDebug() << "connectToDevice: OK";
    return true;
}

void BluetoothPort::disconnect()
{
    qDebug() << "disconnect()";
    if (!connected) return;
    socket->disconnectFromService();
}

void BluetoothPort::setReadTimeout(int value)
{
    qDebug() << "setReadTimeout(" << value << ")";
    readTimeout = value;
}

void BluetoothPort::setWriteTimeout(int value)
{
    qDebug() << "setWriteTimeout(" << value << ")";
    writeTimeout = value;
}

void BluetoothPort::setConnectTimeout(int value)
{
    connectTimeout = value;
}

uint8_t BluetoothPort::readByte()
{
    return readBytes(1)[0];
}

QByteArray BluetoothPort::readBytes(int count)
{
    connectToDevice();

    QByteArray packet;
    QElapsedTimer timer;
    timer.start();
    while (true) {
        int n = count - packet.length();
        packet.append(socket->read(n));
        if (packet.length() >= count)
            break;
        socket->waitForReadyRead(readTimeout);
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
        if (timer.elapsed() > readTimeout) {
            qCritical() << "Timeout error";
            throw new PortException("Timeout error");
        }
    }
    qDebug() << "<- " << StringUtils::dataToHex(packet);
    return packet;
}

void BluetoothPort::writeByte(char data)
{
    QByteArray ba;
    ba.append(data);
    writeBytes(ba);
}

void BluetoothPort::writeBytes(const QByteArray& data)
{
    qDebug() << "-> " << StringUtils::dataToHex(data);

    connectToDevice();
    socket->write(data);
}
