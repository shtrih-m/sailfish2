/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SocketPort.cpp
 * Author: V.Kravtsov
 * 
 * Created on June 29, 2016, 9:51 PM
 */

#include <QByteArray>
#include <QDebug>
#include <QElapsedTimer>
#include <QThread>
#include <QTime>

#include "SocketPort.h"
#include "utils.h"
#include "logger.h"

SocketPort::SocketPort(std::string address, int port, Logger* logger)
{
    (void)logger;
    this->address = address;
    this->port = port;
    readTimeout = 1000;
    writeTimeout = 1000;
}

SocketPort::~SocketPort()
{
}

// retryTimeout

bool SocketPort::connectToDevice(int timeout)
{
    long retryTimeout = 100; // retry after 100 ms
    disconnect();

    QTime timer;
    timer.start();
    while (true) {
        socket.connectToHost(address.c_str(), port);
        if (socket.waitForConnected(timeout))
            return true;
        if (timer.elapsed() > timeout)
        {
            logger->write("Timeout connecting to host");
            return false;
        }
        QSleepThread::msleep(retryTimeout);
        socket.abort();
    }
    return true;
}

void SocketPort::disconnect()
{
    if (socket.state() == QAbstractSocket::UnconnectedState)
        return;
    socket.disconnectFromHost();
    if (socket.state() != QAbstractSocket::UnconnectedState) {
        if (!socket.waitForDisconnected(5000))
            socket.abort();
    }
}

void SocketPort::setReadTimeout(int value)
{
    readTimeout = value;
}

void SocketPort::setWriteTimeout(int value)
{
    writeTimeout = value;
}

void SocketPort::checkConnected()
{
    if (socket.state() != QAbstractSocket::ConnectedState) {
        throw new PortException("Not connected");
    }
}

uint8_t SocketPort::readByte()
{
    return readBytes(1)[0];
}

QByteArray SocketPort::readBytes(int count)
{
    connectToDevice();
    QByteArray packet;
    QElapsedTimer timer;
    timer.start();
    while (true) {
        int n = count - packet.length();
        packet.append(socket.read(n));
        if (packet.length() >= count)
            break;
        socket.waitForReadyRead(readTimeout);
        if (timer.elapsed() > readTimeout) {
            qCritical() << "Timeout error";
            throw new PortException("Timeout error");
        }
    }
    return packet;
}

void SocketPort::writeByte(char data)
{
    QByteArray ba;
    ba.append(data);
    writeBytes(ba);
}

void SocketPort::writeBytes(const QByteArray& data)
{
    connectToDevice();
    socket.write(data);
    if (!socket.waitForBytesWritten(writeTimeout)) {
        throw new PortException("Timeout writing");
    }
}
