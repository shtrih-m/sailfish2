#include <QFile>
#include <QVector>
#include <QDebug>
#include <QString>
#include "textfilter.h"
#include "src/ShtrihFiscalPrinter.h"

uint64_t round2(double value){
    return (value + 0.5);
}

QString amountToStr(long value)
{
    QString buffer;
    double d = value;
    return buffer.sprintf("%.2f", d / 100.0);
}

QString quantityToStr(long value) {
    QString buffer;
    double d = value;
    return buffer.sprintf("%.3f", d / 1000.0);
}

QString summToStr(uint64_t amount) {
    return "=" + amountToStr(amount);
}

QString summToStr(uint64_t price, uint64_t quantity)
{
    double d = price * quantity;
    return summToStr(round2(d / 1000.0));
}

QString getTaxLetter(int tax)
{
    QString taxLetters = "АБВГДЕ";
    if ((tax >= 1)&&(tax <= 6)){
        return taxLetters.at(tax-1);
    }
    return "";
}

QString getTaxData(int tax1, int tax2, int tax3, int tax4)
{
    QString result = "";
    result += getTaxLetter(tax1);
    result += getTaxLetter(tax2);
    result += getTaxLetter(tax3);
    result += getTaxLetter(tax4);
    if (!result.isEmpty()){
        result = "_" + result;
    }
    return result;
}

QString stringOfChar(QString ch, int count){
    QString result;
    for (int i=0;i<count;i++){
        result += ch;
    }
    return result;
}

TextFilter::TextFilter(ShtrihFiscalPrinter* aprinter)
{
    this->printer = aprinter;
    connected = false;
    deviceName = "";
    operatorNumber = 0;
    isFiscal = false;
    isEJPresent = false;
    for (int i=0;i<4;i++){
        paymentNames[i] = "";
    }
    docNames[0] = SSaleText;
    docNames[1] = SBuyText;
    docNames[2] = SRetSaleText;
    docNames[3] = SRetBuyText;
    fileName = "journal.txt";
    receiptOpened = false;
}

QString TextFilter::getFileName(){
    return fileName;
}

void TextFilter::setFileName(QString fileName)
{
    this->fileName = fileName;
}

void TextFilter::connect()
{
    if (connected) return;

    DeviceTypeCommand data;
    printer->check(printer->readDeviceType(data));
    QString deviceName = data.name;
    if (deviceName.contains("ПТК")) {
        deviceName = "ПТК";
    } else {
        deviceName = "ККМ";
    }
    ReadLongStatusCommand status;
    printer->check(printer->readLongStatus(status));
    operatorNumber = status.operatorNumber;
    isFiscal = (status.operatorNumber > 0);
    isEJPresent = testBit(status.flags, 5);
    for (int i = 0; i <= 3; i++)
    {
        paymentNames[i] = printer->readPaymentName(i+1);
     }
    header = printer->readHeader();
    trailer = printer->readTrailer();
    connected = true;
}

void TextFilter::addFiscalSign()
{
    if (isFiscal) {
        addCenter("*", SFiscalSign);
    }
}

QStringList TextFilter::getLines(){
    return lines;
}

void TextFilter::addCenter(QString c, QString text)
{
    int lineLength = printer->getLineLength(1);
    int l = (lineLength - text.length()) / 2;
    QString line = stringOfChar(c, l) + text;
    line = line + stringOfChar(c, lineLength - line.length());
    add(line);
}

void TextFilter::add(QString text)
{
    qDebug() << "TextFilter: " << text;

    QFile file(fileName);
    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        file.seek(file.size());
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << text.toUtf8() << endl;
        file.close();
    } else
    {
        qDebug() << "Failed to open file";
    }
}

void TextFilter::add(QString s1, QString s2)
{
    int printWidth = printer->getLineLength(1);
    int width = printWidth - s2.length() - 1;
    QString line = s1.left(width);
    line += stringOfChar(" ", printWidth - line.length() - s2.length()) + s2;
    add(line);
}

void TextFilter::add(QString text, uint64_t amount)
{
    add(text, summToStr(amount));
}

void TextFilter::printString(uint8_t event, PrintStringCommand& data)
{
    if (event == EVENT_AFTER){
        add(data.text);
    }
}

void TextFilter::printDocHeader(uint8_t event, PrintDocHeaderCommand& data){
    if (event == EVENT_AFTER){
        add(data.text);
    }
}

void TextFilter::cutPaper(uint8_t event, CutCommand& data){
    (void)(event);
    (void)(data);
}

