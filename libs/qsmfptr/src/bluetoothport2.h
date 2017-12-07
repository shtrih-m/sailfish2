#ifndef BLUETOOTHPORT2_H
#define BLUETOOTHPORT2_H

#include "lib/bluetooth.h"
#include "logger.h"
#include "fiscalprinter.h"

class BluetoothPort2 : public QObject, public PrinterPort {
    Q_OBJECT
public:
    BluetoothPort2(Logger* logger);

    int connectToDevice();
    int disconnect();
    int writeByte(char data);
    int readByte(uint8_t& data);
    int writeBytes(const QByteArray& data);
    int readBytes(int count, QByteArray& packet);

    void setReadTimeout(int value);
    void setWriteTimeout(int value);
    void setConnectTimeout(int value);
    void setConnectRetries(int value);
    void setAddress(QString value);
    QString findDevice();
private:
    Logger* logger;
    bool isConnected;
    QString address;
    int readTimeout;
    int writeTimeout;
    int connectTimeout;
    int connectRetries;
    int sk;
    void waitRead();
    int bachk(const char* str);
    int str2ba(const char* str, bdaddr_t* ba);
};

#endif // BLUETOOTHPORT2_H
