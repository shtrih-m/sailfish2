/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PrinterCommand.cpp
 * Author: V.Kravtsov
 * 
 * Created on June 28, 2016, 8:39 PM
 */

#include "PrinterCommand.h"
#include "ShtrihFiscalPrinter.h"
#include "utils.h"
#include <QDebug>

PrinterCommand::PrinterCommand(uint16_t code)
{
    this->code = code;
    codec = QTextCodec::codecForName("Windows-1251");
}

PrinterCommand::~PrinterCommand()
{
}

QByteArray PrinterCommand::encode()
{
    QByteArray ba;
    if (code > 0xFF)
    {
        ba.append((code >> 8) & 0xFF);
    }
    ba.append(code & 0xFF);
    ba.append(txbuffer);
    return ba;
}

void PrinterCommand::warn(int resultCode)
{
    if (resultCode != 0)
    {
        QString text = ShtrihFiscalPrinter::getErrorText(resultCode);
        qWarning() << text;
    }
}

int PrinterCommand::decode(QByteArray data)
{
    rxdata.clear();
    rxdata.append(data);

    int resultCode = 0;
    uint16_t rxCode = data.at(0);
    if (rxCode == 0xFF){
        rxCode = (data.at(0) << 8) + data.at(1);
        resultCode = data.at(2);
        data.remove(0, 3);
    } else{
        resultCode = data.at(1);
        data.remove(0, 2);
    }
    warn(resultCode);

    rxbuffer = data;
    pos = 0;
    return resultCode;
}

uint16_t PrinterCommand::getCode()
{
    return code;
}

QByteArray PrinterCommand::getTxBuffer()
{
    return txbuffer;
}

QByteArray PrinterCommand::getRxBuffer()
{
    return rxbuffer;
}

QByteArray PrinterCommand::getRxData()
{
    return rxdata;
}

void PrinterCommand::setBuffer(QByteArray data)
{
    rxbuffer.clear();
    rxbuffer.append(data);
    pos = 0;
}

void PrinterCommand::write8(uint8_t value)
{
    write((uint64_t)value, 1);
}

void PrinterCommand::write16(uint16_t value)
{
    write((uint64_t)value, 2);
}

void PrinterCommand::write32(uint32_t value)
{
    write((uint64_t)value, 4);
}

void PrinterCommand::write(uint64_t value, int len)
{
    for (int i = 0; i < len; i++) {
        txbuffer.append((value >> i * 8) & 0xFF);
    }
}

void PrinterCommand::write(QString text)
{
    txbuffer.append(to1251(text));
}

void PrinterCommand::writeStr(QString text, int len)
{
    QByteArray buf = to1251(text.left(len));
    txbuffer.append(buf);

    int count = len - buf.size();
    if (count > 0) {
        QByteArray ba;
        ba.fill(0x00, count);
        txbuffer.append(ba);
    }
}

void PrinterCommand::writeStr(char* buf, int len)
{
    txbuffer.append(buf, len);
}

void PrinterCommand::write(QByteArray data){
    txbuffer.append(data);
}

void PrinterCommand::write(QByteArray data, int minlen)
{
    txbuffer.append(data);
    int count = minlen - data.length();
    if (count > 0) {
        QByteArray ba;
        ba.fill(0x00, count);
        txbuffer.append(ba);
    }
}

void PrinterCommand::write(QString text, int minLen)
{
    txbuffer.append(to1251(text));
    int count = minLen - text.length();
    if (count > 0) {
        QByteArray ba;
        ba.fill(0x00, count);
        txbuffer.append(ba);
    }
}

QByteArray PrinterCommand::to1251(QString text)
{
    QByteArray result;
    result.append(text);
    if (codec) {
        result = codec->fromUnicode(text);
    }
    return result;
}

QString PrinterCommand::from1251(QByteArray data)
{
    QString result(data);
    if (codec) {
        result = codec->toUnicode(data);
    }
    return result;
}

QByteArray PrinterCommand::readBytes(){
    QByteArray ba = rxbuffer.right(rxbuffer.length() - pos);
    return ba;
}

uint8_t PrinterCommand::readChar()
{
    return read8();
}

uint8_t PrinterCommand::read8()
{
    uint8_t c = rxbuffer.at(pos);
    pos++;
    return c;
}

QString PrinterCommand::readStr()
{
    QByteArray ba = rxbuffer.right(rxbuffer.length() - pos);
    QString result = from1251(ba);
    return result;
}

QString PrinterCommand::readStr(int count)
{
    QString result;
    for (int i = 0; i < count; i++)
    {
        uint8_t c = readChar();
        if (c != 0){
            result.append(c);
        }
    }
    return result;
}

uint16_t PrinterCommand::readShort()
{
    return (uint16_t)read(2);
}

uint16_t PrinterCommand::read16()
{
    return (uint16_t)read(2);
}

uint32_t PrinterCommand::readLong()
{
    return (uint32_t)read(4);
}

uint32_t PrinterCommand::read32()
{
    return (uint32_t)read(4);
}

uint64_t PrinterCommand::read(int count)
{
    uint64_t result = 0;
    for (int i = 0; i < count; i++)
    {   uint64_t c = readChar();
        result += c << (i*8);
    }
    return result;
}

void PrinterCommand::read(char* buf, int count)
{
    for (int i = 0; i < count; i++) {
        buf[i] = readChar();
    }
}

void PrinterCommand::write(PrinterTime time)
{
    write8(time.hour);
    write8(time.min);
    write8(time.sec);
}

void PrinterCommand::write(PrinterDate date)
{
    write8(date.day);
    write8(date.month);
    write8(date.year);
}

PrinterDate PrinterCommand::readDate()
{
    PrinterDate result;
    result.day = readChar();
    result.month = readChar();
    result.year = readChar();
    return result;
}

PrinterDate PrinterCommand::readDate2()
{
    PrinterDate result;
    result.year = readChar();
    result.month = readChar();
    result.day = readChar();
    return result;
}

PrinterTime PrinterCommand::readTime()
{
    PrinterTime result;
    result.hour = readChar();
    result.min = readChar();
    result.sec = readChar();
    return result;
}

PrinterTime PrinterCommand::readTime2()
{
    PrinterTime result;
    result.hour = readChar();
    result.min = readChar();
    result.sec = 0;
    return result;
}

int PrinterCommand::getTimeout()
{
    switch (code) {
    case 0x16:
        return 60000;
    case 0xB2:
        return 20000;
    case 0xB4:
        return 40000;
    case 0xB5:
        return 40000;
    case 0xB6:
        return 150000;
    case 0xB7:
        return 150000;
    case 0xB8:
        return 100000;
    case 0xB9:
        return 100000;
    case 0xBA:
        return 40000;
    case 0x40:
        return 30000;
    case 0x41:
        return 30000;
    case 0x61:
        return 20000;
    case 0x62:
        return 30000;
    default:
        return 10000;
    }
}
