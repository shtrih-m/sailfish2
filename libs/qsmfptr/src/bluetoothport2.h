#ifndef BLUETOOTHPORT2_H
#define BLUETOOTHPORT2_H

#include "lib/bluetooth.h"
#include "logger.h"
#include "fiscalprinter.h"

class BluetoothPort2 : public QObject, public PrinterPort {
    Q_OBJECT
public:
    BluetoothPort2(Logger* logger);

    bool connectToDevice();
    void disconnect();
    uint8_t readByte();
    void setReadTimeout(int value);
    void setWriteTimeout(int value);
    void setConnectTimeout(int value);
    void setConnectRetries(int value);
    QByteArray readBytes(int count);
    void writeByte(char data);
    void writeBytes(const QByteArray& data);
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
