#ifndef FISCALPRINTERSERVICE_H
#define FISCALPRINTERSERVICE_H

#include <QString>
#include <QException>
#include "textlogger.h"
#include "upos/services/FiscalPrinterService114.h"


class FiscalPrinterService: public FiscalPrinterService114
{
private:
    TextLogger logger;
    FiscalPrinterService114* impl;
public:
    FiscalPrinterService();
    void handleException(QException e);

    // BaseService
    // Properties
    QString  getCheckHealthText();
    bool getClaimed();
    bool getDeviceEnabled();
    void    setDeviceEnabled(bool deviceEnabled)
                      ;
    QString  getDeviceServiceDescription();
    int     getDeviceServiceVersion();
    bool getFreezeEvents();
    void    setFreezeEvents(bool freezeEvents);
    QString  getPhysicalDeviceDescription();
    QString  getPhysicalDeviceName();
    int     getState();

    // Methods
    void    claim(int timeout);
    void    close();
    void    checkHealth(int level);
    void    directIO(int command, int[] data, Object object)
                      ;
    void    open(QString logicalName, EventCallbacks cb)
                       ;
    void    release();

    // Capabilities
    bool getCapAdditionalLines();
    bool getCapAmountAdjustment();
    bool getCapAmountNotPaid();
    bool getCapCheckTotal();
    bool getCapCoverSensor();
    bool getCapDoubleWidth();
    bool getCapDuplicateReceipt();
    void setDuplicateReceipt(bool duplicateReceipt);
    bool getCapFixedOutput();
    bool getCapHasVatTable();
    bool getCapIndependentHeader();
    bool getCapItemList();
    bool getCapJrnEmptySensor();
    bool getCapJrnNearEndSensor();
    bool getCapJrnPresent();
    bool getCapNonFiscalMode();
    bool getCapOrderAdjustmentFirst();
    bool getCapPercentAdjustment();
    bool getCapPositiveAdjustment();
    bool getCapPowerLossReport();
    int     getCapPowerReporting();
    bool getCapPredefinedPaymentLines();
    bool getCapReceiptNotPaid();
    bool getCapRecEmptySensor();
    bool getCapRecNearEndSensor();
    bool getCapRecPresent();
    bool getCapRemainingFiscalMemory();
    bool getCapReservedWord();
    bool getCapSetHeader();
    bool getCapSetPOSID();
    bool getCapSetStoreFiscalID();
    bool getCapSetTrailer();
    bool getCapSetVatTable();
    bool getCapSlpEmptySensor();
    bool getCapSlpFiscalDocument();
    bool getCapSlpFullSlip();
    bool getCapSlpNearEndSensor();
    bool getCapSlpPresent();
    bool getCapSlpValidation();
    bool getCapSubAmountAdjustment();
    bool getCapSubPercentAdjustment();
    bool getCapSubtotal();
    bool getCapTrainingMode();
    bool getCapValidateJournal();
    bool getCapXReport();

    // Properties
    int     getOutputID();
    int     getPowerNotify();
    void    setPowerNotify(int powerNotify);
    int     getPowerState();

    int     getAmountDecimalPlace();
    bool getAsyncMode();
    void    setAsyncMode(bool asyncMode);
    bool getCheckTotal();
    void    setCheckTotal(bool checkTotal);
    int     getCountryCode();
    bool getCoverOpen();
    bool getDayOpened();
    int     getDescriptionLength();
    bool getDuplicateReceipt();
    int     getErrorLevel();
    int     getErrorOutID();
    int     getErrorState();
    int     getErrorStation();
    QString getErrorQString();
    bool getFlagWhenIdle();
    void    setFlagWhenIdle(bool flagWhenIdle);
    bool getJrnEmpty();
    bool getJrnNearEnd();
    int     getMessageLength();
    int     getNumHeaderLines();
    int     getNumTrailerLines();
    int     getNumVatRates();
    QString  getPredefinedPaymentLines();
    int     getPrinterState();
    int     getQuantityDecimalPlaces();
    int     getQuantityLength();
    bool getRecEmpty();
    bool getRecNearEnd();
    int     getRemainingFiscalMemory();
    QString  getReservedWord();
    bool getSlpEmpty();
    bool getSlpNearEnd();
    int     getSlipSelection();
    void    setSlipSelection(int slipSelection);
    bool getTrainingModeActive();

