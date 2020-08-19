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

#include <QTime>
#include <QDebug>
#include <QThread>
#include <QByteArray>
#include <QElapsedTimer>

#include "utils.h"
#include "logger.h"
#include "socketport.h"

SocketPort::SocketPort(std::string address, int port, Logger* logger)
{
    this->address = address;
    this->port = port;
    this->logger = logger;
    readTimeout = 1000;
    writeTimeout = 1000;
}

SocketPort::~SocketPort()
{
}

// retryTimeout

int SocketPort::connectToDevice(int timeout)
{
    long retryTimeout = 100; // retry after 100 ms
    disconnect();

    QTime timer;
    timer.start();
    while (true) {
        socket.connectToHost(address.c_str(), port);
        if (socket.waitForConnected(timeout))
            return SMFPTR_OK;
        if (timer.elapsed() > timeout)
        {
            logger->write("Timeout connecting to host");
            return SMFPTR_E_NOCONNECTION;
        }
        QSleepThread::msleep(retryTimeout);
        socket.abort();
    }
    return SMFPTR_OK;
}

int SocketPort::disconnect()
{
    if (socket.state() == QAbstractSocket::UnconnectedState)
        return SMFPTR_OK;
    socket.disconnectFromHost();
    if (socket.state() != QAbstractSocket::UnconnectedState) {
        if (!socket.waitForDisconnected(5000))
            socket.abort();
    }
    return SMFPTR_OK;
}

void SocketPort::setReadTimeout(int value)
{
    readTimeout = value;
}

void SocketPort::setWriteTimeout(int value)
{
    writeTimeout = value;
}

int SocketPort::readByte(uint8_t& B)
{
    QByteArray rx;
    int rc = readBytes(1, rx);
    if (rc == 0){
        B = rx[0];
    }
    return rc;
}

int SocketPort::readBytes(int count, QByteArray& rx)
{
    int rc = connectToDevice();
    if (rc != 0) return rc;

    QElapsedTimer timer;
    timer.start();
    while (true) {
        int n = count - rx.length();
        rx.append(socket.read(n));
        if (rx.length() >= count)
            break;
        socket.waitForReadyRead(readTimeout);
        if (timer.elapsed() > readTimeout)
        {
            logger->write("Timeout error");
            return SMFPTR_E_NOCONNECTION;
        }
    }
    return SMFPTR_OK;
}

int SocketPort::writeByte(char data)
{
    QByteArray ba;
    ba.append(data);
    return writeBytes(ba);
}

int SocketPort::writeBytes(const QByteArray& data)
{
    int rc = connectToDevice();
    if (rc != 0) return rc;

    socket.write(data);
    if (!socket.waitForBytesWritten(writeTimeout))
    {
        logger->write("Timeout writing");
        return SMFPTR_E_NOCONNECTION;
    }
    return SMFPTR_OK;
}
