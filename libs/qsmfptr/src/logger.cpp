#include "logger.h"
#include "utils.h"
#include <QDebug>

Logger::Logger(QString fileName)
{
    file = NULL;
    m_showDate = true;
    this->fileName = fileName;
}

Logger::~Logger()
{
    closeFile();
}

bool Logger::openFile()
{
    if ((file == 0)&&(!fileName.isEmpty()))
    {
        file = new QFile;
        file->setFileName(fileName);
        file->open(QIODevice::Append | QIODevice::Text);
    }
    return isOpened();
}

bool Logger::isOpened(){
    return file != 0;
}

void Logger::closeFile()
{
    if (file){
        file->close();
        delete file;
        file = 0;
    }
}

void Logger::write(const QString &value)
{
    qDebug() << value;

    if (!openFile()) return;
    QString text = value;// + "";
    if (m_showDate)
        text = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss ") + text + "\r\n";
    QTextStream out(file);
    out.setCodec("UTF-8");
    if (file != 0) {
        out << text;
    }
}

void Logger::writeTx(QByteArray ba){
    write("-> ", ba);
}

void Logger::writeRx(QByteArray ba){
    write("<- ", ba);
}

void Logger::write(QString prefix, QByteArray ba)
{
    int blockSize = 20;
    int count = (ba.size()+blockSize-1)/blockSize;
    for (int i=0;i<count;i++)
    {
        int len = std::min(blockSize, ba.size()-i*blockSize);
        QByteArray block = ba.mid(i*blockSize, len);
        write(prefix + StringUtils::dataToHex(block));
    }
}

void Logger::setShowDateTime(bool value) {
    m_showDate = value;
}