void TextFilter::feedPaper(uint8_t event, FeedPaperCommand& data)
{
    if (event == EVENT_AFTER){
        for (int i=0;i<data.count;i++){
            add(" ");
        }
    }
}

void TextFilter::printStringFont(uint8_t event, PrintStringFontCommand& data){
    if (event == EVENT_AFTER){
        add(data.text);
    }
}

void TextFilter::printXReport(uint8_t event, PasswordCommand& data)
{
    (void)data;
    if (event == EVENT_BEFORE){
        xreport = readXReport();
    } else{
        beginDocument();
        add(SXReportText, buffer.sprintf("№%.4d", xreport.xReportNumber));
        addXZReport(xreport);
        endDocument();
    }
}

void TextFilter::printZReport(uint8_t event, PasswordCommand& data)
{
    (void)data;
    if (event == EVENT_BEFORE){
        xreport = readXReport();
    } else{
        beginDocument();
        add(SZReportText, buffer.sprintf("№%.4d", xreport.zReportNumber));
        addXZReport(xreport);
        addDocMac();
        addCenter("*", SDayClosed);
        endDocument();
    }
}

QString TextFilter::getDocumentNumber(uint16_t value) {
    return buffer.sprintf("%.4d", value);
}

void TextFilter::printCashIn(uint8_t event, CashCommand& data)
{
    if (event == EVENT_AFTER){
        isDocumentPrinted = true;
        operatorNumber = data.operatorNumber;
        uint16_t docNumber = printer->readOperationRegister(155);

        beginDocument();
        add(SCashInText, getDocumentNumber(docNumber));
        add("", amountToStr(data.amount));
        endDocument();
    }
}

void TextFilter::printCashOut(uint8_t event, CashCommand& data){
    if (event == EVENT_AFTER){
        isDocumentPrinted = true;
        operatorNumber = data.operatorNumber;
        uint64_t docNumber = printer->readOperationRegister(156);

        beginDocument();
        add(SCashOutText, getDocumentNumber(docNumber));
        add("", amountToStr(data.amount));
        endDocument();
    }
}

void TextFilter::printHeader(uint8_t event, PasswordCommand& data){
    (void)data;
    if (event == EVENT_AFTER){
        addHeader();
    }
}

void TextFilter::printDocEnd(uint8_t event, PrintDocEndCommand& data){
    (void)data;
    if (event == EVENT_AFTER){
        addTrailer();
    }
}

void TextFilter::openReceipt2(int receiptType)
{
    qDebug() << "openReceipt2";
    if (!receiptOpened)
    {
        receiptOpened = true;
        isDocumentPrinted = false;
        beginDocument();
        uint16_t recNumber = getNextRecNumber(getRecNumber(receiptType));
        add(docNames[receiptType], buffer.sprintf("№%.4d", recNumber));
    }
}

void TextFilter::printReceiptItem(ReceiptItemCommand& data)
{
    add(data.text);

    QString line = quantityToStr(data.quantity) + " X " +
        amountToStr(data.price);
    add("", line);

    line = summToStr(data.price, data.quantity) +
        getTaxData(data.tax1, data.tax2, data.tax3, data.tax4);
    add(buffer.sprintf("%d", data.department), line);
}

void TextFilter::printSale(uint8_t event, ReceiptItemCommand& data)
{
    if (event == EVENT_AFTER){
        qDebug() << "TextFilter::printSale";
        operatorNumber = data.operatorNumber;
        openReceipt2(SMFP_RECTYPE_SALE);
        printReceiptItem(data);
    }
}

void TextFilter::printBuy(uint8_t event, ReceiptItemCommand& data){
    if (event == EVENT_AFTER){
        operatorNumber = data.operatorNumber;
        openReceipt2(SMFP_RECTYPE_BUY);
        printReceiptItem(data);
    }
}

void TextFilter::printRetSale(uint8_t event, ReceiptItemCommand& data){
    if (event == EVENT_AFTER){
        operatorNumber = data.operatorNumber;
        openReceipt2(SMFP_RECTYPE_RETSALE);
        printReceiptItem(data);
    }
}

void TextFilter::printRetBuy(uint8_t event, ReceiptItemCommand& data){
    if (event == EVENT_AFTER){
        operatorNumber = data.operatorNumber;
        openReceipt2(SMFP_RECTYPE_RETBUY);
        printReceiptItem(data);
    }
}

void TextFilter::printTrailer(uint8_t event, PasswordCommand& data){
    (void)data;
    if (event == EVENT_AFTER){
        addTrailer();
    }
}

