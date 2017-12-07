#ifndef BLUETOOTHPORT_H
#define BLUETOOTHPORT_H

#include "fiscalprinter.h"
#include <QBluetoothServiceInfo>
#include <QBluetoothSocket>
#include "logger.h"

class BluetoothPort : public QObject, public PrinterPort {
    Q_OBJECT
public:
    explicit BluetoothPort(QObject* parent, Logger* logger);

    int connectToDevice();
    int disconnect();
    int writeByte(char data);
    int readByte(uint8_t& data);
    int writeBytes(const QByteArray& data);
    int readBytes(int count, QByteArray& packet);
    void setReadTimeout(int value);
    void setWriteTimeout(int value);
    void setConnectTimeout(int value);
    void setAddress(QString value);
private:
    Logger* logger;
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
