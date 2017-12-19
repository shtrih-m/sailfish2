#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QBluetoothLocalDevice>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothServiceInfo>
#include <QDebug>
#include <QElapsedTimer>
#include <QImage>
#include <QPainter>
#include <QPixmap>

#include "socketport.h"
#include "bluetoothport.h"
#include "bluetoothport2.h"
#include "printerprotocol1.h"
#include "printerprotocol2.h"
#include "printertest.h"
#include "utils.h"
#include "zint.h"
#include "textfilter.h"
#include "journalprinter.h"


PrinterTest::PrinterTest(QObject* parent)
    : QObject(parent)
{
    logger = new Logger("PrinterTest.log");
    printer = new ShtrihFiscalPrinter(0, logger);

    port = new BluetoothPort2(logger);
    port->setAddress("00:01:90:C5:60:F7");
    port->setReadTimeout(10000);
    port->setWriteTimeout(10000);
    port->setConnectTimeout(3000);
    port->setConnectRetries(3);

    protocol = new PrinterProtocol2(port, logger);
    printer->setProtocol(protocol);
    printer->setTimeout(10000);
    printer->setFdoThreadEnabled(true);
    printer->setJournalEnabled(true);
}

PrinterTest::~PrinterTest()
{
    delete port;
    delete protocol;
    delete printer;
    delete logger;
}

void PrinterTest::show(QStringList lines)
{
    for (int i=0;i<lines.length();i++)
    {
        qDebug() << "SHOW: " << lines.at(i);
    }
}

void PrinterTest::execute()
{
    qDebug("PrinterTest::execute");

    JournalPrinter journal("journal.txt");

    qDebug("Read FS document 21");
    journal.setDocNumber(FSDocNumber);
    journal.show(journal.readDoc(21));

    qDebug("Read FS document 22");
    journal.setDocNumber(FSDocNumber);
    journal.show(journal.readDoc(22));

    qDebug("Read FP document 24");
    journal.setDocNumber(FPDocNumber);
    journal.show(journal.readDoc(24));

    qDebug("Read FP document 25");
    journal.setDocNumber(FPDocNumber);
    journal.show(journal.readDoc(25));

    // connectPrinter();
    // testHeaderTrailer();
    // testHeaderEnabled();
    // disconnectPrinter();
}

/*
    JournalPrinter journal("journal.txt");
    journal.show(journal.readDoc(28));
    //journal.show(journal.readAll());
    //journal.show(journal.readDay(7));
    //journal.show(journal.readDocRange(21, 22));
    //journal.deleteFile();

    while (true)
    {
        try
        {
            connectPrinter();
            //readShortStatus();
            printString();
            QThread::msleep(1000);

        } catch (...) {
            qDebug() << "Exception caught...";
        }
    }
    disconnectPrinter();

    //journal.show(journal.readDay(17));
    //journal.show(journal.readDay(5));
    //journal.show(journal.readDoc(121));
    //journal.show(journal.readDoc(122));
    //journal.show(journal.readDocRange(121, 122));

    //printer->jrnPrintAll();
    //printer->jrnPrintDoc(114);
    //printer->jrnPrintCurrentDay();
    //printer->jrnPrintDay(1);
    //printer->jrnPrintDocRange(107, 109);
    //disconnectPrinter();
    */

void PrinterTest::testHeaderEnabled()
{
    printer->writeParameter(FPTR_PARAMETER_HEADER_ENABLED, "0");
    PasswordCommand command;
    printer->printHeader(command);
    printSaleReceipt();

    printer->writeParameter(FPTR_PARAMETER_HEADER_ENABLED, "1");
    printSaleReceipt();
}