    // Methods
    void    beginFiscalDocument(int documentAmount)
                      ;
    void    beginFiscalReceipt(bool printHeader)
                      ;
    void    beginFixedOutput(int station, int documentType)
                      ;
    void    beginInsertion(int timeout);
    void    beginItemList(int vatID);
    void    beginNonFiscal();
    void    beginRemoval(int timeout);
    void    beginTraining();
    void    clearError();
    void    clearOutput();
    void    endFiscalDocument();
    void    endFiscalReceipt(bool printHeader);
    void    endFixedOutput();
    void    endInsertion();
    void    endItemList();
    void    endNonFiscal();
    void    endRemoval();
    void    endTraining();
    void    getData(int dataItem, int* optArgs, QString* data);
    QString    getDate();
    QString getTotalizer(int vatID, int optArgs);
    int getVatEntry(int vatID, int optArgs);
    void  printDuplicateReceipt();
    void    printFiscalDocumentLine(QString documentLine)
                      ;
    void    printFixedOutput(int documentType, int lineNumber,
                       QString data);
    void    printNormal(int station, QString data);
    void    printPeriodicTotalsReport(QString date1, QString date2)
                      ;
    void    printPowerLossReport();
    void    printRecItem(QString description, long price, int quantity,
                       int vatInfo, long unitPrice, QString unitName)
                      ;
    void    printRecItemAdjustment(int adjustmentType,
                       QString description, long amount, int vatInfo)
                      ;
    void    printRecMessage(QString message);
    void    printRecNotPaid(QString description, long amount)
                      ;
    void    printRecRefund(QString description, long amount, int vatInfo)
                      ;
    void    printRecSubtotal(long amount);
    void    printRecSubtotalAdjustment(int adjustmentType,
                       QString description, long amount);
    void    printRecTotal(long total, long payment, QString description)
                      ;
    void    printRecVoid(QString description);
    void    printRecVoidItem(QString description, long amount,
                       int quantity, int adjustmentType, long adjustment,
                       int vatInfo);
    void    printReport(int reportType, QString startNum, QString endNum);
    void  printXReport();
    void  printZReport();
    void  resetPrinter();
    void  setDate(QString date);
    void  setHeaderLine(int lineNumber, QString text,
                       bool doubleWidth);
    void    setPOSID(QString POSID, QString cashierID)
                      ;
    void    setStoreFiscalID(QString ID);
    void    setTrailerLine(int lineNumber, QString text,
                       bool doubleWidth);
    void    setVatTable();
    void    setVatValue(int vatID, QString vatValue)
                      ;
    void    verifyItem(QString itemName, int vatID);

    // FiscalPrinterService16
        // Capabilities
        virtual bool getCapAdditionalHeader();
        virtual bool getCapAdditionalTrailer();
        virtual bool getCapChangeDue();
        virtual bool getCapEmptyReceiptIsVoidable();
        virtual bool getCapFiscalReceiptStation();
        virtual bool getCapFiscalReceiptType();
        virtual bool getCapMultiContractor();
        virtual bool getCapOnlyVoidLastItem();
        virtual bool getCapPackageAdjustment();
        virtual bool getCapPostPreLine();
        virtual bool getCapSetCurrency();
        virtual bool getCapTotalizerType();

        // Properties
        virtual int     getActualCurrency();
        virtual QString  getAdditionalHeader();
        virtual void    setAdditionalHeader(QString additionalHeader)
                          ;
        virtual QString  getAdditionalTrailer();
        virtual void    setAdditionalTrailer(QString additionalTrailer)
                          ;
        virtual QString  getChangeDue();
        virtual void    setChangeDue(QString changeDue);
        virtual int     getContractorId();
        virtual void    setContractorId(int contractorId);
        virtual int     getDateType();
        virtual void    setDateType(int dateType);
        virtual int     getFiscalReceiptStation();
        virtual void    setFiscalReceiptStation(int fiscalReceiptStation)
                          ;
        virtual int     getFiscalReceiptType();
        virtual void    setFiscalReceiptType(int fiscalReceiptType)
                          ;
        virtual int     getMessageType();
        virtual void    setMessageType(int messageType);
        virtual QString  getPostLine();
        virtual void    setPostLine(QString postLine);
        virtual QString  getPreLine();
        virtual void    setPreLine(QString preLine);
        virtual int     getTotalizerType();
        virtual void    setTotalizerType(int totalizerType);

        // Methods
        virtual void    setCurrency(int newCurrency);
        virtual void    printRecCash(long amount);
        virtual void    printRecItemFuel(QString description, long price,
                           int quantity, int vatInfo, long unitPrice,
                           QString unitName, long specialTax, QString specialTaxName)
                          ;
        virtual void    printRecItemFuelVoid(QString description, long price,
                           int vatInfo, long specialTax);
        virtual void    printRecPackageAdjustment(int adjustmentType,
                           QString description, QString vatAdjustment)
                          ;
        virtual void    printRecPackageAdjustVoid(int adjustmentType,
                           QString vatAdjustment);
        virtual void    printRecRefundVoid(QString description, long amount,
                           int vatInfo);
        virtual void    printRecSubtotalAdjustVoid(int adjustmentType, long amount)
                          ;
        virtual void    printRecTaxID(QString taxID);


    // FiscalPrinterService16
    virtual int getAmountDecimalPlaces();

    // FiscalPrinterService18
  // Capabilities
  virtual bool getCapCompareFirmwareVersion();
  virtual bool getCapUpdateFirmware();
  // Methods
  virtual int compareFirmwareVersion(QString firmwareFileName);
  virtual void updateFirmware(QString firmwareFileName);

    // FiscalPrinterService111
      // Methods
      virtual void printRecItemRefund(QString description,
                                        long amount,
                                        int quantity,
                                        int vatInfo,
                                        long unitAmount,
                                        QString unitName);
      virtual void printRecItemRefundVoid(QString description,
                                            long amount,
                                            int quantity,
                                            int vatInfo,
                                            long unitAmount,
                                            QString unitName)= 0;
};

#endif // FISCALPRINTERSERVICE_H
