#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include <QByteArray>
#include <QTcpSocket>


struct ServerParams{
    QString address;
    int port;
    int connectTimeout;
    int readTimeout;
    int writeTimeout;
    int connectRetries;
};

class ServerConnection
{
private:
    int sk;
    bool connected;
    QString errorText;
    ServerParams params;

    void waitRead();
    QString getErrorText(int code);
public:
    ServerConnection();

    bool connect();
    void disconnect();
    void clearError();
    QString getErrorText();
    bool error(QString text);
    void write(const QByteArray& data);
    void setParams(ServerParams params);
    bool read(int size, QByteArray& packet);
};

#endif // SERVERCONNECTION_H
