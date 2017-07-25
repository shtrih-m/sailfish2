#ifndef PRINTERPROTOCOL1_H
#define PRINTERPROTOCOL1_H

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
    PrinterProtocol1(PrinterPort* port);

    void connect();
    void disconnect();
    int send(PrinterCommand& command);

private:
    PrinterPort* port;
    void sendENQ();
    int byteTimeout;
    QByteArray readAnswer(int timeout);
    QByteArray send(QByteArray& data, int timeout);
    void writeCommand(QByteArray data);
};

#endif // PRINTERPROTOCOL1_H