void PrinterTest::testHeaderTrailer()
{
    qDebug("Header1: ");
    show(printer->readHeader());
    qDebug("Trailer1: ");
    show(printer->readTrailer());

    printer->writeTableStr(4, 11, 1, "Header line 1");
    printer->writeTableStr(4, 12, 1, "Header line 2");
    printer->writeTableStr(4, 13, 1, "Header line 3");
    printer->writeTableStr(4, 14, 1, "Header line 4");
    printer->writeTableStr(4, 1, 1, "Trailer line 1");
    printer->writeTableStr(4, 2, 1, "Trailer line 2");
    printer->writeTableStr(4, 3, 1, "Trailer line 3");
    qDebug("Header2: ");
    show(printer->readHeader());
    qDebug("Trailer2: ");
    show(printer->readTrailer());

    qDebug() << "HeaderEnabled: " << printer->readParameter(FPTR_PARAMETER_HEADER_ENABLED);
}

void PrinterTest::deleteLogFile(){
    QFile file("PrinterTest.log");
    file.remove();
}

void PrinterTest::showLogFile()
{
    QFile file("PrinterTest.log");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        while (!stream.atEnd())
        {
            QString line = stream.readLine();
            qDebug() << line;
        }
        file.close();
    }
}

void PrinterTest::printText()
{
    QString text = "Line_01234567890123456789012345678901234567890123456789\r\n0123456789012345678901234567890123456789012345678901234567890123456789";
    printer->printText(text);
}

void PrinterTest::readLastDocMac()
{
    FSStatus status;
    printer->check(printer->fsReadStatus(status));
    FSFindDocument document;
    document.docNum = status.docNumber;
    printer->check(printer->fsFindDocument(document));
    qDebug() << "FindDocument: " << StringUtils::dataToHex(document.docData);
    qDebug() << "document.docType: " << document.docType;
    uint32_t docMac = printer->getDocumentMac(document);
    qDebug() << QString("ФД:%1 ФПД:%2").arg(status.docNumber).arg(docMac);
}


void PrinterTest::testTextFile()
{
    qDebug() << "testTextFile";
    QString fileName = "test.txt";
    QFile file(fileName);
    file.remove();
    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        QString line = "Строка 1";
        stream << line.toUtf8() << endl;
        line = "";
        stream << line.toUtf8() << endl;
        line = "Строка 2";
        stream << line.toUtf8() << endl;
        line = "Строка 3";
        stream << line.toUtf8() << endl;
        file.close();
    }

    QFile file1(fileName);
    if (file1.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file1);
        stream.setCodec("UTF-8");
        while (!stream.atEnd())
        {
            QString line = stream.readLine();
            qDebug() << line;
        }
        file1.close();
    }
}

void PrinterTest::testFile()
{
    qDebug() << "testFile.0";

    /*
    PrintStringCommand command;
    command.flags = SMFP_STATION_REC;
    command.text = "Строка 0";
    printer->printString(command);
    */

    QString line;
    QString fileName = "Journal.txt";

    qDebug() << "writeFile";
    QFile file(fileName);
    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        line = "Строка 1";
        stream << line.toUtf8() << endl;
        file.close();
    }

    qDebug() << "readFile";
    QFile file1(fileName);
    if (file1.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file1);
        stream.setCodec("UTF-8");
        stream.seek(0);

        qDebug() << stream.readLine();

        file1.close();
    }
    qDebug() << "testFile.1";
}

void PrinterTest::waitForDocuments()
{
    while (true)
    {
        int docCount = 0;
        int rc = printer->fsReadDocCount(docCount);
        if (rc != 0) break;
        qDebug() << "docCount = " << docCount;
        if (docCount == 0) break;

        QThread::msleep(3000);
    }
}

