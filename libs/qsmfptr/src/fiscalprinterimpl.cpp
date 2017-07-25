#include "fiscalprinterimpl.h"


/*
    private FSService fsService = new FSService(null);
    public int logoPosition = SMFPTR_LOGO_PRINT;
    private final FptrParameters params;
    private boolean freezeEvents;
    private static final int MaxStateCount = 3;
    private final FiscalPrinterFilters filters = new FiscalPrinterFilters();
    private Thread asyncThread = null;
    private Thread deviceThread = null;
    private Thread eventThread = null;
    private boolean asyncThreadEnabled = false;
    private boolean deviceThreadEnabled = false;
    private boolean eventThreadEnabled = false;
    private CompositeLogger logger = CompositeLogger.getLogger(FiscalPrinterImpl.class);
    private boolean deviceEnabled = false;
    private EventCallbacks cb = null;
    private final Vector events = new Vector();
    private final FontNumber doubleWidthFont;
    private final FiscalDay fiscalDay = new FiscalDay();
    private final Vector requests = new Vector();
    private PrinterHeader header;
    private PrinterHeader trailer;
    private final int[] vatValues = new int[4];
    private PrinterPort port;
    private PrinterProtocol device;
    public SMFiscalPrinter printer;
    private ReceiptPrinter receiptPrinter;
    private final FiscalPrinterStatistics statistics = new FiscalPrinterStatistics();
    // --------------------------------------------------------------------------
    // Variables
    // --------------------------------------------------------------------------
    // Instance Data Set in Derived Class
    public boolean claimed = false;
    public String checkHealthText = "";
    private String physicalDeviceDescription = "";
    private String deviceServiceDescription = "";
    private int deviceServiceVersion;
    private String physicalDeviceName = "";
    private int state = JposConst.JPOS_S_CLOSED;
    public final FiscalPrinterState printerState = new FiscalPrinterState();
    // Capabilities
    private boolean capAdditionalLines;
    private boolean capAmountAdjustment;
    private boolean capAmountNotPaid;
    private boolean capCheckTotal;
    private boolean capFixedOutput;
    private boolean capIndependentHeader;
    private boolean capItemList;
    private boolean capNonFiscalMode;
    private boolean capOrderAdjustmentFirst;
    private boolean capPercentAdjustment;
    private boolean capPositiveAdjustment;
    private boolean capPowerLossReport;
    private boolean capPredefinedPaymentLines;
    private boolean capReceiptNotPaid;
    private boolean capRemainingFiscalMemory;
    private boolean capReservedWord;
    private boolean capSetHeader;
    private boolean capSetPOSID;
    private boolean capSetStoreFiscalID;
    private boolean capSetTrailer;
    private boolean capSetVatTable;
    private boolean capSlpFiscalDocument;
    private boolean capSlpFullSlip;
    private boolean capSlpValidation;
    private boolean capSubAmountAdjustment;
    private boolean capSubPercentAdjustment;
    private boolean capSubtotal;
    private boolean capTrainingMode;
    private boolean capValidateJournal;
    private boolean capXReport;
    // 1.6
    public boolean capAdditionalHeader;
    public boolean capAdditionalTrailer;
    public boolean capChangeDue;
    private boolean capEmptyReceiptIsVoidable;
    private boolean capFiscalReceiptStation;
    private boolean capFiscalSlipStation;
    private boolean capFiscalReceiptType;
    private boolean capMultiContractor;
    private boolean capOnlyVoidLastItem;
    private boolean capPackageAdjustment;
    private boolean capPostPreLine;
    private boolean capSetCurrency;
    private boolean capTotalizerType;
    // 1.8
    private boolean capStatisticsReporting;
    private boolean capUpdateStatistics;
    // 1.9
    private boolean capCompareFirmwareVersion;
    private boolean capUpdateFirmware;
    // 1.11
    private boolean capPositiveSubtotalAdjustment;
    // State
    private boolean duplicateReceipt;
    private int outputID;
    private int powerNotify = JPOS_PN_DISABLED;
    private int powerState;
    private int countryCode;
    private int errorLevel;
    private int errorOutID;
    private int errorState;
    private int errorStation;
    private String errorString = "";
    private boolean flagWhenIdle;
    private String predefinedPaymentLines = "";
    private int quantityDecimalPlaces;
    private int quantityLength;
    private boolean recEmpty;
    private boolean recNearEnd;
    private boolean jrnEmpty;
    private boolean jrnNearEnd;
    private boolean slpEmpty;
    private boolean slpNearEnd;
    private int slipSelection;
    private boolean trainingModeActive;
    private boolean coverOpened = false;
    private boolean dayOpened = false;
    private String reservedWord = "";
    // 1.6
    private int actualCurrency;
    private String additionalHeader = "";
    private String additionalTrailer = "";
    private String changeDue = "";
    private int contractorId;
    private int dateType;
    private int fiscalReceiptStation;
    private int fiscalReceiptType;
    private int messageType;
    private int totalizerType;
    public int graphicsLine = 1;
    private boolean isReceiptOpened = false;
    private String logicalName = "";
    private boolean asyncMode = false;
    private FiscalReceipt receipt = new NullReceipt();
    private final Vector printItems = new Vector();
    private final PrinterReceipt printerReceipt = new PrinterReceipt();
    private boolean connected = false;
    private boolean isLicenseValid = false;
    private EJStatus statusEJ = null;
    private EJActivation aEJActivation = new EJActivation();
    private final MonitoringServer monitoringServer = new MonitoringServer(this);
    private int receiptType = 0;
    private boolean isRecPresent = true;
    private boolean inAfterCommand = false;
    private boolean isInReceiptTrailer = false;

    class DeviceTarget implements Runnable {

        private final FiscalPrinterImpl fiscalPrinter;

        public DeviceTarget(FiscalPrinterImpl fiscalPrinter) {
            this.fiscalPrinter = fiscalPrinter;
        }

        public void run() {
            fiscalPrinter.deviceProc();
        }
    }

    class EventTarget implements Runnable {

        private final FiscalPrinterImpl fiscalPrinter;

        public EventTarget(FiscalPrinterImpl fiscalPrinter) {
            this.fiscalPrinter = fiscalPrinter;
        }

        public void run() {
            fiscalPrinter.eventProc();
        }
    }

    class AsyncTarget implements Runnable {

        private final FiscalPrinterImpl fiscalPrinter;

        public AsyncTarget(FiscalPrinterImpl fiscalPrinter) {
            this.fiscalPrinter = fiscalPrinter;
        }

        public void run() {
            fiscalPrinter.asyncProc();
        }
    }

*/
FiscalPrinterImpl::FiscalPrinterImpl()
{
    /*
    params = new FptrParameters();
    doubleWidthFont = FontNumber.getNormalFont();
    statistics.unifiedPOSVersion = "1.13";
    statistics.deviceCategory = "CashDrawer";
    statistics.manufacturerName = "SHTRIH-M";
    statistics.modelName = "Fiscal printer";
    statistics.serialNumber = "";
    statistics.firmwareRevision = "";
    statistics.physicalInterface = RS232_DEVICE_BUS;
    statistics.installationDate = "";
    */
    initializeData();
}

void FiscalPrinterImpl::initializeData()
{
    state = JPOS_S_CLOSED;
    capPositiveSubtotalAdjustment = true;
    deviceServiceDescription = "Fiscal Printer Service , SHTRIH-M, 2015";
    physicalDeviceDescription = "SHTRIH-M fiscal printer";
    physicalDeviceName = "SHTRIH-M fiscal printer";
    capAdditionalLines = true;
    capAmountAdjustment = true;
    capAmountNotPaid = false;
    capCheckTotal = true;
    capFixedOutput = false;
    capIndependentHeader = true;
    capItemList = false;
    capNonFiscalMode = true;
    capOrderAdjustmentFirst = false;
    capPercentAdjustment = true;
    capPositiveAdjustment = true;
    capPowerLossReport = false;
    capPredefinedPaymentLines = true;
    capReceiptNotPaid = false;
    capRemainingFiscalMemory = true;
    capReservedWord = false;
    capSetHeader = true;
    capSetPOSID = true;
    capSetStoreFiscalID = false;
    capSetTrailer = true;
    capSetVatTable = true;
    capSlpFiscalDocument = false;
    capSlpFullSlip = false;
    capSlpValidation = false;
    capSubAmountAdjustment = true;
    capSubPercentAdjustment = true;
    capSubtotal = true;
    capTrainingMode = false;
    capValidateJournal = false;
    capXReport = true;
    capAdditionalHeader = true;
    capAdditionalTrailer = true;
    capChangeDue = false;
    capEmptyReceiptIsVoidable = true;
    capFiscalReceiptStation = true;
    capFiscalSlipStation = false;
    capFiscalReceiptType = true;
    capMultiContractor = false;
    capOnlyVoidLastItem = false;
    capPackageAdjustment = true;
    capPostPreLine = true;
    capSetCurrency = false;
    capTotalizerType = true;
    capCompareFirmwareVersion = false;
    capUpdateFirmware = false;
    // state
    duplicateReceipt = false;
    outputID = 0;
    powerNotify = JPOS_PN_DISABLED;
    powerState = JPOS_PS_UNKNOWN;
    countryCode = FPTR_CC_RUSSIA;
    errorLevel = FPTR_EL_NONE;
    errorOutID = 0;
    errorState = 0;
    errorStation = 0;
    errorString = "";
    flagWhenIdle = false;
    jrnEmpty = false;
    jrnNearEnd = false;
    predefinedPaymentLines = "";
    printerState.setValue(FPTR_PS_MONITOR);
    quantityDecimalPlaces = 3;
    quantityLength = 10;
    recEmpty = false;
    recNearEnd = false;
    reservedWord = "";
    slpEmpty = false;
    slpNearEnd = false;
    slipSelection = FPTR_SS_FULL_LENGTH;
    trainingModeActive = false;
    actualCurrency = FPTR_AC_RUR;
    additionalHeader = "";
    additionalTrailer = "";
    changeDue = "";
    contractorId = FPTR_CID_SINGLE;
    dateType = FPTR_DT_RTC;
    fiscalReceiptStation = FPTR_RS_RECEIPT;
    fiscalReceiptType = FPTR_RT_SALES;
    messageType = FPTR_MT_FREE_TEXT;
    totalizerType = FPTR_TT_DAY;
    capUpdateStatistics = true;
    capStatisticsReporting = true;
    deviceServiceVersion = deviceVersion113 + 278;
    freezeEvents = true;
}

