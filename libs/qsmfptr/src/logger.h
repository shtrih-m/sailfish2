#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QDateTime>
#include <QTextStream>

class Logger
{
#include <QTextStream>
public:
    Logger(QString fileName);
    ~Logger();

    bool isOpened();
    bool openFile();
    void closeFile();
    void writeTx(QByteArray ba);
    void writeRx(QByteArray ba);
    void write(const QString &value);
    void write(QString prefix, QByteArray ba);
    void setShowDateTime(bool value);
private:
    QFile *file;
    QString fileName;
    bool m_showDate;
};

#endif // LOGGER_H
