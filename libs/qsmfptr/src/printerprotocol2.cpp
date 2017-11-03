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

void PrinterProtocol2::connect()
{
    port->connectToDevice();
    synchronizeFrames(syncTimeout);
}

void PrinterProtocol2::disconnect()
{
    port->disconnect();
}

void PrinterProtocol2::synchronizeFrames(int timeout)
{
    if (isSynchronized)
        return;

    port->setReadTimeout(timeout);
    for (int i = 0; i < maxRepeatCount; i++)
    {
        try {
            QByteArray ba;
            ba = frame.encode(ba);
            port->writeBytes(ba);
            frame.setNumber(readAnswer(true));
            isSynchronized = true;
            frame.incNumber();
            break;
        } catch (...)
        {
            logger->write("Error synchronizing frames");
        }
    }
}

int PrinterProtocol2::send(PrinterCommand& command)
{
    int retryNum = 1;
    port->setReadTimeout(command.getTimeout());
    QByteArray ba = frame.encode(command.encode());
    port->writeBytes(ba);
    int frameNum = readAnswer(false);
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
int PrinterProtocol2::readAnswer(bool sync)
{
    int num = 0;
    while (true)
    {
        while (readByte() != STX) {
        }
        rx.clear();
        int len = readWord();

        if (len == 0x0100){
            num = readWord();
            if (sync) break;
        } else
        {
            if (len > 1) {
                num = readWord();
                rx.append(readBytes(len - 2));
            }
            uint16_t crc = readWord();

            MemStream stream;
            stream.writeShort(len);
            stream.writeShort(num);
            stream.writeBytes(rx);

            uint16_t frameCrc = frame.getCRC(stream.getBuffer());
            if (crc != frameCrc) {
                logger->write("Invalid CRC !!!");
                throw new TextException("Invalid CRC");
            }
            break;
        }
    }
    return num;
}

QByteArray PrinterProtocol2::readBytes(int count)
{
    QByteArray ba;
    for (int i = 0; i < count; i++) {
        ba.append(readByte());
    }
    return ba;
}

long PrinterProtocol2::readWord()
{
    uint8_t b1 = readByte();
    uint8_t b2 = readByte();
    long result = b1 + (b2 * 256);
    return result;
}

uint8_t PrinterProtocol2::readByte()
{
    uint8_t C = port->readByte();
    if (C == ESC) {
        C = port->readByte();
        if (C == TSTX) {
            C = STX;
        } else {
            if (C == TESC) {
                C = ESC;
            }
        }
    }
    return C;
}
