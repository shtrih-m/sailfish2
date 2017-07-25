/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PrinterCommand.h
 * Author: V.Kravtsov
 *
 * Created on June 28, 2016, 8:39 PM
 */

#ifndef PRINTERCOMMAND_H
#define PRINTERCOMMAND_H

#include <cstdint>
#include <vector>

#include "PrinterTypes.h"
#include <QTextCodec>

class PrinterCommand {
public:
    PrinterCommand(uint16_t code);
    virtual ~PrinterCommand();

    void warn(int resultCode);
    void write8(uint8_t value);
    void write16(uint16_t value);
    void write32(uint32_t value);
    void write(uint64_t value, int len);
    void write(QString text);
    void write(QByteArray data);
    void write(QByteArray data, int minlen);
    void write(QString text, int minLen);
    void writeStr(char* buf, int len);
    void writeStr(QString text, int len);

    uint8_t readChar();
    uint8_t read8();
    uint16_t read16();
    uint32_t read32();
    uint16_t readShort();
    uint32_t readLong();
    void read(char* buf, int count);
    PrinterDate readDate();
    PrinterDate readDate2();
    PrinterTime readTime();
    PrinterTime readTime2();
    void write(PrinterTime time);
    void write(PrinterDate date);
    QString readStr(int count);
    QString readStr();
    uint64_t read(int count);
    QByteArray readBytes();

    QByteArray encode();
    int decode(QByteArray data);
    QByteArray getTxBuffer();
    QByteArray getRxBuffer();
    void setBuffer(QByteArray data);

    uint16_t getCode();
    int getTimeout();
    QByteArray to1251(QString text);
    QString from1251(QByteArray data);
private:
    uint16_t code;
    int pos;
    QByteArray txbuffer;
    QByteArray rxbuffer;
    QTextCodec* codec;
};

#endif /* PRINTERCOMMAND_H */
