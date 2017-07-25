#include "fiscalprinterservice.h"

FiscalPrinterService::FiscalPrinterService()
{

}

void FiscalPrinterService::handleException(QException e) {
    //impl->handleException(e); !!!
}


bool FiscalPrinterService::getCapCompareFirmwareVersion()
{
    logger.debug("getCapCompareFirmwareVersion()");
    bool result = false;
    try {
        result = impl->getCapCompareFirmwareVersion();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapCompareFirmwareVersion = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapUpdateFirmware() {
    logger.debug("getCapUpdateFirmware()");
    bool result = false;
    try {
        result = impl->getCapUpdateFirmware();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapUpdateFirmware = " + QString::number(result));
    return result;
}


int FiscalPrinterService::compareFirmwareVersion(QString firmwareFileName)
{
    logger.debug("compareFirmwareVersion(" + firmwareFileName + ")");
    try {
        return impl->compareFirmwareVersion(firmwareFileName);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("compareFirmwareVersion: OK");
}


void FiscalPrinterService::updateFirmware(QString firmwareFileName) {
    logger.debug("updateFirmware(" + firmwareFileName + ")");
    try {
        impl->updateFirmware(firmwareFileName);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("updateFirmware: OK");
}


QString FiscalPrinterService::getCheckHealthText() {
    logger.debug("getCheckHealthText()");

    QString result = "";
    try {
        result = impl->getCheckHealthText();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCheckHealthText = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getClaimed() {
    logger.debug("getClaimed()");
    bool result = false;
    try {
        result = impl->getClaimed();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getClaimed = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getDeviceEnabled() {
    logger.debug("getDeviceEnabled()");
    bool result = false;
    try {
        result = impl->getDeviceEnabled();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getDeviceEnabled = " + QString::number(result));
    return result;
}


void FiscalPrinterService::setDeviceEnabled(bool deviceEnabled) {
    logger.debug("setDeviceEnabled(" + QString::number(deviceEnabled) + ")");
    try {
        impl->setDeviceEnabled(deviceEnabled);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setDeviceEnabled: OK");
}


QString FiscalPrinterService::getDeviceServiceDescription() {
    logger.debug("getDeviceServiceDescription()");
    QString result = "";
    try {
        result = impl->getDeviceServiceDescription();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getDeviceServiceDescription = " + QString::number(result));
    return result;
}


int FiscalPrinterService::getDeviceServiceVersion() {
    logger.debug("getDeviceServiceVersion()");
    int result = 0;
    try {
        result = impl->getDeviceServiceVersion();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getDeviceServiceVersion = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getFreezeEvents() {
    logger.debug("getFreezeEvents()");
    bool result = false;
    try {
        result = impl->getFreezeEvents();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getFreezeEvents = " + QString::number(result));
    return result;
}


void FiscalPrinterService::setFreezeEvents(bool freezeEvents) {
    logger.debug("setFreezeEvents(" + QString::number(freezeEvents) + ")");
    try {
        impl->setFreezeEvents(freezeEvents);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setFreezeEvents: OK");
}


QString FiscalPrinterService::getPhysicalDeviceDescription() {
    logger.debug("getPhysicalDeviceDescription()");
    QString result = "";
    try {
        result = impl->getPhysicalDeviceDescription();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getPhysicalDeviceDescription = " + QString::number(result));
    return result;
}


QString FiscalPrinterService::getPhysicalDeviceName() {
    logger.debug("getPhysicalDeviceName()");
    QString result = "";
    try {
        result = impl->getPhysicalDeviceName();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getPhysicalDeviceName = " + QString::number(result));
    return result;
}


int FiscalPrinterService::getState() {
    logger.debug("getState()");
    int result = 0;
    try {
        result = impl->getState();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getState = " + QString::number(result));
    return result;
}


void FiscalPrinterService::claim(int timeout) {
    logger.debug("claim(" + QString::number(timeout) + ")");
    try {
        impl->claim(timeout);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("claim: OK");
}


void FiscalPrinterService::close() {
    logger.debug("close()");
    try {
        impl->close();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("close: OK");
}


void FiscalPrinterService::checkHealth(int level) {
    logger.debug("checkHealth(" + QString::number(level) + ")");
    try {
        impl->checkHealth(level);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("checkHealth: OK");
}


void FiscalPrinterService::directIO(int command, int[] data, Object object)
        {
    logger.debug("directIO(" + QString::number(command) + ", "
            + QString::number(data) + ", " + QString::number(object) + ")");
    try {
        impl->directIO(command, data, object);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("directIO: OK");
}


void FiscalPrinterService::setJposEntry(JposEntry entry) {
    jposEntry = entry;
}


void FiscalPrinterService::open(QString logicalName, EventCallbacks cb)
        {
    logger.debug("open(" + QString::number(logicalName) + ")");
    try {
        impl->setJposEntry(jposEntry);
        impl->open(logicalName, cb);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("open: OK");
}


void FiscalPrinterService::release() {
    logger.debug("release()");
    try {
        impl->release();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("release: OK");
}


bool FiscalPrinterService::getCapAdditionalLines() {
    logger.debug("getCapAdditionalLines()");
    bool result = false;
    try {
        result = impl->getCapAdditionalLines();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapAdditionalLines = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapAmountAdjustment() {
    logger.debug("getCapAmountAdjustment()");
    bool result = false;
    try {
        result = impl->getCapAmountAdjustment();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapAmountAdjustment = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapAmountNotPaid() {
    logger.debug("getCapAmountNotPaid()");
    bool result = false;
    try {
        result = impl->getCapAmountNotPaid();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapAmountNotPaid = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapCheckTotal() {
    logger.debug("getCapCheckTotal()");
    bool result = false;
    try {
        result = impl->getCapCheckTotal();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapCheckTotal = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapCoverSensor() {
    logger.debug("getCapCoverSensor()");
    bool result = false;
    try {
        result = impl->getCapCoverSensor();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapCoverSensor = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapDoubleWidth() {
    logger.debug("getCapDoubleWidth()");
    bool result = false;
    try {
        result = impl->getCapDoubleWidth();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapDoubleWidth = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapDuplicateReceipt() {
    logger.debug("getCapDuplicateReceipt()");
    bool result = false;
    try {
        result = impl->getCapDuplicateReceipt();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapDuplicateReceipt = " + QString::number(result));
    return result;
}


void FiscalPrinterService::setDuplicateReceipt(bool duplicateReceipt)
        {
    logger.debug("setDuplicateReceipt(" + QString::number(duplicateReceipt)
            + ")");
    try {
        impl->setDuplicateReceipt(duplicateReceipt);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setDuplicateReceipt: OK");
}


bool FiscalPrinterService::getCapFixedOutput() {
    logger.debug("getCapFixedOutput()");
    bool result = false;
    try {
        result = impl->getCapFixedOutput();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapFixedOutput = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapHasVatTable() {
    logger.debug("getCapHasVatTable()");
    bool result = false;
    try {
        result = impl->getCapHasVatTable();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapHasVatTable = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapIndependentHeader() {
    logger.debug("getCapIndependentHeader()");
    bool result = false;
    try {
        result = impl->getCapIndependentHeader();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapIndependentHeader = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapItemList() {
    logger.debug("getCapItemList()");
    bool result = false;
    try {
        result = impl->getCapItemList();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapItemList = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapJrnEmptySensor() {
    logger.debug("getCapJrnEmptySensor()");
    bool result = false;
    try {
        result = impl->getCapJrnEmptySensor();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapJrnEmptySensor = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapJrnNearEndSensor() {
    logger.debug("getCapJrnNearEndSensor()");
    bool result = false;
    try {
        result = impl->getCapJrnNearEndSensor();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapJrnNearEndSensor = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapJrnPresent() {
    logger.debug("getCapJrnPresent()");
    bool result = false;
    try {
        result = impl->getCapJrnPresent();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapJrnPresent = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapNonFiscalMode() {
    logger.debug("getCapNonFiscalMode()");
    bool result = false;
    try {
        result = impl->getCapNonFiscalMode();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapNonFiscalMode = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapOrderAdjustmentFirst() {
    logger.debug("getCapOrderAdjustmentFirst()");
    bool result = false;
    try {
        result = impl->getCapOrderAdjustmentFirst();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapOrderAdjustmentFirst = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapPercentAdjustment() {
    logger.debug("getCapPercentAdjustment()");
    bool result = false;
    try {
        result = impl->getCapPercentAdjustment();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapPercentAdjustment = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapPositiveAdjustment() {
    logger.debug("getCapPositiveAdjustment()");
    bool result = false;
    try {
        result = impl->getCapPositiveAdjustment();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapPositiveAdjustment = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapPowerLossReport() {
    logger.debug("getCapPowerLossReport()");
    bool result = false;
    try {
        result = impl->getCapPowerLossReport();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapPowerLossReport = " + QString::number(result));
    return result;
}


int FiscalPrinterService::getCapPowerReporting() {
    logger.debug("getCapPowerReporting()");
    int result = 0;
    try {
        result = impl->getCapPowerReporting();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapPowerReporting = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapPredefinedPaymentLines() {
    logger.debug("getCapPredefinedPaymentLines()");
    bool result = false;
    try {
        result = impl->getCapPredefinedPaymentLines();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapPredefinedPaymentLines = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapReceiptNotPaid() {
    logger.debug("getCapReceiptNotPaid()");
    bool result = false;
    try {
        result = impl->getCapReceiptNotPaid();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapReceiptNotPaid = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapRecEmptySensor() {
    logger.debug("getCapRecEmptySensor()");
    bool result = false;
    try {
        result = impl->getCapRecEmptySensor();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapRecEmptySensor = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapRecNearEndSensor() {
    logger.debug("getCapRecNearEndSensor()");
    bool result = false;
    try {
        result = impl->getCapRecNearEndSensor();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapRecNearEndSensor = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapRecPresent() {
    logger.debug("getCapRecPresent()");
    bool result = false;
    try {
        result = impl->getCapRecPresent();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapRecPresent = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapRemainingFiscalMemory() {
    logger.debug("getCapRemainingFiscalMemory()");
    bool result = false;
    try {
        result = impl->getCapRemainingFiscalMemory();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapRemainingFiscalMemory = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapReservedWord() {
    logger.debug("getCapReservedWord()");
    bool result = false;
    try {
        result = impl->getCapReservedWord();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapReservedWord = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapSetHeader() {
    logger.debug("getCapSetHeader()");
    bool result = false;
    try {
        result = impl->getCapSetHeader();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapSetHeader = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapSetPOSID() {
    logger.debug("getCapSetPOSID()");
    bool result = false;
    try {
        result = impl->getCapSetPOSID();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapSetPOSID = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapSetStoreFiscalID() {
    logger.debug("getCapSetStoreFiscalID()");
    bool result = false;
    try {
        result = impl->getCapSetStoreFiscalID();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapSetStoreFiscalID = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapSetTrailer() {
    logger.debug("getCapSetTrailer()");
    bool result = false;
    try {
        result = impl->getCapSetTrailer();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapSetTrailer = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapSetVatTable() {
    logger.debug("getCapSetVatTable()");
    bool result = false;
    try {
        result = impl->getCapSetVatTable();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapSetVatTable = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapSlpEmptySensor() {
    logger.debug("getCapSlpEmptySensor()");
    bool result = false;
    try {
        result = impl->getCapSlpEmptySensor();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapSlpEmptySensor = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapSlpFiscalDocument() {
    logger.debug("getCapSlpFiscalDocument()");
    bool result = false;
    try {
        result = impl->getCapSlpFiscalDocument();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapSlpFiscalDocument = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapSlpFullSlip() {
    logger.debug("getCapSlpFullSlip()");
    bool result = false;
    try {
        result = impl->getCapSlpFullSlip();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapSlpFullSlip = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapSlpNearEndSensor() {
    logger.debug("getCapSlpNearEndSensor()");
    bool result = false;
    try {
        result = impl->getCapSlpNearEndSensor();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapSlpNearEndSensor = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapSlpPresent() {
    logger.debug("getCapSlpPresent()");
    bool result = false;
    try {
        result = impl->getCapSlpPresent();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapSlpPresent = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapSlpValidation() {
    logger.debug("getCapSlpValidation()");
    bool result = false;
    try {
        result = impl->getCapSlpValidation();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapSlpValidation = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapSubAmountAdjustment() {
    logger.debug("getCapSubAmountAdjustment()");
    bool result = false;
    try {
        result = impl->getCapSubAmountAdjustment();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapSubAmountAdjustment = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapSubPercentAdjustment() {
    logger.debug("getCapSubPercentAdjustment()");
    bool result = false;
    try {
        result = impl->getCapSubPercentAdjustment();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapSubPercentAdjustment = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapSubtotal() {
    logger.debug("getCapSubtotal()");
    bool result = false;
    try {
        result = impl->getCapSubtotal();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapSubtotal = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapTrainingMode() {
    logger.debug("getCapTrainingMode()");
    bool result = false;
    try {
        result = impl->getCapTrainingMode();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapTrainingMode = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapValidateJournal() {
    logger.debug("getCapValidateJournal()");
    bool result = false;
    try {
        result = impl->getCapValidateJournal();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapValidateJournal = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapXReport() {
    logger.debug("getCapXReport()");
    bool result = false;
    try {
        result = impl->getCapXReport();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapXReport = " + QString::number(result));
    return result;
}


int FiscalPrinterService::getOutputID() {
    logger.debug("getOutputID()");
    int result = 0;
    try {
        result = impl->getOutputID();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getOutputID = " + QString::number(result));
    return result;
}


int FiscalPrinterService::getPowerNotify() {
    logger.debug("getPowerNotify()");
    int result = 0;
    try {
        result = impl->getPowerNotify();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getPowerNotify = " + QString::number(result));
    return result;
}


void FiscalPrinterService::setPowerNotify(int powerNotify) {
    logger.debug("setPowerNotify(" + QString::number(powerNotify) + ")");
    try {
        impl->setPowerNotify(powerNotify);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setPowerNotify: OK");
}


int FiscalPrinterService::getPowerState() {
    logger.debug("getPowerState()");
    int result = 0;
    try {
        result = impl->getPowerState();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getPowerState = " + QString::number(result));
    return result;
}


int FiscalPrinterService::getAmountDecimalPlace() {
    logger.debug("getAmountDecimalPlace()");
    int result = 0;
    try {
        result = impl->getAmountDecimalPlace();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getAmountDecimalPlace = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getAsyncMode() {
    logger.debug("getAsyncMode()");
    bool result = false;
    try {
        result = impl->getAsyncMode();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getAsyncMode = " + QString::number(result));
    return result;
}


void FiscalPrinterService::setAsyncMode(bool asyncMode) {
    logger.debug("setAsyncMode(" + QString::number(asyncMode) + ")");
    try {
        impl->setAsyncMode(asyncMode);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setAsyncMode: OK");
}


bool FiscalPrinterService::getCheckTotal() {
    logger.debug("getCheckTotal()");
    bool result = false;
    try {
        result = impl->getCheckTotal();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCheckTotal = " + QString::number(result));
    return result;
}


void FiscalPrinterService::setCheckTotal(bool checkTotal) {
    logger.debug("setCheckTotal(" + QString::number(checkTotal) + ")");
    try {
        impl->setCheckTotal(checkTotal);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setCheckTotal: OK");
}


int FiscalPrinterService::getCountryCode() {
    logger.debug("getCountryCode()");
    int result = 0;
    try {
        result = impl->getCountryCode();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCountryCode = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCoverOpen() {
    logger.debug("getCoverOpen()");
    bool result = false;
    try {
        result = impl->getCoverOpen();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCoverOpen = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getDayOpened() {
    logger.debug("getDayOpened()");
    bool result = false;
    try {
        result = impl->getDayOpened();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getDayOpened = " + QString::number(result));
    return result;
}


int FiscalPrinterService::getDescriptionLength() {
    logger.debug("getDescriptionLength()");
    int result = 0;
    try {
        result = impl->getDescriptionLength();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getDescriptionLength = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getDuplicateReceipt() {
    logger.debug("getDuplicateReceipt()");
    bool result = false;
    try {
        result = impl->getDuplicateReceipt();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getDuplicateReceipt = " + QString::number(result));
    return result;
}


int FiscalPrinterService::getErrorLevel() {
    logger.debug("getErrorLevel()");
    int result = 0;
    try {
        result = impl->getErrorLevel();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getErrorLevel = " + QString::number(result));
    return result;
}


int FiscalPrinterService::getErrorOutID() {
    logger.debug("getErrorOutID()");
    int result = 0;
    try {
        result = impl->getErrorOutID();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getErrorOutID = " + QString::number(result));
    return result;
}


int FiscalPrinterService::getErrorState() {
    logger.debug("getErrorState()");
    int result = 0;
    try {
        result = impl->getErrorState();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getErrorState = " + QString::number(result));
    return result;
}


int FiscalPrinterService::getErrorStation() {
    logger.debug("getErrorStation()");
    int result = 0;
    try {
        result = impl->getErrorStation();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getErrorStation = " + QString::number(result));
    return result;
}


QString FiscalPrinterService::getErrorQString() {
    logger.debug("getErrorQString()");
    QString result = "";
    try {
        result = impl->getErrorQString();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getErrorQString = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getFlagWhenIdle() {
    logger.debug("getFlagWhenIdle()");
    bool result = false;
    try {
        result = impl->getFlagWhenIdle();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getFlagWhenIdle = " + QString::number(result));
    return result;
}


void FiscalPrinterService::setFlagWhenIdle(bool flagWhenIdle) {
    logger.debug("setFlagWhenIdle(" + QString::number(flagWhenIdle) + ")");
    try {
        impl->setFlagWhenIdle(flagWhenIdle);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setFlagWhenIdle: OK");
}


bool FiscalPrinterService::getJrnEmpty() {
    logger.debug("getJrnEmpty()");
    bool result = false;
    try {
        result = impl->getJrnEmpty();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getJrnEmpty = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getJrnNearEnd() {
    logger.debug("getJrnNearEnd()");
    bool result = false;
    try {
        result = impl->getJrnNearEnd();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getJrnNearEnd = " + QString::number(result));
    return result;
}


int FiscalPrinterService::getMessageLength() {
    logger.debug("getMessageLength()");
    int result = 0;
    try {
        result = impl->getMessageLength();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getMessageLength = " + QString::number(result));
    return result;
}


int FiscalPrinterService::getNumHeaderLines() {
    logger.debug("getNumHeaderLines()");
    int result = 0;
    try {
        result = impl->getNumHeaderLines();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getNumHeaderLines = " + QString::number(result));
    return result;
}


int FiscalPrinterService::getNumTrailerLines() {
    logger.debug("getNumTrailerLines()");
    int result = 0;
    try {
        result = impl->getNumTrailerLines();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getNumTrailerLines = " + QString::number(result));
    return result;
}


int FiscalPrinterService::getNumVatRates() {
    logger.debug("getNumVatRates()");
    int result = 0;
    try {
        result = impl->getNumVatRates();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getNumVatRates = " + QString::number(result));
    return result;
}


QString FiscalPrinterService::getPredefinedPaymentLines() {
    logger.debug("getPredefinedPaymentLines()");
    QString result = "";
    try {
        result = impl->getPredefinedPaymentLines();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getPredefinedPaymentLines = " + QString::number(result));
    return result;
}


int FiscalPrinterService::getPrinterState() {
    logger.debug("getPrinterState()");
    int result = 0;
    try {
        result = impl->getPrinterState();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getPrinterState = " + QString::number(result));
    return result;
}


int FiscalPrinterService::getQuantityDecimalPlaces() {
    logger.debug("getQuantityDecimalPlaces()");
    int result = 0;
    try {
        result = impl->getQuantityDecimalPlaces();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getQuantityDecimalPlaces = " + QString::number(result));
    return result;
}


int FiscalPrinterService::getQuantityLength() {
    logger.debug("getQuantityLength()");
    int result = 0;
    try {
        result = impl->getQuantityLength();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getQuantityLength = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getRecEmpty() {
    logger.debug("getRecEmpty()");
    bool result = false;
    try {
        result = impl->getRecEmpty();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getRecEmpty = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getRecNearEnd() {
    logger.debug("getRecNearEnd()");
    bool result = false;
    try {
        result = impl->getRecNearEnd();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getRecNearEnd = " + QString::number(result));
    return result;
}


int FiscalPrinterService::getRemainingFiscalMemory() {
    logger.debug("getRemainingFiscalMemory()");
    int result = 0;
    try {
        result = impl->getRemainingFiscalMemory();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getRemainingFiscalMemory = " + QString::number(result));
    return result;
}


QString FiscalPrinterService::getReservedWord() {
    logger.debug("getReservedWord()");
    QString result = "";
    try {
        result = impl->getReservedWord();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getReservedWord = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getSlpEmpty() {
    logger.debug("getSlpEmpty()");
    bool result = false;
    try {
        result = impl->getSlpEmpty();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getSlpEmpty = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getSlpNearEnd() {
    logger.debug("getSlpNearEnd()");
    bool result = false;
    try {
        result = impl->getSlpNearEnd();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getSlpNearEnd = " + QString::number(result));
    return result;
}


int FiscalPrinterService::getSlipSelection() {
    logger.debug("getSlipSelection()");
    int result = 0;
    try {
        result = impl->getSlipSelection();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getSlipSelection = " + QString::number(result));
    return result;
}


void FiscalPrinterService::setSlipSelection(int slipSelection) {
    logger.debug("setSlipSelection(" + QString::number(slipSelection) + ")");
    try {
        impl->setSlipSelection(slipSelection);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setSlipSelection: OK");
}


bool FiscalPrinterService::getTrainingModeActive() {
    logger.debug("getTrainingModeActive()");
    bool result = false;
    try {
        result = impl->getTrainingModeActive();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getTrainingModeActive = " + QString::number(result));
    return result;
}


void FiscalPrinterService::beginFiscalDocument(int documentAmount) {
    logger.debug("beginFiscalDocument(" + QString::number(documentAmount)
            + ")");
    try {
        impl->beginFiscalDocument(documentAmount);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("beginFiscalDocument: OK");
}


void FiscalPrinterService::beginFiscalReceipt(bool printHeader) {
    logger.debug("beginFiscalReceipt(" + QString::number(printHeader) + ")");
    try {
        impl->beginFiscalReceipt(printHeader);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("beginFiscalReceipt: OK");
}


void FiscalPrinterService::beginFixedOutput(int station, int documentType)
        {
    logger.debug("beginFixedOutput(" + QString::number(station) + ", "
            + QString::number(documentType) + ")");
    try {
        impl->beginFixedOutput(station, documentType);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("beginFixedOutput: OK");
}


void FiscalPrinterService::beginInsertion(int timeout) {
    logger.debug("beginInsertion(" + QString::number(timeout) + ")");
    try {
        impl->beginInsertion(timeout);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("beginInsertion: OK");
}


void FiscalPrinterService::beginItemList(int vatID) {
    logger.debug("beginItemList(" + QString::number(vatID) + ")");
    try {
        impl->beginItemList(vatID);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("beginItemList: OK");
}


void FiscalPrinterService::beginNonFiscal() {
    logger.debug("beginNonFiscal()");
    try {
        impl->beginNonFiscal();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("beginNonFiscal: OK");
}


void FiscalPrinterService::beginRemoval(int timeout) {
    logger.debug("beginRemoval(" + QString::number(timeout) + ")");
    try {
        impl->beginRemoval(timeout);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("beginRemoval: OK");
}


void FiscalPrinterService::beginTraining() {
    logger.debug("beginTraining()");
    try {
        impl->beginTraining();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("beginTraining: OK");
}


void FiscalPrinterService::clearError() {
    logger.debug("clearError()");
    try {
        impl->clearError();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("clearError: OK");
}


void FiscalPrinterService::clearOutput() {
    logger.debug("clearOutput()");
    try {
        impl->clearOutput();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("clearOutput: OK");
}


void FiscalPrinterService::endFiscalDocument() {
    logger.debug("endFiscalDocument()");
    try {
        impl->endFiscalDocument();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("endFiscalDocument: OK");
}


void FiscalPrinterService::endFiscalReceipt(bool printHeader) {
    logger.debug("endFiscalReceipt(" + QString::number(printHeader) + ")");
    try {
        impl->endFiscalReceipt(printHeader);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("endFiscalReceipt: OK");
}


void FiscalPrinterService::endFixedOutput() {
    logger.debug("endFixedOutput()");
    try {
        impl->endFixedOutput();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("endFixedOutput: OK");
}


void FiscalPrinterService::endInsertion() {
    logger.debug("endInsertion()");
    try {
        impl->endInsertion();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("endInsertion: OK");
}


void FiscalPrinterService::endItemList() {
    logger.debug("endItemList()");
    try {
        impl->endItemList();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("endItemList: OK");
}


void FiscalPrinterService::endNonFiscal() {
    logger.debug("endNonFiscal()");
    try {
        impl->endNonFiscal();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("endNonFiscal: OK");
}


void FiscalPrinterService::endRemoval() {
    logger.debug("endRemoval()");
    try {
        impl->endRemoval();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("endRemoval: OK");
}


void FiscalPrinterService::endTraining() {
    logger.debug("endTraining()");
    try {
        impl->endTraining();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("endTraining: OK");
}


void FiscalPrinterService::getData(int dataItem, int[] optArgs, QString[] data)
        {
    logger.debug("getData(" + QString::number(dataItem) + ", "
            + QString::number(optArgs) + ", " + QString::number(data) + ")");
    try {
        impl->getData(dataItem, optArgs, data);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getData: OK");
}


void FiscalPrinterService::getDate(QString[] Date) {
    logger.debug("getDate(" + QString::number(Date) + ")");
    try {
        impl->getDate(Date);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getDate: OK");
}


void FiscalPrinterService::getTotalizer(int vatID, int optArgs, QString[] data)
        {
    logger.debug("getTotalizer(" + QString::number(vatID) + ", "
            + QString::number(optArgs) + ", " + QString::number(data) + ")");
    try {
        impl->getTotalizer(vatID, optArgs, data);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getTotalizer: OK");
}


void FiscalPrinterService::getVatEntry(int vatID, int optArgs, int[] vatRate)
        {
    logger.debug("getVatEntry(" + QString::number(vatID) + ", "
            + QString::number(optArgs) + ", " + QString::number(vatRate)
            + ")");
    try {
        impl->getVatEntry(vatID, optArgs, vatRate);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getVatEntry: OK");
}


void FiscalPrinterService::printDuplicateReceipt() {
    logger.debug("printDuplicateReceipt()");
    try {
        impl->printDuplicateReceipt();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printDuplicateReceipt: OK");
}


void FiscalPrinterService::printFiscalDocumentLine(QString documentLine)
        {
    logger.debug("printFiscalDocumentLine(" + QString::number(documentLine)
            + ")");
    try {
        impl->printFiscalDocumentLine(documentLine);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printFiscalDocumentLine: OK");
}


void FiscalPrinterService::printFixedOutput(int documentType, int lineNumber, QString data)
        {
    logger.debug("printFixedOutput(" + QString::number(documentType) + ", "
            + QString::number(lineNumber) + ", " + QString::number(data)
            + ")");
    try {
        impl->printFixedOutput(documentType, lineNumber, data);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printFixedOutput: OK");
}


void FiscalPrinterService::printNormal(int station, QString data) {
    logger.debug("printNormal(" + QString::number(station) + ", "
            + QString::number(data) + ")");
    try {
        impl->printNormal(station, data);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printNormal: OK");
}


void FiscalPrinterService::printPeriodicTotalsReport(QString date1, QString date2)
        {
    logger.debug("printPeriodicTotalsReport(" + QString::number(date1)
            + ", " + QString::number(date2) + ")");
    try {
        impl->printPeriodicTotalsReport(date1, date2);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printPeriodicTotalsReport: OK");
}


void FiscalPrinterService::printPowerLossReport() {
    logger.debug("printPowerLossReport()");
    try {
        impl->printPowerLossReport();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printPowerLossReport: OK");
}


void FiscalPrinterService::printRecItem(QString description, long price, int quantity,
        int vatInfo, long unitPrice, QString unitName) {
    logger.debug("printRecItem(" + QString::number(description) + ", "
            + QString::number(price) + ", " + QString::number(quantity)
            + ", " + QString::number(vatInfo) + ", "
            + QString::number(unitPrice) + ", " + QString::number(unitName)
            + ")");
    try {
        impl->printRecItem(description, price, quantity, vatInfo, unitPrice,
                unitName);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printRecItem: OK");
}


void FiscalPrinterService::printRecItemAdjustment(int adjustmentType, QString description,
        long amount, int vatInfo) {
    logger.debug("printRecItemAdjustment(" + QString::number(adjustmentType)
            + ", " + QString::number(description) + ", "
            + QString::number(amount) + ", " + QString::number(vatInfo) + ")");
    try {
        impl->printRecItemAdjustment(adjustmentType, description, amount,
                vatInfo);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printRecItemAdjustment: OK");
}


void FiscalPrinterService::printRecMessage(QString message) {
    logger.debug("printRecMessage(" + QString::number(message) + ")");
    try {
        impl->printRecMessage(message);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printRecMessage: OK");
}


void FiscalPrinterService::printRecNotPaid(QString description, long amount)
        {
    logger.debug("printRecNotPaid(" + QString::number(description) + ", "
            + QString::number(amount) + ")");
    try {
        impl->printRecNotPaid(description, amount);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printRecNotPaid: OK");
}


void FiscalPrinterService::printRecRefund(QString description, long amount, int vatInfo)
        {
    logger.debug("printRecRefund(" + QString::number(description) + ", "
            + QString::number(amount) + ", " + QString::number(vatInfo) + ")");
    try {
        impl->printRecRefund(description, amount, vatInfo);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printRecRefund: OK");
}


void FiscalPrinterService::printRecItemRefund(QString description, long amount,
        int quantity, int vatInfo, long unitAmount, QString unitName)
        {
    logger.debug("printRecItemRefund(" + QString::number(description) + ", "
            + QString::number(amount) + ", " + QString::number(quantity)
            + ", " + QString::number(vatInfo) + ", "
            + QString::number(unitAmount) + ", " + QString::number(unitName)
            + ")");
    try {
        impl->printRecItemRefund(description, amount, quantity, vatInfo,
                unitAmount, unitName);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printRecItemRefund: OK");
}


void FiscalPrinterService::printRecItemRefundVoid(QString description, long amount,
        int quantity, int vatInfo, long unitAmount, QString unitName)
        {
    logger.debug("printRecItemRefundVoid(" + QString::number(description)
            + ", " + QString::number(amount) + ", "
            + QString::number(quantity) + ", " + QString::number(vatInfo)
            + ", " + QString::number(unitAmount) + ", "
            + QString::number(unitName) + ")");
    try {
        impl->printRecItemRefundVoid(description, amount, quantity, vatInfo,
                unitAmount, unitName);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printRecItemRefundVoid: OK");
}


void FiscalPrinterService::printRecItemAdjustmentVoid(int adjustmentType,
        QString description, long amount, int vatInfo) {
    logger.debug("printRecItemAdjustmentVoid("
            + QString::number(adjustmentType) + ", "
            + QString::number(description) + ", " + QString::number(amount)
            + ", " + QString::number(vatInfo) + ")");
    try {
        impl->printRecItemAdjustmentVoid(adjustmentType, description,
                amount, vatInfo);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printRecItemAdjustmentVoid: OK");
}


void FiscalPrinterService::printRecSubtotal(long amount) {
    logger.debug("printRecSubtotal(" + QString::number(amount) + ")");
    try {
        impl->printRecSubtotal(amount);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printRecSubtotal: OK");
}


void FiscalPrinterService::printRecSubtotalAdjustment(int adjustmentType,
        QString description, long amount) {
    logger.debug("printRecSubtotalAdjustment("
            + QString::number(adjustmentType) + ", "
            + QString::number(description) + ", " + QString::number(amount)
            + ")");
    try {
        impl->printRecSubtotalAdjustment(adjustmentType, description, amount);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printRecSubtotalAdjustment: OK");
}


void FiscalPrinterService::printRecTotal(long total, long payment, QString description)
        {
    logger.debug("printRecTotal(" + QString::number(total) + ", "
            + QString::number(payment) + ", " + QString::number(description)
            + ")");
    try {
        impl->printRecTotal(total, payment, description);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printRecTotal: OK");
}


void FiscalPrinterService::printRecVoid(QString description) {
    logger.debug("printRecVoid(" + QString::number(description) + ")");
    try {
        impl->printRecVoid(description);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printRecVoid: OK");
}


void FiscalPrinterService::printRecVoidItem(QString description, long amount, int quantity,
        int adjustmentType, long adjustment, int vatInfo)
        {
    logger.debug("printRecVoidItem(" + QString::number(description) + ", "
            + QString::number(amount) + ", " + QString::number(quantity)
            + ", " + QString::number(adjustmentType) + ", "
            + QString::number(adjustment) + ", " + QString::number(vatInfo)
            + ")");
    try {
        impl->printRecVoidItem(description, amount, quantity,
                adjustmentType, adjustment, vatInfo);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printRecVoidItem: OK");
}


void FiscalPrinterService::printReport(int reportType, QString startNum, QString endNum)
        {
    logger.debug("printReport(" + QString::number(reportType) + ", "
            + QString::number(startNum) + ", " + QString::number(endNum)
            + ")");
    try {
        impl->printReport(reportType, startNum, endNum);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printReport: OK");
}


void FiscalPrinterService::printXReport() {
    logger.debug("printXReport()");
    try {
        impl->printXReport();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printXReport: OK");
}


void FiscalPrinterService::printZReport() {
    logger.debug("printZReport()");
    try {
        impl->printZReport();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printZReport: OK");
}


void FiscalPrinterService::resetPrinter() {
    logger.debug("resetPrinter()");
    try {
        impl->resetPrinter();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("resetPrinter: OK");
}


void FiscalPrinterService::setDate(QString date) {
    logger.debug("setDate(" + QString::number(date) + ")");
    try {
        impl->setDate(date);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setDate: OK");
}


void FiscalPrinterService::setHeaderLine(int lineNumber, QString text, bool doubleWidth)
        {
    logger.debug("setHeaderLine(" + QString::number(lineNumber) + ", "
            + QString::number(text) + ", " + QString::number(doubleWidth)
            + ")");
    try {
        impl->setHeaderLine(lineNumber, text, doubleWidth);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setHeaderLine: OK");
}


void FiscalPrinterService::setPOSID(QString POSID, QString cashierID) {
    logger.debug("setPOSID(" + QString::number(POSID) + ", "
            + QString::number(cashierID) + ")");
    try {
        impl->setPOSID(POSID, cashierID);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setPOSID: OK");
}


void FiscalPrinterService::setStoreFiscalID(QString ID) {
    logger.debug("setStoreFiscalID(" + QString::number(ID) + ")");
    try {
        impl->setStoreFiscalID(ID);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setStoreFiscalID: OK");
}


void FiscalPrinterService::setTrailerLine(int lineNumber, QString text, bool doubleWidth)
        {
    logger.debug("setTrailerLine(" + QString::number(lineNumber) + ", "
            + QString::number(text) + ", " + QString::number(doubleWidth)
            + ")");
    try {
        impl->setTrailerLine(lineNumber, text, doubleWidth);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setTrailerLine: OK");
}


void FiscalPrinterService::setVatTable() {
    logger.debug("setVatTable()");
    try {
        impl->setVatTable();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setVatTable: OK");
}


void FiscalPrinterService::setVatValue(int vatID, QString vatValue) {
    logger.debug("setVatValue(" + QString::number(vatID) + ", "
            + QString::number(vatValue) + ")");
    try {
        impl->setVatValue(vatID, vatValue);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setVatValue: OK");
}


void FiscalPrinterService::verifyItem(QString itemName, int vatID) {
    logger.debug("verifyItem(" + QString::number(itemName) + ", "
            + QString::number(vatID) + ")");
    try {
        impl->verifyItem(itemName, vatID);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("verifyItem: OK");
}


bool FiscalPrinterService::getCapAdditionalHeader() {
    logger.debug("getCapAdditionalHeader()");
    bool result = false;
    try {
        result = impl->getCapAdditionalHeader();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapAdditionalHeader = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapAdditionalTrailer() {
    logger.debug("getCapAdditionalTrailer()");
    bool result = false;
    try {
        result = impl->getCapAdditionalTrailer();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapAdditionalTrailer = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapChangeDue() {
    logger.debug("getCapChangeDue()");
    bool result = false;
    try {
        result = impl->getCapChangeDue();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapChangeDue = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapEmptyReceiptIsVoidable() {
    logger.debug("getCapEmptyReceiptIsVoidable()");
    bool result = false;
    try {
        result = impl->getCapEmptyReceiptIsVoidable();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapEmptyReceiptIsVoidable = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapFiscalReceiptStation() {
    logger.debug("getCapFiscalReceiptStation()");
    bool result = false;
    try {
        result = impl->getCapFiscalReceiptStation();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapFiscalReceiptStation = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapFiscalReceiptType() {
    logger.debug("getCapFiscalReceiptType()");
    bool result = false;
    try {
        result = impl->getCapFiscalReceiptType();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapFiscalReceiptType = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapMultiContractor() {
    logger.debug("getCapMultiContractor()");
    bool result = false;
    try {
        result = impl->getCapMultiContractor();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapMultiContractor = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapOnlyVoidLastItem() {
    logger.debug("getCapOnlyVoidLastItem()");
    bool result = false;
    try {
        result = impl->getCapOnlyVoidLastItem();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapOnlyVoidLastItem = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapPackageAdjustment() {
    logger.debug("getCapPackageAdjustment()");
    bool result = false;
    try {
        result = impl->getCapPackageAdjustment();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapPackageAdjustment = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapPostPreLine() {
    logger.debug("getCapPostPreLine()");
    bool result = false;
    try {
        result = impl->getCapPostPreLine();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapPostPreLine = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapSetCurrency() {
    logger.debug("getCapSetCurrency()");
    bool result = false;
    try {
        result = impl->getCapSetCurrency();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapSetCurrency = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapTotalizerType() {
    logger.debug("getCapTotalizerType()");
    bool result = false;
    try {
        result = impl->getCapTotalizerType();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapTotalizerType = " + QString::number(result));
    return result;
}


int FiscalPrinterService::getActualCurrency() {
    logger.debug("getActualCurrency()");
    int result = 0;
    try {
        result = impl->getActualCurrency();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getActualCurrency = " + QString::number(result));
    return result;
}


QString FiscalPrinterService::getAdditionalHeader() {
    logger.debug("getAdditionalHeader()");
    QString result = "";
    try {
        result = impl->getAdditionalHeader();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getAdditionalHeader = " + QString::number(result));
    return result;
}


void FiscalPrinterService::setAdditionalHeader(QString additionalHeader)
        {
    logger.debug("setAdditionalHeader(" + QString::number(additionalHeader)
            + ")");
    try {
        impl->setAdditionalHeader(additionalHeader);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setAdditionalHeader: OK");
}


QString FiscalPrinterService::getAdditionalTrailer() {
    logger.debug("getAdditionalTrailer()");
    QString result = "";
    try {
        result = impl->getAdditionalTrailer();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getAdditionalTrailer = " + QString::number(result));
    return result;
}


void FiscalPrinterService::setAdditionalTrailer(QString additionalTrailer)
        {
    logger.debug("setAdditionalTrailer("
            + QString::number(additionalTrailer) + ")");
    try {
        impl->setAdditionalTrailer(additionalTrailer);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setAdditionalTrailer: OK");
}


QString FiscalPrinterService::getChangeDue() {
    logger.debug("getChangeDue()");
    QString result = "";
    try {
        result = impl->getChangeDue();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getChangeDue = " + QString::number(result));
    return result;
}


void FiscalPrinterService::setChangeDue(QString changeDue) {
    logger.debug("setChangeDue(" + QString::number(changeDue) + ")");
    try {
        impl->setChangeDue(changeDue);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setChangeDue: OK");
}


int FiscalPrinterService::getContractorId() {
    logger.debug("getContractorId()");
    int result = 0;
    try {
        result = impl->getContractorId();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getContractorId = " + QString::number(result));
    return result;
}


void FiscalPrinterService::setContractorId(int contractorId) {
    logger.debug("setContractorId(" + QString::number(contractorId) + ")");
    try {
        impl->setContractorId(contractorId);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setContractorId: OK");
}


int FiscalPrinterService::getDateType() {
    logger.debug("getDateType()");
    int result = 0;
    try {
        result = impl->getDateType();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getDateType = " + QString::number(result));
    return result;
}


void FiscalPrinterService::setDateType(int dateType) {
    logger.debug("setDateType(" + QString::number(dateType) + ")");
    try {
        impl->setDateType(dateType);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setDateType: OK");
}


int FiscalPrinterService::getFiscalReceiptStation() {
    logger.debug("getFiscalReceiptStation()");
    int result = 0;
    try {
        result = impl->getFiscalReceiptStation();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getFiscalReceiptStation = " + QString::number(result));
    return result;
}


void FiscalPrinterService::setFiscalReceiptStation(int fiscalReceiptStation)
        {
    logger.debug("setFiscalReceiptStation("
            + QString::number(fiscalReceiptStation) + ")");
    try {
        impl->setFiscalReceiptStation(fiscalReceiptStation);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setFiscalReceiptStation: OK");
}


int FiscalPrinterService::getFiscalReceiptType() {
    logger.debug("getFiscalReceiptType()");
    int result = 0;
    try {
        result = impl->getFiscalReceiptType();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getFiscalReceiptType = " + QString::number(result));
    return result;
}


void FiscalPrinterService::setFiscalReceiptType(int fiscalReceiptType)
        {
    logger.debug("setFiscalReceiptType("
            + QString::number(fiscalReceiptType) + ")");
    try {
        impl->setFiscalReceiptType(fiscalReceiptType);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setFiscalReceiptType: OK");
}


int FiscalPrinterService::getMessageType() {
    logger.debug("getMessageType()");
    int result = 0;
    try {
        result = impl->getMessageType();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getMessageType = " + QString::number(result));
    return result;
}


void FiscalPrinterService::setMessageType(int messageType) {
    logger.debug("setMessageType(" + QString::number(messageType) + ")");
    try {
        impl->setMessageType(messageType);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setMessageType: OK");
}


QString FiscalPrinterService::getPostLine() {
    logger.debug("getPostLine()");
    QString result = "";
    try {
        result = impl->getPostLine();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getPostLine = " + QString::number(result));
    return result;
}


void FiscalPrinterService::setPostLine(QString postLine) {
    logger.debug("setPostLine(" + QString::number(postLine) + ")");
    try {
        impl->setPostLine(postLine);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setPostLine: OK");
}


QString FiscalPrinterService::getPreLine() {
    logger.debug("getPreLine()");
    QString result = "";
    try {
        result = impl->getPreLine();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getPreLine = " + QString::number(result));
    return result;
}


void FiscalPrinterService::setPreLine(QString preLine) {
    logger.debug("setPreLine(" + QString::number(preLine) + ")");
    try {
        impl->setPreLine(preLine);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setPreLine: OK");
}


int FiscalPrinterService::getTotalizerType() {
    logger.debug("getTotalizerType()");
    int result = 0;
    try {
        result = impl->getTotalizerType();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getTotalizerType = " + QString::number(result));
    return result;
}


void FiscalPrinterService::setTotalizerType(int totalizerType) {
    logger.debug("setTotalizerType(" + QString::number(totalizerType) + ")");
    try {
        impl->setTotalizerType(totalizerType);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setTotalizerType: OK");
}


void FiscalPrinterService::setCurrency(int newCurrency) {
    logger.debug("setCurrency(" + QString::number(newCurrency) + ")");
    try {
        impl->setCurrency(newCurrency);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("setCurrency: OK");
}


void FiscalPrinterService::printRecCash(long amount) {
    logger.debug("printRecCash(" + QString::number(amount) + ")");
    try {
        impl->printRecCash(amount);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printRecCash: OK");
}


void FiscalPrinterService::printRecItemFuel(QString description, long price, int quantity,
        int vatInfo, long unitPrice, QString unitName, long specialTax,
        QString specialTaxName) {
    logger.debug("printRecItemFuel(" + QString::number(description) + ", "
            + QString::number(price) + ", " + QString::number(quantity)
            + ", " + QString::number(vatInfo) + ", "
            + QString::number(unitPrice) + ", " + QString::number(unitName)
            + ", " + QString::number(specialTax) + ", "
            + QString::number(specialTaxName) + ")");
    try {
        impl->printRecItemFuel(description, price, quantity, vatInfo,
                unitPrice, unitName, specialTax, specialTaxName);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printRecItemFuel: OK");
}


void FiscalPrinterService::printRecItemFuelVoid(QString description, long price,
        int vatInfo, long specialTax) {
    logger.debug("printRecItemFuelVoid(" + QString::number(description)
            + ", " + QString::number(price) + ", " + QString::number(vatInfo)
            + ", " + QString::number(specialTax) + ")");
    try {
        impl->printRecItemFuelVoid(description, price, vatInfo, specialTax);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printRecItemFuelVoid: OK");
}


void FiscalPrinterService::printRecPackageAdjustment(int adjustmentType,
        QString description, QString vatAdjustment) {
    logger.debug("printRecPackageAdjustment("
            + QString::number(adjustmentType) + ", "
            + QString::number(description) + ", "
            + QString::number(vatAdjustment) + ")");
    try {
        impl->printRecPackageAdjustment(adjustmentType, description,
                vatAdjustment);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printRecPackageAdjustment: OK");
}


void FiscalPrinterService::printRecPackageAdjustVoid(int adjustmentType,
        QString vatAdjustment) {
    logger.debug("printRecPackageAdjustVoid("
            + QString::number(adjustmentType) + ", "
            + QString::number(vatAdjustment) + ")");
    try {
        impl->printRecPackageAdjustVoid(adjustmentType, vatAdjustment);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printRecPackageAdjustVoid: OK");
}


void FiscalPrinterService::printRecRefundVoid(QString description, long amount, int vatInfo)
        {
    logger.debug("printRecRefundVoid(" + QString::number(description) + ", "
            + QString::number(amount) + ", " + QString::number(vatInfo) + ")");
    try {
        impl->printRecRefundVoid(description, amount, vatInfo);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printRecRefundVoid: OK");
}


void FiscalPrinterService::printRecSubtotalAdjustVoid(int adjustmentType, long amount)
        {
    logger.debug("printRecSubtotalAdjustVoid("
            + QString::number(adjustmentType) + ", "
            + QString::number(amount) + ")");
    try {
        impl->printRecSubtotalAdjustVoid(adjustmentType, amount);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printRecSubtotalAdjustVoid: OK");
}


void FiscalPrinterService::printRecTaxID(QString taxID) {
    logger.debug("printRecTaxID(" + QString::number(taxID) + ")");
    try {
        impl->printRecTaxID(taxID);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printRecTaxID: OK");
}


int FiscalPrinterService::getAmountDecimalPlaces() {
    logger.debug("getAmountDecimalPlaces()");
    int result = 0;
    try {
        result = impl->getAmountDecimalPlaces();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getAmountDecimalPlaces = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapStatisticsReporting() {
    logger.debug("getCapStatisticsReporting()");
    bool result = false;
    try {
        result = impl->getCapStatisticsReporting();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapStatisticsReporting = " + QString::number(result));
    return result;
}


bool FiscalPrinterService::getCapUpdateStatistics() {
    logger.debug("getCapUpdateStatistics()");
    bool result = false;
    try {
        result = impl->getCapUpdateStatistics();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapUpdateStatistics = " + QString::number(result));
    return result;
}


void FiscalPrinterService::resetStatistics(QString statisticsBuffer) {
    logger.debug("resetStatistics(" + QString::number(statisticsBuffer)
            + ")");
    try {
        impl->resetStatistics(statisticsBuffer);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("resetStatistics: OK");
}


void FiscalPrinterService::retrieveStatistics(QString[] statisticsBuffer)
        {
    logger.debug("retrieveStatistics(" + QString::number(statisticsBuffer)
            + ")");
    try {
        impl->retrieveStatistics(statisticsBuffer);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("retrieveStatistics: OK");
}


void FiscalPrinterService::updateStatistics(QString statisticsBuffer) {
    logger.debug("updateStatistics(" + QString::number(statisticsBuffer)
            + ")");
    try {
        impl->updateStatistics(statisticsBuffer);
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("updateStatistics: OK");
}


void FiscalPrinterService::printRecItemVoid(QString description, long price, int quantity,
        int vatInfo, long unitPrice, QString unitName) {
    logger.debug("printRecItemVoid(" + QString::number(description) + ", "
            + QString::number(price) + ", " + QString::number(quantity)
            + ", " + QString::number(vatInfo) + ", "
            + QString::number(unitPrice) + ", " + QString::number(unitName)
            + ")");
    try {
        impl->printRecItemVoid(description, price, quantity, vatInfo,
                unitPrice, unitName);

    } catch (QException e) {
        handleException(e);
    }
    logger.debug("printRecItemVoid: OK");
}


bool FiscalPrinterService::getCapPositiveSubtotalAdjustment() {
    logger.debug("getCapPositiveSubtotalAdjustment()");
    bool result = false;
    try {
        result = impl->getCapPositiveSubtotalAdjustment();
    } catch (QException e) {
        handleException(e);
    }
    logger.debug("getCapPositiveSubtotalAdjustment = "
            + QString::number(result));
    return result;
}

