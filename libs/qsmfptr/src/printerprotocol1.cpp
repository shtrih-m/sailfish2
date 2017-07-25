
#include "printerprotocol1.h"
#include "ShtrihFiscalPrinter.h"
#include "utils.h"
#include <QDebug>

const char STX = 0x02;
const char ENQ = 0x05;
const char ACK = 0x06;
const char NAK = 0x15;
// maximum counters
const int maxEnqNumber = 3;
const int maxNakCommandNumber = 3;
const int maxNakAnswerNumber = 3;
const int maxAckNumber = 3;
const int maxRepeatCount = 1;

PrinterProtocol1::PrinterProtocol1(PrinterPort* port)
{
    this->port = port;
    byteTimeout = 1000;
}

void PrinterProtocol1::connect()
{
    port->connectToDevice();
    sendENQ();
}

void PrinterProtocol1::sendENQ()
{
    int ackNumber = 0;
    int enqNumber = 0;

    for (;;) {
        try {
            port->setReadTimeout(byteTimeout);
            port->writeByte(ENQ);

            char B = port->readByte();
            switch (B) {
            case ACK:
                readAnswer(0);
                ackNumber++;
                break;

            case NAK:
                return;

            default: {
                QSleepThread::msleep(100);
                enqNumber++;
            }
            }

        } catch (PortException e) {
            enqNumber++;
        }
        if (ackNumber >= maxAckNumber) {
            throw new DeviceException(SMFPTR_E_NOCONNECTION, "No connection");
        }

        if (enqNumber >= maxEnqNumber) {
            throw new DeviceException(SMFPTR_E_NOCONNECTION, "No connection");
        }
    }
}

void PrinterProtocol1::disconnect()
{
    port->disconnect();
}

int PrinterProtocol1::send(PrinterCommand& command)
{
    int timeout = command.getTimeout();
    QByteArray tx;
    tx = Frame::encode(command.encode());
    QByteArray rx = send(tx, timeout);
    return command.decode(rx);
}

QByteArray PrinterProtocol1::send(QByteArray& data, int timeout)
{
    int ackNumber = 0;
    int enqNumber = 0;

    for (;;) {
        try {
            port->setReadTimeout(byteTimeout);
            port->writeByte(ENQ);

            int B = port->readByte();
            switch (B) {
            case ACK:
                readAnswer(timeout);
                ackNumber++;
                break;

            case NAK:
                writeCommand(data);
                return readAnswer(timeout);

            default:
                QSleepThread::msleep(100);
                enqNumber++;
            }
        } catch (QException e) {
            enqNumber++;
        }
        if (ackNumber >= maxAckNumber) {
            throw new DeviceException(SMFPTR_E_NOCONNECTION, "No connection");
        }

        if (enqNumber >= maxEnqNumber) {
            throw new DeviceException(SMFPTR_E_NOCONNECTION, "No connection");
        }
    }
}

void PrinterProtocol1::writeCommand(QByteArray data)
{
    char nakCommandNumber = 0;
    while (true) {
        port->writeBytes(data);
        switch (port->readByte()) {
        case ACK:
            return;
        case NAK:
            nakCommandNumber++;
            if (nakCommandNumber >= maxNakCommandNumber) {
                throw new DeviceException(SMFPTR_E_NOCONNECTION,
                    "nakCommandNumber >= maxNakCommandNumber");
            }
        default:
            return;
        }
    }
}

QByteArray PrinterProtocol1::readAnswer(int timeout)
{
    int enqNumber = 0;
    for (;;) {
        port->setReadTimeout(timeout + byteTimeout);
        // STX
        while (port->readByte() != STX) {
        }
        // set byte timeout
        port->setReadTimeout(byteTimeout);
        // data length
        int dataLength = port->readByte() + 1;
        // command data
        QByteArray commandData = port->readBytes(dataLength);
        // check CRC
        char crc = commandData.at(commandData.length() - 1);
        commandData.chop(1);
        if (Frame::getCrc(commandData) == crc) {
            port->writeByte(ACK);
            return commandData;
        } else {
            port->writeByte(NAK);
            port->writeByte(ENQ);
            int B = port->readByte();
            switch (B) {
            case ACK:
                break;
            case NAK:
                return commandData;
            default:
                enqNumber++;
                if (enqNumber >= maxEnqNumber) {
                    throw new TextException("readAnswerError");
                }
            }
        }
    }
}
