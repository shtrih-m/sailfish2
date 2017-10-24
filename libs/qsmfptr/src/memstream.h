/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MemStream.h
 * Author: V.Kravtsov
 *
 * Created on June 29, 2016, 6:47 PM
 */

#ifndef MEMSTREAM_H
#define MEMSTREAM_H

#include <QByteArray>

class MemStream {
public:
    MemStream();
    virtual ~MemStream();
    void write(char* buf, int len);
    void writeBytes(QByteArray data);
    void writeChar(uint8_t value);
    void writeShort(uint16_t value);
    void writeLong(uint32_t value);
    void write(uint64_t value, int len);
    void insert(int index, char value);

    uint8_t readChar();
    uint16_t readShort();
    uint32_t readLong();
    void read(char* buf, int count);
    uint64_t read(int count);

    int length();
    QByteArray getBuffer();
    void setBuffer(QByteArray data);

private:
    int pos;
    QByteArray buffer;
};

#endif /* MEMSTREAM_H */
