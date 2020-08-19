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

    int connectToDevice(int timeout = 0);
    int disconnect();
    int writeByte(char data);
    int readByte(uint8_t& C);
    int writeBytes(const QByteArray& tx);
    int readBytes(int count, QByteArray& rx);
    void setReadTimeout(int value);
    void setWriteTimeout(int value);
private:
    Logger* logger;
    std::string address;
    uint16_t port;
    int readTimeout;
    int writeTimeout;
    QTcpSocket socket;
};

#endif /* SOCKETPORT_H */
