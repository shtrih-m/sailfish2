#ifndef JOURNALPRINTER_H
#define JOURNALPRINTER_H

#include <QString>
#include <QStringList>

class JournalPrinter
{
public:
    JournalPrinter(QString afileName);

private:
    QString fileName;
    QStringList lines;
public:
    void reset();
    QString getFileName();
    void deleteFile();
    void show(QStringList lines);
    void setFileName(QString value);

    QStringList readAll();
    QStringList readCurrentDay();
    QStringList readDay(int dayNumber);
    QStringList readDoc(int docNumber);
    QStringList readDocRange(int N1, int N2);

    int getDayNumber(QString line);
    QStringList getDocument(int docNumber);
    QStringList searchZReport(int dayNumber);

    bool isDocumentHeader(QString line);
    int getDocumentNumber(QString line);
    int findNextDocument(QStringList lines, int index);
    QStringList copyLines(QStringList lines, int index1, int index2);
};

#endif // JOURNALPRINTER_H
