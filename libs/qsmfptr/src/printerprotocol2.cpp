#include "printerprotocol2.h"

#include <QDebug>
#include <QDataStream>

#include "utils.h"
#include "logger.h"
#include "memstream.h"


const uint8_t STX = 0x8F;
const uint8_t ESC = 0x9F;
const uint8_t TSTX = 0x81;
const uint8_t TESC = 0x83;

ProtocolFrame2::ProtocolFrame2()
{
    number = 0;
}

int ProtocolFrame2::getNumber()
{
    return number;
}

void ProtocolFrame2::setNumber(int value)
{
    number = value;
}

void ProtocolFrame2::incNumber()
{
    if (number == 0xFFFF) {
        number = 0;
    } else {
        number++;
    }
}

QByteArray ProtocolFrame2::stuffing(QByteArray bytes)
{
    QByteArray ba;
    for (int i = 0; i < bytes.size(); i++) {
        uint16_t c = bytes.at(i);
        if (c == STX) {
            ba.append(ESC);
            ba.append(TSTX);
        } else {
            if (c == ESC) {
                ba.append(ESC);
                ba.append(TESC);
            } else {
                ba.append(c);
            }
        }
    }
    return ba;
}

QByteArray ProtocolFrame2::encode(QByteArray data)
{
    MemStream stream;
    if (data.size() == 0) {
        stream.writeShort(0);
    } else {
        stream.writeShort(data.size() + 2);
        stream.writeShort(number);
        stream.writeBytes(data);
    }
    stream.writeShort(getCRC(stream.getBuffer()));
    stream.setBuffer(stuffing(stream.getBuffer()));
    stream.insert(0, STX);
    return stream.getBuffer();
}

uint16_t ProtocolFrame2::Short(long value)
{
    return value & 0xFFFF;
}

uint16_t ProtocolFrame2::updateCRC(uint16_t CRC, uint16_t value)
{
    long result = Short(((CRC >> 8) | (Short(CRC << 8))));
    result = Short(result ^ (Short(value)));
    result = Short(result ^ Short((result & 0x00FF) >> 4));
    result = Short(result ^ (Short(result << 12)));
    result = Short(result ^ (Short((result & 0x00FF) << 5)));
    return result;
}

uint16_t ProtocolFrame2::getCRC(QByteArray data)
{
    uint16_t result = 0xFFFF;
    for (int i = 0; i < data.size(); i++) {
        result = updateCRC(result, (uint8_t)data.at(i));
    }
    return result;
}

PrinterProtocol2::PrinterProtocol2(PrinterPort* port, Logger* logger)
{
    this->port = port;
    this->logger = logger;

    maxRepeatCount = 3;
    syncTimeout = 1000;
    isSynchronized = false;
}

int PrinterProtocol2::connect()
{
    int rc = port->connectToDevice();
    if (rc != 0) return rc;
    return synchronizeFrames(syncTimeout);
}

int PrinterProtocol2::disconnect()
{
    return port->disconnect();
}

int PrinterProtocol2::synchronizeFrames(int timeout)
{
    int rc = 0;
    if (isSynchronized)
        return rc;

    port->setReadTimeout(timeout);
    for (int i = 0; i < maxRepeatCount; i++)
    {
        QByteArray ba;
        ba = frame.encode(ba);
        rc = port->writeBytes(ba);
        if (rc != 0) continue;

        uint16_t num;
        rc = readAnswer(true, num);
        if (rc != 0) continue;

        frame.setNumber(num);
        isSynchronized = true;
        frame.incNumber();
        break;
    }
    return rc;
}

int PrinterProtocol2::send(PrinterCommand& command)
{
    int rc = 0;
    int retryNum = 1;
    port->setReadTimeout(command.getTimeout());
    QByteArray ba = frame.encode(command.encode());
    port->writeBytes(ba);
    uint16_t frameNum;
    rc = readAnswer(false, frameNum);
    if (rc != 0) return rc;

    if (frameNum != frame.getNumber()) {
        if ((retryNum != 1) && (frameNum == (frame.getNumber() - 1))) {
            frame.setNumber(frameNum);
            frame.incNumber();
            command.setBuffer(rx);
            return true;
        } else {
            frame.setNumber(frameNum);
            frame.incNumber();
            return false;
        }
    }
    frame.incNumber();
    return command.decode(rx);
}

// 8F 00 01 09 00
int PrinterProtocol2::readAnswer(bool sync, uint16_t& num)
{
    num = 0;
    int rc = 0;
    while (true)
    {
        uint8_t C = 0;
        while (C != STX)
        {
            rc = readByte(C);
            if (rc != 0) return rc;
        }
        rx.clear();
        uint16_t len;
        rc = readWord(len);
        if (rc != 0) return rc;

        if (len == 0x0100)
        {
            rc = readWord(num);
            if (rc != 0) return rc;

            if (sync) break;
        } else
        {
            if (len > 1) {
                rc = readWord(num);
                if (rc != 0) return rc;

                QByteArray ba;
                rc = readBytes(len - 2, ba);
                if (rc != 0) return rc;
                rx.append(ba);
            }
            uint16_t crc;
            rc = readWord(crc);
            if (rc != 0) return rc;

            MemStream stream;
            stream.writeShort(len);
            stream.writeShort(num);
            stream.writeBytes(rx);

            uint16_t frameCrc = frame.getCRC(stream.getBuffer());
            if (crc != frameCrc) {
                logger->write("Invalid CRC.");
                return SMFPTR_INVALID_CRC;
            }
            break;
        }
    }
    return rc;
}

int PrinterProtocol2::readBytes(int count, QByteArray& rx)
{
    uint8_t B;
    int rc = 0;
    for (int i = 0; i < count; i++)
    {
        rc = readByte(B);
        if (rc != 0) return rc;
        rx.append(B);
    }
    return rc;
}

int PrinterProtocol2::readWord(uint16_t& V)
{
    int rc = 0;
    uint8_t b1;
    rc = readByte(b1);
    if (rc != 0) return rc;

    uint8_t b2;
    rc = readByte(b2);
    if (rc != 0) return rc;

    V = b1 + (b2 * 256);
    return rc;
}

int PrinterProtocol2::readByte(uint8_t& C)
{
    int rc = port->readByte(C);
    if (rc != 0) return rc;

    if (C == ESC)
    {
        rc = port->readByte(C);
        if (rc != 0) return rc;

        if (C == TSTX) {
            C = STX;
        } else {
            if (C == TESC) {
                C = ESC;
            }
        }
    }
    return rc;
}
