#ifndef XINPUTSTREAM_H
#define XINPUTSTREAM_H

#include <QByteArray>
#include <QTextCodec>
#include "PrinterTypes.h"

class XInputStream
{
public:
    XInputStream();

    uint8_t read8();
    uint16_t read16();
    uint32_t read32();
    uint64_t read64();
    uint64_t read(int count);
    PrinterDate readDate();
    PrinterDate readDate2();
    PrinterTime readTime();
    PrinterTime readTime2();
    QString readStr(int count);
    QString readStr();
    QByteArray readBytes();
    void read(char* buf, int count);
    QString from1251(QByteArray data);
    void setBuffer(QByteArray buf);
private:
    int pos;
    QByteArray buffer;
    QTextCodec* codec;
};

#endif // XINPUTSTREAM_H
