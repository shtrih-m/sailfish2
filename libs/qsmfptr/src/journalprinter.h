#ifndef JOURNALPRINTER_H
#define JOURNALPRINTER_H

#include <QString>
#include <QStringList>

enum DocNumber {FPDocNumber, FSDocNumber};

class JournalPrinter
{
public:
    JournalPrinter(QString afileName);

private:
    QString fileName;
    QStringList lines;
    DocNumber docNumber;
public:
    void reset();
    QString getFileName();
    void deleteFile();
    void show(QStringList lines);
    void setFileName(QString value);

    QStringList readAll();
    QStringList readDay(int dayNumber);
    QStringList readDoc(int docNumber);
    QStringList strip(QStringList lines);
    QStringList readDocRange(int N1, int N2);

    int getDayNumber(QString line);
    QStringList getDocument(int docNumber);
    QStringList searchZReport(int dayNumber);

    bool isDocumentSeparator(QString line);
    bool isDocumentHeader(QString line);
    int getDocumentNumber(QString line);
    int getFPDocumentNumber(QString line);
    int getFSDocumentNumber(QString line);
    int findNextDocument(QStringList lines, int index);
    int findPrevDocument(QStringList lines, int index);
    QStringList copyLines(QStringList lines, int index1, int index2);
    DocNumber getDocNumber();
    void setDocNumber(DocNumber value);

};

#endif // JOURNALPRINTER_H
