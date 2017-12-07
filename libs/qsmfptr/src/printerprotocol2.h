#ifndef PRINTERPROTOCOL2_H
#define PRINTERPROTOCOL2_H

#include <QByteArray>
#include "fiscalprinter.h"
#include "logger.h"

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
    Logger* logger;
    QByteArray rx;
    PrinterPort* port;
    ProtocolFrame2 frame;
    bool isSynchronized;
    int maxRepeatCount;
    int syncTimeout;

    int readByte(uint8_t& C);
    int readWord(uint16_t& V);
    int readAnswer(bool sync, uint16_t& num);
    int readBytes(int count, QByteArray& rx);
    int synchronizeFrames(int timeout);

public:
    PrinterProtocol2(PrinterPort* port, Logger* logger);

    int connect();
    int disconnect();
    int send(PrinterCommand& command);
};

#endif // PRINTERPROTOCOL2_H
