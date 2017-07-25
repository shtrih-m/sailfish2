#ifndef PRINTERPROTOCOL2_H
#define PRINTERPROTOCOL2_H

#include "fiscalprinter.h"
#include <QByteArray>

class ProtocolFrame2 {
private:
    int number;
    uint16_t Short(long value);
    uint16_t updateCRC(uint16_t CRC, uint16_t value);

public:
    ProtocolFrame2();
    int getNumber();
    void incNumber();
    void setNumber(int value);
    QByteArray encode(QByteArray data);
    QByteArray stuffing(QByteArray data);
    uint16_t getCRC(QByteArray data);
};

class PrinterProtocol2 : public PrinterProtocol {
private:
    QByteArray rx;
    PrinterPort* port;
    ProtocolFrame2 frame;
    bool isSynchronized;
    int maxRepeatCount;
    int syncTimeout;

    int readAnswer(bool sync);
    long readWord();
    uint8_t readByte();
    QByteArray readBytes(int count);
    void synchronizeFrames(int timeout);

public:
    PrinterProtocol2(PrinterPort* port);

    void connect();
    void disconnect();
    int send(PrinterCommand& command);
};

#endif // PRINTERPROTOCOL2_H
