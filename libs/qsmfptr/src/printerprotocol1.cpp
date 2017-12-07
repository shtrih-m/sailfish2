
#include "printerprotocol1.h"
#include "shtrihfiscalprinter.h"
#include "utils.h"
#include "logger.h"
#include <QDebug>

const uint8_t STX = 0x02;
const uint8_t ENQ = 0x05;
const uint8_t ACK = 0x06;
const uint8_t NAK = 0x15;
// maximum counters
const int maxEnqNumber = 3;
const int maxNakCommandNumber = 3;
const int maxNakAnswerNumber = 3;
const int maxAckNumber = 3;
const int maxRepeatCount = 1;

PrinterProtocol1::PrinterProtocol1(PrinterPort* port, Logger* logger)
{
    this->port = port;
    this->logger = logger;
    byteTimeout = 1000;
}

int PrinterProtocol1::connect()
{
    int rc = port->connectToDevice();
    if (rc != 0) return rc;
    return sendENQ();
}

int PrinterProtocol1::sendENQ()
{
    int rc = 0;
    int ackNumber = 0;
    int enqNumber = 0;

    for (;;) {
            port->setReadTimeout(byteTimeout);
            port->writeByte(ENQ);

            uint8_t B;
            rc = port->readByte(B);
            if (rc != 0)
            {
                ackNumber++;
                continue;
            }

            QByteArray rx;
            switch (B) {
            case ACK:
                readAnswer(0, rx);
                ackNumber++;
                break;

            case NAK:
                return 0;

            default: {
                QSleepThread::msleep(100);
                enqNumber++;
            }
        }

        if (ackNumber >= maxAckNumber)
        {
            logger->write("No connection");
            return SMFPTR_E_NOCONNECTION;
        }

        if (enqNumber >= maxEnqNumber)
        {
            logger->write("No connection");
            return SMFPTR_E_NOCONNECTION;
        }
    }
}

int PrinterProtocol1::disconnect()
{
    return port->disconnect();
}

int PrinterProtocol1::send(PrinterCommand& command)
{
    int timeout = command.getTimeout();
    QByteArray tx;
    QByteArray rx;
    tx = Frame::encode(command.encode());
    int rc = send(tx, timeout, rx);
    if (rc != 0) return rc;
    return command.decode(rx);
}

int PrinterProtocol1::send(QByteArray& tx, int timeout, QByteArray& rx)
{
    int rc = 0;
    int ackNumber = 0;
    int enqNumber = 0;

    for (;;)
    {
            port->setReadTimeout(byteTimeout);
            rc = port->writeByte(ENQ);
            if (rc != 0){
                enqNumber++;
                continue;
            }

            uint8_t B;
            rc = port->readByte(B);
            if (rc != 0){
                enqNumber++;
                continue;
            }

            switch (B) {
            case ACK:
                rc = readAnswer(timeout, rx);
                ackNumber++;
                break;

            case NAK:
                rc = writeCommand(tx);
                if (rc != 0){
                    enqNumber++;
                    continue;
                }
                return readAnswer(timeout, rx);

            default:
                QSleepThread::msleep(100);
                enqNumber++;
            }
        if (ackNumber >= maxAckNumber)
        {
            logger->write("No connection");
            return SMFPTR_E_NOCONNECTION;
        }

        if (enqNumber >= maxEnqNumber) {
            logger->write("No connection");
            return SMFPTR_E_NOCONNECTION;
        }
    }
    return rc;
}

int PrinterProtocol1::writeCommand(QByteArray tx)
{
    int rc = 0;
    char nakCommandNumber = 0;
    while (true) {
        rc = port->writeBytes(tx);
        if (rc != 0) return rc;

        uint8_t B;
        rc = port->readByte(B);
        if (rc != 0) return rc;

        switch (B) {
        case ACK:
            return rc;
        case NAK:
            nakCommandNumber++;
            if (nakCommandNumber >= maxNakCommandNumber)
            {
                logger->write("nakCommandNumber >= maxNakCommandNumber");
                return SMFPTR_E_NOCONNECTION;
            }
        default:
            return rc;
        }
    }
}

int PrinterProtocol1::readAnswer(int timeout, QByteArray& rx)
{
    int rc = 0;
    int enqNumber = 0;
    for (;;) {
        port->setReadTimeout(timeout + byteTimeout);
        // STX
        uint8_t B = 0;
        while (B != STX)
        {
            rc = port->readByte(B);
            if (rc != 0) return rc;
        }
        // set byte timeout
        port->setReadTimeout(byteTimeout);
        // data length
        rc = port->readByte(B);
        if (rc != 0) return rc;
        int dataLength = B + 1;
        // command data
        rc = port->readBytes(dataLength, rx);
        if (rc != 0) return rc;
        // check CRC
        char crc = rx.at(rx.length() - 1);
        rx.chop(1);
        if (Frame::getCrc(rx) == crc) {
            port->writeByte(ACK);
            return rc;
        } else {
            rc = port->writeByte(NAK);
            if (rc != 0) return rc;

            rc = port->writeByte(ENQ);
            if (rc != 0) return rc;

            rc = port->readByte(B);
            if (rc != 0) return rc;

            switch (B) {
            case ACK:
                break;
            case NAK:
                return rc;
            default:
                enqNumber++;
                if (enqNumber >= maxEnqNumber)
                {
                    logger->write("readAnswerError");
                    return SMFPTR_E_NOCONNECTION;
                }
            }
        }
    }
}