/**************************************************************************
 * Запрос статуса ФН
 * Код команды FF01h. Длина сообщения: 6 байт.
 * Пароль системного администратора: 4 байт
 *
 * Ответ: FF01h	Длина сообщения: 31 байт.
 * Код ошибки (1 байт)
 * Состояние фазы жизни (1 байт)
 * Бит 0 – проведена настройка ФН
 * Бит 1 – открыт фискальный режим
 * Бит 2 – закрыт фискальный режим
 * Бит 3 – закончена передача фискальных данных в ОФД
 * Текущий документ (1 байт)
 * 00h – нет открытого документа
 * 01h – отчет о фискализации
 * 02h – отчет об открытии смены
 * 04h – кассовый чек
 * 08h – отчет о закрытии смены
 * 10h – отчет о закрытии фискального режима
 * Данные документа (1 байт)
 * 00 – нет данных документа
 * 01 – получены данные документа
 * Состояние смены (1 байт)
 * 00 – смена закрыта
 * 01 – смена открыта
 * Флаги предупреждения (1 байт)
 * Бит 0 – Срочная замена КС (до окончания срока действия 3 дня)
 * Бит 1 – Исчерпание ресурса КС (до окончания срока действия 30 дней)
 * Бит 2 – Переполнение памяти ФН (Архив ФН заполнен на 90 %)
 * Бит 3 – Превышено время ожидания ответа ОФД*
 * Дата и время (5 байт)
 * Номер ФН (16 байт) ASCII
 * Номер последнего ФД (4 байт)
 ***************************************************************************/

void PrinterTest::fsReadStatus()
{
    qDebug() << "fsReadStatus";
    FSStatus status;
    if (printer->fsReadStatus(status) != 0) return;

    qDebug() << "-----------------------------------------";
    qDebug() << "  Состояние ФН";
    qDebug() << "-----------------------------------------";
    qDebug() << "Состояние фазы жизни      :" << status.mode.value;
    qDebug() << "Проведена настройка ФН    :" << status.mode.isConfigured;
    qDebug() << "Открыт фискальный режим   :" << status.mode.isFiscalOpened;
    qDebug() << "Закрыт фискальный режим   :" << status.mode.isFiscalClosed;
    qDebug() << "Закончена передача в ОФД  :" << status.mode.isDataSended;
    qDebug() << "Текущий документ          :" << status.docType;
    qDebug() << "Получены данные документа :" << status.isDocReceived;
    qDebug() << "Cмена открыта             :" << status.isDayOpened;
    qDebug() << "Флаги предупреждения      :" << status.flags.value;
    qDebug() << "Дата                      :" << printer->PrinterDateToStr(status.date);
    qDebug() << "Время                     :" << printer->PrinterTimeToStr(status.time);
    qDebug() << "Номер ФН                  :" << status.fsSerial;
    qDebug() << "Номер последнего ФД       :" << status.docNumber;
    qDebug() << "-----------------------------------------";

    QString serial;
    if (printer->fsReadSerial(serial) != 0) return;
    qDebug() << "Серийный номер ФН         :" << serial;
    qDebug() << "-----------------------------------------";

    PrinterDate date;
    if (printer->fsReadExpDate(date) != 0) return;
    qDebug() << "Срок действия ФН          :" << printer->PrinterDateToStr(date);
    qDebug() << "-----------------------------------------";

    FSVersion version;
    if (printer->fsReadVersion(version) != 0) return;
    qDebug() << "Версия ПО ФН              :" << version.text;
    qDebug() << "Версия является серийной  :" << version.isRelease;
    qDebug() << "-----------------------------------------";

    FSReadFiscalization fiscalization;
    if (printer->fsReadFiscalization(fiscalization) != 0) return;

    qDebug() << "Итоги фискализации";
    qDebug() << "Дата и время :" << printer->PrinterDateToStr(fiscalization.date) << ", " << printer->PrinterTimeToStr(fiscalization.time);
    qDebug() << "ИНН :" << fiscalization.inn;
    qDebug() << "РНМ :" << fiscalization.rnm;
    qDebug() << "Код налогообложения :" << fiscalization.taxSystem;
    qDebug() << "Код причины перерегистрации :" << fiscalization.regCode;
    qDebug() << "Номер ФД :" << fiscalization.docNum;
    qDebug() << "Фискальный признак :" << fiscalization.docMac;
}

