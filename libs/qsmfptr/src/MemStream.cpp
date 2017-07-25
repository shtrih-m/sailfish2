/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MemStream.cpp
 * Author: V.Kravtsov
 * 
 * Created on June 29, 2016, 6:47 PM
 */

#include "MemStream.h"
#include <QByteArray>
#include <QDebug>

MemStream::MemStream()
{
}

MemStream::~MemStream()
{
}

int MemStream::length()
{
    return buffer.size();
}

QByteArray MemStream::getBuffer()
{
    return buffer;
}

void MemStream::setBuffer(QByteArray data)
{
    buffer.clear();
    buffer.append(data);
    pos = 0;
}

void MemStream::writeChar(uint8_t value)
{
    write(value, 1);
}

void MemStream::writeShort(uint16_t value)
{
    write(value, 2);
}

void MemStream::writeLong(uint32_t value)
{
    write(value, 4);
}

void MemStream::writeBytes(QByteArray data)
{
    buffer.append(data);
}

void MemStream::insert(int index, char value)
{
    buffer.insert(index, value);
}

void MemStream::write(char* buf, int len)
{
    buffer.append(buf, len);
}

void MemStream::write(uint64_t value, int len)
{
    for (int i = 0; i < len; i++) {
        buffer.append((value >> i * 8) & 0xFF);
    }
}

uint8_t MemStream::readChar()
{
    char c = buffer.at(pos);
    pos++;
    return c;
}

uint16_t MemStream::readShort()
{
    return read(2);
}

uint32_t MemStream::readLong()
{
    return read(4);
}

uint64_t MemStream::read(int count)
{
    long long result = 0;
    for (int i = 0; i < count; i++) {
        result += readChar() << ((count - i - 1) * 8);
    }
    return result;
}

void MemStream::read(char* buf, int count)
{
    for (int i = 0; i < count; i++) {
        buf[i] = readChar();
    }
}
