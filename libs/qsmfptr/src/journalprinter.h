#ifndef JOURNALPRINTER_H
#define JOURNALPRINTER_H

#include "ShtrihFiscalPrinter.h"

class JournalPrinter
{
public:
    JournalPrinter(QString afileName, ShtrihFiscalPrinter* aprinter);

private:
    QString fileName;
    ShtrihFiscalPrinter* printer;
public:
    void print();
    void deleteFile();
    void printLine(QString line);
    void printLines(QStringList lines);

    void printCurrentDay();
    void printDay(int dayNumber);
    void printDoc(int docNumber);
    void printDocRange(int N1, int N2);

    QStringList loadLines();
    int getDayNumber(QString line);
    QStringList getCurrentDayReport();
    QStringList getDocument(int docNumber);
    QStringList searchZReport(int dayNumber);

    bool isDocumentHeader(QString line);
    int getDocumentNumber(QString line);
    int findNextDocument(QStringList lines, int index);
    QStringList copyLines(QStringList lines, int index1, int index2);
};

#endif // JOURNALPRINTER_H