void TextFilter::printStorno(uint8_t event, ReceiptItemCommand& data)
{
    if (event == EVENT_AFTER){
        operatorNumber = data.operatorNumber;
        // Line 1
        add(data.text);
        // Line 2
        QString line =
            quantityToStr(data.quantity) + " X " +
            amountToStr(data.price);
        add(SStornoText, line);
        // Line 3
        line = summToStr(data.price, data.quantity);
        add(buffer.sprintf("%d", data.department), line);
    }
}

void TextFilter::closeReceipt(uint8_t event, CloseReceiptCommand& data)
{
    if (event == EVENT_AFTER){
        receiptOpened = false;
        add(data.text);

        uint64_t receiptTotal = 0;
        ReadSubtotalCommand command;
        printer->readSubtotal(command);
        if (printer->succeeded(command.resultCode)){
            receiptTotal = command.amount;
        }

        if (data.discount > 0) {
            // ВСЕГО
            add(STotalText, summToStr(receiptTotal));
            // СКИДКА
            long discountAmount = round(receiptTotal * data.discount / 100.0);
            QString line = QString(SDiscountText) + " " + amountToStr(data.discount) + "%";
            add(line, summToStr(discountAmount)
                    + getTaxData(data.tax1, data.tax2, data.tax3, data.tax4));
            receiptTotal = receiptTotal - discountAmount;
        }
        // TOTAL =123.34
        add(SReceiptTotal, summToStr(receiptTotal));
        // Payments
        if (data.amount1 > 0) {
            add(SCashPayment, summToStr(data.amount1));
        }
        if (data.amount2 > 0) {
            add(SCreditPayment, summToStr(data.amount2));
        }
        if (data.amount3 > 0) {
            add(STarePayment, summToStr(data.amount3));
        }
        if (data.amount4 > 0) {
            add(SCardPayment, summToStr(data.amount4));
        }
        // Change
        if (data.change > 0)
            add(SChangeText, summToStr(data.change));

        addDocMac();
        addFiscalSign();
        endDocument();
    }
}

void TextFilter::printDiscount(uint8_t event, AmountAjustCommand& data){
    if (event == EVENT_AFTER){
        add(data.text);
        add(SDiscountText, summToStr(data.amount));
    }
}

void TextFilter::printCharge(uint8_t event, AmountAjustCommand& data){
    if (event == EVENT_AFTER){
        add(data.text);
        add(SChargeText, summToStr(data.amount));
    }
}

void TextFilter::printAmountAjustment(uint8_t event, int code, AmountAjustCommand& data){
    (void)(event);
    (void)(code);
    (void)(data);
}

void TextFilter::cancelReceipt(uint8_t event, PasswordCommand& data){
    (void)data;
    if (event == EVENT_AFTER){
        receiptOpened = false;
        add(SReceiptCancelled);
        endDocument();
    }
}

void TextFilter::printDiscountStorno(uint8_t event, AmountAjustCommand& data){
    if (event == EVENT_AFTER){
        add(data.text);
        add(SDiscountStornoText, summToStr(data.amount));
    }
}

void TextFilter::printChargeStorno(uint8_t event, AmountAjustCommand& data){
    if (event == EVENT_AFTER){
        add(data.text);
        add(SChargeStornoText, summToStr(data.amount));
    }
}

void TextFilter::printCopy(uint8_t event, PasswordCommand& data){
    (void)(event);
    (void)(data);
}

void TextFilter::openDay(uint8_t event, PasswordCommand& data)
{
    (void)data;

    if (event == EVENT_BEFORE)
    {
        ReadLongStatusCommand status;
        printer->readLongStatus(status);
        dayNumber = status.dayNumber + 1;
        if (dayNumber == 10000){
            dayNumber = 9999;
        }
    }

    if (event == EVENT_AFTER)
    {
        beginDocument();
        add(SDayOpenReport);
        add("НОМЕР СМЕНЫ", StringUtils::intToStr(dayNumber));
        addDocMac();
        addCenter("*", SDayOpened);
        endDocument();
    }
}

void TextFilter::openReceipt(uint8_t event, OpenReceiptCommand& data){
    if (event == EVENT_AFTER){
        openReceipt2(data.receiptType);
    }
}

void TextFilter::addHeader(){
    for (int i = 0; i < header.size(); i++) {
        add(header.at(i));
    }
}

void TextFilter::addTrailer()
{
    for (int i = 0; i < trailer.size(); i++) {
        add(trailer.at(i));
    }
}

void TextFilter::beginDocument()
{
    qDebug() << "beginDocument";

    connect();
    addHeader();
    addReceiptHeader();
}