/*
    public SMFiscalPrinter getPrinter() {
        if (printer == null) {
            logger.error("printer is null");
        }
        return printer;
    }

    public String[] parseText(String text) {
        return getPrinter().splitText(text, params.font);
    }

    public void setPrinter(SMFiscalPrinter printer) {
        this.printer = printer;
    }

    public void handleException(Throwable e) throws JposException {
        JposExceptionHandler.handleException(e);
    }

    public PrinterStatus readPrinterStatus() {
        return getPrinter().readPrinterStatus();
    }

    public FlexCommands getCommands() {
        return printer.getCommands();
    }

    public void openReceipt(int receiptType) {
        if ((!isReceiptOpened) && getModel().getCapOpenReceipt()) {
            getPrinter().openReceipt(receiptType);
            getPrinter().waitForPrinting();
            this.receiptType = receiptType;
            isReceiptOpened = true;
        }
    }

    public PrinterModel getModel() {
        return getPrinter().getModel();
    }

    // --------------------------------------------------------------------------
    // Properties
    // --------------------------------------------------------------------------
    public String getCheckHealthText() {
        checkOpened();
        return encodeText(checkHealthText);
    }

    public boolean getClaimed() {
        checkOpened();
        return claimed;
    }

    public boolean getDeviceEnabled() {
        return deviceEnabled;
    }

    private void setPowerState(int powerState) {
        if (powerNotify == JPOS_PN_ENABLED) {
            if (powerState != this.powerState) {
                switch (powerState) {
                    case JPOS_PS_ONLINE:
                        statusUpdateEvent(JPOS_SUE_POWER_ONLINE);
                        break;

                    case JPOS_PS_OFF:
                        statusUpdateEvent(JPOS_SUE_POWER_OFF);
                        break;

                    case JPOS_PS_OFFLINE:
                        statusUpdateEvent(JPOS_SUE_POWER_OFFLINE);
                        break;

                    case JPOS_PS_OFF_OFFLINE:
                        statusUpdateEvent(JPOS_SUE_POWER_OFF_OFFLINE);
                        break;
                }
            }
        }
        this.powerState = powerState;
    }

    private void setState(int value) {
        if (value != state) {
            state = value;
            if ((state == JPOS_S_IDLE) && (flagWhenIdle)) {
                statusUpdateEvent(FPTR_SUE_IDLE);
                flagWhenIdle = false;
            }
        }
    }

    private void addEvent(Runnable event) {
        synchronized (events) {
            events.add(event);
            events.notifyAll();
        }
    }

    private void statusUpdateEvent(int value) {
        logger.debug("statusUpdateEvent("
                + StatusUpdateEventHelper.getName(value) + ")");

        addEvent(new StatusUpdateEventRequest(cb, new StatusUpdateEvent(this,
                value)));
    }

    private void outputCompleteEvent(int outputID) {
        logger.debug("outputCompleteEvent(" + String.valueOf(outputID) + ")");

        addEvent(new OutputCompleteEventRequest(cb, new OutputCompleteEvent(
                this, outputID)));
    }

    public void printerStatusRead(PrinterStatus status) {
        try {
            updateStatus(status);
        } catch (Exception e) {
            logger.error("printerStatusRead", e);
        }
    }

    private void checkPaperStatus(PrinterStatus status) {
        if (isRecPresent) {
            isRecPresent = status.getPrinterFlags().isRecPresent();
        }

        if (status.getPrinterFlags().isRecPresent()) {
            if (status.getSubmode() == ECR_SUBMODE_AFTER) {
                getPrinter().continuePrint();
                getPrinter().waitForPrinting();
            }

            /*
             if (isInReceiptTrailer) {
             printDocEnd();
             isInReceiptTrailer = false;
             isRecPresent = true;
             }

             if (!isRecPresent) {
             printDocEnd();
             isRecPresent = true;
             }



        }
    }

    public void beforeCommand(PrinterCommand command) {
    }

    public void afterCommand(PrinterCommand command) {
        if (inAfterCommand) {
            return;
        }
        inAfterCommand = true;
        try {
            setPowerState(JPOS_PS_ONLINE);
            if (command.getResultCode() != 0) {
                int errorCode = command.getResultCode() + 300;
                logger.debug("ErrorEvent(JPOS_E_EXTENDED, " + errorCode + ")");
                ErrorEvent event = new ErrorEvent(this, JPOS_E_EXTENDED,
                        errorCode, JPOS_EL_OUTPUT, 0);
                addEvent(new ErrorEventRequest(cb, event));
            }

            switch (command.getResultCode()) {
                case SMFP_EFPTR_NO_REC_PAPER:
                    isRecPresent = false;
                    setRecPaperState(true, recNearEnd);
                    break;

                case SMFP_EFPTR_NO_JRN_PAPER:
                    setJrnPaperState(true, jrnNearEnd);
                    break;

                case SMFP_EFPTR_NO_SLP_PAPER:
                    setSlpPaperState(true, slpNearEnd);
                    break;

                case SMFP_EFPTR_WAIT_PRINT_CONTINUE:
                    getPrinter().continuePrint();
                    PrinterStatus status = getPrinter().waitForPrinting();
                    checkPaperStatus(status);
                    command.setRepeatNeeded(true);
                    break;

                case SMFP_EFPTR_PREVCOMMAND:
                    printer.waitForPrinting();
                    command.setRepeatNeeded(true);
                    break;

                case SMFP_EFPTR_FM_CONNECT_ERROR:
                    printer.waitForFiscalMemory();
                    command.setRepeatNeeded(true);
                    break;

                case SMFP_EFPTR_EJ_CONNECT_ERROR:
                case SMFP_EFPTR_EJ_MISSING:
                    printer.waitForElectronicJournal();
                    command.setRepeatNeeded(true);
                    break;

            }
        } catch (Exception e) {
            logger.error("commandExecuted", e);
        }
        inAfterCommand = false;
    }

    public void setRecPaperState(boolean recEmpty, boolean recNearEnd)
            {

        int state = getRecPaperState(recEmpty, recNearEnd);
        int curState = getRecPaperState(this.recEmpty, this.recNearEnd);
        if (state != curState) {
            statusUpdateEvent(state);
        }
        this.recEmpty = recEmpty;
        this.recNearEnd = recNearEnd;
    }

    public int getRecPaperState(boolean recEmpty, boolean recNearEnd)
            {
        if (getCapRecPresent()) {
            if (!getCapRecEmptySensor()) {
                recEmpty = false;
            }
            if (!getCapRecNearEndSensor()) {
                recNearEnd = false;
            }
            if (recEmpty) {
                return FiscalPrinterConst.FPTR_SUE_REC_EMPTY;
            }
            if (recNearEnd) {
                return FiscalPrinterConst.FPTR_SUE_REC_NEAREMPTY;
            }
        }
        return FiscalPrinterConst.FPTR_SUE_REC_PAPEROK;
    }

    public int getJrnPaperState(boolean jrnEmpty, boolean jrnNearEnd)
            {
        if (getCapJrnPresent()) {
            if (!getCapJrnEmptySensor()) {
                jrnEmpty = false;
            }
            if (!getCapJrnNearEndSensor()) {
                jrnNearEnd = false;
            }
            if (jrnEmpty) {
                return FiscalPrinterConst.FPTR_SUE_JRN_EMPTY;
            }
            if (jrnNearEnd) {
                return FiscalPrinterConst.FPTR_SUE_JRN_NEAREMPTY;
            }
        }
        return FiscalPrinterConst.FPTR_SUE_JRN_PAPEROK;
    }

    public int getSlpPaperState(boolean slpEmpty, boolean slpNearEnd)
            {
        if (getCapSlpPresent()) {
            if (!getCapSlpEmptySensor()) {
                slpEmpty = false;
            }
            if (!getCapSlpNearEndSensor()) {
                slpNearEnd = false;
            }
            if (slpEmpty) {
                return FiscalPrinterConst.FPTR_SUE_SLP_EMPTY;
            }
            if (slpNearEnd) {
                return FiscalPrinterConst.FPTR_SUE_SLP_NEAREMPTY;
            }
        }
        return FiscalPrinterConst.FPTR_SUE_SLP_PAPEROK;
    }

    public void setJrnPaperState(boolean jrnEmpty, boolean jrnNearEnd)
            {

        int state = getJrnPaperState(jrnEmpty, jrnNearEnd);
        int curState = getJrnPaperState(this.jrnEmpty, this.jrnNearEnd);
        if (state != curState) {
            statusUpdateEvent(state);
        }
        this.jrnEmpty = jrnEmpty;
        this.jrnNearEnd = jrnNearEnd;
    }

    public void setSlpPaperState(boolean slpEmpty, boolean slpNearEnd)
            {

        int state = getSlpPaperState(slpEmpty, slpNearEnd);
        int curState = getSlpPaperState(this.slpEmpty, this.slpNearEnd);
        if (state != curState) {
            statusUpdateEvent(state);
        }
        this.slpEmpty = slpEmpty;
        this.slpNearEnd = slpNearEnd;
    }

    void setCoverState(boolean isCoverOpened) {
        if (getCapCoverSensor()) {
            if (isCoverOpened != coverOpened) {
                if (isCoverOpened) {
                    statusUpdateEvent(FPTR_SUE_COVER_OPEN);
                } else {
                    statusUpdateEvent(FPTR_SUE_COVER_OK);
                }
                coverOpened = isCoverOpened;
            }
        }
    }

    public void execute(FiscalPrinterRequest request) {
        if (asyncMode) {
            setState(JPOS_S_BUSY);
            synchronized (requests) {
                requests.add(request);
                requests.notifyAll();
                outputID = request.getId();
            }
        } else {
            if (state == JPOS_S_BUSY) {
                throw new JposException(JPOS_E_BUSY);
            }

            checkOnLine();
            synchronized (printer) {
                request.execute(this);
            }
        }
    }

    private void executePrinterRequest(FiscalPrinterRequest request) {
        while (true) {
            try {
                request.execute(this);
                outputCompleteEvent(request.getId());
                break;
            } catch (Exception e) {
                JposException jpose = JposExceptionHandler.getJposException(e);

                setState(JPOS_S_ERROR);
                switch (jpose.getErrorCode()) {
                    case JPOS_E_TIMEOUT:
                        setPowerState(JPOS_PS_OFFLINE);
                        break;
                }

                ErrorEvent event = new ErrorEvent(this, jpose.getErrorCode(),
                        jpose.getErrorCodeExtended(), JPOS_EL_OUTPUT,
                        JPOS_ER_CLEAR);

                /*
                 * if (getFreezeEvents()) { events.add(new ErrorEventRequest(cb,
                 * event)); } else { if (cb != null) cb.fireErrorEvent(event);
                 * if (event.getErrorResponse() != JPOS_ER_RETRY) break; }

            }
        }
    }

    // event delivery routine
    public void eventProc() {
        try {
            while (eventThreadEnabled) {
                synchronized (events) {
                    while (!events.isEmpty()) {
                        ((Runnable) events.remove(0)).run();

                    }
                    events.wait();
                }
            }
        } catch (InterruptedException e) {
            // Restore the interrupted status
            logger.error("InterruptedException", e);
            Thread.currentThread().interrupt();
        }
    }

    public void asyncProc() {
        try {
            while (asyncThreadEnabled) {
                synchronized (requests) {
                    while (!requests.isEmpty()) {
                        setState(JPOS_S_BUSY);
                        executePrinterRequest((FiscalPrinterRequest) requests
                                .remove(0));
                    }
                    requests.wait();
                }
                setState(JPOS_S_IDLE);
            }
        } catch (InterruptedException e) {
            // Restore the interrupted status
            logger.error("InterruptedException", e);
            Thread.currentThread().interrupt();
        }
    }

    public void updateStatus(PrinterStatus status) {

        PrinterFlags flags = status.getPrinterFlags();
        setRecPaperState(flags.isRecEmpty(), flags.isRecNearEnd());
        setJrnPaperState(flags.isJrnEmpty(), flags.isJrnNearEnd());
        setSlpPaperState(flags.isSlpEmpty(), flags.isSlpNearEnd());
        setCoverState(flags.isCoverOpened());
        dayOpened = status.getPrinterMode().getDayOpened();
    }

    private void startPoll() {
        deviceThreadEnabled = true;
        deviceThread = new Thread(new DeviceTarget(this));
        deviceThread.start();
    }

    private void stopPoll() {
        if (deviceThreadEnabled) {
            deviceThreadEnabled = false;
            deviceThread.join();
            deviceThread = null;
            setPowerState(JPOS_PS_UNKNOWN);
        }
    }

    public void setDeviceEnabled(boolean deviceEnabled) {
        checkClaimed();
        if (this.deviceEnabled != deviceEnabled) {
            if (deviceEnabled) {
                searchDevice(0);
                connected = true;
                setPowerState(JPOS_PS_ONLINE);
                setJrnPaperState(true, true);

                updateDeviceMetrics();
                checkEcrMode();
                cancelReceipt();
                writeTables();
                readTables();
                readPrinterStatus();
                readEJActivation();
                updateHeaderTrailer();

                // if polling enabled - create device thread
                if (params.pollEnabled) {
                    startPoll();
                }
                if (params.FSEnabled) {
                    fsService.setHost(params.FSHost);
                    fsService.setPort(params.FSPort);
                    fsService.setConnectTimeout(params.FSConnectTimeout);
                    fsService.setPollPeriod(params.FSPollPeriod);
                    fsService.start();
                }
            } else {
                stopPoll();
                connected = false;
                if (params.FSEnabled) {
                    fsService.stop();
                }
            }
            this.deviceEnabled = deviceEnabled;
        }
    }

    public void updateHeaderTrailer() {
        if (isHeaderInDriver()) {
            setNumHeaderLines(params.numHeaderLines);
            setNumTrailerLines(params.numTrailerLines);
        } else {
            setNumHeaderLines(getModel().getNumHeaderLines());
            setNumTrailerLines(getModel().getNumTrailerLines());
        }
    }

    public void setNumHeaderLines(int numHeaderLines) {
        params.numHeaderLines = numHeaderLines;
        header.setCount(numHeaderLines);
    }

    public void setNumTrailerLines(int numTrailerLines) {
        params.numTrailerLines = numTrailerLines;
        trailer.setCount(numTrailerLines);
    }

    public LongPrinterStatus readLongStatus() {
        return getPrinter().readLongStatus();
    }

    private void updateDeviceMetrics() {
        ShortPrinterStatus shortStatus = null;
        try {
            if (getModel().getCapShortStatus()) {
                shortStatus = getPrinter().readShortStatus();
            }
        } catch (SmFiscalPrinterException e) {
            logger.error("readShortStatus error", e);
        }
        if (shortStatus != null) {
            LogWriter.write(shortStatus);
        }
        LongPrinterStatus longStatus = readLongStatus();
        LogWriter.write(longStatus);
        LogWriter.write(getDeviceMetrics());
        LogWriter.writeSeparator();

        physicalDeviceName = getDeviceMetrics().getDeviceName() + ", № "
                + longStatus.getSerial();

        String formatString = Localizer
                .getString(Localizer.PhysicalDeviceDescription);

        // iceDescription = "%s,  %s, ПО ФР: %s.%d, %s, ПО ФП: %s.%d, %s"
        physicalDeviceDescription = getDeviceMetrics().getDeviceName() + ", "
                + longStatus.getSerial() + ", ПО ФР: "
                + longStatus.getFirmwareVersion() + "."
                + String.valueOf(longStatus.getFirmwareBuild()) + ", "
                + longStatus.getFirmwareDate().toString() + ", ПО ФП: "
                + longStatus.getFMFirmwareVersion() + "."
                + String.valueOf(longStatus.getFMFirmwareBuild()) + ", "
                + longStatus.getFMFirmwareDate().toString();

        logger.debug("PhysicalDeviceName: " + physicalDeviceName);
        logger.debug("PhysicalDeviceDescription: " + physicalDeviceDescription);

        // update device parameters
        statistics.serialNumber = longStatus.getSerial();
        statistics.firmwareRevision = longStatus.getFirmwareRevision();
        getPrinterImages().setMaxSize(getMaxGraphicsHeight());
        checkLicense(longStatus.getSerial());
        // read EJ status if it present
        if (getModel().getCapEJPresent()
                && longStatus.getPrinterFlags().isEJPresent()
                && (longStatus.getRegistrationNumber() != 0)) {
            ReadEJStatus readEJStatus = getPrinter().readEJStatus();
            if (readEJStatus.getResultCode() == 0) {
                statusEJ = readEJStatus.getStatus();
            }
        }
    }

    private void readEJActivation() {
        LongPrinterStatus longStatus = readLongStatus();
        if ((longStatus.getRegistrationNumber() > 0)
                && (longStatus.getPrinterFlags().isEJPresent())) {
            String[] lines = printer.readEJActivationText(6);
            aEJActivation = EJReportParser.parseEJActivation(lines);
        } else {
            aEJActivation = new EJActivation();
        }
    }

    private void checkLicense(String serial) {

        if ((getModel().getModelID() == PrinterConst.PRINTER_MODEL_SHTRIH_MINI_FRK)
                || (getModel().getModelID() == PrinterConst.PRINTER_MODEL_SHTRIH_MINI_FRK2)) {
            if (isLicenseValid) {
                return;
            }

            logger.debug("Reading license file...");
            try {
                String fileName = "shtrihjavapos.lic";
                FileReader fileReader = new FileReader(fileName);
                try {
                    BufferedReader input = new BufferedReader(fileReader);
                    try {
                        String license = input.readLine();
                        if (license != null) {
                            logger.debug("Checking license data...");
                            MessageDigest md = MessageDigest.getInstance("MD5");
                            md.update(serial.getBytes());
                            String digest = bytesToHex(md.digest());
                            isLicenseValid = license.equals(digest);
                            if (!isLicenseValid) {
                                throw new Exception(
                                        "Invalid license file for this device");
                            }
                        }
                    } finally {
                        input.close();
                    }
                } finally {
                    fileReader.close();
                }
            } catch (Exception e) {
                logger.error(e);
            }
        }
    }
    final protected static char[] hexArray = "0123456789ABCDEF".toCharArray();

    public static String bytesToHex(byte[] bytes) {
        char[] hexChars = new char[bytes.length * 2];
        for (int j = 0; j < bytes.length; j++) {
            int v = bytes[j] & 0xFF;
            hexChars[j * 2] = hexArray[v >>> 4];
            hexChars[j * 2 + 1] = hexArray[v & 0x0F];
        }
        return new String(hexChars);
    }

    public void setPollEnabled(boolean value) {
        if (value != params.pollEnabled) {
            if (value) {
                if (deviceEnabled) {
                    startPoll();
                }
            } else {
                stopPoll();
            }
        }
        params.pollEnabled = value;
    }

    public String getDeviceServiceDescription() {
        checkOpened();
        return encodeText(deviceServiceDescription);
    }

    public int getDeviceServiceVersion() {
        checkOpened();
        return deviceServiceVersion;
    }

    public boolean getFreezeEvents() {
        checkOpened();
        return freezeEvents;
    }

    public void setFreezeEvents(boolean freezeEvents) {
        checkOpened();
        if (freezeEvents != getFreezeEvents()) {
            this.freezeEvents = freezeEvents;
            if (freezeEvents) {
                stopEventThread();
            } else {
                startEventThread();
            }
        }
    }

    private void stopEventThread() {
        if (eventThreadEnabled) {
            eventThreadEnabled = false;
            synchronized (events) {
                events.notifyAll();
            }
            if (eventThread != null) {
                eventThread.join();
            }
            eventThread = null;
        }
    }

    private void startEventThread() {
        if (!eventThreadEnabled) {
            eventThreadEnabled = true;
            if (eventThread == null) {
                eventThread = new Thread(new EventTarget(this));
                eventThread.start();
            }
        }
    }

    public String getPhysicalDeviceDescription() {
        checkOpened();
        return encodeText(physicalDeviceDescription);
    }

    public String getPhysicalDeviceName() {
        checkOpened();
        return encodeText(physicalDeviceName);
    }

    public int getState() {
        return state;
    }

    // --------------------------------------------------------------------------
    // Properties
    // --------------------------------------------------------------------------
    // Capabilities
    public boolean getCapAdditionalLines() {
        return capAdditionalLines;
    }

    public boolean getCapAmountAdjustment() {
        return capAmountAdjustment;
    }

    public boolean getCapAmountNotPaid() {
        return capAmountNotPaid;
    }

    public boolean getCapCheckTotal() {
        return capCheckTotal;
    }

    public boolean getCapCoverSensor() {
        return getModel().getCapCoverSensor();
    }

    public boolean getCapDoubleWidth() {
        return getModel().getCapDoubleWidth();
    }

    public boolean getCapDuplicateReceipt() {
        return getModel().getCapDuplicateReceipt();
    }

    public void setDuplicateReceipt(boolean aduplicateReceipt) {
        if (!getCapDuplicateReceipt()) {
            throw new JposException(
                    JPOS_E_ILLEGAL,
                    Localizer
                    .getString(Localizer.receiptDuplicationNotSupported));
        }
        duplicateReceipt = aduplicateReceipt;
    }

    public boolean getCapFixedOutput() {
        return capFixedOutput;
    }

    public boolean getCapHasVatTable() {
        return getModel().getCapHasVatTable();
    }

    public boolean getCapIndependentHeader() {
        return capIndependentHeader;
    }

    public boolean getCapItemList() {
        return capItemList;
    }

    public boolean getCapJrnEmptySensor() {
        return getModel().getCapJrnEmptySensor();
    }

    public boolean getCapJrnNearEndSensor() {
        return getModel().getCapJrnNearEndSensor();
    }

    public boolean getCapJrnPresent() {
        return getModel().getCapJrnPresent();
    }

    public boolean getCapNonFiscalMode() {
        return capNonFiscalMode;
    }

    public boolean getCapOrderAdjustmentFirst() {
        return capOrderAdjustmentFirst;
    }

    public boolean getCapPercentAdjustment() {
        return capPercentAdjustment;
    }

    public boolean getCapPositiveAdjustment() {
        return capPositiveAdjustment;
    }

    public boolean getCapPowerLossReport() {
        return capPowerLossReport;
    }

    public int getCapPowerReporting() {
        return JPOS_PR_STANDARD;
    }

    public boolean getCapPredefinedPaymentLines() {
        return capPredefinedPaymentLines;
    }

    public boolean getCapReceiptNotPaid() {
        return capReceiptNotPaid;
    }

    public boolean getCapRecEmptySensor() {
        return getModel().getCapRecEmptySensor();
    }

    public boolean getCapRecNearEndSensor() {
        return getModel().getCapRecNearEndSensor();
    }

    public boolean getCapRecPresent() {
        return getModel().getCapRecPresent();
    }

    public boolean getCapRemainingFiscalMemory() {
        return capRemainingFiscalMemory;
    }

    public boolean getCapReservedWord() {
        return capReservedWord;
    }

    public boolean getCapSetHeader() {
        return capSetHeader;
    }

    public boolean getCapSetPOSID() {
        return capSetPOSID;
    }

    public boolean getCapSetStoreFiscalID() {
        return capSetStoreFiscalID;
    }

    public boolean getCapSetTrailer() {
        return capSetTrailer;
    }

    public boolean getCapSetVatTable() {
        return capSetVatTable;
    }

    public boolean getCapSlpEmptySensor() {
        return getModel().getCapSlpEmptySensor();
    }

    public boolean getCapSlpFiscalDocument() {
        return capSlpFiscalDocument;
    }

    public boolean getCapSlpFullSlip() {
        return capSlpFullSlip;
    }

    public boolean getCapSlpNearEndSensor() {
        return getModel().getCapSlpNearEndSensor();
    }

    public boolean getCapSlpPresent() {
        return getModel().getCapSlpPresent();
    }

    public boolean getCapSlpValidation() {
        return capSlpValidation;
    }

    public boolean getCapSubAmountAdjustment() {
        return capSubAmountAdjustment;
    }

    public boolean getCapSubPercentAdjustment() {
        return capSubPercentAdjustment;
    }

    public boolean getCapSubtotal() {
        return capSubtotal;
    }

    public boolean getCapTrainingMode() {
        return capTrainingMode;
    }

    public boolean getCapValidateJournal() {
        return capValidateJournal;
    }

    public boolean getCapXReport() {
        return capXReport;
    }

    public int getOutputID() {
        checkOpened();
        return outputID;
    }

    public int getPowerNotify() {
        checkOpened();
        return powerNotify;
    }

    public void setPowerNotify(int powerNotify) {
        if (deviceEnabled) {
            throw new JposException(JPOS_E_ILLEGAL,
                    Localizer.getString(Localizer.deviceIsEnabled));
        }
        this.powerNotify = powerNotify;
    }

    public int getPowerState() {
        checkOpened();
        return powerState;
    }

    public int getAmountDecimalPlace() {
        return getModel().getAmountDecimalPlace();
    }

    public boolean getAsyncMode() {
        return asyncMode;
    }

    public void setAsyncMode(boolean asyncMode) {
        if (asyncMode != this.asyncMode) {
            if (asyncMode) {
                asyncThreadEnabled = true;
                asyncThread = new Thread(new AsyncTarget(this));
                asyncThread.start();
            } else {
                asyncThreadEnabled = false;
                asyncThread.join();
                asyncThread = null;
            }
            this.asyncMode = asyncMode;
        }
    }

    public boolean getCheckTotal() {
        return params.checkTotal;
    }

    public void setCheckTotal(boolean value) {
        params.checkTotal = value;
    }

    public int getCountryCode() {
        checkEnabled();
        return countryCode;
    }

    public boolean getCoverOpen() {
        checkEnabled();
        if (!getCapCoverSensor()) {
            return false;
        } else {
            return coverOpened;
        }
    }

    public boolean getDayOpened() {
        checkEnabled();
        return dayOpened;
    }

    public int getDescriptionLength() {
        return getModel().getTextLength(params.font);
    }

    public boolean getDuplicateReceipt() {
        if (!getCapDuplicateReceipt()) {
            return false;
        }
        return duplicateReceipt;
    }

    public int getErrorLevel() {
        return errorLevel;
    }

    public int getErrorOutID() {
        checkEnabled();
        return errorOutID;
    }

    public int getErrorState() {
        return errorState;
    }

    public int getErrorStation() {
        return errorStation;
    }

    public String getErrorString() {
        return encodeText(errorString);
    }

    public boolean getFlagWhenIdle() {
        return flagWhenIdle;
    }

    public void setFlagWhenIdle(boolean value) {
        if (value != flagWhenIdle) {
            flagWhenIdle = value;

            // If the State is already set to S_IDLE
            // when this property is set to true, then a
            // StatusUpdateEvent is enqueued immediately.
            if ((state == JPOS_S_IDLE) && (flagWhenIdle)) {
                statusUpdateEvent(FPTR_SUE_IDLE);

                // This property is automatically reset to false when
                // the status event is delivered.
                // We reset this property when status event is enqueued.
                // I think that it is a feature, not a code bug.
                flagWhenIdle = false;
            }
        }
    }

    public boolean getJrnEmpty() {
        checkEnabled();
        return jrnEmpty;
    }

    public boolean getJrnNearEnd() {
        checkEnabled();
        return jrnNearEnd;
    }

    public int getMessageLength() {
        return getModel().getTextLength(printer.getParams().getFont());
    }

    public int getNumHeaderLines() {
        return params.numHeaderLines;
    }

    public int getNumTrailerLines() {
        return params.numTrailerLines;
    }

    public int getNumVatRates() {
        return getModel().getNumVatRates();
    }

    public String getPredefinedPaymentLines() {
        return encodeText(predefinedPaymentLines);
    }

    public int getPrinterState() {
        checkEnabled();
        return printerState.getValue();
    }

    public int getQuantityDecimalPlaces() {
        checkEnabled();
        return quantityDecimalPlaces;
    }

    public int getQuantityLength() {
        checkEnabled();
        return quantityLength;
    }

    public boolean getRecEmpty() {
        checkEnabled();
        if (getCapRecEmptySensor()) {
            return recEmpty;
        } else {
            return false;
        }
    }

    public boolean getRecNearEnd() {
        checkEnabled();
        if (getCapRecNearEndSensor()) {
            return recNearEnd;
        }
        return false;
    }

    public int getRemainingFiscalMemory() {
        checkEnabled();
        return readLongStatus().getFMFreeRecords();
    }

    public String getReservedWord() {
        return encodeText(reservedWord);
    }

    public boolean getSlpEmpty() {
        checkEnabled();
        return slpEmpty;
    }

    public boolean getSlpNearEnd() {
        checkEnabled();
        return slpNearEnd;
    }

    public int getSlipSelection() {
        checkEnabled();
        return slipSelection;
    }

    public void setSlipSelection(int value) {
        checkEnabled();
        if (value == FPTR_SS_FULL_LENGTH) {
            slipSelection = value;
        } else {
            throw new JposException(JPOS_E_ILLEGAL,
                    Localizer.getString(Localizer.invalidParameterValue)
                    + "SlipSelection");
        }
    }

    public boolean getTrainingModeActive() {
        checkEnabled();
        return trainingModeActive;
    }

    public boolean getCapAdditionalHeader() {
        return capAdditionalHeader;
    }

    public boolean getCapAdditionalTrailer() {
        return capAdditionalTrailer;
    }

    public boolean getCapChangeDue() {
        return capChangeDue;
    }

    public boolean getCapEmptyReceiptIsVoidable() {
        return capEmptyReceiptIsVoidable;
    }

    public boolean getCapFiscalReceiptStation() {
        switch (fiscalReceiptStation) {
            case FPTR_RS_RECEIPT:
                return capFiscalReceiptStation;

            case FPTR_RS_SLIP:
                return capFiscalSlipStation;

            default:
                throw new JposException(JPOS_E_ILLEGAL,
                        Localizer.getString(Localizer.invalidPropertyValue)
                        + "fiscalReceiptStation");
        }
    }

    public boolean getCapFiscalReceiptType() {
        return capFiscalReceiptType;
    }

    public boolean getCapMultiContractor() {
        return capMultiContractor;
    }

    public boolean getCapOnlyVoidLastItem() {
        return capOnlyVoidLastItem;
    }

    public boolean getCapPackageAdjustment() {
        return capPackageAdjustment;
    }

    public boolean getCapPostPreLine() {
        return capPostPreLine;
    }

    public boolean getCapSetCurrency() {
        return capSetCurrency;
    }

    public boolean getCapTotalizerType() {
        return capTotalizerType;
    }

    // Properties
    public int getActualCurrency() {
        checkEnabled();
        return actualCurrency;
    }

    public String getAdditionalHeader() {
        checkEnabled();
        if (!getCapAdditionalHeader()) {
            throw new JposException(JPOS_E_ILLEGAL,
                    Localizer.getString(Localizer.additionalHeaderNotSupported));
        }
        return encodeText(additionalHeader);
    }

    public void setAdditionalHeader(String value) {
        checkEnabled();
        if (!getCapAdditionalHeader()) {
            throw new JposException(JPOS_E_ILLEGAL,
                    Localizer.getString(Localizer.additionalHeaderNotSupported));
        }
        additionalHeader = decodeText(value);
    }

    public String getAdditionalTrailer() {
        checkEnabled();
        if (!getCapAdditionalTrailer()) {
            throw new JposException(JPOS_E_ILLEGAL,
                    Localizer
                    .getString(Localizer.additionalTrailerNotSupported));
        }
        return encodeText(additionalTrailer);
    }

    public void setAdditionalTrailer(String value) {
        checkEnabled();
        if (!getCapAdditionalTrailer()) {
            throw new JposException(JPOS_E_ILLEGAL,
                    Localizer
                    .getString(Localizer.additionalTrailerNotSupported));
        }
        additionalTrailer = decodeText(value);
        trailer.setAdditionalText(additionalTrailer);
    }

    public String getChangeDue() {
        if (!getCapChangeDue()) {
            throw new JposException(JPOS_E_ILLEGAL,
                    Localizer.getString(Localizer.changeDueTextNotSupported));
        }
        return encodeText(changeDue);
    }

    public void setChangeDue(String value) {
        if (!getCapChangeDue()) {
            throw new JposException(JPOS_E_ILLEGAL,
                    Localizer.getString(Localizer.changeDueTextNotSupported));
        }
        changeDue = decodeText(value);
    }

    public int getContractorId() {
        checkEnabled();
        return contractorId;
    }

    public void setContractorId(int value) {
        checkEnabled();
        if (!getCapMultiContractor()) {
            throw new JposException(
                    JPOS_E_ILLEGAL,
                    Localizer
                    .getString(Localizer.multipleContractorsNotSupported));
        }
        contractorId = value;
    }

    public int getDateType() {
        checkEnabled();
        return dateType;
    }

    public void setDateType(int value) {
        checkEnabled();
        switch (value) {
            case FPTR_DT_EOD:
            case FPTR_DT_RTC:

                dateType = value;
                break;
            default:
                throw new JposException(JPOS_E_ILLEGAL,
                        Localizer.getString(Localizer.invalidParameterValue)
                        + "DateType");
        }
    }

    public int getFiscalReceiptStation() {
        checkEnabled();
        return fiscalReceiptStation;
    }

    public void setFiscalReceiptStation(int value) {
        checkEnabled();
        // Check if the Fiscal Printer is currently in the Monitor State
        checkPrinterState(FPTR_PS_MONITOR);

        switch (value) {
            case FPTR_RS_RECEIPT:

                fiscalReceiptStation = value;
                break;

            case FPTR_RS_SLIP:
                if (!capFiscalSlipStation) {
                    throw new JposException(JPOS_E_ILLEGAL,
                            Localizer.getString(Localizer.slipStationNotPresent));
                }
                fiscalReceiptStation = value;
                break;
            default:
                throw new JposException(JPOS_E_ILLEGAL,
                        Localizer.getString(Localizer.invalidParameterValue));
        }
    }

    public int getFiscalReceiptType() {
        checkEnabled();
        return fiscalReceiptType;
    }

    public int getMessageType() {
        return messageType;
    }

    public void setMessageType(int value) {
        if (value == FPTR_MT_FREE_TEXT) {
            messageType = value;
        } else {
            throw new JposException(JPOS_E_ILLEGAL,
                    Localizer.getString(Localizer.messageTypeNotSupported));
        }
    }

    public String getPostLine() {
        checkEnabled();
        return encodeText(params.postLine);
    }

    public void setPostLine(String value) {
        checkEnabled();
        params.postLine = decodeText(value);
    }

    public String getPreLine() {
        checkEnabled();
        return encodeText(params.preLine);
    }

    public void setPreLine(String value) {
        checkEnabled();
        params.preLine = decodeText(value);
    }

    public int getTotalizerType() {
        checkEnabled();
        return totalizerType;
    }

    public void setTotalizerType(int value) {
        checkEnabled();
        switch (value) {
            case FPTR_TT_DOCUMENT: // Document totalizer
            case FPTR_TT_DAY: // Day totalizer
            case FPTR_TT_RECEIPT: // Receipt totalizer
            case FPTR_TT_GRAND: // Grand totalizer
                totalizerType = value;
                break;

            default:
                throw new JposException(JPOS_E_ILLEGAL,
                        Localizer.getString(Localizer.invalidParameterValue));
        }
    }

    private void checkOpened() {
        if (state == JPOS_S_CLOSED) {
            throw new JposException(JPOS_E_CLOSED);
        }
    }

    // write fields if file specified
    private void writeFieldsFile() {
        logger.debug("writeFieldsFile");
        try {
            if (params.fieldsFilesPath.length() != 0) {
                writeFieldsFileFromPath();

            } else if (params.fieldsFileName.length() != 0) {
                writeFieldsFileFromFileName();
            }
        } catch (Exception e) {
            logger.error(e.getMessage());
        }
    }

    private void writeFieldsFileFromPath() {
        logger.debug("writeFieldsFileFromPath");
        File dir = new File(params.fieldsFilesPath);
        if (!dir.exists()) {
            logger.debug("Directory does not exists");
            return;
        }
        File[] files = dir.listFiles();
        if (files == null) {
            logger.debug("dir.listFiles() returns null");
            return;
        }
        logger.debug("files.length = " + files.length);
        for (int i = 0; i < files.length; i++) {
            File file = files[i];
            logger.debug("Found file '" + file.getAbsolutePath() + "')");
            if (FileUtils.getExtention(file.getName()).equals(".csv")) {
                if (file.exists()) {
                    PrinterFields fields = new PrinterFields();
                    CsvTablesReader reader = new CsvTablesReader();
                    reader.load(file.getAbsolutePath(), fields);

                    String fileModelName = fields.getModelName();
                    String deviceModelName = getPrinter().getDeviceMetrics().getDeviceName();
                    if (fileModelName.equalsIgnoreCase(deviceModelName)) {
                        logger.debug("Write fields values from file '" + file.getAbsolutePath() + "')");
                        // set font for driver text
                        PrinterField field = fields.find(8, 1, 1);
                        if (field != null) {
                            int fontNumber = Integer.parseInt(field.getValue());
                            logger.debug("Font number changed to " + fontNumber);
                            params.font = new FontNumber(fontNumber);
                        }
                        writeTables(fields);
                        break;
                    } else {
                        logger.debug("fileModelName <> deviceModelName, " + fileModelName + " <> " + deviceModelName);
                    }
                }

            }
        }
    }

    private void writeFieldsFileFromFileName() {
        File file = new File(params.fieldsFileName);
        if (!file.exists()) {
            logger.debug("File not exists: " + params.fieldsFileName);
            return;
        }
        PrinterFields fields = new PrinterFields();
        CsvTablesReader reader = new CsvTablesReader();
        reader.load(file.getAbsolutePath(), fields);

        String fileModelName = fields.getModelName();
        String deviceModelName = getPrinter().getDeviceMetrics().getDeviceName();
        if (fileModelName.length() > 0) {
            if (!fileModelName.equalsIgnoreCase(deviceModelName)) {
                logger.error("File model name does not match device name");
                logger.error("'" + fileModelName + "' <> '" + deviceModelName + "'");
                return;
            }
        }
        logger.debug("Write fields values from file '" + file.getAbsolutePath() + "')");
        writeTables(fields);
    }

    private void writePaymentNames() {
        // payment names
        Vector paymentNames = params.getPaymentNames();
        for (int i = 0; i < paymentNames.size(); i++) {
            FptrPaymentName paymentName = (FptrPaymentName) paymentNames.get(i);
            if (paymentName.getCode() != PrinterConst.SMFP_TABLE_TAX_ROW_CASH) {
                int result = getPrinter().writeTable(
                        PrinterConst.SMFP_TABLE_PAYTYPE, paymentName.getCode(),
                        1, paymentName.getName());
                if (printer.failed(result)) {
                    break;
                }
            }
        }
    }

    private void writeTables() {
        if (params.tableMode == SMFPTR_TABLE_MODE_AUTO) {
            header.initDevice();
            writePaymentNames();
            getPrinter().writeParameter(SMFP_PARAMID_CUT_MODE, 0);
            getPrinter().writeParameter(SMFP_PARAMID_DRAWER_ENABLED,
                    params.autoOpenDrawer);
        }
        writeFieldsFile();
    }

    public void readTables() {
        try {
            // read tax names
            String[] fieldValue = new String[1];
            for (int i = 0; i < 4; i++) {
                fieldValue[0] = "";
                if (printer.readTable(6, i + 1, 2, fieldValue) == 0) {
                    params.taxNames[i] = fieldValue[0];
                }
            }
        } catch (Exception e) {
            logger.error(e.getMessage());
        }
    }

    public void claim(int timeout) {
        checkOpened();
        if (!claimed) {
            port.setPortName(params.portName);
            port.setBaudRate(params.getBaudRate());
            port.open(timeout);
            claimed = true;
        }
    }

    public void close() {
        checkOpened();
        setEventCallbacks(null);
        setFreezeEvents(true);
        if (claimed) {
            release();
        }
        setState(JPOS_S_CLOSED);
        statistics.save(params.statisticFileName);
        monitoringServer.stop();
    }

    public void checkHealth(int level) {
        checkEnabled();
        checkOnLine();
        switch (level) {
            case JPOS_CH_INTERNAL:
                checkHealthText = InternalCheckHelthReport.getReport(printer);
                break;

            case JPOS_CH_EXTERNAL:
                checkHealthText = checkHealthExternal();
                break;

            default:
                throw new JposException(JPOS_E_ILLEGAL,
                        Localizer.getString(Localizer.invalidParameterValue)
                        + ", level");
        }
    }

    public PrinterImages getPrinterImages() {
        return getPrinter().getPrinterImages();
    }

    public void printImage(int index) {
        printImage(getPrinterImages().get(index));
    }

    public void printImage(PrinterImage image) {
        if (image == null) {
            return;
        }
        printer.loadImage(image, true);
        if (getModel().getModelID() == PrinterConst.PRINTER_MODEL_SHTRIH_M_FRK) {
            printer.printGraphics(image.getStartPos() + 1, image.getEndPos() + 1);
        } else {
            printer.printGraphics(image.getStartPos(), image.getEndPos());
        }
    }

     // Write tables from CSV file and don't check *
    public void writeTables(String fileName) {
        PrinterFields fields = new PrinterFields();
        CsvTablesReader reader = new CsvTablesReader();
        reader.load(fileName, fields);
        writeTables(fields);
    }

    public void writeTables(PrinterFields fields) {
        String[] fieldValue = new String[1];
        for (int i = 0; i < fields.size(); i++) {
            fieldValue[0] = "";
            PrinterField field = fields.get(i);
            // Do not write tables SMFP_TABLE_TEXT and SMFP_TABLE_PAYTYPE
            // to not break correct driver behavior
            if ((field.getTable() != PrinterConst.SMFP_TABLE_TEXT)
                    && (field.getTable() != PrinterConst.SMFP_TABLE_PAYTYPE)) {
                // if field have correct number
                if (printer.isValidField(field.getTable(), field.getRow(),
                        field.getField())) {
                    getPrinter().check(
                            printer.readTable(field.getTable(), field.getRow(),
                                    field.getField(), fieldValue));

                    if (fieldValue[0].compareTo(field.getValue()) != 0) {
                        getPrinter().writeTable(field.getTable(),
                                field.getRow(), field.getField(),
                                field.getValue());
                    }
                }
            }
        }
    }

    // Read tables from CSV file *
    public void readTables(String fileName) {
        PrinterTables tables = new PrinterTables();
        CsvTablesWriter writer = new CsvTablesWriter();

        getPrinter().readTables(tables);
        writer.save(SysUtils.getFilesPath() + fileName, tables);
    }

    public void directIO(int command, int[] data, Object object)
            throws Exception
    {
        if (params.compatibilityLevel == 1){
        (new DirectIOHandler(this)).directIO(command, data, object);
        } else{
            (new DirectIOHandler2(this)).directIO(command, data, object);
        }
    }

    // try to connect to device
    private boolean connectDevice(String searchPortName, int searchBaudRate,
            int searchTimeout) {
        logger.debug("connectDevice(" + searchPortName + ", " + searchBaudRate + ", "
                + searchTimeout + ")");
        try {
            port.setPortName(searchPortName);
            port.setBaudRate(searchBaudRate);
            port.open(searchTimeout);

            printer.connect();
            printer.readDeviceMetrics();
            getPrinter().initialize();

            // always set port parameters to update byte
            // receive timeout in fiscal printer
            int baudRateIndex = printer.getBaudRateIndex(params.getBaudRate());
            printer.writePortParams(0, baudRateIndex, params.getDeviceByteTimeout());
            params.setBaudRate(getModel().getSupportedBaudRates()[baudRateIndex]);

            // if baudrate changed - reopen port
            if (searchBaudRate != params.getBaudRate()) {
                port.setPortName(searchPortName);
                port.setBaudRate(params.getBaudRate());
                port.open(searchTimeout);
            }
            return true;
        } catch (DeviceException e) {
            logger.error(e);
            return false;
        }
    }

    private void searchDevice(int timeout) {
        if (port.isSearchByBaudRateEnabled()) {
            searchSerialDevice(timeout);
        } else {
            port.setPortName(params.portName);
            port.open(timeout);
            printer.connect();
            printer.readDeviceMetrics();
            getPrinter().initialize();
        }
    }

    // search device on ports and baudrates
    private void searchSerialDevice(int timeout) {
        if (connectDevice(params.portName, params.getBaudRate(), timeout)) {
            return;
        }

        if (params.searchByPortEnabled) {
            String[] ports = SerialPrinterPort.getPortNames();
            for (int i = 0; i < ports.length; i++) {
                String portName = ports[i];
                if (!params.portName.equalsIgnoreCase(portName)) {
                    if (params.searchByBaudRateEnabled) {
                        if (searchByBaudRates(portName, timeout)) {
                            return;
                        }
                    } else if (connectDevice(portName, params.getBaudRate(), timeout)) {
                        return;
                    }
                }
            }
        } else if (params.searchByBaudRateEnabled) {
            if (searchByBaudRates(params.portName, timeout)) {
                return;
            }
        }
        throw new JposException(JPOS_E_NOHARDWARE);
    }

    private boolean searchByBaudRates(String portName, int timeout)
            {
        int[] deviceBaudRates = {4800, 9600, 19200, 38400, 57600, 115200, 2400};
        for (int i = 0; i < deviceBaudRates.length; i++) {
            int baudRate = deviceBaudRates[i];
            if (baudRate != params.getBaudRate()) {
                if (connectDevice(portName, baudRate, timeout)) {
                    return true;
                }
            }
        }
        return false;
    }

    public void setEventCallbacks(EventCallbacks cb) {
        this.cb = cb;
    }

    public void open(String logicalName, EventCallbacks cb) {
        initializeData();
        logicalName = decodeText(logicalName);
        this.logicalName = logicalName;
        setEventCallbacks(cb);
        if (jposEntry != null) {
            params.loadLogEnabled(jposEntry);
            logger.setEnabled(params.logEnabled);

            logger.debug("-----------------------------------------------");
            logger.debug("SHTRIH-M JavaPos FiscalPrinter service");
            logger.debug("DeviceServiceVersion: "
                    + String.valueOf(deviceServiceVersion));
            logger.debug("Java version: " + System.getProperty("java.version"));
            logger.debug("File encoding: " + System.getProperty("file.encoding"));
            logger.debug("-----------------------------------------------");

            params.load(jposEntry);
            port = PrinterPortFactory.createInstance(params);
            device = ProtocolFactory.getProtocol(params, port);
            printer = new SMFiscalPrinterImpl(port, device, params, this);
            fsService = new FSService(printer);
            if (params.headerMode == SMFPTR_HEADER_MODE_DRIVER) {
                header = new DriverHeader(printer);
                trailer = new DriverTrailer(printer);
            } else {
                header = new DeviceHeader(printer);
                trailer = new DeviceTrailer(printer);
            }

            printer.setEscPrinter(new NCR7167Printer(this));

            getPrinter().addEvents(this);
            if (params.receiptReportEnabled) {
                getPrinter()
                        .addEvents(new ReceiptReportFilter(printer, params));
            }
            receiptPrinter = new ReceiptPrinterImpl(printer, params);
            getPrinter().setTaxPassword(params.taxPassword);
            getPrinter().setUsrPassword(params.usrPassword);
            getPrinter().setSysPassword(params.sysPassword);

            statistics.load(params.statisticFileName);
            Localizer.init(params.messagesFileName);
            getPrinter().setWrapText(params.wrapText);
            createFilters();

            if (params.monitoringEnabled) {
                monitoringServer.start(params.getMonitoringPort());
            }
        }
        loadProperties();
        receipt = new NonfiscalReceipt(createReceiptContext());
        state = JPOS_S_IDLE;
        setFreezeEvents(false);
    }

    private void createFilters() {
        SimpleDateFormat timeFormat = new SimpleDateFormat("HH:mm");
        Date time1 = timeFormat.parse(params.zeroPriceFilterTime1);
        Date time2 = timeFormat.parse(params.zeroPriceFilterTime2);

        FiscalPrinterFilter113 filter = new ZeroPriceFilter(
                params.zeroPriceFilterEnabled, time1, time2,
                params.zeroPriceFilterErrorText);
        filters.clear();
        filters.add(filter);

        if (params.discountFilterEnabled) {
            filter = new DiscountFilter(this);
            filters.add(filter);
        }
    }

    public void release() {
        saveProperties();
        setDeviceEnabled(false);
        claimed = false;
        getPrinter().closePort();
    }

    // ////////////////////////////////////////////////////////////////////////////
    // Fiscal Document
    // ////////////////////////////////////////////////////////////////////////////
    private void noSlipStationError() {
        throw new JposException(JPOS_E_ILLEGAL,
                Localizer.getString(Localizer.slipStationNotPresent));
    }

    public void beginFiscalDocument(int documentAmount) {
        checkEnabled();
        noSlipStationError();
    }

    public void printFiscalDocumentLine(String documentLine) {
        checkEnabled();
        noSlipStationError();
    }

    public void endFiscalDocument() {
        checkEnabled();
        noSlipStationError();
    }

    // ////////////////////////////////////////////////////////////////////////////
    // Fixed Output
    // ////////////////////////////////////////////////////////////////////////////
    public void beginFixedOutput(int station, int documentType)
            {
        checkEnabled();
        throw new JposException(JPOS_E_ILLEGAL);
    }

    public void printFixedOutput(int documentType, int lineNumber, String data)
            {
        checkEnabled();
        throw new JposException(JPOS_E_ILLEGAL);
    }

    public void endFixedOutput() {
        checkEnabled();
        throw new JposException(JPOS_E_ILLEGAL);
    }

    // ////////////////////////////////////////////////////////////////////////////
    // Slip
    // ////////////////////////////////////////////////////////////////////////////
    public void beginInsertion(int timeout) {
        checkEnabled();
        throw new JposException(JPOS_E_ILLEGAL);
    }

    public void beginItemList(int vatID) {
        checkEnabled();
        throw new JposException(JPOS_E_ILLEGAL);
    }

    public void beginRemoval(int timeout) {
        checkEnabled();
        throw new JposException(JPOS_E_ILLEGAL);
    }

    public void endInsertion() {
        checkEnabled();
        throw new JposException(JPOS_E_ILLEGAL);
    }

    public void endItemList() {
        checkEnabled();
        throw new JposException(JPOS_E_ILLEGAL);
    }

    public void endRemoval() {
        checkEnabled();
        throw new JposException(JPOS_E_ILLEGAL);
    }

    // ////////////////////////////////////////////////////////////////////////////
    // NonFiscal
    // ////////////////////////////////////////////////////////////////////////////
    public void beginNonFiscal() {
        checkOnLine();
        checkPrinterState(FPTR_PS_MONITOR);
        setPrinterState(FPTR_PS_NONFISCAL);
        receipt = new NonfiscalReceipt(createReceiptContext());
    }

    public void printNormalAsync(int station, String data) {
        checkOnLine();
        data = decodeText(data);
        logoPosition = SMFPTR_LOGO_PRINT;
        receipt.printNormal(station, data);
    }

    public void printNormal(int station, String data) {
        checkOnLine();
        execute(new PrintNormalRequest(station, data));
    }

    private PrinterImage getPrinterImage(int position) {
        ReceiptImage image = getReceiptImages().imageByPosition(position);
        if (image != null) {
            int index = image.getImageIndex();
            if (getPrinterImages().validIndex(index)) {
                return getPrinterImages().get(index);
            }
        }
        return null;
    }

    public void printRecLine(String line) {
        getPrinter().printLine(SMFP_STATION_REC, line, params.font);
    }

    private boolean isHeaderInDriver() {
        return (params.headerMode == SmFptrConst.SMFPTR_HEADER_MODE_DRIVER);
    }

    private void printDocEnd() {
        trailer.printRecMessages(printItems);
        trailer.print();
        printRecLine(" ");
        header.print();
    }

    private void printReportEnd() {
        try {
            getPrinter().waitForPrinting();
            printer.printReceiptImage(SMFPTR_LOGO_BEFORE_TRAILER);
            trailer.printRecMessages(printItems);
            trailer.print();
            printRecLine(" ");
            printer.printReceiptImage(SMFPTR_LOGO_AFTER_TRAILER);
            printer.printReceiptImage(SMFPTR_LOGO_AFTER_ADDTRAILER);
            header.print();
        } catch (Exception e) {
            // ignore print errors
            logger.error("printReportEnd: " + e.getMessage());
        }
    }

    public void endNonFiscal() {
        checkOnLine();
        checkPrinterState(FPTR_PS_NONFISCAL);
        setPrinterState(FPTR_PS_MONITOR);
        printDocEnd();
        receipt = new NonfiscalReceipt(createReceiptContext());
    }

    // ////////////////////////////////////////////////////////////////////////////
    // Training
    // ////////////////////////////////////////////////////////////////////////////
    public void beginTraining() {
        checkEnabled();
        checkPrinterState(FPTR_PS_MONITOR);
        trainingModeActive = true;
    }

    public void endTraining() {
        checkEnabled();
        if (trainingModeActive) {
            trainingModeActive = false;
        } else {
            throwWrongStateError();
        }
    }

    // ////////////////////////////////////////////////////////////////////////////
    public void clearError() {
        checkEnabled();
    }

    public void clearOutput() {
        checkClaimed();
        // Clears all buffered output data, including all asynchronous output
        // Also, when possible, halts outputs that are in progress
        synchronized (requests) {
            requests.clear();
        }
        // Any output error events that are enqueued –
        // usually waiting for FreezeEvents to
        // be set to false – are also cleared.
        synchronized (events) {
            for (int i = events.size() - 1; i >= 0; i--) {
                if (events.get(i) instanceof ErrorEventRequest) {
                    events.remove(i);
                }
            }
        }
        setState(JPOS_S_IDLE);
    }

    private String getDataFirmware(int[] optArgs) {
        LongPrinterStatus status = readLongStatus();
        String result = "";
        switch (optArgs[0]) {
            // printer firmware version number
            case 0:
                result = status.getFirmwareVersion();
                break;

            // printer firmware build number
            case 1:
                result = Long.toString(status.getFirmwareBuild());
                break;

            // fiscal memory firmware version number
            case 2:
                result = status.getFMFirmwareVersion();
                break;

            // fiscal memory firmware build number
            case 3:
                result = Long.toString(status.getFMFirmwareBuild());
                break;

            default:
                result += "Printer firmware : " + status.getFirmwareVersion();
                result += ", build " + String.valueOf(status.getFirmwareBuild());
                result += " from " + status.getFirmwareDate().toString();
                result += " Fiscal memory firmware: "
                        + status.getFMFirmwareVersion();
                result += ", build " + String.valueOf(status.getFMFirmwareBuild());
                result += " from " + status.getFMFirmwareDate().toString();
        }
        return result;
    }

    private long getSubtotal() {
        long total = 0;
        PrinterStatus status = readPrinterStatus();
        if (status.getPrinterMode().isReceiptOpened()) {
            total = getPrinter().getSubtotal();
        }
        return total;
    }

    public String getTenderData(int optArg) {
        switch (optArg) {
            // Cash
            case FPTR_PDL_CASH:
            // Cheque.
            case FPTR_PDL_CHEQUE:
            // Chitty.
            case FPTR_PDL_CHITTY:
            // Coupon.
            case FPTR_PDL_COUPON:
            // Currency.
            case FPTR_PDL_CURRENCY:
            case FPTR_PDL_DRIVEN_OFF:
            // Printer EFT.
            case FPTR_PDL_EFT_IMPRINTER:
            // Terminal EFT.
            case FPTR_PDL_EFT_TERMINAL:
            case FPTR_PDL_TERMINAL_IMPRINTER:
            // Gift.
            case FPTR_PDL_FREE_GIFT:
            // Giro.
            case FPTR_PDL_GIRO:
            // Home.
            case FPTR_PDL_HOME:
            case FPTR_PDL_IMPRINTER_WITH_ISSUER:
            // Local account.
            case FPTR_PDL_LOCAL_ACCOUNT:
            // Local card account.
            case FPTR_PDL_LOCAL_ACCOUNT_CARD:
            // Pay card.
            case FPTR_PDL_PAY_CARD:
            // Manual pay card.
            case FPTR_PDL_PAY_CARD_MANUAL:
            // Prepay.
            case FPTR_PDL_PREPAY:
            // Pump test.
            case FPTR_PDL_PUMP_TEST:
            // Credit.
            case FPTR_PDL_SHORT_CREDIT:
            // Staff.
            case FPTR_PDL_STAFF:
            // Voucher.
            case FPTR_PDL_VOUCHER:
                return "0";

            default:
                throw new JposException(JPOS_E_ILLEGAL,
                        Localizer.getString(Localizer.invalidParameterValue)
                        + ", OptArgs");
        }
    }

    public int getLineCountData(int optArg) {
        switch (optArg) {
            // Number of item lines.
            case FPTR_LC_ITEM:
                return 2;
            // Number of voided item lines.
            case FPTR_LC_ITEM_VOID:
                return 2;
            // Number of printDiscount lines.
            case FPTR_LC_DISCOUNT:
                return 2;
            // Number of voided printDiscount lines.
            case FPTR_LC_DISCOUNT_VOID:
                return 2;
            // Number of surcharge lines.
            case FPTR_LC_SURCHARGE:
                return 2;
            // Number of voided surcharge lines.
            case FPTR_LC_SURCHARGE_VOID:
                return 2;
            // Number of refund lines.
            case FPTR_LC_REFUND:
                return 2;
            // Number of voided refund lines.
            case FPTR_LC_REFUND_VOID:
                return 2;
            // Number of subtotal printDiscount lines.
            case FPTR_LC_SUBTOTAL_DISCOUNT:
                return 2;
            // Number of voided subtotal printDiscount lines.
            case FPTR_LC_SUBTOTAL_DISCOUNT_VOID:
                return 2;
            // Number of subtotal surcharge lines.
            case FPTR_LC_SUBTOTAL_SURCHARGE:
                return 2;
            // Number of voided subtotal surcharge lines.
            case FPTR_LC_SUBTOTAL_SURCHARGE_VOID:
                return 2;
            // Number of comment lines.
            case FPTR_LC_COMMENT:
                return 0;
            // Number of subtotal lines.
            case FPTR_LC_SUBTOTAL:
                return 1;
            // Number of total lines.
            case FPTR_LC_TOTAL:
                return 2;

            default:
                throw new JposException(JPOS_E_ILLEGAL,
                        Localizer.getString(Localizer.invalidParameterValue)
                        + ", OptArgs");
        }
    }

    public int getDataDescriptionLength(int optArg) {
        switch (optArg) {
            // printRecItem method
            case FPTR_DL_ITEM:
            // printRecItemAdjustment method.
            case FPTR_DL_ITEM_ADJUSTMENT:
            // printRecItemFuel method.
            case FPTR_DL_ITEM_FUEL:
            // printRecItemFuelVoid method.
            case FPTR_DL_ITEM_FUEL_VOID:
            // printRecNotPaid method.
            case FPTR_DL_NOT_PAID:
            // printRecPackageAdjustment method.
            case FPTR_DL_PACKAGE_ADJUSTMENT:
            // printRecRefund method.
            case FPTR_DL_REFUND:
            // printRecRefundVoid method.
            case FPTR_DL_REFUND_VOID:
            // printRecSubtotalAdjustment method.
            case FPTR_DL_SUBTOTAL_ADJUSTMENT:
            // printRecTotal method.
            case FPTR_DL_TOTAL:
            // printRecVoid method.
            case FPTR_DL_VOID:
            // printRecItemVoid and printRecItemAdjustmentVoid methods.
            case FPTR_DL_VOID_ITEM:
                return getModel().getTextLength(getFont());

            default:
                throw new JposException(JPOS_E_ILLEGAL,
                        Localizer.getString(Localizer.invalidParameterValue)
                        + ", OptArgs");
        }
    }

    public void getData(int dataItem, int[] optArgs, String[] data)
            {
        checkOnLine();
        String result = "";
        LongPrinterStatus status;

        long number;
        switch (dataItem) {
            // Get the Fiscal Printer’s firmware release number.
            case FPTR_GD_FIRMWARE:
                result = getDataFirmware(optArgs);
                break;

            // Get the Fiscal Printer’s fiscal ID.
            case FPTR_GD_PRINTER_ID:
                status = readLongStatus();
                result = status.getSerial();
                break;

            // Get the Fiscal Printer’s fiscal ID.
            case FPTR_GD_CURRENT_TOTAL:
                result = StringUtils.amountToString(getSubtotal());
                break;

            // Get the daily total.
            case FPTR_GD_DAILY_TOTAL:
                long amount = 0;
                if ((optArgs == null) || (optArgs.length < 1)) {
                    amount = getDailyTotal(SMFPTR_DAILY_TOTAL_ALL);
                } else {
                    amount = getDailyTotal(optArgs[0]);
                }
                result = StringUtils.amountToString(amount);
                break;

            // Get the Fiscal Printer’s grand total.
            case FPTR_GD_GRAND_TOTAL:
                result = readGrandTotal(optArgs);
                break;

            // Get the total number of voided receipts.
            case FPTR_GD_MID_VOID:
                result = Long.toString(printer.readOperationRegister(166));
                break;

            // Get the current total of not paid receipts.
            case FPTR_GD_NOT_PAID:
                result = "0";
                break;

            // Get the number of fiscal receipts printed
            case FPTR_GD_RECEIPT_NUMBER:
                status = readLongStatus();
                PrinterMode mode = status.getPrinterMode();
                int documentNumber = status.getDocumentNumber();
                result = Long.toString(documentNumber);
                break;

            // Get the current total of refunds.
            case FPTR_GD_REFUND:
                result = Long.toString(printer.readOperationRegister(146));
                break;

            // Get the current total of voided refunds.
            case FPTR_GD_REFUND_VOID:
                throw new JposException(JPOS_E_ILLEGAL,
                        Localizer.getString(Localizer.invalidParameterValue)
                        + "dataItem");

            // Get the number of daily fiscal documents.
            case FPTR_GD_FISCAL_DOC:
                result = Long.toString(fiscalDay.getFiscalDocNumber());
                break;

            // Get the number of daily voided fiscal documents
            case FPTR_GD_FISCAL_DOC_VOID:
                result = Long.toString(fiscalDay.getFiscalDocVoidNumber());
                break;

            // Get the number of daily fiscal sales receipts
            case FPTR_GD_FISCAL_REC:
                number = getPrinter().readOperationRegister(144)
                        + getPrinter().readOperationRegister(145)
                        + getPrinter().readOperationRegister(146)
                        + getPrinter().readOperationRegister(147);

                result = Long.toString(number);
                break;

            case FPTR_GD_FISCAL_REC_VOID:
                number = getPrinter().readOperationRegister(179)
                        + getPrinter().readOperationRegister(180)
                        + getPrinter().readOperationRegister(181)
                        + getPrinter().readOperationRegister(182);

                result = Long.toString(number);
                break;

            case FPTR_GD_NONFISCAL_DOC:
                result = Long.toString(fiscalDay.getNonFiscalDocNumber());
                break;

            case FPTR_GD_NONFISCAL_DOC_VOID:
                result = Long.toString(fiscalDay.getNonFiscalDocVoidNumber());
                break;

            case FPTR_GD_NONFISCAL_REC:
                result = Long.toString(fiscalDay.getNonFiscalRecNumber());
                break;

            case FPTR_GD_SIMP_INVOICE:
                result = Long.toString(fiscalDay.getSimpInvoiceNumber());
                break;

            case FPTR_GD_Z_REPORT:
                status = readLongStatus();
                // Printer is not fiscalized
                if (status.getRegistrationNumber() == 0) {
                    // Z-report number before fiscalization
                    result = Long.toString(printer.readOperationRegister(159));
                } else {
                    result = Long.toString(status.getDayNumber());
                }
                break;

            case FPTR_GD_TENDER:
                result = getTenderData(optArgs[0]);
                break;

            case FPTR_GD_LINECOUNT:
                result = String.valueOf(getLineCountData(optArgs[0]));
                break;

            case FPTR_GD_DESCRIPTION_LENGTH:
                result = String.valueOf(getDataDescriptionLength(optArgs[0]));
                break;

            default:
                throw new JposException(JPOS_E_ILLEGAL,
                        Localizer.getString(Localizer.invalidParameterValue)
                        + "dataItem");
        }
        data[0] = encodeText(result);
    }

    public String readGrandTotal(int[] optArgs) {
        String result = "0;0;0;0";
        boolean isFiscalized = printer.readLongStatus().isFiscalized();
        if (isFiscalized) {
            int mode = 0;
            if ((optArgs != null) && (optArgs.length > 0)) {
                mode = optArgs[0];
            }
            FMTotals totals = printer.readFMTotals(mode).getFMTotals();
            result = String.valueOf(totals.getSalesAmount()) + ";"
                    + String.valueOf(totals.getBuyAmount()) + ";"
                    + String.valueOf(totals.getRetSaleAmount()) + ";"
                    + String.valueOf(totals.getRetBuyAmount());
        }
        return result;
    }

    public long getDailyTotal(int mode) {
        long amount = 0;
        switch (mode) {
            case SmFptrConst.SMFPTR_DAILY_TOTAL_ALL:
                for (int i = 0; i < 4; i++) {
                    amount += printer.readCashRegister(193 + i * 4);
                    amount -= printer.readCashRegister(194 + i * 4);
                    amount -= printer.readCashRegister(195 + i * 4);
                    amount += printer.readCashRegister(196 + i * 4);
                }
                amount += printer.readCashRegister(242);
                amount -= printer.readCashRegister(243);
                break;

            case SmFptrConst.SMFPTR_DAILY_TOTAL_CASH:
                amount += printer.readCashRegister(193);
                amount -= printer.readCashRegister(194);
                amount -= printer.readCashRegister(195);
                amount += printer.readCashRegister(196);
                amount += printer.readCashRegister(242);
                amount -= printer.readCashRegister(243);
                break;

            case SmFptrConst.SMFPTR_DAILY_TOTAL_PT2:
                amount += printer.readCashRegister(197);
                amount -= printer.readCashRegister(198);
                amount -= printer.readCashRegister(199);
                amount += printer.readCashRegister(200);
                break;

            case SmFptrConst.SMFPTR_DAILY_TOTAL_PT3:
                amount += printer.readCashRegister(201);
                amount -= printer.readCashRegister(202);
                amount -= printer.readCashRegister(203);
                amount += printer.readCashRegister(204);
                break;

            case SmFptrConst.SMFPTR_DAILY_TOTAL_PT4:
                amount += printer.readCashRegister(205);
                amount -= printer.readCashRegister(206);
                amount -= printer.readCashRegister(207);
                amount += printer.readCashRegister(208);
                break;

            default:
                throw new Exception("Invalid optional parameter");
        }
        return amount;
    }

    public void getDate(String[] Date) {
        checkOnLine();
        String result = "";
        if (Date.length < 1) {
            throw new JposException(JPOS_E_ILLEGAL,
                    Localizer.getString(Localizer.invalidParameterValue)
                    + "Date");
        }

        switch (dateType) {
            // Date of last end of day.
            case FPTR_DT_EOD:
                ReadFMLastRecordDate lastFmRecordDate = getPrinter()
                        .readFMLastRecordDate();
                if (lastFmRecordDate.getRecordType() == 1) {
                    PrinterDate date = lastFmRecordDate.getRecordDate();
                    JposFiscalPrinterDate jposDate = new JposFiscalPrinterDate(
                            date.getDay(), date.getMonth(), date.getYear() + 2000,
                            0, 0);
                    result = jposDate.toString();
                }
                break;

            // Real time clock of the Fiscal Printer.
            case FPTR_DT_RTC:
                LongPrinterStatus status = readLongStatus();
                PrinterDate printerDate = status.getDate();
                PrinterTime printerTime = status.getTime();

                JposFiscalPrinterDate jposDate = new JposFiscalPrinterDate(
                        printerDate.getDay(), printerDate.getMonth(),
                        printerDate.getYear() + 2000, printerTime.getHour(),
                        printerTime.getMin());
                result = jposDate.toString();
                break;

            default:
                throw new JposException(JPOS_E_ILLEGAL,
                        Localizer.getString(Localizer.invalidParameterValue)
                        + "DateType");
        }
        Date[0] = encodeText(result);
    }

    public void getTotalizer(int vatID, int optArgs, String[] data)
            {
        checkOnLine();
        String result = "";
        /*
         * switch (totalizerType) { case FPTR_TT_DAY: case FPTR_TT_DOCUMENT:
         * case FPTR_TT_RECEIPT: case FPTR_TT_GRAND: switch (optArgs) { case
         * FPTR_GT_GROSS: data[0] = String.valueOf(printer.getTotal()); break;
         * case FPTR_GT_DISCOUNT: GetDiscountTotalizer(data); break; case
         * FPTR_GT_ITEM: GetItemTotalizer(data); break; case FPTR_GT_REFUND:
         * GetRefundTotalizer(data); break; case FPTR_GT_SURCHARGE:
         * GetSurchargeTotalizer(data); break; default: throw new
         * JposException(JPOS_E_ILLEGAL, "Invalid optArgs parameter value" ); }
         * }
         *
    }

    private boolean isSalesReceipt() {
        switch (fiscalReceiptType) {
            case FPTR_RT_SALES:
            case FPTR_RT_GENERIC:
            case FPTR_RT_SERVICE:
            case FPTR_RT_SIMPLE_INVOICE:
            case FPTR_RT_REFUND:
                return true;
            default:
                return false;
        }
    }

    private void dayEndRequiredError() {
        throw new JposException(JPOS_E_EXTENDED, JPOS_EFPTR_DAY_END_REQUIRED,
                "Day end required");
    }

    private void checkDayEnd() {
        PrinterStatus status = readPrinterStatus();
        if (status.getPrinterMode().isDayEndRequired()) {
            dayEndRequiredError();
        }
    }

    // ////////////////////////////////////////////////////////////////////////////
    // Fiscal Receipt
    // ////////////////////////////////////////////////////////////////////////////
    public void beginFiscalReceipt(boolean printHeader) {
        checkOnLine();
        checkPrinterState(FPTR_PS_MONITOR);

        receipt = createReceipt(fiscalReceiptType);

        // Cancel receipt if it opened
        cancelReceipt();
        // check end of day
        if (isSalesReceipt()) {
            checkDayEnd();
        }

        fiscalDay.open();

        if (additionalHeader.length() > 0) {
            getPrinter().printText(SMFP_STATION_REC, additionalHeader,
                    getFont());
        }

        setPrinterState(FPTR_PS_FISCAL_RECEIPT);
        printItems.clear();
        receipt.beginFiscalReceipt(printHeader);
    }

    public void endFiscalReceipt(boolean printHeader) {
        logger.debug("endFiscalReceipt");

        synchronized (printer) {
            checkOnLine();
            checkPrinterState(FPTR_PS_FISCAL_RECEIPT_ENDING);

            if (receipt.isCancelled()) {
                receiptPrinter.printText(FptrParameters.TEXT_REC_CANCEL);
            } else {
                receipt.endFiscalReceipt(printHeader);
            }
            isInReceiptTrailer = true;
            if (!receipt.getCapAutoCut()) {
                try {
                    getPrinter().waitForPrinting();
                    printDocEnd();
                    isInReceiptTrailer = false;
                } catch (Exception e) {
                    // ignore print errors because cashin is succeeded
                    logger.error("endFiscalReceipt: " + e.getMessage());
                }
            }
            setPrinterState(FPTR_PS_MONITOR);
            receipt = new NonfiscalReceipt(createReceiptContext());
        }
    }

    public void printDuplicateReceipt() {
        checkOnLine();
        checkPrinterState(FPTR_PS_MONITOR);
        if (!getCapDuplicateReceipt()) {
            throw new JposException(
                    JPOS_E_ILLEGAL,
                    Localizer
                    .getString(Localizer.receiptDuplicationNotSupported));
        }
        getPrinter().duplicateReceipt();
        printDocEnd();
        duplicateReceipt = false;
    }

    /**
     * ************************************************************************
     * Prints a report of totals for a range of dates on the receipt. This
     * method is always performed synchronously. The dates are strings in the
     * format “ddmmyyyyhhmm”, where: dd day of the month (1 - 31) mm month (1 -
     * 12) yyyy year (1997-) hh hour (0-23) mm minutes (0-59)
     * *************************************************************************
     *
    public void printPeriodicTotalsReport(String date1, String date2)
            {
        checkOnLine();
        checkStateBusy();
        checkPrinterState(FPTR_PS_MONITOR);

        date1 = decodeText(date1);
        date2 = decodeText(date2);

        PrinterDate printerDate1 = JposFiscalPrinterDate.valueOf(date1)
                .getPrinterDate();
        PrinterDate printerDate2 = JposFiscalPrinterDate.valueOf(date2)
                .getPrinterDate();

        if (params.reportDevice == SMFPTR_REPORT_DEVICE_EJ) {
            EJDate d1 = new EJDate(printerDate1);
            EJDate d2 = new EJDate(printerDate2);
            getPrinter().printEJDayReportOnDates(d1, d2, params.reportType);
        } else {
            getPrinter().printFMReportDates(printerDate1, printerDate2,
                    params.reportType);
        }
        printReportEnd();
    }

    public void printPowerLossReport() {
        checkEnabled();
        checkOnLine();
        throw new JposException(JPOS_E_ILLEGAL);
    }

    private void checkQuantity(int value) {
        if (value < 0) {
            throw new JposException(JPOS_E_EXTENDED,
                    JPOS_EFPTR_BAD_ITEM_QUANTITY);
        }
    }

    private void checkPrice(long value) {
        if (value < 0) {
            throw new JposException(JPOS_E_EXTENDED, JPOS_EFPTR_BAD_PRICE);
        }
    }

    private void checkSalesReceipt() {
        if (!isSalesReceipt()) {
            throw new JposException(JPOS_E_ILLEGAL,
                    Localizer.getString(Localizer.methodNotSupported));
        }
    }

    private void checkLongParam(long Value, long minValue, long maxValue,
            String propName) {
        if (Value < minValue) {
            throw new JposException(JPOS_E_ILLEGAL,
                    Localizer.getString(Localizer.invalidParameterValue)
                    + propName);
        }
        if (Value > maxValue) {
            throw new JposException(JPOS_E_ILLEGAL,
                    Localizer.getString(Localizer.invalidParameterValue)
                    + propName);
        }
    }

    private void checkVatInfo(long value) {
        checkLongParam(value, 0, 4, "VatInfo");
    }

    private void checkReceiptStation() {
        if (fiscalReceiptStation != FPTR_RS_RECEIPT) {
            throw new JposException(JPOS_E_ILLEGAL,
                    Localizer.getString(Localizer.invalidParameterValue)
                    + "fiscalReceiptStation");
        }
    }

    private long convertAmount(long value) {
        return Math.abs((long) (value * params.amountFactor));
    }

    private int convertQuantity(int value) {
        return (int) (value * params.quantityFactor);
    }

    public void printRecItemAsync(String description, long price, int quantity,
            int vatInfo, long unitPrice, String unitName) {
        unitName = decodeText(unitName);
        description = decodeText(description);
        price = convertAmount(price);
        unitPrice = convertAmount(unitPrice);
        quantity = convertQuantity(quantity);

        checkEnabled();
        checkSalesReceipt();
        checkReceiptStation();
        checkQuantity(quantity);
        checkPrice(price);
        checkPrice(unitPrice);
        checkVatInfo(vatInfo);

        receipt.printRecItem(description, price, quantity, vatInfo, unitPrice,
                unitName);
    }

    public void printRecItem(String description, long price, int quantity,
            int vatInfo, long unitPrice, String unitName) {
        filters.printRecItem(description, price, quantity, vatInfo, unitPrice,
                unitName);

        checkOnLine();
        checkPrinterState(FPTR_PS_FISCAL_RECEIPT);
        execute(new PrintRecItemRequest(description, price, quantity, vatInfo,
                unitPrice, unitName));
    }

    public void printRecMessageAsync(int station, FontNumber font,
            String message) {
        message = decodeText(message);
        if (isReceiptEnding()) {
            printItems.add(new TextLine(station, font, message));
        } else {
            receipt.printRecMessage(station, font, message);
        }
    }

    public boolean isReceiptEnding() {
        return printerState.getValue() == FiscalPrinterConst.FPTR_PS_FISCAL_RECEIPT_ENDING;
    }

    public void printRecMessageAsync(String message) {
        printRecMessageAsync(PrinterConst.SMFP_STATION_RECJRN, getFont(),
                message);
    }

    public void printRecMessage(String message) {
        checkOnLine();
        execute(new PrintRecMessageRequest(message));
    }

    private void checkPercents(long amount) {
        if ((amount < 0) || (amount > 10000)) {
            throw new JposException(JPOS_E_EXTENDED, JPOS_EFPTR_BAD_ITEM_AMOUNT);
        }
    }

    public void checkAdjustment(int adjustmentType, long amount)
            {
        switch (adjustmentType) {
            case FPTR_AT_AMOUNT_DISCOUNT:
            case FPTR_AT_AMOUNT_SURCHARGE:
                break;

            case FPTR_AT_PERCENTAGE_DISCOUNT:
            case FPTR_AT_PERCENTAGE_SURCHARGE:
                checkPercents(amount);
                break;

            default:
                throw new JposException(JPOS_E_ILLEGAL,
                        Localizer.getString(Localizer.invalidParameterValue)
                        + "adjustmentType");
        }
    }

    public void printRecItemAdjustmentAsync(int adjustmentType,
            String description, long amount, int vatInfo) {
        description = decodeText(description);
        amount = convertAmount(amount);

        checkEnabled();
        checkSalesReceipt();
        checkVatInfo(vatInfo);
        checkAdjustment(adjustmentType, amount);

        // filter request
        PrintRecItemAdjustmentRequest request = new PrintRecItemAdjustmentRequest(
                adjustmentType, description, amount, vatInfo);
        request = filters.printRecItemAdjustment(request);
        adjustmentType = request.getAdjustmentType();
        description = request.getDescription();
        amount = request.getAmount();
        vatInfo = request.getVatInfo();

        checkPrinterState(FPTR_PS_FISCAL_RECEIPT);
        receipt.printRecItemAdjustment(adjustmentType, description, amount,
                vatInfo);
    }

    public void printRecItemAdjustment(int adjustmentType, String description,
            long amount, int vatInfo) {
        checkOnLine();
        execute(new PrintRecItemAdjustmentRequest(adjustmentType, description,
                amount, vatInfo));
    }

    private String formatStrings(String line1, String line2) {
        int len;
        String S = "";
        len = getModel().getTextLength(getFont()) - line2.length();

        for (int i = 0; i < len; i++) {
            if (i < line1.length()) {
                S = S + line1.charAt(i);
            } else {
                S = S + " ";
            }
        }
        return S + line2;
    }

    private void printStrings(String line1, String line2) {
        checkOnLine();
        getPrinter().printText(SMFP_STATION_REC, formatStrings(line1, line2),
                getFont());
    }

    public void printRecItemFuelAsync(String description, long price,
            int quantity, int vatInfo, long unitPrice, String unitName,
            long specialTax, String specialTaxName) {
    }

    public void printRecItemFuelVoidAsync(String description, long price,
            int vatInfo, long specialTax) {
    }

    public void printRecNotPaidAsync(String description, long amount)
            {
    }

    public void printRecNotPaid(String description, long amount)
            {
        checkOnLine();
        throw new JposException(JPOS_E_ILLEGAL,
                Localizer.getString(Localizer.notPaidReceiptsNotSupported));
    }

    public void printRecRefund(String description, long amount, int vatInfo)
            {
        checkOnLine();
        execute(new PrintRecRefundRequest(description, amount, vatInfo));
    }

    public void printRecRefundAsync(String description, long amount, int vatInfo)
            {
        description = decodeText(description);
        amount = convertAmount(amount);

        checkEnabled();
        checkSalesReceipt();
        checkVatInfo(vatInfo);

        checkPrinterState(FPTR_PS_FISCAL_RECEIPT);
        receipt.printRecRefund(description, amount, vatInfo);
    }

    private void checkTotal(long total) {
        // update receipt sum
        long receiptSubtotal = getSubtotal();
        if (params.checkTotal && isReceiptOpened) {
            if (total < 0) {
                // negative receipt total
                throw new JposException(JPOS_E_EXTENDED,
                        JPOS_EFPTR_NEGATIVE_TOTAL);
            }
            // compare totals
            if (receiptSubtotal != total) {
                logger.error("Totals compare failed!");
                logger.debug("Receipt total is "
                        + String.valueOf(receiptSubtotal));
                logger.debug("Application total is " + String.valueOf(total));

                cancelReceipt();
                setPrinterState(FPTR_PS_MONITOR);
                throw new JposException(JPOS_E_EXTENDED,
                        JPOS_EFPTR_BAD_ITEM_AMOUNT);
            }
        }
    }

    public void printRecSubtotal(long amount) {
        checkOnLine();
        execute(new PrintRecSubtotalRequest(amount));
    }

    public void printRecSubtotalAsync(long amount) {
        amount = convertAmount(amount);

        checkEnabled();
        checkSalesReceipt();

        checkPrinterState(FPTR_PS_FISCAL_RECEIPT);
        checkTotal(amount);
        receipt.printRecSubtotal(amount);
    }

    public void printRecSubtotalAdjustment(int adjustmentType,
            String description, long amount) {
        checkOnLine();
        execute(new PrintRecSubtotalAdjustmentRequest(adjustmentType,
                description, amount));
    }

    public void printRecSubtotalAdjustmentAsync(int adjustmentType,
            String description, long amount) {
        description = decodeText(description);
        amount = convertAmount(amount);

        checkEnabled();
        checkAdjustment(adjustmentType, amount);
        checkPrinterState(FPTR_PS_FISCAL_RECEIPT);
        receipt.printRecSubtotalAdjustment(adjustmentType, description, amount);
    }

    public void printRecTotal(long total, long payment, String description)
            {
        checkOnLine();
        execute(new PrintRecTotalRequest(total, payment, description));
    }

    public void printRecTotalAsync(long total, long payment, String description)
            {
        logger.debug("printRecTotal");

        total = convertAmount(total);
        payment = convertAmount(payment);
        description = decodeText(description);

        checkEnabled();

        if ((printerState.getValue() != FPTR_PS_FISCAL_RECEIPT)
                && (printerState.getValue() != FPTR_PS_FISCAL_RECEIPT_TOTAL)) {
            throwWrongStateError();
        }
        checkTotal(total);

        long payType = getPayType(description);
        receipt.printRecTotal(total, payment, payType, description);
        if (receipt.isPayed()) {
            setPrinterState(FPTR_PS_FISCAL_RECEIPT_ENDING);
        } else {
            setPrinterState(FPTR_PS_FISCAL_RECEIPT_TOTAL);
        }
    }

    public void printRecVoidAsync(String description) {
        description = decodeText(description);
        if ((printerState.getValue() == FPTR_PS_FISCAL_RECEIPT)
                || (printerState.getValue() == FPTR_PS_FISCAL_RECEIPT_TOTAL)
                || (printerState.getValue() == FPTR_PS_FISCAL_RECEIPT_ENDING)) {
            receipt.printRecVoid(description);
            setPrinterState(FPTR_PS_FISCAL_RECEIPT_ENDING);
        } else {
            throwWrongStateError();
        }
    }

    public void printRecVoid(String description) {
        checkOnLine();
        execute(new PrintRecVoidRequest(description));
    }

    public void printRecVoidItem(String description, long amount, int quantity,
            int adjustmentType, long adjustment, int vatInfo) {
        checkOnLine();
        description = decodeText(description);
        amount = convertAmount(amount);
        quantity = convertQuantity(quantity);

        checkPrinterState(FPTR_PS_FISCAL_RECEIPT);
        checkQuantity(quantity);
        checkVatInfo(vatInfo);
        receipt.printRecVoidItem(description, amount, quantity, adjustmentType,
                adjustment, vatInfo);
    }

    /*
     * Remarks: Prints a report of the fiscal EPROM contents on the receipt that
     * occurred between two end points. This method is always performed
     * synchronously
     *
    public void printReport(int reportType, String startNum, String endNum)
            {
        checkOnLine();
        startNum = decodeText(startNum);
        endNum = decodeText(endNum);

        checkEnabled();
        checkPrinterState(FPTR_PS_MONITOR);

        int day1 = 0;
        int day2 = 0;
        switch (reportType) {
            case FPTR_RT_ORDINAL:
                // case FPTR_RT_EOD_ORDINAL:
                day1 = stringParamToInt(startNum, "startNum");
                day2 = stringParamToInt(endNum, "endNum");
                if (params.reportDevice == SMFPTR_REPORT_DEVICE_EJ) {
                    getPrinter().printEJReportDays(day1, day2, params.reportType);
                } else {
                    getPrinter().printFMReportDays(day1, day2, params.reportType);
                }
                printReportEnd();
                break;

            case FPTR_RT_DATE:
                PrinterDate date1;
                PrinterDate date2;
                // pase dates
                date1 = JposFiscalPrinterDate.valueOf(startNum).getPrinterDate();
                date2 = JposFiscalPrinterDate.valueOf(endNum).getPrinterDate();
                // print report
                if (params.reportDevice == SMFPTR_REPORT_DEVICE_EJ) {
                    getPrinter().printEJDayReportOnDates(new EJDate(date1),
                            new EJDate(date2), params.reportType);
                } else {
                    getPrinter()
                            .printFMReportDates(date1, date2, params.reportType);
                }
                printReportEnd();
                break;

            default:
                throw new JposException(JPOS_E_ILLEGAL,
                        Localizer.getString(Localizer.invalidParameterValue)
                        + "reportType");
        }
    }

    public void printXReport() {
        checkOnLine();
        checkStateBusy();
        checkPrinterState(FPTR_PS_MONITOR);
        getPrinter().printXReport();
        printReportEnd();
    }

    public void printZReport() {
        checkOnLine();
        checkStateBusy();
        checkPrinterState(FPTR_PS_MONITOR);

        PrinterStatus status = readPrinterStatus();
        if (status.getPrinterMode().isDayClosed()) {
            printer.beginFiscalDay();
        }

        saveZReportXml();

        status = readPrinterStatus();
        if (status.getPrinterMode().canPrintZReport()) {
            getPrinter().printZReport();
            fiscalDay.close();
            printReportEnd();
            dayOpened = readPrinterStatus().getPrinterMode().getDayOpened();
        } else {
            throw new JposException(JPOS_E_ILLEGAL);
        }
    }

    public String getDayNumberText(int dayNumber) {
        String result = String.valueOf(dayNumber);
        for (int i = result.length(); i < 4; i++) {
            result = "0" + result;
        }
        return result;
    }

    private void saveZReportXml() {
        if (params.xmlZReportEnabled || params.csvZReportEnabled) {
            try {
                RegisterReport report = new RegisterReport();
                RegisterReportReader.execute(report, printer);
                if (params.xmlZReportEnabled) {
                    try {
                        String fileName = getXmlZReportFileName(report.dayNumber);
                        XmlRegisterReportWriter.execute(report, fileName);
                    } catch (Exception e) {
                        logger.error("Error saving file", e);
                    }
                }
                if (params.csvZReportEnabled) {
                    try {
                        String fileName = getCsvZReportFileName(report.dayNumber);
                        CsvRegisterReportWriter.execute(report, fileName);
                    } catch (Exception e) {
                        logger.error("Error saving file", e);
                    }
                }
            } catch (Exception e) {
                logger.error("Error saving file", e);
            }
        }
    }

    public String getXmlZReportFileName(int dayNumber) {
        String fileName = params.xmlZReportFileName;
        if (params.ZReportDayNumber) {
            fileName = FileUtils.removeExtention(fileName) + "_"
                    + getDayNumberText(dayNumber)
                    + FileUtils.getExtention(fileName);
        }
        fileName = SysUtils.getFilesPath() + fileName;
        return fileName;
    }

    public String getCsvZReportFileName(int dayNumber) {
        String fileName = params.csvZReportFileName;
        if (params.ZReportDayNumber) {
            fileName = FileUtils.removeExtention(fileName) + "_"
                    + getDayNumberText(dayNumber)
                    + FileUtils.getExtention(fileName);
        }
        fileName = SysUtils.getFilesPath() + fileName;
        return fileName;
    }

    public void resetPrinter() {
        params.cancelIO = false;
        checkOnLine();
        cancelReceipt();
        receiptType = 0;
        isReceiptOpened = false;
    }

    public void setDate(String date) {
        checkOnLine();
        date = decodeText(date);

        checkEnabled();
        PrinterStatus status = readPrinterStatus();
        if (!status.getPrinterMode().isDayClosed()) {
            dayEndRequiredError();
        }

        JposFiscalPrinterDate jposDate = JposFiscalPrinterDate.valueOf(date);
        PrinterDate printerDate = jposDate.getPrinterDate();
        PrinterTime printerTime = jposDate.getPrinterTime();

        getPrinter().check(printer.writeDate(printerDate));
        int resultCode = getPrinter().confirmDate(printerDate);
        if (resultCode != 0) {
            // try to set date back
            printerDate = readLongStatus().getDate();
            getPrinter().check(printer.confirmDate(printerDate));

            throw new Exception(
                    Localizer.getString(Localizer.failedConfirmDate)
                    + PrinterError.getFullText(resultCode));
        }
        getPrinter().writeTime(printerTime);
        // check if date and time was set correctly
        LongPrinterStatus fullStatus = readLongStatus();
        if (!PrinterDate.compare(printerDate, fullStatus.getDate())) {
            logger.error("Failed to set printer date: "
                    + PrinterDate.toText(printerDate) + " <> "
                    + PrinterDate.toText(fullStatus.getDate()));
        }
        PrinterTime time = new PrinterTime(fullStatus.getTime().getHour(),
                fullStatus.getTime().getMin(), 0);
        if (!PrinterTime.compare(printerTime, time)) {
            logger.error("Failed to set printer time: "
                    + PrinterTime.toString(printerTime) + " <> "
                    + PrinterTime.toString(fullStatus.getTime()));
        }
    }

    public String getHeaderLine(int index) {
        return header.get(index).getText();
    }

    public String getTrailerLine(int index) {
        return trailer.get(index).getText();
    }

    public void setHeaderLine(int lineNumber, String text, boolean doubleWidth)
            {
        checkOnLine();
        text = decodeText(text);
        logger.debug("setHeaderLine: " + text);

        checkEnabled();
        // reset graphic info
        if (lineNumber == 1) {
            graphicsLine = 1;
        }
        logoPosition = params.headerImagePosition;
        text = getPrinter().processEscCommands(text);
        if (params.centerHeader) {
            text = StringUtils.centerLine(text.trim(), getMessageLength());
        }
        logger.debug("setHeaderLine('" + text + "')");
        header.writeLine(lineNumber, text);
        saveProperties();
        logoPosition = SMFPTR_LOGO_PRINT;
    }

    public void setPOSID(String POSID, String cashierID) {
        checkOnLine();
        cashierID = decodeText(cashierID);

        checkEnabled();
        // write cashier ID
        getPrinter().writeTable(SMFP_TABLE_CASHIER,
                readPrinterStatus().getOperator(), 2, cashierID);
    }

    public void setStoreFiscalID(String ID) {
        checkEnabled();
        throw new JposException(JPOS_E_ILLEGAL);
    }

    public void setTrailerLine(int lineNumber, String text, boolean doubleWidth)
            {
        checkOnLine();
        text = decodeText(text);
        checkEnabled();
        logoPosition = params.trailerImagePosition;
        text = getPrinter().processEscCommands(text);
        if (params.centerHeader) {
            text = StringUtils.centerLine(text.trim(), getMessageLength());
        }
        trailer.writeLine(lineNumber, text);
        saveProperties();
        logoPosition = SMFPTR_LOGO_PRINT;
    }

    /**
     * ************************************************************************
     * Gets the rate associated with a given VAT identifier. Parameter
     * Description vatID - VAT identifier of the required rate. optArgs - For
     * some countries, this additional argument may be needed. Consult the
     * Fiscal Printer Service vendor's documentation for details. vatRate - The
     * rate associated with the VAT identifier
     *
    public void getVatEntry(int vatID, int optArgs, int[] vatRate)
            {
        checkOnLine();
        // 4 tax rates available in SHTRIH-M fiscal printers
        checkParamValue(vatID, 1, vatValues.length, "vatID");
        String[] vatValue = new String[1];
        vatValue[0] = "";

        getPrinter().check(
                printer.readTable(SMFP_TABLE_TAX, vatID, 1, vatValue));
        vatRate[0] = Integer.parseInt(vatValue[0]);
    }

    public void setVatTable() {
        checkOnLine();
        for (int i = 0; i < vatValues.length; i++) {
            getPrinter().check(
                    printer.writeTable(SMFP_TABLE_TAX, i + 1, 1,
                            String.valueOf(vatValues[i])));
        }
    }

    public void setVatValue(int vatID, String vatValue) {
        checkOnLine();
        vatValue = decodeText(vatValue);
        // 4 tax rates available in SHTRIH-M fiscal printers
        checkParamValue(vatID, 1, vatValues.length, "vatID");
        int intVatValue = Integer.parseInt(vatValue);
        checkParamValue(intVatValue, 0, 10000, "vatValue");
        vatValues[vatID - 1] = intVatValue;
    }

    public void verifyItem(String itemName, int vatID) {
        checkOnLine();
        throw new JposException(JPOS_E_ILLEGAL);
    }

    public void setCurrency(int newCurrency) {
        checkEnabled();
        throw new JposException(JPOS_E_ILLEGAL);
    }

    public void printRecCashAsync(long amount) {
        amount = convertAmount(amount);
        checkEnabled();
        checkPrinterState(FPTR_PS_FISCAL_RECEIPT);
        receipt.printRecCash(amount);
    }

    public void printRecCash(long amount) {
        checkOnLine();
        execute(new PrintRecCashRequest(amount));
    }

    public void printRecItemFuel(String description, long price, int quantity,
            int vatInfo, long unitPrice, String unitName, long specialTax,
            String specialTaxName) {
        checkEnabled();
        throw new JposException(JPOS_E_ILLEGAL);
    }

    public void printRecItemFuelVoid(String description, long price,
            int vatInfo, long specialTax) {
        checkEnabled();
        throw new JposException(JPOS_E_ILLEGAL);
    }

    public void printRecPackageAdjustment(int adjustmentType,
            String description, String vatAdjustment) {
        checkOnLine();
        execute(new PrintRecPackageAdjustmentRequest(adjustmentType,
                description, vatAdjustment));
    }

    public void printRecPackageAdjustmentAsync(int adjustmentType,
            String description, String vatAdjustment) {
        description = decodeText(description);
        vatAdjustment = decodeText(vatAdjustment);

        checkEnabled();
        checkSalesReceipt();
        checkPrinterState(FPTR_PS_FISCAL_RECEIPT);
        receipt.printRecPackageAdjustment(adjustmentType, description,
                vatAdjustment);
    }

    public void printRecPackageAdjustVoid(int adjustmentType,
            String vatAdjustment) {
        execute(new PrintRecPackageAdjustVoidRequest(adjustmentType,
                vatAdjustment));
    }

    public void printRecPackageAdjustVoidAsync(int adjustmentType,
            String vatAdjustment) {
        vatAdjustment = decodeText(vatAdjustment);
        checkEnabled();
        checkSalesReceipt();
        checkPrinterState(FPTR_PS_FISCAL_RECEIPT);
        receipt.printRecPackageAdjustVoid(adjustmentType, vatAdjustment);
    }

    public void printRecRefundVoid(String description, long amount, int vatInfo)
            {
        execute(new PrintRecRefundVoidRequest(description, amount, vatInfo));
    }

    public void printRecRefundVoidAsync(String description, long amount,
            int vatInfo) {
        description = decodeText(description);
        amount = convertAmount(amount);

        checkEnabled();
        checkSalesReceipt();
        checkVatInfo(vatInfo);
        checkPrinterState(FPTR_PS_FISCAL_RECEIPT);
        receipt.printRecRefundVoid(description, amount, vatInfo);
    }

    public void printRecSubtotalAdjustVoid(int adjustmentType, long amount)
            {
        execute(new PrintRecSubtotalAdjustVoidRequest(adjustmentType, amount));
    }

    public void printRecSubtotalAdjustVoidAsync(int adjustmentType, long amount)
            {
        amount = convertAmount(amount);
        checkEnabled();
        checkPrinterState(FPTR_PS_FISCAL_RECEIPT);
        receipt.printRecSubtotalAdjustVoid(adjustmentType, amount);
    }

    public void printRecTaxID(String taxID) {
        execute(new PrintRecTaxIDRequest(taxID));
    }

    public void printRecTaxIDAsync(String taxID) {
        checkEnabled();
        checkPrinterState(FPTR_PS_FISCAL_RECEIPT_ENDING);
        getPrinter().printText(SMFP_STATION_REC, decodeText(taxID), getFont());
    }

    public int getAmountDecimalPlaces() {
        checkEnabled();
        return getModel().getAmountDecimalPlace();
    }

    public boolean getCapStatisticsReporting() {
        checkOpened();
        return capStatisticsReporting;
    }

    public boolean getCapUpdateStatistics() {
        checkOpened();
        return capUpdateStatistics;
    }

    public void resetStatistics(String statisticsBuffer) {
        statisticsBuffer = decodeText(statisticsBuffer);
        checkEnabled();
        statistics.reset(statisticsBuffer);
    }

    public void retrieveStatistics(String[] statisticsBuffer) {
        checkEnabled();
        statistics.retrieve(statisticsBuffer);
    }

    public void updateStatistics(String statisticsBuffer) {
        checkEnabled();
        statistics.update(statisticsBuffer);
    }

    public boolean getCapCompareFirmwareVersion() {
        checkOpened();
        return capCompareFirmwareVersion;
    }

    public boolean getCapUpdateFirmware() {
        checkOpened();
        return capUpdateFirmware;
    }

    public void compareFirmwareVersion(String firmwareFileName, int[] result)
            {
        checkEnabled();
        throw new JposException(JPOS_E_ILLEGAL);
    }

    public void updateFirmware(String firmwareFileName) {
        checkEnabled();
        throw new JposException(JPOS_E_ILLEGAL);
    }

    // 1.11
    // Capabilities
    public boolean getCapPositiveSubtotalAdjustment() {
        return capPositiveSubtotalAdjustment;
    }

    // Methods
    public void printRecItemVoidAsync(String description, long price,
            int quantity, int vatInfo, long unitPrice, String unitName)
            {
        price = convertAmount(price);
        quantity = convertQuantity(quantity);
        unitPrice = convertAmount(unitPrice);
        description = decodeText(description);
        unitName = decodeText(unitName);

        checkEnabled();
        checkQuantity(quantity);
        checkVatInfo(vatInfo);
        receipt.printRecItemVoid(description, price, quantity, vatInfo,
                unitPrice, unitName);
    }

    public void printRecItemVoid(String description, long price, int quantity,
            int vatInfo, long unitPrice, String unitName) {
        checkPrinterState(FPTR_PS_FISCAL_RECEIPT);
        PrintRecItemVoidRequest request = new PrintRecItemVoidRequest(
                description, price, quantity, vatInfo, unitPrice, unitName);
        request = filters.printRecItemVoid(request);
        execute(request);
    }

    public void printRecItemAdjustmentVoidAsync(int adjustmentType,
            String description, long amount, int vatInfo) {
        description = decodeText(description);
        amount = convertAmount(amount);

        checkEnabled();
        checkSalesReceipt();
        checkVatInfo(vatInfo);
        receipt.printRecItemAdjustmentVoid(adjustmentType, description, amount,
                vatInfo);
    }

    public void printRecItemAdjustmentVoid(int adjustmentType,
            String description, long amount, int vatInfo) {
        checkPrinterState(FPTR_PS_FISCAL_RECEIPT);
        execute(new PrintRecItemAdjustmentVoidRequest(adjustmentType,
                description, amount, vatInfo));
    }

    // //////////////////////////
    private void checkPrinterState(int value) {
        if (printerState.getValue() != value) {
            throwWrongStateError();
        }
    }

    private void setPrinterState(int newState) {
        printerState.setValue(newState);
    }

    private void checkParamValue(int value, int minValue, int maxValue,
            String paramText) {
        if ((value < minValue) || (value > maxValue)) {
            throw new JposException(JPOS_E_ILLEGAL,
                    Localizer.getString(Localizer.invalidParameterValue)
                    + paramText);
        }
    }

    private int stringParamToInt(String s, String paramName) {
        try {
            int result = Integer.parseInt(s);
            return result;
        } catch (Exception e) {
            throw new JposException(JPOS_E_ILLEGAL,
                    Localizer.getString(Localizer.invalidParameterValue)
                    + paramName);
        }
    }

    private String checkHealthExternal() {
        PrinterStatus status = getPrinter().waitForPrinting();
        if (!status.getPrinterMode().isTestMode()) {
            getPrinter().startTest(1);
            getPrinter().waitForPrinting();
        }
        getPrinter().stopTest();
        getPrinter().waitForPrinting();
        return "External HCheck: OK";
    }

    private void checkStateBusy() {
        if (state == JPOS_S_BUSY) {
            logger.error("JPOS_E_BUSY");
            throw new JposException(JPOS_E_BUSY);
        }
    }

    private void cancelReceipt() {
        setPrinterState(FPTR_PS_MONITOR);
        PrinterStatus status = getPrinter().waitForPrinting();
        if (status.getPrinterMode().isReceiptOpened()) {
            getPrinter().sysAdminCancelReceipt();
            printDocEnd();
        }
    }

    public int getPayType(String key) {
        // logger.debug("getPayType(" + String.valueOf(userPayType) + ")");

        PayType payType = params.getPayTypes().get(key);
        if (payType == null) {
            return 0;
        } else {
            return payType.getValue();
        }
    }

    public FontNumber getFont(boolean doubleWidth) {
        if (doubleWidth) {
            return doubleWidthFont;
        } else {
            return getFont();
        }
    }

    public String encodeText(String text) {
        String result;
        if (getStringEncoding().length() == 0) {
            result = text;
        } else {
            try {
                result = new String(StringUtils.getBytes(text,
                        getStringEncoding()));
            } catch (UnsupportedEncodingException e) {
                logger.error(e);
                result = text;
            }
        }
        return result;
    }

    public String decodeText(String text) {
        String result;
        if (getStringEncoding().length() == 0) {
            result = text;
        } else {
            try {
                result = new String(text.getBytes(), getStringEncoding());
            } catch (UnsupportedEncodingException e) {
                logger.error(e);
                result = text;
            }
        }
        return result;
    }

    class PackageAdjustment {

        public int vat;
        public long amount;
    }

    class PackageAdjustments extends Vector {

        public PackageAdjustment addItem(int vat, long amount) {
            PackageAdjustment result = new PackageAdjustment();
            result.vat = vat;
            result.amount = amount;
            add(result);
            return result;
        }

        public PackageAdjustment getItem(int Index) {
            return (PackageAdjustment) get(Index);
        }

        public void parse(String s) {
            String[] items = StringUtils.split(s, ';');
            for (int i = 0; i < items.length; i++) {
                String[] fields = StringUtils.split(items[i], ',');
                if (fields.length >= 2) {
                    addItem(Integer.parseInt(fields[0]),
                            Long.parseLong(fields[1]));
                }
            }
        }
    }

    private void checkClaimed() {
        if (!claimed) {
            throw new JposException(JPOS_E_NOTCLAIMED);
        }
    }

    private void checkEnabled() {
        checkClaimed();
        if (!deviceEnabled) {
            throw new JposException(JPOS_E_DISABLED);
        }
    }

    private void checkOnLine() {
        checkEnabled();
    }

    public void printFiscalDocumentLineAsync(String documentLine)
            {
        checkEnabled();
        noSlipStationError();
    }

    public void printFixedOutputAsync(int documentType, int lineNumber,
            String data) {
        checkEnabled();
        throw new JposException(JPOS_E_ILLEGAL);
    }

    public int getMaxGraphicsHeight() {
        return getModel().getMaxGraphicsHeight();
    }

    public void loadGraphics(int lineNumber, int lineCount, byte[] data)
            {
        getPrinter().loadGraphics(lineNumber, lineCount, data);
    }

    private static final int TimeToSleep = 100;

    private void checkEcrMode() {
        logger.debug("checkEcrMode");
        int endDumpCount = 0;
        int confirmDateCount = 0;
        int writePointCount = 0;
        int stopTestCount = 0;

        for (;;) {
            PrinterStatus status = getPrinter().waitForPrinting();
            switch (status.getPrinterMode().getValue()) {
                case MODE_DUMPMODE:
                    getPrinter().endDump();
                    endDumpCount++;
                    if (endDumpCount >= MaxStateCount) {
                        throw new Exception(
                                Localizer.getString(Localizer.endDumpFailed));
                    }
                    break;

                case MODE_LOCKED:
                    throw new Exception(
                            Localizer.getString(Localizer.LockedTaxPassword));

                case MODE_WAITDATE:
                    PrinterDate date = readLongStatus().getDate();
                    getPrinter().confirmDate(date);
                    confirmDateCount++;
                    if (confirmDateCount >= MaxStateCount) {
                        throw new Exception(
                                Localizer.getString(Localizer.ConfirmDateFailed));
                    }
                    break;

                case MODE_POINTPOS:
                    getPrinter().writeDecimalPoint(SMFP_POINT_POSITION_2);
                    writePointCount++;
                    if (writePointCount >= MaxStateCount) {
                        throw new Exception(
                                Localizer
                                .getString(Localizer.WriteDecimalPointFailed));
                    }
                    break;

                case MODE_TECH:
                    technoReset();
                    break;

                case MODE_TEST:
                    getPrinter().stopTest();
                    stopTestCount++;
                    if (stopTestCount >= MaxStateCount) {
                        throw new Exception(
                                Localizer.getString(Localizer.StopTestFailed));
                    }
                    break;

                case MODE_FULLREPORT:
                case MODE_EJREPORT:
                case MODE_SLPPRINT:
                    getPrinter().sleep(TimeToSleep);
                    break;

                default:
                    return;
            }
        }
    }

    private void technoReset() {
        Calendar now = Calendar.getInstance();
        PrinterDate date = new PrinterDate();
        PrinterTime time = new PrinterTime();

        getPrinter().resetFM();
        getPrinter().writeDate(date);
        getPrinter().confirmDate(date);
        getPrinter().writeTime(time);
    }

    public void throwTestError() {
        throw new SmFiscalPrinterException(0x71, "Cutter failure");
    }

    public void checkDeviceStatus() {
        try {
            synchronized (printer) {
                PrinterStatus status = getPrinter().readPrinterStatus();
                checkPaperStatus(status);
            }

        } catch (Exception e) {
            logger.error(e);
            if (e instanceof IOException) {
                setPowerState(JPOS_PS_OFFLINE);
            }
        }
    }

    public void deviceProc() {
        logger.debug("deviceProc.begin");
        try {
            while (deviceThreadEnabled) {
                checkDeviceStatus();
                SysUtils.sleep(params.pollInterval);
            }
        } catch (InterruptedException e) {
            logger.error("InterruptedException", e);
            Thread.currentThread().interrupt();
        }
        logger.debug("deviceProc.end");
    }

    public int getMaxGraphicsWidth() {
        return getModel().getMaxGraphicsWidth();
    }

    public LongPrinterStatus getLongStatus() {
        return getPrinter().getLongStatus();
    }

    public EJStatus getEJStatus() {
        return statusEJ;
    }

    public DeviceMetrics getDeviceMetrics() {
        return getPrinter().getDeviceMetrics();
    }

    public void printBarcode(PrinterBarcode barcode) {
        if (isReceiptEnding()) {
            printItems.add(barcode);
        } else {
            getPrinter().printBarcode(barcode);
        }
    }

    public void printRecItemRefund(String description, long amount,
            int quantity, int vatInfo, long unitAmount, String unitName)
            {
        checkOnLine();
        checkPrinterState(FPTR_PS_FISCAL_RECEIPT);

        filters.printRecItemRefund(description, amount, quantity, vatInfo,
                unitAmount, unitName);

        execute(new PrintRecItemRefundRequest(description, amount, quantity,
                vatInfo, unitAmount, unitName));
    }

    public void printRecItemRefundAsync(String description, long amount,
            int quantity, int vatInfo, long unitAmount, String unitName)
            {
        unitName = decodeText(unitName);
        description = decodeText(description);
        amount = convertAmount(amount);
        unitAmount = convertAmount(unitAmount);
        quantity = convertQuantity(quantity);

        checkEnabled();
        checkSalesReceipt();
        checkReceiptStation();
        checkQuantity(quantity);
        checkPrice(amount);
        checkPrice(unitAmount);
        checkVatInfo(vatInfo);

        receipt.printRecItemRefund(description, amount, quantity, vatInfo,
                unitAmount, unitName);
    }

    public void printRecItemRefundVoid(String description, long amount,
            int quantity, int vatInfo, long unitAmount, String unitName)
            {
        checkOnLine();
        checkPrinterState(FPTR_PS_FISCAL_RECEIPT);

        filters.printRecItemRefundVoid(description, amount, quantity, vatInfo,
                unitAmount, unitName);

        execute(new PrintRecItemRefundVoidRequest(description, amount,
                quantity, vatInfo, unitAmount, unitName));
    }

    public void printRecItemRefundVoidAsync(String description, long amount,
            int quantity, int vatInfo, long unitAmount, String unitName)
            {
        amount = convertAmount(amount);
        quantity = convertQuantity(quantity);
        unitAmount = convertAmount(unitAmount);
        description = decodeText(description);
        unitName = decodeText(unitName);

        checkEnabled();
        checkQuantity(quantity);
        checkVatInfo(vatInfo);

        receipt.printRecItemRefundVoid(description, amount, quantity, vatInfo,
                unitAmount, unitName);
    }

    public void saveXmlZReport(String fileName) throws JposException {
        try {
            RegisterReport report = new RegisterReport();
            RegisterReportReader.execute(report, printer);
            XmlRegisterReportWriter.execute(report, fileName);
        } catch (Exception e) {
            logger.error("Error saving file", e);
            throw new JposException(JPOS_E_FAILURE, e.getMessage());
        }
    }

    public void saveCsvZReport(String fileName) throws JposException {
        try {
            RegisterReport report = new RegisterReport();
            RegisterReportReader.execute(report, printer);
            CsvRegisterReportWriter.execute(report, fileName);
        } catch (Exception e) {
            logger.error("Error saving file", e);
            throw new JposException(JPOS_E_FAILURE, e.getMessage());
        }
    }

    private String getPropsFileName() {
        return SysUtils.getFilesPath() + "shtrihjavapos.xml";
    }

    protected JposEntry createJposEntry(String logicalName,
            String factoryClass, String serviceClass, String vendorName,
            String vendorURL, String deviceCategory, String jposVersion,
            String productName, String productDescription, String productURL) {
        JposEntry jposEntry = new SimpleEntry();

        jposEntry.addProperty(JposEntry.LOGICAL_NAME_PROP_NAME, logicalName);
        jposEntry.addProperty(JposEntry.SI_FACTORY_CLASS_PROP_NAME,
                factoryClass);
        jposEntry.addProperty(JposEntry.SERVICE_CLASS_PROP_NAME, serviceClass);
        jposEntry.addProperty(JposEntry.VENDOR_NAME_PROP_NAME, vendorName);
        jposEntry.addProperty(JposEntry.VENDOR_URL_PROP_NAME, vendorURL);
        jposEntry.addProperty(JposEntry.DEVICE_CATEGORY_PROP_NAME,
                deviceCategory);
        jposEntry.addProperty(JposEntry.JPOS_VERSION_PROP_NAME, jposVersion);
        jposEntry.addProperty(JposEntry.PRODUCT_NAME_PROP_NAME, productName);
        jposEntry.addProperty(JposEntry.PRODUCT_DESCRIPTION_PROP_NAME,
                productDescription);
        jposEntry.addProperty(JposEntry.PRODUCT_URL_PROP_NAME, productURL);

        return jposEntry;
    }

    public void saveProperties() {
        try {
            XmlPropWriter writer = new XmlPropWriter("FiscalPrinter",
                    logicalName);
            writer.write(getPrinterImages());
            writer.write(printer.getReceiptImages());
            writer.writeHeader(header);
            writer.writeTrailer(trailer);
            writer.save(getPropsFileName());
        } catch (Exception e) {
            logger.error("saveProperties", e);
        }
    }

    private void loadProperties() {
        logger.debug("loadProperties");
        try {
            String fileName = getPropsFileName();
            File f = new File(fileName);
            if (f.exists()) {
                XmlPropReader reader = new XmlPropReader();
                reader.load("FiscalPrinter", logicalName, fileName);
                reader.read(getPrinterImages());
                reader.read(printer.getReceiptImages());
                reader.readHeader(header);
                reader.readTrailer(trailer);
            }
        } catch (Exception e) {
            logger.error("Failed to load properties", e);
        }
    }

    private void throwWrongStateError() {
        throw new JposException(JPOS_E_EXTENDED, JPOS_EFPTR_WRONG_STATE,
                Localizer.getString(Localizer.wrongPrinterState) + "("
                + String.valueOf(printerState) + ", " + "\""
                + PrinterState.getText(printerState.getValue()) + "\"");
    }

    public void writeParameter(String paramName, int value) {
        getPrinter().writeParameter(paramName, value);
    }

    public void writeParameter(String paramName, boolean value) {
        getPrinter().writeParameter(paramName, value);
    }

    public void writeParameter(String paramName, String value) {
        getPrinter().writeParameter(paramName, value);
    }

    public String readParameter(String paramName) {
        return getPrinter().readParameter(paramName);
    }

    public void setFiscalReceiptType(int value) {
        fiscalReceiptType = value;
    }

    public FiscalReceipt createReceipt(int fiscalReceiptType) {
        checkEnabled();
        checkPrinterState(FPTR_PS_MONITOR);
        switch (fiscalReceiptType) {
            case FPTR_RT_CASH_IN:
                return new CashInReceipt(createReceiptContext());

            case FPTR_RT_CASH_OUT:
                return new CashOutReceipt(createReceiptContext());

            case FPTR_RT_GENERIC:
            case FPTR_RT_SALES:
            case FPTR_RT_SERVICE:
            case FPTR_RT_SIMPLE_INVOICE: // case FPTR_RT_REFUND:
                return createSalesReceipt(PrinterConst.SMFP_RECTYPE_SALE);

            case FPTR_RT_REFUND:
                return createSalesReceipt(PrinterConst.SMFP_RECTYPE_RETSALE);

            default:
                throw new JposException(JPOS_E_ILLEGAL,
                        Localizer.getString(Localizer.invalidParameterValue));
        }
    }

    private FiscalReceipt createSalesReceipt(int receiptType) {
        FiscalReceipt result;
        if (params.FSEnabled) {
            result = new FSSalesReceipt(createReceiptContext(), receiptType);
        } else if (params.salesReceiptType == SMFPTR_RECEIPT_NORMAL) {
            result = new SalesReceipt(createReceiptContext(), receiptType);
        } else {
            result = new GlobusSalesReceipt(createReceiptContext(),
                    PrinterConst.SMFP_RECTYPE_SALE);
        }
        return result;
    }

    private ReceiptContext createReceiptContext() {
        return new ReceiptContext(receiptPrinter, params, fiscalDay,
                printerReceipt, printerState);
    }

    public FptrParameters getParams() {
        return params;
    }

    public PrinterReceipt getReceipt() {
        return printerReceipt;
    }

    public FontNumber getFont() {
        return params.getFont();
    }

    public String getStringEncoding() {
        return params.stringEncoding;
    }

    public boolean handleDeviceException(Exception e) {
        if (connected
                && (params.searchMode == SmFptrConst.SMFPTR_SEARCH_ON_ERROR)) {
            searchDevice(0);
            connected = true;
            return true;
        } else {
            return false;
        }
    }

    public int getFontNumber() {
        return params.getFont().getValue();
    }

    public void setFontNumber(int value) {
        params.setFont(new FontNumber(value));
    }

    public int getCommandTimeout(int code) {
        return printer.getCommandTimeout(code);
    }

    public EJActivation getEJActivation() {
        return aEJActivation;
    }

    public int loadLogo(String fileName) {
        return loadLogo(fileName, logoPosition);
    }

    public int loadLogo(String fileName, int logoPosition) {
        int imageIndex = -1;
        PrinterImage image = new PrinterImage(fileName);
        if (logoPosition < SMFPTR_LOGO_NONE) {
            printer.loadImage(image, true);
            imageIndex = getPrinterImages().getIndex(image);
            ReceiptImage receiptImage = new ReceiptImage();
            receiptImage.setPosition(logoPosition);
            receiptImage.setImageIndex(imageIndex);
            getReceiptImages().add(receiptImage);
            saveProperties();
        } else {
            printer.loadImage(image, false);
        }
        if (logoPosition == SMFPTR_LOGO_PRINT) {
            printer.printImage(image);
        }
        return imageIndex;
    }

    public ReceiptImages getReceiptImages() {
        return printer.getReceiptImages();
    }
}

 */
