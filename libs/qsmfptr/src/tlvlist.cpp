#include "tlvlist.h"
#include <QString>
#include <QByteArray>
#include "fiscalprinter.h"

TLVList::TLVList()
{

}

QByteArray TLVList::to866(QString value)
{
    QTextCodec* codec = QTextCodec::codecForName("IBM 866");
    if (codec != nullptr)
    {
        return codec->fromUnicode(value);
    }
    return value.toLocal8Bit();
}

void TLVList::add(uint16_t tagId, QString tagValue)
{
    addInt(tagId);
    addInt(static_cast<uint16_t>(tagValue.length()));
    buffer.append(to866(tagValue));
}

void TLVList::add(uint16_t tagId, long tagValue, uint16_t len)
{
    addInt(tagId);
    addInt(len);
    buffer.append(intToTLV(tagValue, len));
}

QByteArray TLVList::intToTLV(long value, uint16_t len)
{
    if ((len > 8) || (len < 1)) {
        throw new TextException("Invalid length");
    }
    QByteArray result;
    for (int i = 0; i < len; i++)
    {
        uint8_t b = static_cast<uint8_t>((value >> (i * 8)) & 0xFF);
        result.append(b);
    }
    return result;
}

void TLVList::addInt(uint16_t value)
{
    buffer.append((value >> 0) & 0xFF);
    buffer.append((value >> 8) & 0xFF);
}

QByteArray TLVList::getData() {
    return buffer;
}