void PrinterTest::sendBlock()
{
    qDebug() << "sendBlock";

    QByteArray block;
    QByteArray answer;
    if (!printer->readBlock(block)){
        qDebug() << "No receipt data available";
        return;
    }

    // telnet 109.73.43.4 19082
    ServerParams params;
    params.address = "109.73.43.4";
    params.port = 19082;
    params.connectTimeout = 20000;
    params.readTimeout = 20000;
    params.writeTimeout = 20000;
    params.connectRetries = 1;
    printer->setServerParams(params);

    qDebug() << "-> " << StringUtils::dataToHex(block);
    if (!printer->sendBlock(block, answer))
    {
        qDebug() << "sendBlock: FAILED!";
        return;
    }
    qDebug() << "<- " << StringUtils::dataToHex(block);
    printer->writeBlock(answer);
    qDebug() << "sendBlock: OK";
}


void PrinterTest::sendBlocks()
{
    qDebug() << "sendBlocks";

    // telnet 109.73.43.4 19082
    ServerParams params;
    params.address = "109.73.43.4";
    params.port = 19082;
    params.connectTimeout = 20000;
    params.readTimeout = 20000;
    params.writeTimeout = 20000;
    params.connectRetries = 1;
    printer->setServerParams(params);

    printer->sendBlocks();
    qDebug() << "sendBlocks: OK";
}

void PrinterTest::testSprintf()
{
    QString buffer;
    qDebug() << buffer.sprintf("%.2f", 0.455);
    qDebug() << buffer.sprintf("%.3f", 0.455);
    qDebug() << buffer.sprintf("%.3f", 1.0);
    qDebug() << buffer.sprintf("%.03f", 1.0);
    qDebug() << buffer.sprintf("%.12d", 123);
}


// SM-MOBILE-PTK#12345678"  ( "68:AA:D2:01:D7:8E" )"

void PrinterTest::connectPrinter()
{
    printer->connectDevice();
    qDebug("Printer connected!");
}

void PrinterTest::disconnectPrinter()
{
    printer->disconnectDevice();
    qDebug("Printer disconnected!");
}

void PrinterTest::readFileTest()
{
    qDebug() << "readFileTest";

    QString line1 = "Привет, Мир 4!!!";

    QFile file("journal.txt");
    //file.remove();
    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        file.seek(file.size());
        QTextStream out(&file);
        out.setCodec("UTF-8");
        qDebug() << "out.pos() = " << out.pos();
        qDebug() << "out.atEnd() = " << out.atEnd();

        out << line1 << endl;
        file.close();
    }

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        while (!in.atEnd()){
            QString line = in.readLine();
            qDebug() << "readLine: " << line;
        }
    }
    qDebug() << "Test failed !!!";
}

void PrinterTest::printBarcode()
{
    PrinterBarcode barcode;
    barcode.text = "Barcode test";
    barcode.type = BARCODE_QRCODE;
    printer->printBarcode(barcode);
}

void PrinterTest::printImages()
{
    qDebug() << "printImages()";
    printer->clearImages();
    //printImage(":/images/res/Logo.bmp");

    /*

    QImage image(100, 100, QImage::Format_MonoLSB);
    image.fill(Qt::white);
    QPainter painter(&image);
    painter.setPen(Qt::black);
    painter.setPen();
    painter.drawLine();
    */
}

void PrinterTest::printImage(QString path)
{
    QImage image;
    bool loaded = image.load(path);
    if (!loaded) {
        qDebug() << "Failed to load image '" << path << "'";
        return;
    }
    printImage(image);
}

void PrinterTest::printImage(QImage image)
{
    (void) image;
    /*
    PrintStringCommand cmd;
    cmd.flags = 2;
    cmd.text = path;
    printer->printString(cmd);

    QElapsedTimer timer;
    timer.start();
    printer->printImage3(image);

    QString text;
    text = QString("Printed in %1 ms.").arg(timer.elapsed());
    cmd.text = text;
    printer->printString(cmd);
    */
}

