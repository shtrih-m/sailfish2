#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include <QByteArray>
#include <QTcpSocket>
#include "logger.h"

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
    Logger* logger;
    bool connected;
    QString errorText;
    ServerParams params;

    void waitRead();
    QString getErrorText(int code);
public:
    ServerConnection(Logger* logger);

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
