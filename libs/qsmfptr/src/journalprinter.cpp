#include "journalprinter.h"

#include <QFile>
#include <QDebug>
#include <QTextStream>
#include "fiscalprinter.h"

#define SZReportText "СУТОЧНЫЙ ОТЧЕТ С ГАШЕНИЕМ"

JournalPrinter::JournalPrinter(QString afileName)
{
    fileName = afileName;
}

QString JournalPrinter::getFileName(){
    return fileName;
}

void JournalPrinter::setFileName(QString value){
    fileName = value;
}

void JournalPrinter::reset(){
    lines.clear();
}

void JournalPrinter::deleteFile(){
    QFile file(fileName);
    file.remove();
}

QStringList JournalPrinter::readDay(int dayNumber)
{
    qDebug() << "readDay(" << dayNumber << ")";

    QStringList dst = searchZReport(dayNumber);
    QString header;
    header.sprintf("Контрольная лента Смена № %d", dayNumber);
    dst.insert(0, header);
    return dst;
}

QStringList JournalPrinter::readDoc(int docNumber)
{
    qDebug() << "readDoc(" << docNumber << ")";

    QStringList dst = getDocument(docNumber);
    QString header;
    header.sprintf("Контрольная лента Документ № %d", docNumber);
    dst.insert(0, header);
    return dst;
}

QStringList JournalPrinter::readDocRange(int N1, int N2)
{
    qDebug() << "readDocRange(" << N1 << ", " << N2 << ")";

    if (N1 > N2)
    {
        QString text;
        text.sprintf("Номер первого документа больше второго (%d > %d)", N1, N2);
        qDebug() << text;

        throw new TextException(text);
    }

    if (N1 == N2) {
        return readDoc(N1);
    }

    QStringList dst;
    QString header;
    header.sprintf("Контрольная лента Документ с № %d по № %d", N1, N2);
    dst.insert(0, header);

    for (int i = N1; i <= N2; i++)
    {
        QStringList document = getDocument(i);
        dst.append(document);
    }
    return dst;
}

QStringList JournalPrinter::readAll()
{
    qDebug() << "readAll()" << lines.size();
    if (lines.size() == 0)
    {
        QFile file(fileName);
        if (file.exists()){
            if (file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                file.seek(0);
                QTextStream stream(&file);
                stream.setCodec("UTF-8");
                while (!stream.atEnd())
                {
                    QString line = stream.readLine();
                    qDebug() << "LINE: " << line;
                    lines.append(line);
                }
                file.close();
            } else
            {
                qDebug() << "Failed to open file";
            }
        } else
        {
            qDebug() << "File is not exists";
        }
    }
    return lines;
}

int JournalPrinter::getDocumentNumber(QString line)
{
    if (line.contains("#"))
    {
        int startIndex = line.indexOf("#") + 1;
        QString number = line.mid(startIndex, 4);
        return number.toInt();
    }
    return -1;
}

bool JournalPrinter::isDocumentSeparator(QString line)
{
    return line.contains("----------");
}

bool JournalPrinter::isDocumentHeader(QString line)
{
    return line.contains("ИНН");
}

int JournalPrinter::findNextDocument(QStringList lines, int index)
{
    for (int i = index; i < lines.size(); i++)
    {
        QString line = lines.at(i);
        if (isDocumentSeparator(line)) {
            return i;
        }
    }
    return lines.size();
}

int JournalPrinter::findPrevDocument(QStringList lines, int index)
{
    for (int i = index; i >= 0; i--)
    {
        QString line = lines.at(i);
        if (isDocumentSeparator(line)) {
            return i+1;
        }
    }
    return 0;
}

QStringList JournalPrinter::getDocument(int docNumber)
{
    int index1 = -1;
    int index2 = -1;
    QStringList lines = readAll();

    for (int i = lines.size() - 1; i >= 0; i--) {
        QString line = lines.at(i);
        int docNum = getDocumentNumber(line);

        if (docNum == -1)
            continue;

        if (docNum == docNumber) {
            if (index2 == -1)
                index2 = findNextDocument(lines, i + 1);

            if (index2 != -1)
                index2 -= 1;

            index1 = findPrevDocument(lines, i);
            break;
        }
    }
    if (index1 == -1)
    {
        QString text;
        text.sprintf("Документ № %d не найден", docNumber);
        qDebug() << text;
        throw new TextException(text);
    }
    lines = copyLines(lines, index1, index2);
    return lines;
}

QStringList JournalPrinter::copyLines(
        QStringList lines, int index1, int index2)
{
    QStringList result;
    if (index1 < 0) index1 = 0;
    if (index1 >= lines.size()) return result;
    if (index2 >= lines.size()) index2 = lines.size()-1;

    for (int i = index1; i <= index2; i++)
    {
        result.append(lines.at(i));
    }
    return strip(result);
}

QStringList JournalPrinter::searchZReport(int dayNumber){
    QStringList lines = readAll();
    int index1 = -1;
    int index2 = -1;

    for (int i = 0; i < lines.size(); i++) {
        QString line = lines.at(i);

        int dayNum = getDayNumber(line);
        if (dayNum == -1) continue;

        if ((dayNum == dayNumber)&&(index1 == -1)) {
            index1 = findPrevDocument(lines, i);
        }
        if ((dayNum == (dayNumber + 1))&&(index2 == -1)) {
            index2 = findPrevDocument(lines, i);
            break;
        }
    }
    if (index1 == -1){
        QString text;
        text.sprintf("Смена № %d не найдена", dayNumber);
        qDebug() << text;
        throw new TextException(text);
    }
    if (index2 == -1) {
        index2 = lines.size()-1;
    }
    return copyLines(lines, index1, index2);
}

int JournalPrinter::getDayNumber(QString line)
{
    int index = line.indexOf("СМЕНА:");
    if (index >= 0)
    {
        index = index + 6;
        QString number = line.mid(index, line.length()-index);
        qDebug() << "getDayNumber: " << number.toInt();
        return number.toInt();
    }
    return -1;
}

QStringList JournalPrinter::strip(QStringList lines)
{
    QStringList result;
    for (int i=0;i<lines.size();i++)
    { QString line = lines.at(i);
        if (!isDocumentSeparator(line)){
            result.append(line);
        }
    }
    return result;
}

void JournalPrinter::show(QStringList lines)
{
    qDebug() << "show";
    for (int i=0;i<lines.length();i++)
    {
        qDebug() << "SHOW: " << lines.at(i);
    }
}
