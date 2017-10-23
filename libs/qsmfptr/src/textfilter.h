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
#define SDayOpened          "СМЕНА ОТКРЫТА"
#define SDayOpenReport      "ОТЧЕТ ОБ ОТКРЫТИИ СМЕНЫ"

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
    bool receiptOpened;
    QString deviceName;
    uint8_t operatorNumber;
    QString paymentNames[4];
    ShtrihFiscalPrinter* printer;
    QStringList header;
    QStringList trailer;
    QVector<PrinterOperator> printerOperators;
    QString docNames[4];
    QStringList lines;
    QString fileName;
    ReadLongStatusCommand status;

    void connect();
    void addHeader();
    void addTrailer();
    void endDocument();
    void beginDocument(bool isDayClose = false);
    void addReceiptHeader(bool isDayClose = false);
    void add(QString text);
    void addEJLine(QString s);
    void addXZReport(XReport report);
    void add(QString s1, QString s2);
    void add(QString text, uint64_t amount);
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
    void readFSReport();
    void addDocMac();
public:
    TextFilter(ShtrihFiscalPrinter* aprinter);

    QString getFileName();
    void setFileName(QString fileName);

    void printString(uint8_t event, PrintStringCommand& data);
    void printDocHeader(uint8_t event, PrintDocHeaderCommand& data);
    void cutPaper(uint8_t event, CutCommand& data);
    void feedPaper(uint8_t event, FeedPaperCommand& data);
    void printStringFont(uint8_t event, PrintStringFontCommand& data);
    void printXReport(uint8_t event, PasswordCommand& data);
    void printZReport(uint8_t event, PasswordCommand& data);
    void printCashIn(uint8_t event, CashCommand& data);
    void printCashOut(uint8_t event, CashCommand& data);
    void printHeader(uint8_t event, PasswordCommand& data);
    void printDocEnd(uint8_t event, PrintDocEndCommand& data);
    void printSale(uint8_t event, ReceiptItemCommand& data);
    void printBuy(uint8_t event, ReceiptItemCommand& data);
    void printRetSale(uint8_t event, ReceiptItemCommand& data);
    void printRetBuy(uint8_t event, ReceiptItemCommand& data);
    void printTrailer(uint8_t event, PasswordCommand& data);
    void printStorno(uint8_t event, ReceiptItemCommand& data);
    void closeReceipt(uint8_t event, CloseReceiptCommand& data);
    void printDiscount(uint8_t event, AmountAjustCommand& data);
    void printCharge(uint8_t event, AmountAjustCommand& data);
    void printAmountAjustment(uint8_t event, int code, AmountAjustCommand& data);
    void cancelReceipt(uint8_t event, PasswordCommand& data);
    void printDiscountStorno(uint8_t event, AmountAjustCommand& data);
    void printChargeStorno(uint8_t event, AmountAjustCommand& data);
    void printCopy(uint8_t event, PasswordCommand& data);
    void openReceipt(uint8_t event, OpenReceiptCommand& data);
    void openDay(uint8_t event, PasswordCommand& data);
};

#endif // TEXTFILTER_H
