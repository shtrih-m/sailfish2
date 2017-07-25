#ifndef BLUETOOTHPORT_H
#define BLUETOOTHPORT_H

#include "fiscalprinter.h"
#include <QBluetoothServiceInfo>
#include <QBluetoothSocket>

class BluetoothPort : public QObject, public PrinterPort {
    Q_OBJECT
public:
    explicit BluetoothPort(QObject* parent = 0);

    bool connectToDevice();
    void disconnect();
    uint8_t readByte();
    void setReadTimeout(int value);
    void setWriteTimeout(int value);
    void setConnectTimeout(int value);
    QByteArray readBytes(int count);
    void writeByte(char data);
    void writeBytes(const QByteArray& data);
    void setAddress(QString value);

private:
    bool connected;
    QString address;
    int readTimeout;
    int writeTimeout;
    int connectTimeout;
    QBluetoothSocket* socket;
public slots:
    void socketConnected();
    void socketDisconnected();
    void socketError(QBluetoothSocket::SocketError error);
    void socketStateChanged(QBluetoothSocket::SocketState state);
};

#endif // BLUETOOTHPORT_H
