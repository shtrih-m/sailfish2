#ifndef TLVLIST_H
#define TLVLIST_H

#include <QByteArray>

class TLVList
{
public:
    TLVList();
    QByteArray getData();
    void addInt(uint16_t value);
    QByteArray to866(QString value);
    void add(uint16_t tagId, QString tagValue);
    void add(uint16_t tagId, long tagValue, uint16_t len);
    QByteArray intToTLV(long value, uint16_t len);
private:
    QByteArray buffer;
};

#endif // TLVLIST_H
