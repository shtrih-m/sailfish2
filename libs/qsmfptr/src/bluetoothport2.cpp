
#include "lib/bluetooth.h"
#include "lib/rfcomm.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include <QBluetoothSocket>
#include <QBluetoothUuid>
#include <QElapsedTimer>
#include <QObject>

#include "utils.h"
#include "logger.h"
#include "bluetoothport2.h"

static inline void convertAddress(quint64 from, quint8 (&to)[6])
{
    to[0] = (from >> 0) & 0xff;
    to[1] = (from >> 8) & 0xff;
    to[2] = (from >> 16) & 0xff;
    to[3] = (from >> 24) & 0xff;
    to[4] = (from >> 32) & 0xff;
    to[5] = (from >> 40) & 0xff;
}

BluetoothPort2::BluetoothPort2(Logger* logger)
{
    this->logger = logger;
    address = "";
    isConnected = false;
    readTimeout = 1000;
    writeTimeout = 1000;
    connectRetries = 3;
    connectTimeout = 3000;
    sk = 0;
}

void BluetoothPort2::setAddress(QString value)
{
    address = value;
}

int BluetoothPort2::connectToDevice()
{

    int rc = 0;
    if (isConnected)
        return rc;

    logger->write("connectToDevice");
    struct sockaddr_rc raddr;
    raddr.rc_family = AF_BLUETOOTH;
    std::string saddress = address.toStdString();
    str2ba(saddress.c_str(), &raddr.rc_bdaddr);
    raddr.rc_channel = 1;

    for (int i = 0; i < connectRetries; i++) {
        if (i != 0) {
            logger->write(QString("Connecting RFCOMM socket, retry %1").arg(i));
        }

        sk = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
        if (sk < 0) {
            logger->write("Failed to create RFCOMM socket");
            return SMFPTR_E_NOCONNECTION;
        } else{
            logger->write("RFCOMM socket created.");
        }

        ::fcntl(sk, F_SETFL, O_NONBLOCK);

        if (::connect(sk, (struct sockaddr*)&raddr, sizeof(raddr)) >= 0) {
            logger->write("Connected to RFCOMM socket!");
            isConnected = true;
            return rc;
        }

        fd_set fdset;
        struct timeval tv;
        FD_ZERO(&fdset);
        FD_SET(sk, &fdset);
        tv.tv_sec = 0;
        tv.tv_usec = connectTimeout * 1000;
        if (select(sk + 1, NULL, &fdset, NULL, &tv) == 1) {
            int so_error;
            socklen_t len = sizeof so_error;
            getsockopt(sk, SOL_SOCKET, SO_ERROR, &so_error, &len);
            if (so_error == 0) {
                logger->write("Connected to RFCOMM socket!");
                isConnected = true;
                return rc;
            } else{
                logger->write("Select failed");
            }
        }
        logger->write("Failed to connect RFCOMM socket");
        close(sk);
    }
    return rc;
}

int BluetoothPort2::disconnect()
{
    int rc = 0;
    logger->write("disconnect");

    if (!isConnected)
        return rc;
    close(sk);
    isConnected = false;
    return rc;
}

void BluetoothPort2::setReadTimeout(int value)
{
    readTimeout = value;
}

void BluetoothPort2::setWriteTimeout(int value)
{
    writeTimeout = value;
}

void BluetoothPort2::setConnectTimeout(int value)
{
    connectTimeout = value;
}

void BluetoothPort2::setConnectRetries(int value)
{
    connectRetries = value;
}


int BluetoothPort2::readByte(uint8_t& data)
{
    QByteArray packet;
    int rc = readBytes(1, packet);
    if (rc == 0){
        data = packet[0];
    }
    return rc;
}

int BluetoothPort2::readBytes(int count, QByteArray& packet)
{
    int rc = connectToDevice();
    if (rc != 0) return rc;

    char buf[count];
    while (true)
    {
        waitRead();
        int toread = count - packet.length();
        int n = read(sk, &buf, toread);
        if (n < 0)
        {
            disconnect();
            logger->write(ErrorText::getErrorText(errno));
            return SMFPTR_TIMEOUT_ERROR;
        }
        packet.append(buf, n);
        if (packet.length() >= count){
            break;
        }
    }
    return SMFPTR_OK;
}

void BluetoothPort2::waitRead()
{
    fd_set fdset;
    struct timeval tv;
    FD_ZERO(&fdset);
    FD_SET(sk, &fdset);
    tv.tv_sec = 0;
    tv.tv_usec = readTimeout * 1000;
    if (select(sk + 1, &fdset, NULL, NULL, &tv) == 1) {
        int so_error;
        socklen_t len = sizeof so_error;
        getsockopt(sk, SOL_SOCKET, SO_ERROR, &so_error, &len);
        if (so_error != 0) {
            logger->write(QString("ERROR: %1").arg(so_error));
        }
    }
}

int BluetoothPort2::writeByte(char data)
{
    QByteArray ba;
    ba.append(data);
    return writeBytes(ba);
}

int BluetoothPort2::writeBytes(const QByteArray& data)
{
    int rc = connectToDevice();
    if (rc != 0) return rc;

    rc = write(sk, data.data(), data.length());
    if (rc < 0){
        disconnect();
        logger->write("ERROR writing to socket");
        return SMFPTR_E_NOCONNECTION;
    }
    return SMFPTR_OK;
}

QString BluetoothPort2::findDevice()
{
    return "";
}

int BluetoothPort2::bachk(const char* str)
{
    if (!str) {
        logger->write("!str");
        return -1;
    }

    if (strlen(str) != 17) {
        logger->write("strlen(str) != 17");
        return -1;
    }

    while (*str) {
        if (!isxdigit(*str++)) {
            logger->write("!isxdigit(*str++).1");
            return -1;
        }

        if (!isxdigit(*str++)) {
            logger->write("!isxdigit(*str++).2");
            return -1;
        }

        if (*str == 0)
            break;

        if (*str++ != ':') {
            logger->write("*str++ != ':'");
            return -1;
        }
    }

    return 0;
}

int BluetoothPort2::str2ba(const char* str, bdaddr_t* ba)
{
    if (bachk(str) < 0) {
        memset(ba, 0, sizeof(*ba));
        return -1;
    }

    for (int i = 5; i >= 0; i--, str += 3)
        ba->b[i] = strtol(str, NULL, 16);

    return 0;
}


