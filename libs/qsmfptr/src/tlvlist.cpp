#include "tlvlist.h"
#include <QString>
#include <QByteArray>
#include "fiscalprinter.h"

TLVList::TLVList()
{

}

void TLVList::add(uint16_t tagId, QString tagValue)
{
    addInt(tagId);
    addInt(tagValue.length());
    buffer.append(tagValue.toLatin1());
}

void TLVList::add(uint16_t tagId, long tagValue, int len)
{
    addInt(tagId);
    addInt(len);
    buffer.append(intToTLV(tagValue, len));
}

QByteArray TLVList::intToTLV(long value, uint8_t len)
{
    if ((len > 8) || (len < 1)) {
        throw new TextException("Invalid length");
    }
    QByteArray result;
    for (int i = 0; i < len; i++)
    {
        uint8_t b = (uint8_t) ((value >> (i * 8)) & 0xFF);
        result.append(b);
    }
    return result;
}

void TLVList::addInt(int value)
{
    buffer.append((value >> 0) & 0xFF);
    buffer.append((value >> 8) & 0xFF);
}

QByteArray TLVList::getData() {
    return buffer;
}

