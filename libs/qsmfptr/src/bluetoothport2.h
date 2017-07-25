#ifndef BLUETOOTHPORT2_H
#define BLUETOOTHPORT2_H

#include "fiscalprinter.h"

class BluetoothPort2 : public QObject, public PrinterPort {
    Q_OBJECT
public:
    explicit BluetoothPort2(QObject* parent = 0);

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
    bool isConnected;
    QString address;
    int readTimeout;
    int writeTimeout;
    int connectTimeout;
    int connectRetries;
    int sk;
    void waitRead();
};

#endif // BLUETOOTHPORT2_H
