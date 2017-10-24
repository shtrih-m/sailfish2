#ifndef FISCALPRINTER_H
#define FISCALPRINTER_H

#include "printercommand.h"
#include <QByteArray>
#include <QException>
#include <QObject>
#include <QString>

const int SMFPTR_E_NOCONNECTION = -1;
const int SMFPTR_E_LARGEWIDTH = -2;
const int SMFPTR_E_LARGEHEIGHT = -3;
const int SMFPTR_E_NOGRAPHICS = -4;

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
    virtual bool connectToDevice() = 0;
    virtual void disconnect() = 0;
    virtual uint8_t readByte() = 0;
    virtual QByteArray readBytes(int count) = 0;
    virtual void writeByte(char data) = 0;
    virtual void writeBytes(const QByteArray& data) = 0;
    virtual void setReadTimeout(int value) = 0;
    virtual void setWriteTimeout(int value) = 0;
    virtual ~PrinterPort() = default;
};

class PrinterProtocol {
public:
    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual int send(PrinterCommand& command) = 0;
    virtual ~PrinterProtocol() = default;
};

#endif // FISCALPRINTER_H