void TextFilter::endDocument()
{
    addTrailer();
    addCenter("-", "-");
}

void TextFilter::addReceiptHeader()
{
    qDebug() << "addReceiptHeader";

    QString line1 = "";
    QString line2 = "";

    printer->check(printer->readLongStatus(status));
    int documentNumber = status.documentNumber;
    if (!isDocumentPrinted){
        documentNumber += 1;
    }

    // РН ККТ
    line1 = "РН ККТ:" + printer->readParameter(FPTR_PARAMETER_REG_NUMBER);
    line2 = buffer.sprintf("%.2d.%.2d.%.2d %.2d:%.2d",
        status.date.day, status.date.month, status.date.year,
        status.time.hour, status.time.min);
    add(line1, line2);

    // ЗН ККТ
    line1 = "ЗН ККТ:" + printer->readParameter(FPTR_PARAMETER_SERIAL_NUMBER);
    line2 = buffer.sprintf("СМЕНА:%d", dayNumber);
    add(line1, line2);

    // ИНН - ФН
    line1 = "ИНН:" + printer->readParameter(FPTR_PARAMETER_FISCAL_ID);
    line2 = "ФН:" + printer->readParameter(FPTR_PARAMETER_FS_SERIAL_NUMBER);
    add(line1, line2);

    // Кассир - document number
    line1 = "Кассир:" + getOperatorName();
    line2 = buffer.sprintf("#%.4d", documentNumber);
    add(line1, line2);
}

QString TextFilter::getOperatorName()
{
    PrinterOperator printerOperator;
    for (int i = 0; i < printerOperators.size(); i++)
    {
        printerOperator = printerOperators.at(i);
        if (printerOperator.number == operatorNumber) {
            return printerOperator.name;
        }
    }
    printerOperator.number = operatorNumber;
    printerOperator.name = printer->readTable(SMFP_TABLE_CASHIER, operatorNumber, 2);
    printerOperators.append(printerOperator);
    return printerOperator.name;
}

XReport TextFilter::readXReport()
{
    XReport report;

    report.salesAmountBefore = 0;
    report.buyAmountBefore = 0;

    FMReadTotalsCommand command;
    command.type = 1;
    int rc = printer->fmReadTotals(command);
    if (printer->succeeded(rc))
    {
        report.salesAmountBefore = command.saleTotal;
        report.buyAmountBefore = command.buyTotal;
    }

    report.xReportNumber = printer->readOperationRegister(158) + 1;
    report.zReportNumber = printer->readDayNumber() + 1;

    for (int i = 0; i <= 3; i++)
    {
        Receipt receipt;
        receipt.number = printer->readOperationRegister(148 + i);
        receipt.count = printer->readOperationRegister(144 + i);
        long total = 0;
        for (int j = 0; j <= 3; j++) {
            long amount = printer->readCashRegister(193 + i + j * 4);
            total = total + amount;
            receipt.payments[j].amount = amount;
            receipt.payments[j].text = paymentNames[j];
        }
        receipt.total = total;
        report.receipts[i] = receipt;
    }
    report.cashIn.number = printer->readOperationRegister(155);
    report.cashIn.count = printer->readOperationRegister(153);
    report.cashIn.total = printer->readCashRegister(242);
    report.cashOut.number = printer->readOperationRegister(156);
    report.cashOut.count = printer->readOperationRegister(154);
    report.cashOut.total = printer->readCashRegister(243);
    report.voidCount = printer->readOperationRegister(166);
    report.dayVoidCount = printer->readOperationRegister(157);
    report.cashInDrawer = printer->readCashRegister(241);
    report.income =
            report.receipts[0].total -
            report.receipts[1].total -
            report.receipts[2].total +
            report.receipts[3].total;
    report.salesAmount = report.salesAmountBefore + report.receipts[0].total;
    report.buyAmount = report.buyAmountBefore + report.receipts[1].total;
    for (int i = 0; i <= 3; i++) {
        report.voidedReceipts[i].count = printer->readOperationRegister(179 + i);
        report.voidedReceipts[i].total = printer->readCashRegister(249 + i);
    }
    return report;
}