void PrinterTest::startDeviceDiscovery()
{
    qDebug("PrinterTest::startDiscovery");
    if (localDevice.isValid()) {

        qDebug() << "Bluetooth is available on this device";
        localDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
        localDevice.powerOn();
        localDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
        qDebug() << "Local device: " << localDevice.name() << " ("
                 << localDevice.address().toString().trimmed() << ")";

        // Create a discovery agent and connect to its signals

        connect(&discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(deviceDiscovered(QBluetoothDeviceInfo)));

        connect(&discoveryAgent, SIGNAL(finished()),
            this, SLOT(deviceDiscoverFinished()));

        discoveryAgent.start();
        qDebug() << "Device discover started";
    } else {
        qDebug() << "Bluetooth is not available on this device";
    }
}

void PrinterTest::startServiceDiscovery()
{
    qDebug() << "startServiceDiscovery";

    // Create a discovery agent and connect to its signals
    QBluetoothServiceDiscoveryAgent* discoveryAgent = new QBluetoothServiceDiscoveryAgent(this);
    connect(discoveryAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
        this, SLOT(serviceDiscovered(QBluetoothServiceInfo)));
    // Start a discovery
    discoveryAgent->start();
}

// In your local slot, read information about the found devices
void PrinterTest::serviceDiscovered(const QBluetoothServiceInfo& service)
{
    qDebug() << "Found new service:" << service.serviceName()
             << "(" << service.device().address().toString() << "): "
             << service.serviceUuid();
}

void PrinterTest::deviceDiscovered(const QBluetoothDeviceInfo& device)
{
    qDebug() << "Device discovered";

    qDebug() << device.name().trimmed()
             << " ("
             << device.address().toString().trimmed()
             << ")";
    QList<QBluetoothUuid> services = device.serviceUuids();
    for (int i = 0; i < services.length(); i++) {
        QBluetoothUuid uuid = services.at(i);
        qDebug() << uuid.Name << ": " << uuid.toString();
    }
}

/**
*   Device discover finished. Get a list of devices.
*   Read information about the found devices,
*   print to qDebug() a list of devices names and addresses and
*   send it to GUI (QML), where user can select a prefered device.
*/

void PrinterTest::deviceDiscoverFinished()
{
    qDebug() << "Device discover finished";

    QList<QBluetoothDeviceInfo> devices = discoveryAgent.discoveredDevices();

    if (devices.isEmpty()) {
        qDebug() << "No devices found";
        return;
    }

    qDebug() << "Found new devices:";
    for (int i = 0; i < devices.size(); i++) {
        QBluetoothDeviceInfo device = devices.at(i);
        qDebug() << device.name().trimmed()
                 << " ("
                 << device.address().toString().trimmed()
                 << ")";
    }
}

void PrinterTest::check(int rc)
{
    if (rc != 0) {
        qDebug() << "FAILED: " << ShtrihFiscalPrinter::getErrorText2(rc);
    }
}

void PrinterTest::beep()
{
    qDebug("Printer beep...");
    BeepCommand beepCommand;
    check(printer->beep(beepCommand));
}

void PrinterTest::readShortStatus()
{
    qDebug("Read short status...");
    ReadShortStatusCommand command;
    check(printer->readShortStatus(command));
    if (printer->succeeded(command.resultCode))
    {
        qDebug() << "Read short status: OK";
        /*
        qDebug() << "Operator                 : " << command.operatorNumber;

        qDebug().nospace() << "Flags                    : 0x" << StringUtils::intToHex(command.flags, 2) << ", " << command.flags;

        qDebug() << "Mode                     : 0x" << StringUtils::intToHex(command.mode, 1) << ", " << command.mode << ", " << printer->getModeText(command.mode);

        qDebug() << "Submode                  : 0x" << StringUtils::intToHex(command.submode, 1) << ", " << command.submode << ", " << printer->getSubmodeText(command.submode);

        qDebug() << "Number of operations     : " << command.numOperations;
        qDebug() << "Fiscal memory error      : " << command.fmError;
        qDebug() << "Electronic journal error : " << command.ejError;
        qDebug() << "Battery voltage          : " << command.batteryVoltage;
        qDebug() << "Supply voltage           : " << command.supplyVoltage;
        */
    }
}

