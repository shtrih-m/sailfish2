#include "serverconnection.h"

#include <QByteArray>
#include <QDebug>
#include <QElapsedTimer>
#include <QThread>
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include "utils.h"
#include "fiscalprinter.h"

#include <fcntl.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <logger.h>

ServerConnection::ServerConnection(Logger* logger)
{
    connected = false;
    this->logger = logger;
}

void ServerConnection::setParams(ServerParams params)
{
    this->params = params;
}

void ServerConnection::disconnect()
{
    if (!connected) return;

    logger->write("disconnect");
    shutdown(sk, SHUT_RDWR);
    close(sk);
    connected = false;
    logger->write("disconnect: OK");
}

QString ServerConnection::getErrorText(){
    return errorText;
}

void ServerConnection::clearError(){
    errorText = "No errors";
}

bool ServerConnection::error(QString text){
    errorText = text;
    return false;
}

bool ServerConnection::connect()
{
    clearError();
    if (connected) return true;
    logger->write("connect");

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    std::string address = params.address.toStdString();
    addr.sin_addr.s_addr = inet_addr(address.c_str());
    addr.sin_port = htons(params.port);

    for (int i = 0; i < params.connectRetries; i++) {
        if (i != 0) {
            logger->write("Connecting, retry " + i);
        }

        sk = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sk < 0) {
            return error("Failed to create socket");
        }

        ::fcntl(sk, F_SETFL, O_NONBLOCK);

        if (::connect(sk, (struct sockaddr*)&addr, sizeof(addr)) >= 0) {
            logger->write("Connected to socket!");
            connected = true;
            return true;
        }

        fd_set fdset;
        struct timeval tv;
        FD_ZERO(&fdset);
        FD_SET(sk, &fdset);
        tv.tv_sec = 0;
        tv.tv_usec = params.connectTimeout * 1000;
        if (select(sk + 1, NULL, &fdset, NULL, &tv) == 1) {
            int so_error;
            socklen_t len = sizeof so_error;
            getsockopt(sk, SOL_SOCKET, SO_ERROR, &so_error, &len);
            if (so_error == 0) {
                logger->write("Connected to socket!");
                connected = true;
                return true;
            }
        }
        close(sk);
    }
    return error("Failed to create socket");
}

bool ServerConnection::read(int size, QByteArray& packet)
{
    QString text;
    logger->write(text.sprintf("read(%d)", size));

    connect();

    QElapsedTimer timer;
    timer.start();
    char buf[size];
    while (true) {
        waitRead();
        int toread = size - packet.length();
        int n = ::read(sk, &buf, toread);
        if (n < 0)
        {
            if (errno != EAGAIN)
            {
                return error("ERROR reading from socket, " + getErrorText(errno));
            }
        }
        packet.append(buf, n);
        if (packet.length() >= size)
            break;
        if (timer.elapsed() > params.readTimeout)
        {
            return error("Timeout error");
        }
    }
    return true;
}

void ServerConnection::waitRead()
{
    fd_set fdset;
    struct timeval tv;
    FD_ZERO(&fdset);
    FD_SET(sk, &fdset);
    tv.tv_sec = 0;
    tv.tv_usec = params.readTimeout * 1000;
    if (select(sk + 1, &fdset, NULL, NULL, &tv) == 1) {
        int so_error;
        socklen_t len = sizeof so_error;
        getsockopt(sk, SOL_SOCKET, SO_ERROR, &so_error, &len);
        if (so_error != 0)
        {
            QString text;
            logger->write(text.sprintf("ERROR: %d, ", so_error) + getErrorText(so_error));
        }
    }
}

void ServerConnection::write(const QByteArray& data)
{
    logger->write("-> " + StringUtils::dataToHex(data));
    if (data.length() == 0) return;

    connect();
    int rc = ::write(sk, data.data(), data.length());
    if (rc < 0)
    {
        QString text;
        logger->write(text.sprintf("ERROR writing to socket: %d, ", rc) + getErrorText(rc));
    }
}

QString ServerConnection::getErrorText(int code){
    switch (code)
    {
        case EPERM: return "EPERM, Operation not permitted";
        case ENOENT: return "ENOENT, No such file or directory";
        case ESRCH: return "ESRCH, No such process";
        case EINTR: return "EINTR, Interrupted system call";
        case EIO: return "EIO, I/O error";
        case ENXIO: return "ENXIO, No such device or address";
        case E2BIG: return "E2BIG, Argument list too long";
        case ENOEXEC: return "ENOEXEC, Exec format error";
        case EBADF: return "EBADF, Bad file number";
        case ECHILD: return "ECHILD, No child processes";
        case EAGAIN: return "EAGAIN, Try again";
        case ENOMEM: return "ENOMEM, Out of memory";
        case EACCES: return "EACCES, Permission denied";
        case EFAULT: return "EFAULT, Bad address";
        case ENOTBLK: return "ENOTBLK, Block device required";
        case EBUSY: return "EBUSY, Device or resource busy";
        case EEXIST: return "EEXIST, File exists";
        case EXDEV: return "EXDEV, Cross-device link";
        case ENODEV: return "ENODEV, No such device";
        case ENOTDIR: return "ENOTDIR, Not a directory";
        case EISDIR: return "EISDIR, Is a directory";
        case EINVAL: return "EINVAL, Invalid argument";
        case ENFILE: return "ENFILE, File table overflow";
        case EMFILE: return "EMFILE, Too many open files";
        case ENOTTY: return "ENOTTY, Not a typewriter";
        case ETXTBSY: return "ETXTBSY, Text file busy";
        case EFBIG: return "EFBIG, File too large";
        case ENOSPC: return "ENOSPC, No space left on device";
        case ESPIPE: return "ESPIPE, Illegal seek";
        case EROFS: return "EROFS, Read-only file system";
        case EMLINK: return "EMLINK, Too many links";
        case EPIPE: return "EPIPE, Broken pipe";
        case EDOM: return "EDOM, Math argument out of domain of func";
        case ERANGE: return "ERANGE, Math result not representable";
    }
    return "Unknown";
}


