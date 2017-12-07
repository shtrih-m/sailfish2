#ifndef PRINTERPROTOCOL1_H
#define PRINTERPROTOCOL1_H

#include "logger.h"
#include "fiscalprinter.h"

class Frame {
public:
    static char getCrc(QByteArray data)
    {
        char crc = (char)data.length();
        for (int i = 0; i < data.length(); i++) {
            crc ^= data.at(i);
        }
        return crc;
    }

    static QByteArray encode(QByteArray data)
    {
        QByteArray ba;
        ba.append(0x02);
        ba.append(data.length());
        ba.append(data);
        ba.append(getCrc(data));
        return ba;
    }
};

class PrinterProtocol1 : public PrinterProtocol {
public:
    PrinterProtocol1(PrinterPort* port, Logger* logger);

    int connect();
    int disconnect();
    int send(PrinterCommand& command);

private:
    Logger* logger;
    PrinterPort* port;

    int sendENQ();
    int byteTimeout;
    int readAnswer(int timeout, QByteArray& rx);
    int send(QByteArray& tx, int timeout, QByteArray& rx);
    int writeCommand(QByteArray tx);
};

#endif // PRINTERPROTOCOL1_H