void TextFilter::addXZReport(XReport report)
{
    QString ReceiptText[4] = { "ЧЕКОВ ПРОДАЖ", "ЧЕКОВ ПОКУПОК", "ЧЕКОВ ВОЗВРАТОВ ПРОДАЖ", "ЧЕКОВ ВОЗВРАТОВ ПОКУПОК" };
    QString voidedReceiptText[4] = { "ПРОДАЖ", "ПОКУПОК", "ВОЗВР.ПРОДАЖ", "ВОЗВР.ПОКУПОК" };
    add("НЕОБНУЛ.СУММА ПРОДАЖ НА НАЧ.СМЕНЫ", report.salesAmountBefore);
    add("НЕОБНУЛ.СУММА ПОКУПОК НА НАЧ.СМЕНЫ", report.buyAmountBefore);
    for (int i = 0; i <= 3; i++)
    {
        Receipt receipt = report.receipts[i];
        add(ReceiptText[i], buffer.sprintf("%.4d", receipt.number));
        add(buffer.sprintf("%.4d", receipt.count), receipt.total);
        add(receipt.payments[0].text, receipt.payments[0].amount);
        add(receipt.payments[1].text, receipt.payments[1].amount);
        add(receipt.payments[2].text, receipt.payments[2].amount);
        add(receipt.payments[3].text, receipt.payments[3].amount);
    }
    add("ВНЕСЕНИЙ", buffer.sprintf("%.4d", report.cashIn.number));
    add(buffer.sprintf("%.4d", report.cashIn.count), report.cashIn.total);
    add("ВЫПЛАТ", buffer.sprintf("%.4d", report.cashOut.number));
    add(buffer.sprintf("%.4d", report.cashOut.count), report.cashOut.total);
    add("АННУЛИРОВАННЫХ ЧЕКОВ", buffer.sprintf("%.4d", report.voidCount));
    add(buffer.sprintf("%.4d", report.dayVoidCount));
    for (int i = 0; i <= 3; i++)
    {
        QString text = buffer.sprintf("%.4d ", report.voidedReceipts[i].count);
        text += voidedReceiptText[i];
        add(text, report.voidedReceipts[i].total);
    }
    add("НАЛ. В КАССЕ", report.cashInDrawer);
    add("ВЫРУЧКА", report.income);
    add("НЕОБНУЛ. СУММА ПРОДАЖ", report.salesAmount);
    add("НЕОБ. СУММА ПОКУПОК", report.buyAmount);
}

uint16_t TextFilter::getRecNumber(int docType)
{
    switch (docType)
    {
        case SMFP_RECTYPE_SALE:
        return printer->readOperationRegister(148);

        case SMFP_RECTYPE_BUY:
        return printer->readOperationRegister(149);

        case SMFP_RECTYPE_RETSALE:
        return printer->readOperationRegister(150);

        case SMFP_RECTYPE_RETBUY:
        return printer->readOperationRegister(151);

        default: {
            return 0;
        }
    }
}

uint16_t TextFilter::getNextRecNumber(uint16_t recNumber)
{
    recNumber = recNumber + 1;
    if (recNumber > 9999) {
        recNumber = 1;
    }
    return recNumber;
}

void TextFilter::addDocMac()
{
    if (isEJPresent)
    {
        if (isFiscal) readEJReport(true);
    } else{
        readFSReport();
    }
}

void TextFilter::readFSReport()
{
    FSStatus status;
    if (printer->fsReadStatus(status) == 0)
    {
        FSFindDocument data;
        data.docNum = status.docNumber;
        if (printer->fsFindDocument(data) == 0)
        {
            uint32_t docMac = printer->getDocumentMac(data);
            add(buffer.sprintf("ФД:%u ФП:%u", status.docNumber, docMac));
        }
    }
}

void TextFilter::readEJReport(bool isReceipt)
{
    if (!(isFiscal && isEJPresent))
        return;

    QVector<QString> lines;

    ReadEJStatus1 status1;
    int rc = printer->readEJStatus1(status1);
    if (printer->failed(rc)) return;

    ReadEJDocument document;
    document.macNumber = status1.macNumber;
    rc = printer->readEJDocument(document);
    if (printer->failed(rc)) return;
    lines.append(document.text);

    ReadEJDocumentLine line;
    rc = 0;
    while (printer->succeeded(rc))
    {
        rc = printer->readEJDocumentLine(line);
        if (printer->succeeded(rc))
        {
            lines.append(line.text);
            if ((line.text.length() > 0) && (line.text.contains("#"))) {
                printer->cancelEJDocument();
                break;
            }

        }
    }

    if (isReceipt) {
        // 2 last lines
        if (lines.size() > 2) {
            addEJLine(lines.at(2));
            addEJLine(lines.at(lines.size() - 1));
        }
    } else {
        for (int i = 0; i < lines.size(); i++) {
            addEJLine(lines.at(i));
        }
    }
}

void TextFilter::addEJLine(QString s)
{
    add("        " + s);
}


