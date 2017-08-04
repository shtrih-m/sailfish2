#ifndef TLVLIST_H
#define TLVLIST_H

#include <QByteArray>

class TLVList
{
public:
    TLVList();
    QByteArray getData();
    void addInt(int value);
    QByteArray to866(QString value);
    void add(uint16_t tagId, QString tagValue);
    void add(uint16_t tagId, long tagValue, int len);
    QByteArray intToTLV(long value, uint8_t len);
private:
    QByteArray buffer;
};

#endif // TLVLIST_H
