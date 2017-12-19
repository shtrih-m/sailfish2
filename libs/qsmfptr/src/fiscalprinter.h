#ifndef FISCALPRINTER_H
#define FISCALPRINTER_H

#include "printercommand.h"
#include <QByteArray>
#include <QException>
#include <QObject>
#include <QString>

const int SMFPTR_E_NOCONNECTION     = -1;
const int SMFPTR_E_LARGEWIDTH       = -2;
const int SMFPTR_E_LARGEHEIGHT      = -3;
const int SMFPTR_E_NOGRAPHICS       = -4;
const int SMFPTR_E_INVALID_PARAM_ID = -5;



class TextException : public QException {
public:
    explicit TextException(QString text)
    {
        this->text = text;
    }

    virtual ~TextException() throw()
    {
    }

    QString getText() const
    {
        return text;
    }

protected:
    QString text;
};

class PortException : public TextException {
public:
    explicit PortException(QString text)
        : TextException(text)
    {
    }
};

class DeviceException : public TextException {
private:
    int code;

public:
    explicit DeviceException(int code, QString text)
        : TextException(text)
    {
        this->code = code;
    }

    ~DeviceException() throw()
    {
    }

    int getCode()
    {
        return code;
    }
};

class PrinterPort {
public:
    virtual int connectToDevice() = 0;
    virtual int disconnect() = 0;
    virtual int readByte(uint8_t& data) = 0;
    virtual int readBytes(int count, QByteArray& data) = 0;
    virtual int writeByte(char data) = 0;
    virtual int writeBytes(const QByteArray& data) = 0;
    virtual void setReadTimeout(int value) = 0;
    virtual void setWriteTimeout(int value) = 0;
    virtual ~PrinterPort() = default;
};

class PrinterProtocol {
public:
    virtual int connect() = 0;
    virtual int disconnect() = 0;
    virtual int send(PrinterCommand& command) = 0;
    virtual ~PrinterProtocol() = default;
};

#endif // FISCALPRINTER_H
