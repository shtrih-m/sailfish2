
#ifndef PRINTERTEST_H
#define PRINTERTEST_H

#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QBluetoothLocalDevice>
#include <QBluetoothServiceInfo>

#include "printertest.h"
#include "shtrihfiscalprinter.h"
#include "bluetoothport2.h"
#include "printerprotocol1.h"
#include "printerprotocol2.h"
#include "logger.h"


class PrinterTest : public QObject {
    Q_OBJECT
public:
    explicit PrinterTest(QObject* parent = 0);
    ~PrinterTest();

    void execute();
    void startDeviceDiscovery();
    void startServiceDiscovery();
public Q_SLOTS:
    void deviceDiscoverFinished();
    void deviceDiscovered(const QBluetoothDeviceInfo& device);
    void serviceDiscovered(const QBluetoothServiceInfo& service);

private:
    int password;
    Logger* logger;
    BluetoothPort2* port;
    PrinterProtocol2* protocol;
    ShtrihFiscalPrinter* printer;


    QBluetoothLocalDevice localDevice;
    QBluetoothDeviceDiscoveryAgent discoveryAgent;

    int readMode();
    void setModeClosed();
    void checkMode(int mode);
    void testCorrectDate();
    void testCorrectDate2();
    void testResetPrinter();
    void testSale2();
    void testWriteTLVOperation();
    void testHeaderTrailer();
    void testHeaderEnabled();
    void show(QStringList lines);
    void fsReadStatus();
    void printReceipts();
    void sendBlock();
    void sendBlocks();
    void testSprintf();
    void connectPrinter();
    void disconnectPrinter();
    void readFileTest();
    void check(int rc);
    void beep();
    void readShortStatus();
    void readLongStatus();
    void printBoldString();
    void writePortParameters();
    void readPortParameters();
    void printString();
    void printDocHeader();
    void startTest();
    void readCashRegisters();
    void readOperRegisters();
    void readTable();
    void printSaleReceipt();
    void printRefundReceipt();
    void printXReport();
    void printZReport();
    void printDayIsOpened();
    void printBarcode();
    void printImages();
    void printImage(QString path);
    void printImage(QImage image);
    void waitForDocuments();
    void testFile();
    void testTextFile();
    void printReceiptCopy();
    void readLastDocMac();
    void printText();
    void deleteLogFile();
    void showLogFile();
};

#endif // PRINTERTEST_H
