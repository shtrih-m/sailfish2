/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SocketPort.h
 * Author: V.Kravtsov
 *
 * Created on June 29, 2016, 9:51 PM
 */

#ifndef SOCKETPORT_H
#define SOCKETPORT_H

#include "fiscalprinter.h"
#include <QByteArray>
#include <QTcpSocket>
#include <string>
#include "logger.h"

class SocketPort : public PrinterPort {
public:
    SocketPort(std::string address, int port, Logger* logger);
    virtual ~SocketPort();

    bool connectToDevice(int timeout = 0);
    void disconnect();
    uint8_t readByte();
    void setReadTimeout(int value);
    void setWriteTimeout(int value);
    QByteArray readBytes(int count);
    void writeByte(char data);
    void writeBytes(const QByteArray& data);

private:
    Logger* logger;
    std::string address;
    int port;
    int readTimeout;
    int writeTimeout;
    QTcpSocket socket;
    void checkConnected();
};

#endif /* SOCKETPORT_H */