void PrinterTest::readLongStatus()
{
    qDebug("Read long status...");
    ReadLongStatusCommand command;
    check(printer->readLongStatus(command));
    if (printer->succeeded(command.resultCode)) {
        qDebug() << "Operator number : " << command.operatorNumber;
        qDebug() << "Flags           : " << command.flags;
        qDebug() << "Mode            : " << command.mode;
        qDebug() << "Submode         : " << command.submode;
        qDebug() << "Document number : " << command.documentNumber;
        qDebug() << "Fiscal ID       : " << command.fiscalID;
    }
}

void PrinterTest::printBoldString()
{
    qDebug("Print bold string...");
    PrintBoldCommand command;
    command.flags = 0x03;
    command.text = "Привет, мир!!!";
    check(printer->printBold(command));
}

void PrinterTest::writePortParameters()
{
    qDebug("Set communication parameters...");
    PortParametersCommand command;
    command.port = 0;
    command.timeout = 1000;
    command.baudRate = 115200;
    check(printer->writePortParameters(command));
}

void PrinterTest::readPortParameters()
{
    qDebug("Read port parameters...");
    PortParametersCommand command;
    command.port = 0;
    command.baudRate = 0;
    command.timeout = 0;
    check(printer->readPortParameters(command));
    if (printer->succeeded(command.resultCode)) {
        qDebug() << "BaudRate : " << command.baudRate;
        qDebug() << "Timeout  : " << command.timeout;
    }
}

void PrinterTest::printString()
{
    PrintStringCommand command;
    command.flags = 3;
    command.text = "Привет, мир!!!";
    check(printer->printString(command));
}

void PrinterTest::printDocHeader()
{
    // Print document header
    qDebug("Print document header...");
    PrintDocHeaderCommand command;
    command.text = "Новый документ";
    command.number = 1234;
    check(printer->printDocHeader(command));
}

void PrinterTest::startTest()
{
    // Start test mode
    qDebug("Start test mode...");
    StartTestCommand command;
    command.periodInMinutes = 1;
    check(printer->startTest(command));
}

void PrinterTest::readCashRegisters()
{
    qDebug("Read cash register...");
    ReadCashRegCommand command;
    for (int i = 0; i < 0xFF; i++) {
        command.number = i;
        printer->readRegister(command);
        if (printer->failed(command.resultCode))
            break;
        qDebug() << "Cash register " << command.number
                 << ": " << command.value;
    }
    check(command.resultCode);
}

void PrinterTest::readOperRegisters()
{
    qDebug("Read operation register...");
    ReadOperRegCommand command;
    for (int i = 0; i < 0xFF; i++) {
        command.number = i;
        printer->readRegister(command);
        if (printer->failed(command.resultCode))
            break;
        qDebug() << "Operation register " << command.number
                 << ": " << command.value;
    }
    check(command.resultCode);
}

void PrinterTest::readTable()
{
    qDebug("Write table...");
    QByteArray ba;
    ba.append(0x12);
    TableValueCommand command;
    command.table = 1;
    command.row = 1;
    command.field = 1;
    command.value = ba;
    check(printer->writeTable(command));
    if (command.resultCode != 0)
        return;
    qDebug() << "Table 1.1.1 = " << command.value;
    qDebug("Read table...");
    command.value = "";
    check(printer->readTable(command));
    if (command.resultCode != 0)
        return;
    qDebug() << "Table 1.1.1 = " << command.value;
}

