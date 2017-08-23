#include "journalprinter.h"

#include <QFile>
#include <QTextStream>
#include "ShtrihFiscalPrinter.h"

#define SZReportText "СУТОЧНЫЙ ОТЧЕТ С ГАШЕНИЕМ"

JournalPrinter::JournalPrinter(QString afileName, ShtrihFiscalPrinter* aprinter)
{
    fileName = afileName;
    printer = aprinter;
}

void JournalPrinter::deleteFile(){
    QFile file(fileName);
    file.remove();
}

void JournalPrinter::print()
{
    printLines(loadLines());
}

void JournalPrinter::printLines(QStringList lines){
    for (int i=0;i<lines.length();i++){
        printLine(lines.at(i));
    }
}

void JournalPrinter::printLine(QString line)
{
    PrintStringCommand command;
    command.flags = SMFP_STATION_REC;
    command.text = line;
    printer->check(printer->printString(command));
}

void JournalPrinter::printDay(int dayNumber)
{
    QStringList dst = searchZReport(dayNumber);
    QString header;
    header.sprintf("Контрольная лента Смена № %d", dayNumber);
    dst.insert(0, header);
    printLines(dst);
}

void JournalPrinter::printDoc(int docNumber)
{
    QStringList dst = getDocument(docNumber);
    QString header;
    header.sprintf("Контрольная лента Документ № %d", docNumber);
    dst.insert(0, header);
    printLines(dst);
}

void JournalPrinter::printDocRange(int N1, int N2)
{
    if (N1 > N2)
    {
        QString text;
        text.sprintf("Номер первого документа больше второго (%d > %d)", N1, N2);
        throw new TextException(text);
    }

    if (N1 == N2) {
        printDoc(N1);
        return;
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
    printLines(dst);
}

void JournalPrinter::printCurrentDay()
{
    QStringList dst = getCurrentDayReport();
    printLines(dst);
}

QStringList JournalPrinter::loadLines()
{
    QStringList lines;
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        file.seek(0);
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        for (;;)
        {
            QString line = stream.readLine();
            if (line == NULL) break;
            lines.append(line);
        }
        file.close();
    }
    return lines;
}

int JournalPrinter::getDocumentNumber(QString line)
{
    if (line.contains("ИНН") && line.contains("№"))
    {
        int startIndex = line.indexOf("№") + 1;
        QString number = line.mid(startIndex, 4);
        return number.toInt();
    }
    return -1;
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
        if (isDocumentHeader(line)) {
            return i;
        }
    }
    return lines.size();
}

QStringList JournalPrinter::getDocument(int docNumber)
{
    int index1 = -1;
    int index2 = -1;
    QStringList lines = loadLines();

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

            index1 = i;
            break;
        }
    }
    if (index1 == -1)
    {
        QString text;
        text.sprintf("Документ № %d не найден", docNumber);
        throw new TextException(text);
    }
    lines = copyLines(lines, index1, index2);
    return lines;
}

QStringList JournalPrinter::copyLines(
        QStringList lines, int index1, int index2)
{
    QStringList result;
    for (int i = index1; i <= index2; i++) {
        if (i < 0)
            return result;
        if (i >= lines.length())
            return result;
        result.append(lines.at(i));
    }
    return result;
}

QStringList JournalPrinter::searchZReport(int dayNumber){
    QStringList lines = loadLines();
    int index1 = 0;

    for (int i = 0; i < lines.size(); i++) {
        QString line = lines.at(i);

        int dayNum = getDayNumber(line);
        if (dayNum == -1)
            continue;

        if (dayNum == dayNumber) {
            int index2 = findNextDocument(lines, i + 1);
            if (index2 != -1)
                index2 -= 1;

            return copyLines(lines, index1, index2);
        } else {
            index1 = findNextDocument(lines, i + 1);
        }
    }
    QString text;
    text.sprintf("Смена № %d не найдена", dayNumber);
    throw new TextException(text);
}

int JournalPrinter::getDayNumber(QString line)
{
    if (line.contains(SZReportText))
    {
        int startIndex = line.length() - 4;
        QString number = line.mid(startIndex, startIndex + 4);
        return number.toInt();
    }
    return -1;
}

QStringList JournalPrinter::getCurrentDayReport()
{
    QStringList lines = loadLines();
    int index1 = 0;

    for (int i = 0; i < lines.length(); i++) {
        QString line = lines.at(i);

        int dayNum = getDayNumber(line);
        if (dayNum == -1)
            continue;

        index1 = findNextDocument(lines, i + 1);
    }

    if(index1 == -1 || index1 == lines.length())
    {
        throw new TextException("Смена не найдена");
    }
    return copyLines(lines, index1, lines.size());
}
