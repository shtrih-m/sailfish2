#include "xinputstream.h"

#include <QByteArray>

XInputStream::XInputStream()
{

}

void XInputStream::setBuffer(QByteArray buf){
    buffer = buf;
    pos = 0;
}

QString XInputStream::from1251(QByteArray data)
{
    QString result(data);
    if (codec) {
        result = codec->toUnicode(data);
    }
    return result;
}

QByteArray XInputStream::readBytes()
{
    QByteArray ba = buffer.right(buffer.length() - pos);
    return ba;
}

uint8_t XInputStream::read8()
{
    uint8_t c = buffer.at(pos);
    pos++;
    return c;
}

QString XInputStream::readStr()
{
    QByteArray ba = buffer.right(buffer.length() - pos);
    QString result = from1251(ba);
    return result;
}

QString XInputStream::readStr(int count)
{
    QString result;
    for (int i = 0; i < count; i++)
    {
        uint8_t c = read8();
        if (c != 0){
            result.append(c);
        }
    }
    return result;
}

uint16_t XInputStream::read16()
{
    return static_cast<uint16_t>(read(2));
}

uint32_t XInputStream::read32()
{
    return static_cast<uint32_t>(read(4));
}

uint64_t XInputStream::read64()
{
    return read(8);
}

uint64_t XInputStream::read(int count)
{
    uint64_t result = 0;
    for (int i = 0; i < count; i++)
    {   uint64_t c = read8();
        result += c << (i*8);
    }
    return result;
}

void XInputStream::read(char* buf, int count)
{
    for (int i = 0; i < count; i++) {
        buf[i] = read8();
    }
}

PrinterDate XInputStream::readDate()
{
    PrinterDate result;
    result.day = read8();
    result.month = read8();
    result.year = read8();
    return result;
}

PrinterDate XInputStream::readDate2()
{
    PrinterDate result;
    result.year = read8();
    result.month = read8();
    result.day = read8();
    return result;
}

PrinterTime XInputStream::readTime()
{
    PrinterTime result;
    result.hour = read8();
    result.min = read8();
    result.sec = read8();
    return result;
}

PrinterTime XInputStream::readTime2()
{
    PrinterTime result;
    result.hour = read8();
    result.min = read8();
    result.sec = 0;
    return result;
}