void PrinterTest::printReceipts()
{
    for (int i=0;i<3;i++){
        printSaleReceipt();
    }
}

void PrinterTest::printReceiptCopy()
{
    qDebug() << "printReceiptCopy";
    PasswordCommand command;
    check(printer->printCopy(command));
}

void PrinterTest::printSaleReceipt()
{
    qDebug() << "printSaleReceipt.0";

    check(printer->resetPrinter());
    // Open receipt
    OpenReceiptCommand openReceipt;
    openReceipt.receiptType = SMFP_RECTYPE_SALE;
    check(printer->openReceipt(openReceipt));
    check(printer->waitForPrinting());
    // Item 1
    QString itemText = "Line1";
    ReceiptItemCommand itemCommand;
    itemCommand.quantity = 1000;
    itemCommand.price = 123456;
    itemCommand.department = 1;
    itemCommand.tax1 = 1;
    itemCommand.tax2 = 0;
    itemCommand.tax3 = 0;
    itemCommand.tax4 = 0;
    itemCommand.text = itemText;
    check(printer->printSale(itemCommand));
    check(printer->waitForPrinting());
    // Close receipt
    CloseReceiptCommand closeCommand;
    closeCommand.amount1 = 123456;
    closeCommand.amount2 = 0;
    closeCommand.amount3 = 0;
    closeCommand.amount4 = 0;
    closeCommand.discount = 0;
    closeCommand.tax1 = 0;
    closeCommand.tax2 = 0;
    closeCommand.tax3 = 0;
    closeCommand.tax4 = 0;
    closeCommand.text = "Закрытие чека";
    check(printer->closeReceipt(closeCommand));
    //check(printer->waitForPrinting());

    qDebug() << "printSaleReceipt.1";
}

void PrinterTest::printRefundReceipt()
{
    // Cancel receipt
    PasswordCommand command;
    check(printer->cancelReceipt(command));
    check(printer->waitForPrinting());
    // Item 1
    ReceiptItemCommand itemCommand;
    itemCommand.quantity = 1000;
    itemCommand.price = 123456;
    itemCommand.department = 1;
    itemCommand.tax1 = 0;
    itemCommand.tax2 = 0;
    itemCommand.tax3 = 0;
    itemCommand.tax4 = 0;
    itemCommand.text = "Товар 1";
    check(printer->printRetSale(itemCommand));
    check(printer->waitForPrinting());
    // Close receipt
    CloseReceiptCommand closeCommand;
    closeCommand.amount1 = 123456;
    closeCommand.amount2 = 0;
    closeCommand.amount3 = 0;
    closeCommand.amount4 = 0;
    closeCommand.discount = 0;
    closeCommand.tax1 = 0;
    closeCommand.tax2 = 0;
    closeCommand.tax3 = 0;
    closeCommand.tax4 = 0;
    closeCommand.text = "Закрытие чека";
    check(printer->closeReceipt(closeCommand));
    check(printer->waitForPrinting());
}

void PrinterTest::printXReport()
{
    PasswordCommand command;
    printer->printXReport(command);
    check(printer->waitForPrinting());
}

void PrinterTest::printDayIsOpened()
{
    if (printer->isDayOpened()) {
        qDebug() << "Day is opened";
    } else {
        qDebug() << "Day is closed";
    }
}

void PrinterTest::printZReport()
{
    printDayIsOpened();

    PasswordCommand command;
    printer->printZReport(command);
    //check(printer->waitForPrinting());

    printDayIsOpened();
}

/*
QGuiApplication* app = SailfishApp::application(argc, argv);
QString fileName = ":/translations/translations/QmlTest.qm";
QFile file(fileName);
if (!file.exists()){
    qDebug() << "File not exists, " << fileName;
}

QTranslator translator;
if (!translator.load(fileName)){
    qDebug() << "Failed to load translation file, " << fileName;
}
if (!app->installTranslator(&translator)){
    qDebug() << "Failed to install translation";
}
*/

