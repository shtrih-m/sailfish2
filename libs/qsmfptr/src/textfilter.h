#ifndef TEXTFILTER_H
#define TEXTFILTER_H

#include <QFile>
#include <QVector>
#include <QStringList>
#include "src/ShtrihFiscalPrinter.h"
#include "utils.h"


#define SFiscalSign         "ФП"
#define SSaleText           "ПРОДАЖА"
#define SBuyText            "ПОКУПКА"
#define SRetSaleText        "ВОЗВРАТ ПРОДАЖИ"
#define SRetBuyText         "ВОЗВРАТ ПОКУПКИ"
#define SCashInText         "ВНЕСЕНИЕ"
#define SCashOutText        "ВЫПЛАТА"
#define SStornoText         "СТОРНО"
#define STotalText          "ВСЕГО"
#define SDiscountText       "СКИДКА"
#define SChargeText         "НАДБАВКА"
#define SReceiptTotal       "ИТОГ"
#define SCashPayment        "НАЛИЧНЫМИ"
#define SCreditPayment      "КРЕДИТОМ"
#define STarePayment        "ТАРОЙ"
#define SCardPayment        "ПЛАТ. КАРТОЙ"
#define SChangeText         "СДАЧА"
#define SReceiptCancelled   "ЧЕК АННУЛИРОВАН"
#define SDiscountStornoText "СТОРНО СКИДКИ"
#define SChargeStornoText   "СТОРНО НАДБАВКИ"
#define SXReportText        "СУТОЧНЫЙ ОТЧЕТ БЕЗ ГАШЕНИЯ"
#define SZReportText        "СУТОЧНЫЙ ОТЧЕТ С ГАШЕНИЕМ"
#define SDayClosed          "СМЕНА ЗАКРЫТА"

struct PrinterOperator
{
    uint8_t number;
    QString name;
};

struct Payment
{
    uint64_t amount;
    QString text;
};


struct Cash {
    uint16_t number;
    uint16_t count;
    uint64_t total;
};

struct Receipt
{
    uint16_t number;
    uint16_t count;
    uint64_t total;
    Payment payments[4];
};

struct VoidReceipt
{
    uint16_t count;
    uint64_t total;
};

struct XReport
{
    uint64_t salesAmountBefore;
    uint64_t buyAmountBefore;
    uint16_t xReportNumber;
    uint16_t zReportNumber;
    Receipt receipts[4];
    VoidReceipt voidedReceipts[4];
    Cash cashIn;
    Cash cashOut;
    uint16_t voidCount;
    uint16_t dayVoidCount;
    uint64_t cashInDrawer;
    uint64_t income;
    uint64_t salesAmount;
    uint64_t buyAmount;
};

class TextFilter: public PrinterFilter
{
private:
    XReport xreport;
    QString buffer;
    bool isFiscal;
    bool connected;
    bool isEJPresent;
    bool isDocumentPrinted;
    bool receiptOpened;
    QString deviceName;
    int operatorNumber;
    QString paymentNames[4];
    ShtrihFiscalPrinter* printer;
    QStringList header;
    QStringList trailer;
    QVector<PrinterOperator> printerOperators;
    QString docNames[4];
    QStringList lines;
    QString fileName;

    void connect();
    void addHeader();
    void addTrailer();
    void endDocument();
    void beginDocument();
    void addReceiptHeader();
    void add(QString text);
    void add(QString s1, QString s2);
    void add(QString text, uint64_t amount);
    void addXZReport(XReport report);
    void addEJLine(QString s);
    QStringList getLines();
    XReport readXReport();


    void addFiscalSign();
    QString getOperatorName();
    uint16_t getRecNumber(int docType);
    void openReceipt2(int receiptType);
    void addCenter(QString c, QString text);
    QString getDocumentNumber(uint16_t value);
    uint16_t getNextRecNumber(uint16_t recNumber);
    void printReceiptItem(ReceiptItemCommand& data);
    void readEJReport(bool isReceipt);
public:
    TextFilter(ShtrihFiscalPrinter* aprinter);

    QString getFileName();
    void setFileName(QString fileName);

    void printString(int event, PrintStringCommand& data);
    void printDocHeader(int event, PrintDocHeaderCommand& data);
    void cutPaper(int event, CutCommand& data);
    void feedPaper(int event, FeedPaperCommand& data);
    void printStringFont(int event, PrintStringFontCommand& data);
    void printXReport(int event, PasswordCommand& data);
    void printZReport(int event, PasswordCommand& data);
    void printCashIn(int event, CashCommand& data);
    void printCashOut(int event, CashCommand& data);
    void printHeader(int event, PasswordCommand& data);
    void printDocEnd(int event, PrintDocEndCommand& data);
    void printSale(int event, ReceiptItemCommand& data);
    void printBuy(int event, ReceiptItemCommand& data);
    void printRetSale(int event, ReceiptItemCommand& data);
    void printRetBuy(int event, ReceiptItemCommand& data);
    void printTrailer(int event, PasswordCommand& data);
    void printStorno(int event, ReceiptItemCommand& data);
    void closeReceipt(int event, CloseReceiptCommand& data);
    void printDiscount(int event, AmountAjustCommand& data);
    void printCharge(int event, AmountAjustCommand& data);
    void printAmountAjustment(int event, int code, AmountAjustCommand& data);
    void cancelReceipt(int event, PasswordCommand& data);
    void printDiscountStorno(int event, AmountAjustCommand& data);
    void printChargeStorno(int event, AmountAjustCommand& data);
    void printCopy(int event, PasswordCommand& data);
    void openReceipt(int event, OpenReceiptCommand& data);
};

#endif // TEXTFILTER_H
