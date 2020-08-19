/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ShtrihFiscalPrinter.h
 * Author: V.Kravtsov
 *
 * Created on June 28, 2016, 8:01 PM
 */

#ifndef SHTRIHFISCALPRINTER_H
#define SHTRIHFISCALPRINTER_H

#include <QMutex>
#include <QImage>
#include <QThread>
#include <vector>

#include "tlvtag.h"
#include "printercommand.h"
#include "printertypes.h"
#include "fiscalprinter.h"
#include "serverconnection.h"
#include "journalprinter.h"
#include "logger.h"

// ////////////////////////////////////////////////////////////////////////
// Parameter ID

#define FPTR_PARAMETER_REG_NUMBER           0
#define FPTR_PARAMETER_SERIAL_NUMBER        1
#define FPTR_PARAMETER_FISCAL_ID            2
#define FPTR_PARAMETER_FS_SERIAL_NUMBER     3
#define FPTR_PARAMETER_HEADER_ENABLED       4

// ////////////////////////////////////////////////////////////////////////
// Field types

#define FIELD_TYPE_INT  0
#define FIELD_TYPE_STR 1

// ////////////////////////////////////////////////////////////////////////
// Table numbers

// ECR type and mode
#define SMFP_TABLE_SETUP  1
// Cashier and admin's passwords
#define SMFP_TABLE_CASHIER 2
// Time conversion table
#define SMFP_TABLE_TIME 3
// Text in receipt
#define SMFP_TABLE_TEXT 4
// Payment type names
#define SMFP_TABLE_PAYTYPE 5
// Tax rates
#define SMFP_TABLE_TAX 6
// Department names
#define SMFP_TABLE_DEPARTMENT 7
// Font settings
#define SMFP_TABLE_FONTS 8
// Receipt format table
#define SMFP_TABLE_RECFORMAT 9

// ////////////////////////////////////////////////////////////////////////
// Printer submode constants

// Paper is present
#define ECR_SUBMODE_IDLE 0
// Out of paper: passive
#define ECR_SUBMODE_PASSIVE 1
// Out of paper: active
#define ECR_SUBMODE_ACTIVE 2
// After active paper out
#define ECR_SUBMODE_AFTER 3
// Printing reports mode
#define ECR_SUBMODE_REPORT 4
// Printing mode
#define ECR_SUBMODE_PRINT 5

// ////////////////////////////////////////////////////////////////////////
// MODE constants

// Dump mode
#define MODE_DUMPMODE 0x01
// Fiscal day opened, 24 hours have not run out
#define MODE_24NOTOVER 0x02
// Fiscal day opened, 24 hours have run out
#define MODE_24OVER 0x03
// Fiscal day closed
#define MODE_CLOSED 0x04
// Fiscal printer blocked after wrong password of tax inspector
#define MODE_LOCKED 0x05
// Confirm date mode
#define MODE_WAITDATE 0x06
// Mode for changing decimal dot position
#define MODE_POINTPOS 0x07
// Opened fiscal document
#define MODE_REC 0x08
// Fiscal printer reset mode
#define MODE_TECH 0x09
// Print selftest mode
#define MODE_TEST 0x0A
// Printing full fiscal report
#define MODE_FULLREPORT 0x0B
// Printing electronic journal report
#define MODE_EJREPORT 0x0C
// Opened fiscal slip document
#define MODE_SLP 0x0D
// Print fiscal slip mode
#define MODE_SLPPRINT 0x0E
// Fiscal slip ready to print
#define MODE_SLPREADY 0x0F

// ////////////////////////////////////////////////////////////////////////
// Printer submode constants

// Paper is present
#define ECR_SUBMODE_IDLE 0
// Out of paper: passive
#define ECR_SUBMODE_PASSIVE 1
// Out of paper: active
#define ECR_SUBMODE_ACTIVE 2
// After active paper out
#define ECR_SUBMODE_AFTER 3
// Printing reports mode
#define ECR_SUBMODE_REPORT 4
// Printing mode
#define ECR_SUBMODE_PRINT 5

// ////////////////////////////////////////////////////////////////////////
// Receipt types

#define SMFP_RECTYPE_SALE       0 // Sale receipt
#define SMFP_RECTYPE_BUY        1 // Buy receipt
#define SMFP_RECTYPE_RETSALE    2 // Sale return receipt
#define SMFP_RECTYPE_RETBUY     3 // Buy return receipt

// ////////////////////////////////////////////////////////////////////////
// Station types

#define SMFP_STATION_JRN        1 // Journal station
#define SMFP_STATION_REC        2 // Receipt station
#define SMFP_STATION_SLP        4 // Slip station
#define SMFP_STATION_RECJRN     3 // Receipt & journal stations

enum DeviceCode {
    FMStorage1 = 1, // 01 – накопитель ФП 1
    FMStorage2 = 2, // 02 – накопитель ФП 2
    RTC = 3, // 03 – часы
    NVRAM = 4, // 04 – энергонезависимая память
    FMCPU = 5, // 05 – процессор ФП
    FPCPU = 6, // 06 – память программ ФР
    FPRAM = 7, // 07 – оперативная память ФР
    SYSIMAGE = 8, // 08h – образ файловой системы
    ULINUX_IMAGE = 9, // 09h – образ uLinux
    APP_FILE = 0x0A, // 0Ah – исполняемый файл ПО
    APP_MEMORY = 0x86 // память программ ККТ
};

// Payment type (tag 1214)
enum PaymentType {
  PaymentTypeDeposit = 1,           // 1 Предоплата 100%
  PaymentTypePartialDeposit = 2,    // 2 Частичная предоплата
  PaymentTypeAdvance = 3,           // 3 Аванс
  PaymentTypeCash = 4,              // 4 Полный расчет
  PaymentTypeCashCredit = 5,        // 5. Частичный расчет и кредит
  PaymentTypeCredit = 6,            // 6. Передача в кредит
  PaymentTypePayCredit = 7          // 7. Оплата кредита
};

// Payment item (tag 1212)
enum PaymentItem {
  PaymentItemNormal = 1,            // 1. Товар
  PaymentItemExcise = 2,            // 2. Подакцизный товар
  PaymentItemJob = 3,               // 3. Работа
  PaymentItemService = 4,           // 4. Услуга
  PaymentItemBet = 5,               // 5. Ставка азартной игры
  PaymentItemGamePrize = 6,         // 6. Выигрыш азартной игры
  PaymentItemLotteryBill = 7,       // 7. Лотерейный билет
  PaymentItemLotteryPrize = 8,      // 8. Выигрыш лотереи
  PaymentItemIntellect = 9,         // 9. Предоставление РИД
  PaymentItemPayment = 10,          // 10. Платеж
  PaymentItemComposite = 11,        // 11. Составной предмет расчета
  PaymentItemOther = 12             // 12. Иной предмет расчета
  };

enum PrinterAlignment {
    Left = 0,
    Center = 1,
    Right = 2
};

struct StartDumpCommand {
    DeviceCode deviceCode; // in, код устройства
    int resultCode; // out, результат выполнения
    uint16_t blockCount; // out, количество блоков
};

struct ReadDumpCommand {
    int resultCode;         // out, результат выполнения
    DeviceCode deviceCode;  // out, код устройства
    uint16_t blockNumber;   // out, номер блока
    char blockData[32];     // out, данные блока
};

struct StopDumpCommand {
    int resultCode; // out, результат выполнения
};

struct FiscalizationCommand {
    uint32_t newPassword; // in, Пароль новый (4 байта)
    uint64_t inn; // in, ИНН (6 байт) 000000000000…999999999999
    uint64_t rnm; // in, РНМ (7 байт) 00000000000000…99999999999999
    int resultCode; // out, результат выполнения
    uint8_t fiscalNumber; // out, Номер фискализации (перерегистрации) (1 байт) 1…16
    uint8_t fiscalLeft; // out, Количество оставшихся перерегистраций (1 байт) 0…15
    uint16_t dayNumber; // out, Номер последней закрытой смены
    PrinterDate date; // out, Дата фискализации (перерегистрации) (3 байта) ДД-ММ-ГГ
};

struct BeepCommand {
    int resultCode; // out, результат выполнения
    uint8_t operatorNumber; // out, номер оператора
};

struct WriteLongSerialCommand {
    uint64_t serial; // in, серийный номер
    int resultCode; // out, результат выполнения
};

struct ReadLongSerialCommand {
    int resultCode; // out, результат выполнения
    uint64_t serial; // out, Заводской номер (7 байт) 00000000000000…99999999999999
    uint64_t rnm; // out, РНМ (7 байт) 00000000000000…99999999999999
};

/*
2 – Верхний датчик подкладного документа (0 – нет, 1 – да)
3 – Нижний датчик подкладного документа (0 – нет, 1 – да)
4 – Положение десятичной точки (0 – 0 знаков, 1 – 2 знака)
5 – ЭКЛЗ (0 – нет, 1 – есть)
6 – Оптический датчик операционного журнала (0 – бумаги нет, 1 – бумага есть)
7 – Оптический датчик чековой ленты (0 – бумаги нет, 1 – бумага есть)
8 – Рычаг термоголовки контрольной ленты (0 – поднят, 1 – опущен)
9 – Рычаг термоголовки чековой ленты (0 – поднят, 1 – опущен)
10 – Крышка корпуса ФР (0 – опущена, 1 – поднята)
11 – Денежный ящик (0 – закрыт, 1 – окрыт)
12а – Отказ правого датчика принтера (0 – нет, 1 – да)
12б – Бумага на входе в презентер (0 – нет, 1 – да)
13а – Отказ левого датчика принтера (0 – нет, 1 – да)
13б – Бумага на выходе из презентера (0 – нет, 1 – да)
14 – ЭКЛЗ почти заполнена (0 – нет, 1 – да)
15а – Увеличенная точность количества (0 – нормальная точность, 1 – увеличенная точность) [для ККМ без ЭКЛЗ]
15б – Буфер принтера непуст (0 – пуст, 1 – непуст)
[для POS-системы Штрих ПОС Ф]
*/

struct PrinterFlags {
    bool isJrnNearEnd;
    bool isJrnRollPresent; // Рулон операционного журнала
    bool isRecNearEnd;
    bool isRecRollPresent; // Рулон чековой ленты
    bool isSlipPresent; // Верхний датчик подкладного документа
    bool isSlpEmpty;
    bool isSlipMoving;
    bool isSlpNearEnd;
    bool getAmountPointPosition;
    bool isEJPresent;
    bool isJrnEmpty;
    bool isJrnPresent;
    bool isRecEmpty;
    bool isRecPresent;
    bool isJrnLeverUp;
    bool isRecLeverUp;
    bool isCoverOpened;
    bool isDrawerOpened;
    bool isRSensorFailure;
    bool isPresenterIn;
    bool isLSensorFailure;
    bool isPresenterOut;
    bool isEJNearEnd;
    bool isExtQuantity;
};

////////////////////////////////////////////////////////////////////////////////
// Причина завершения печати или промотки бумаги:
// 0 – печать завершена успешно
// 1 – произошел обрыв бумаги
// 2 – ошибка принтера (перегрев головки, другая ошибка)
// 5 – идет печать

enum PrintResult {
    OK = 0,
    NoPaper = 1,
    PrinterError = 2,
    Printing = 5
};

struct ReadShortStatusCommand {
    int resultCode; // out, результат выполнения
    uint8_t operatorNumber; // out, номер оператора
    uint16_t flags; // out, Флаги ФР (2 байта)
    //PrinterFlags flags;       // out, Флаги ФР (2 байта)
    uint8_t mode; // out, Режим ФР (1 байт)
    uint8_t submode; // out, Подрежим ФР (1 байт)
    uint16_t numOperations; // out, Количество операций в чеке
    double batteryVoltage; // out, Напряжение резервной батареи (1 байт)
    double supplyVoltage; // out, Напряжение источника питания (1 байт)
    uint8_t fmError; // out, Код ошибки ФП (1 байт)
    uint8_t ejError; // out, Код ошибки ЭКЛЗ (1 байт)
    char reserved[3]; // out, Зарезервировано (3 байта)
    PrintResult printResult;
    bool validPrintResult;
};

struct ReadLongStatusCommand {
    int resultCode; // out, результат выполнения
    uint8_t operatorNumber; // out, номер оператора
    uint16_t flags; // out, Флаги ФР (2 байта)
    uint8_t mode; // out, Режим ФР (1 байт)
    uint8_t submode; // out, Подрежим ФР (1 байт)
    QString firmwareVersion;
    uint16_t firmwareBuild;
    PrinterDate firmwareDate;
    uint8_t storeNumber;
    uint16_t documentNumber;
    uint8_t portNumber;
    QString fmFirmwareVersion;
    uint16_t fmFirmwareBuild;
    PrinterDate fmFirmwareDate;
    PrinterDate date;
    PrinterTime time;
    uint8_t fmFlags;
    uint32_t serialNumber;
    uint16_t dayNumber;
    uint16_t fmFreeRecords;
    uint8_t registrationNumber;
    uint8_t freeRegistration;
    uint64_t fiscalID;
};

struct PrintBoldCommand {
    uint8_t flags; // in, Флаги
    QString text; // Печатаемые символы (20 байт)
    int resultCode; // out, результат выполнения
    uint8_t operatorNumber; // out, номер оператора
};

struct PortParametersCommand {
    uint8_t port; // in, Номер порта
    uint32_t baudRate; // Код скорости обмена
    uint32_t timeout; // Тайм аут приема байта
    int resultCode; // out, результат выполнения
};

struct PrintStringCommand {
    uint8_t flags; // in, Флаги
    QString text; // Печатаемые символы (20 байт)
    int resultCode; // out, результат выполнения
    uint8_t operatorNumber; // out, номер оператора
};

struct PrintDocHeaderCommand {
    QString text; // in, Наименование документа (30 байт)
    uint16_t number; // in, Номер документа (2 байта)
    int resultCode; // out, Код ошибки (1 байт)
    uint8_t operatorNumber; // out, Порядковый номер оператора (1 байт) 1…30
    uint16_t documentNumber; // out, Сквозной номер документа (2 байта)
};

struct StartTestCommand {
    uint8_t periodInMinutes; // in, Период вывода в минутах (1 байт) 1…99
    int resultCode; // out, Код ошибки (1 байт)
    uint8_t operatorNumber; // out, Порядковый номер оператора (1 байт) 1…30
};

struct ReadCashRegCommand {
    uint8_t number; // in, Номер регистра (1 байт) 0… 255
    int resultCode; // out, Код ошибки (1 байт)
    uint8_t operatorNumber; // out, Порядковый номер оператора (1 байт) 1…30
    uint64_t value; // out, Содержимое регистра (6 байт)
};

struct ReadOperRegCommand {
    uint8_t number; // in, Номер регистра (1 байт) 0… 255
    int resultCode; // out, Код ошибки (1 байт)
    uint8_t operatorNumber; // out, Порядковый номер оператора (1 байт) 1…30
    uint16_t value; // out, Содержимое регистра (2 байта)
};

struct TableValueCommand {
    uint8_t table; // in, Таблица (1 байт)
    uint16_t row; // in, Ряд (2 байта)
    uint8_t field; // in, Поле (1 байт)
    QByteArray value; // in, Значение (X байт) до 40 байт
    int resultCode; // out, Код ошибки (1 байт)
};

struct TimeCommand {
    PrinterTime time; // in, Время
    int resultCode; // out, Код ошибки (1 байт)
};

struct DateCommand {
    PrinterDate date; // in, Дата
    int resultCode; // out, Код ошибки (1 байт)
};

struct CutCommand {
    uint8_t cutType; // in, Тип отрезки
    int resultCode; // out, Код ошибки (1 байт)
    uint8_t operatorNumber; // out, Порядковый номер оператора (1 байт) 1…30
};

struct ReadFontCommand {
    uint8_t fontNumber; // in, Номер шрифта
    int resultCode; // out, Код ошибки (1 байт)
    uint16_t printWidth;
    uint8_t charWidth;
    uint8_t charHeight;
    uint8_t fontCount;
};

struct FontInfo {
    uint8_t fontNumber; // in, Номер шрифта
    uint16_t printWidth;
    uint8_t charWidth;
    uint8_t charHeight;
};

struct OpenDrawerCommand {
    uint8_t number; // in, Номер денежного ящика
    int resultCode; // out, Код ошибки (1 байт)
    uint8_t operatorNumber; // out, Порядковый номер оператора (1 байт) 1…30
};

struct FeedPaperCommand {
    uint8_t flags;
    uint8_t count;
    int resultCode; // out, Код ошибки (1 байт)
    uint8_t operatorNumber; // out, Порядковый номер оператора (1 байт) 1…30
};

struct PrintStringFontCommand {
    uint8_t flags;
    uint8_t font;
    QString text;
    int resultCode; // out, Код ошибки (1 байт)
    uint8_t operatorNumber; // out, Порядковый номер оператора (1 байт) 1…30
};

struct PasswordCommand {
    int resultCode; // out, Код ошибки (1 байт)
    uint8_t operatorNumber; // out, Порядковый номер оператора (1 байт) 1…30
};

struct CashCommand {
    uint64_t amount; // in, Сумма (5 байт)
    int resultCode; // out, Код ошибки (1 байт)
    uint8_t operatorNumber; // out, Порядковый номер оператора (1 байт) 1…30
    uint16_t documentNumber; // out, Сквозной номер документа (2 байта)
};

struct PrintDocEndCommand {
    uint8_t printad; // in, Параметр (1 байт)
    int resultCode; // out, Код ошибки (1 байт)
    uint8_t operatorNumber; // out, Порядковый номер оператора (1 байт) 1…30
};

struct ReceiptItemCommand {
    uint64_t quantity; // in, Количество (5 байт) 0000000000…9999999999
    uint64_t price; // in, Цена (5 байт) 0000000000…9999999999
    uint8_t department; // in, Номер отдела (1 байт) 0…16
    uint8_t tax1; // in, Налог 1 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    uint8_t tax2; // in, Налог 2 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    uint8_t tax3; // in, Налог 3 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    uint8_t tax4; // in, Налог 4 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    QString text; // Текст (40 байт)
    int resultCode; // out, Код ошибки (1 байт)
    uint8_t operatorNumber; // out, Порядковый номер оператора (1 байт) 1…30
};

struct CloseReceiptCommand {
    uint64_t amount1; // in, Сумма наличных (5 байт) 0000000000…9999999999
    uint64_t amount2; // in, Сумма типа оплаты 2 (5 байт)
    uint64_t amount3; // in, Сумма типа оплаты 3 (5 байт)
    uint64_t amount4; // in, Сумма типа оплаты 4 (5 байт)
    uint16_t discount; // in, Скидка/Надбавка (2 байта со знаком) -9999…9999
    uint8_t tax1; // in, Налог 1 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    uint8_t tax2; // in, Налог 2 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    uint8_t tax3; // in, Налог 3 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    uint8_t tax4; // in, Налог 4 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    QString text; // Текст (40 байт)
    int resultCode; // out, Код ошибки (1 байт)
    uint8_t operatorNumber; // out, Порядковый номер оператора (1 байт) 1…30
    uint64_t change; // out, Сдача (5 байт) 0000000000…9999999999
};

struct CloseReceiptCommand2{
    CloseReceiptCommand closeCommand;
    int documentNumber;
    int receiptNumber;
};

struct AmountAjustCommand {
    uint64_t amount; // in, Сумма наличных (5 байт) 0000000000…9999999999
    uint8_t tax1; // in, Налог 1 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    uint8_t tax2; // in, Налог 2 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    uint8_t tax3; // in, Налог 3 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    uint8_t tax4; // in, Налог 4 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    QString text; // Текст (40 байт)
    int resultCode; // out, Код ошибки (1 байт)
    uint8_t operatorNumber; // out, Порядковый номер оператора (1 байт) 1…30
};

struct ReadSubtotalCommand {
    int resultCode; // out, Код ошибки (1 байт)
    uint8_t operatorNumber; // out, Порядковый номер оператора (1 байт) 1…30
    uint64_t amount; // out, Сумма чека (5 байт) 0000000000…9999999999
};

struct OpenReceiptCommand {
    uint8_t receiptType; // in, Тип чека
    int resultCode; // out, Код ошибки (1 байт)
    uint8_t operatorNumber; // out, Порядковый номер оператора (1 байт) 1…30
};


struct PrintEJDepartmentReportOnDates
{
    uint8_t reportType;     // in, Тип отчета
    uint8_t department;     // in, Номер отдела
    PrinterDate date1;      // in, Дата первой смены
    PrinterDate date2;      // in, Дата последней смены
    int resultCode;     // out, Код ошибки (1 байт)
};

struct PrintEJDepartmentReportOnDays
{
    uint8_t reportType;     // in, Тип отчета
    uint8_t department;     // in, Номер отдела
    uint16_t day1;          // in, Номер первой смены (2 байта) 0000…2100
    uint16_t day2;          // in, Номер последней смены (2 байта) 0000…2100
    PrinterDate date2;      // in, Дата последней смены
    int resultCode;     // out, Код ошибки (1 байт)
};

struct PrintEJDayReportOnDates
{
    uint8_t reportType;     // in, Тип отчета
    PrinterDate date1;      // in, Дата первой смены
    PrinterDate date2;      // in, Дата последней смены
    int resultCode;     // out, Код ошибки (1 байт)
};

struct PrintEJDayReportOnDays
{
    uint8_t reportType;     // in, Тип отчета
    uint16_t day1;          // in, Номер первой смены (2 байта) 0000…2100
    uint16_t day2;          // in, Номер последней смены (2 байта) 0000…2100
    int resultCode;     // out, Код ошибки (1 байт)
};

struct PrintEJDayTotal
{
    uint16_t day;           // in, Номер смены (2 байта) 0000…2100
    int resultCode;     // out, Код ошибки (1 байт)
};

struct PrintEJDocument
{
    uint32_t macNumber;     // in, Номер КПК (4 байта) 00000000…99999999
    int resultCode;     // out, Код ошибки (1 байт)
};

struct PrintEJDayReport
{
    uint16_t day;           // in, Номер смены (2 байта) 0000…2100
    int resultCode;     // out, Код ошибки (1 байт)
};

struct ReadEJSerial
{
    int resultCode;     // out, Код ошибки (1 байт)
    uint64_t serial;        // out, Номер ЭКЛЗ (5 байт)
};

struct EJFlags{
    uint8_t docType;
    bool isArchiveOpened;
    bool isActivated;
    bool isReportMode;
    bool isDocOpened;
    bool isDayOpened;
    bool isFatalError;
};

struct ReadEJStatus1
{
    int resultCode;     // out, Код ошибки (1 байт)
    uint64_t docAmount;     // out, Итог документа последнего КПК (5 байт) 0000000000…9999999999
    PrinterDate docDate;    // out, Дата последнего КПК (3 байта) ДД-ММ-ГГ
    PrinterTime docTime;    // out, Время последнего КПК (2 байта) ЧЧ-ММ
    uint32_t macNumber;     // out, Номер последнего КПК (4 байта) 00000000…99999999
    uint64_t serial;        // out, Номер ЭКЛЗ (5 байт) 0000000000…9999999999
    EJFlags flags;          // out, Флаги ЭКЛЗ (см. описание ЭКЛЗ) (1 байт)
};

struct ReadEJStatus2
{
    int resultCode;     // out, Код ошибки (1 байт)
    uint16_t dayNumber;
    uint64_t saleTotal;
    uint64_t buyTotal;
    uint64_t retSaleTotal;
    uint64_t retBuyTotal;
};

struct ReadEJJournal{
    uint16_t dayNumber;
    int resultCode;     // out, Код ошибки (1 байт)
    QString text;
};

struct ReadEJDocument{
    uint32_t macNumber;
    int resultCode;     // out, Код ошибки (1 байт)
    QString text;
};

struct ReadEJVersion
{
    int resultCode;     // out, Код ошибки (1 байт)
    QString version;
};

struct ReadEJDocumentLine
{
    int resultCode;     // out, Код ошибки (1 байт)
    QString text;
};

struct ReadEJDepartmentReportByDates{
    uint8_t reportType;
    uint8_t department;
    PrinterDate date1;
    PrinterDate date2;
    int resultCode;
    QString text;
};

struct ReadEJReportByDates{
    uint8_t reportType;
    PrinterDate date1;
    PrinterDate date2;
    int resultCode;
    QString text;
};

struct ReadEJReportByDays{
    uint8_t reportType;
    uint16_t day1;
    uint16_t day2;
    int resultCode;
    QString text;
};

struct ReadEJDepartmentReportByDays{
    uint8_t reportType;
    uint8_t department;
    uint16_t day1;
    uint16_t day2;
    int resultCode;
    QString text;
};

struct ReadEJDayTotals{
    uint16_t day;
    int resultCode;
    QString text;
};

struct ReadEJActivation{
    int resultCode;
    QString text;
};

struct LoadGraphicsCommand {
    uint line;               // in, Номер строки
    QByteArray data;         // in, Двнные
    int resultCode;         // out, Код ошибки (1 байт)
    uint operatorNumber;     // out, Порядковый номер оператора (1 байт) 1…30
};

struct PrintGraphicsCommand {
    uint16_t line1; // in, Номер строки 1
    uint16_t line2; // in, Номер строки 2
    int resultCode; // out, Код ошибки (1 байт)
    uint8_t operatorNumber; // out, Порядковый номер оператора (1 байт) 1…30
};

struct PrintBarcodeCommand {
    uint64_t barcode; // in, Штрихкод
    int resultCode; // out, Код ошибки (1 байт)
    uint8_t operatorNumber; // out, Порядковый номер оператора (1 байт) 1…30
};

struct GraphicsLineCommand {
    uint16_t height; // in, Высота
    QByteArray data; // in, Данные
    int resultCode; // out, Код ошибки (1 байт)
    uint8_t operatorNumber; // out, Порядковый номер оператора (1 байт) 1…30
};

struct DeviceTypeCommand {
    int resultCode; // out, Код ошибки (1 байт)
    uint8_t deviceType1;
    uint8_t deviceType2;
    uint8_t protocolVersion1;
    uint8_t protocolVersion2;
    uint8_t model;
    uint8_t language;
    QString name;
};

struct ScaledGraphicsCommand {
    uint8_t line1;
    uint8_t line2;
    uint8_t vscale;
    uint8_t hscale;
    int resultCode; // out, Код ошибки (1 байт)
    uint8_t operatorNumber; // out, Порядковый номер оператора (1 байт) 1…30
};

struct LicenseCommand {
    uint64_t license;
    int resultCode; // out, Код ошибки (1 байт)
};

struct PointCommand {
    uint8_t point;
    int resultCode; // out, Код ошибки (1 байт)
};

struct SlipEjectCommand {
    uint8_t ejectDirection;
    int resultCode; // out, Код ошибки (1 байт)
    uint8_t operatorNumber; // out, Порядковый номер оператора (1 байт) 1…30
};

struct TableInfoCommand {
    uint8_t table; // in, Номер таблицы (1 байт)
    int resultCode; // out, Код ошибки (1 байт)
    QString name; // out, Название таблицы (40 байт)
    uint16_t rows; // out, Количество рядов (2 байта)
    uint8_t fields; // out, Количество полей (1 байт)
};

struct FieldInfoCommand {
    uint8_t table; // in, Номер таблицы (1 байт)
    uint8_t field; // in, Номер поля (1 байт)
    int resultCode; // out, Код ошибки (1 байт)
    QString name; // out, Название поля (40 байт)
    uint8_t type; // out, Тип поля (1 байт) «0» – BIN, «1» – CHAR
    uint8_t size; // out, Количество байт – X (1 байт)
    uint64_t min; // out, Минимальное значение поля – для полей типа BIN (X байт)
    uint64_t max; // out, Максимальное значение поля – для полей типа BIN (X байт)
};


struct PrinterFieldInfo{
    uint8_t table;
    uint8_t field;
    uint16_t row;
    uint64_t min;
    uint64_t max;
    uint8_t type;
    uint8_t size;
    QString name;
};

struct PrinterTableInfo{
    int number;
    QString name;
    int fieldCount;
    int rowCount;
};

struct WriteSerialCommand {
    uint32_t serial; // in, серийный номер
    int resultCode; // out, результат выполнения
};

struct PrinterTotals {
    uint8_t type;       // in,
    int resultCode; // out, результат выполнения
    uint64_t saleTotal;
    uint64_t buyTotal;
    uint64_t retSaleTotal;
    uint64_t retBuyTotal;
};

struct FMReadTotalsCommand {
    uint8_t type; // in,
    int resultCode; // out, результат выполнения
    uint8_t operatorNumber;
    uint64_t saleTotal;
    uint64_t buyTotal;
    uint64_t retSaleTotal;
    uint64_t retBuyTotal;
};

struct FSReadTotalsCommand {
    int resultCode; // out, результат выполнения
    uint64_t saleTotal;
    uint64_t buyTotal;
    uint64_t retSaleTotal;
    uint64_t retBuyTotal;
};


struct FMReadLastDateCommand {
    int resultCode; // out, результат выполнения
    uint8_t operatorNumber;
    uint8_t type; // in,
    PrinterDate date;
};

struct FMReadRangeCommand {
    int resultCode; // out, результат выполнения
    PrinterDate firstDayDate;
    PrinterDate lastDayDate;
    uint16_t firstDayNumber;
    uint16_t lastDayNumber;
};

struct FMDatesReportCommand {
    uint8_t reportType;
    PrinterDate firstDate;
    PrinterDate lastDate;

    int resultCode; // out, результат выполнения
    PrinterDate firstDayDate;
    PrinterDate lastDayDate;
    uint16_t firstDayNumber;
    uint16_t lastDayNumber;
};

struct FMDaysReportCommand {
    uint8_t reportType;
    uint16_t firstDay;
    uint16_t lastDay;

    int resultCode; // out, результат выполнения
    PrinterDate firstDayDate;
    PrinterDate lastDayDate;
    uint16_t firstDayNumber;
    uint16_t lastDayNumber;
};

struct FMReadFiscalizationCommand {
    uint8_t fiscalNumber; // in, Номер фискализации (перерегистрации) (1 байт) 1…16
    int resultCode; // out, результат выполнения
    uint64_t rnm; // out, РНМ (5 байт) 00000000000000…99999999999999
    uint64_t inn; // out, ИНН (6 байт) 000000000000…999999999999
    uint16_t dayNumber; // out, Номер смены до фискализации
    PrinterDate date; // out, Дата фискализации (перерегистрации) (3 байта) ДД-ММ-ГГ
};

struct FMReadCorruptedRecordsCommand {
    uint8_t recordType;     // in, Тип проверяемой записи (1 байт)
    int resultCode;     // out, результат выполнения
    uint8_t operatorNumber; // out, Порядковый номер оператора (1 байт)
    uint16_t recordCount;   // out, Количество сбойных записей (2 байта)
};

struct ReadErrorTextCommand {
    uint8_t errorCode;      // in
    int resultCode;     // out
    QString errorText;      // out
};


struct SlipDocParams {
    uint8_t docType; // Тип документа (1 байт) "0" - продажа, "1" - покупка, "2" - возврат продажи, "3" - возврат покупки
    uint8_t copyType; // Дублирование печати (извещение, квитанция) (1 байт) "0" - колонки, "1" - блоки строк
    uint8_t copyCount; // Количество дублей (1 байт) 0…5
    uint8_t copy1Offset; // Смещение между оригиналом и 1-ым дублем печати (1 байт) *
    uint8_t copy2Offset; // Смещение между 1-ым и 2-ым дублями печати (1 байт) *
    uint8_t copy3Offset; // Смещение между 2-ым и 3-им дублями печати (1 байт) *
    uint8_t copy4Offset; // Смещение между 3-им и 4-ым дублями печати (1 байт) *
    uint8_t copy5Offset; // Смещение между 4-ым и 5-ым дублями печати (1 байт) *
    uint8_t headerFont; // Номер шрифта клише (1 байт)
    uint8_t docHeaderFont; // Номер шрифта заголовка документа (1 байт)
    uint8_t ejSerialFont; // Номер шрифта номера ЭКЛЗ (1 байт)
    uint8_t ejCRCFont; // Номер шрифта значения КПК и номера КПК (1 байт)
    uint8_t headerLine; // Номер строки клише (1 байт)
    uint8_t docHeaderLine; // Номер строки заголовка документа (1 байт)
    uint8_t ejSErialLine; // Номер строки номера ЭКЛЗ (1 байт)
    uint8_t dupSignLine; // Номер строки признака повтора документа (1 байт)
    uint8_t headerLineOffset; // Смещение клише в строке (1 байт)
    uint8_t docHeaderLineOffset; // Смещение заголовка документа в строке (1 байт)
    uint8_t ejSerialLineOffset; // Смещение номера ЭКЛЗ в строке (1 байт)
    uint8_t ejCRCLineOffset; // Смещение КПК и номера КПК в строке (1 байт)
    uint8_t ejDupSignLineOffset; // Смещение признака повтора документа в строке (1 байт)
};

struct SlipOpenDocumentCommand {
    SlipDocParams params;
    int resultCode; // out, результат выполнения
    uint8_t operatorNumber;
    uint16_t docNumber;
};

struct StdSlipParams {
    uint8_t docType; // Тип документа (1 байт) "0" - продажа, "1" - покупка, "2" - возврат продажи, "3" - возврат покупки
    uint8_t copyType; // Дублирование печати (извещение, квитанция) (1 байт) "0" - колонки, "1" - блоки строк
    uint8_t copyCount; // Количество дублей (1 байт) 0…5
    uint8_t copy1Offset; // Смещение между оригиналом и 1-ым дублем печати (1 байт) *
    uint8_t copy2Offset; // Смещение между 1-ым и 2-ым дублями печати (1 байт) *
    uint8_t copy3Offset; // Смещение между 2-ым и 3-им дублями печати (1 байт) *
    uint8_t copy4Offset; // Смещение между 3-им и 4-ым дублями печати (1 байт) *
    uint8_t copy5Offset; // Смещение между 4-ым и 5-ым дублями печати (1 байт) *
};

struct SlipOpenStdDocumentCommand {
    StdSlipParams params;
    int resultCode; // out, результат выполнения
    uint8_t operatorNumber;
    uint16_t docNumber;
};

struct SlipOperationParams {
    uint8_t quantityFormat; // Формат целого количества (1 байт) "0" - без цифр после запятой, "1" - с цифрами после запятой
    uint8_t lineCount; // Количество строк в операции (1 байт) 1…3
    uint8_t textLine; // Номер текстовой строки в операции (1 байт) 0…3, "0" - не печатать
    uint8_t quantityLine; // Номер строки произведения количества на цену в операции (1 байт) 0…3, "0" - не печатать
    uint8_t summLine; // Номер строки суммы в операции (1 байт) 1…3
    uint8_t departmentLine; // Номер строки отдела в операции (1 байт) 1…3
    uint8_t textFont; // Номер шрифта текстовой строки (1 байт)
    uint8_t quantityFont; // Номер шрифта количества (1 байт)
    uint8_t multSignFont; // Номер шрифта знака умножения количества на цену (1 байт)
    uint8_t priceFont; // Номер шрифта цены (1 байт)
    uint8_t sumFont; // Номер шрифта суммы (1 байт)
    uint8_t departmentFont; // Номер шрифта отдела (1 байт)
    uint8_t textWidth; // Количество символов поля текстовой строки (1 байт)
    uint8_t quantityWidth; // Количество символов поля количества (1 байт)
    uint8_t priceWidth; // Количество символов поля цены (1 байт)
    uint8_t sumWidth; // Количество символов поля суммы (1 байт)
    uint8_t departmentWidth; // Количество символов поля отдела (1 байт)
    uint8_t textOffset; // Смещение поля текстовой строки в строке (1 байт)
    uint8_t quantityOffset; // Смещение поля произведения количества на цену в строке (1 байт)
    uint8_t sumOffset; // Смещение поля суммы в строке (1 байт)
    uint8_t departmentOffset; // Смещение поля отдела в строке (1 байт)
};

struct SlipOperation {
    uint8_t lineNumber; // Номер строки ПД с первой строкой
    uint64_t quantity; // Количество (5 байт)
    uint64_t price; // Цена (5 байт)
    uint8_t department; // Отдел (1 байт) 0…16
    uint8_t tax1; // Налог 1 (1 байт) "0" - нет, "1"…"4" - налоговая группа
    uint8_t tax2; // Налог 2 (1 байт) "0" - нет, "1"…"4" - налоговая группа
    uint8_t tax3; // Налог 3 (1 байт) "0" - нет, "1"…"4" - налоговая группа
    uint8_t tax4; // Налог 4 (1 байт) "0" - нет, "1"…"4" - налоговая группа
    QString text; // Текст (40 байт)
};

struct SlipPrintItemCommand {
    SlipOperationParams params;
    SlipOperation item;
    int resultCode;
    uint8_t operatorNumber;
};

struct PrinterBarcode {
    QString text; // barcode text
    uint8_t type; // barcode type
    /*
    QString label;          // barcode label
    uint8_t barWidth;       // barcode bar width, pixels
    uint8_t height;         // barcode height, pixels
    uint8_t textPosition;   // text position
    uint8_t textFont;       // text font
    void* parameters;       // additional parameters
    */
};

struct PrinterImage {
    int startLine;
    int height;
};

struct LoadGraphics3Command {
    uint8_t lineLength;
    uint16_t startLine;
    uint16_t lineCount;
    uint8_t bufferType;
    QByteArray data; // in, Двнные
    int resultCode; // out, Код ошибки (1 байт)
    uint8_t operatorNumber; // out, Порядковый номер оператора (1 байт) 1…30
};

struct PrintGraphics3Command {
    uint16_t startLine;
    uint16_t endLine;
    uint8_t vScale;
    uint8_t hScale;
    uint8_t flags;
    int resultCode; // out, Код ошибки (1 байт)
    uint8_t operatorNumber; // out, Порядковый номер оператора (1 байт) 1…30
};

struct OFDHeader{
    uint16_t protocol;
    uint16_t signature;
    uint16_t sversion;
    uint16_t aversion;
    char fsnumber[16];
    uint16_t size;
    uint16_t flags;
    uint16_t crc;
};

struct FSMode{
    uint8_t value;
    bool isConfigured;      // Бит 0 – проведена настройка ФН
    bool isFiscalOpened;    // Бит 1 – открыт фискальный режим
    bool isFiscalClosed;    // Бит 2 – закрыт постфискальный режим
    bool isDataSended;      // Бит 3 – закончена передача фискальных данных в ОФД
};

struct FSWarnFlags{
    uint8_t value;
    bool cpLockIn3Days;     // Бит 0 – Срочная замена КС (до окончания срока действия 3 дня)
    bool cpLockIn30Days;    // Бит 1 – Исчерпание ресурса КС (до окончания срока действия 30 дней)
    bool fsMemoryOverflow;  // Бит 2 – Переполнение памяти ФН (Архив ФН заполнен на 90 %)
    bool fdoAnswerTimeout;  // Бит 3 – Превышено время ожидания ответа ОФД
};

struct FSStatus {
    FSMode mode;            // Состояние фазы жизни
    uint8_t docType;            // Текущий документ
    bool isDocReceived;     // Получены данные документа
    bool isDayOpened;       // Смена открыта
    FSWarnFlags flags;      // Флаги предупреждения
    PrinterDate date;       // Дата ФН
    PrinterTime time;       // Время ФН
    QString fsSerial;       // Номер ФН
    uint32_t docNumber;         // Номер последнего ФД
};

struct FSVersion{
    QString text;           // Строка версии программного обеспечения ФН:16 байт ASCII
    bool isRelease;         // Тип программного обеспечения ФН: 1 байт
};

struct FSReadFiscalization{
    PrinterDate date;           // out, Дата и время: 5 байт DATE_TIME
    PrinterTime time;
    QString inn;                // out, ИНН : 12 байт ASCII
    QString rnm;                // out, Регистрационный номер ККT: 20 байт ASCII
    uint8_t taxSystem;          // out, Код налогообложения: 1 байт
    uint8_t workMode;           // out, Режим работы: 1 байт
    uint8_t regCode;            // out, Код причины перерегистрации2: 1 байт
    uint32_t docNum;            // out, Номер ФД: 4 байта
    uint32_t docMac;            // out, Фискальный признак: 4 байта
};

struct FSPrintFiscalization{
    QString INN;                // in, ИНН : 12 байт ASCII
    QString RNM;                // in, Регистрационный номер ККТ: 20 байт ASCII
    uint8_t taxMode;            // in, Код налогообложения: 1 байт
    uint8_t workMode;           // in, Режим работы: 1 байт
    uint32_t docNum;            // out, Номер ФД: 4 байта
    uint32_t docMac;            // out, Фискальный признак: 4 байта
};

struct FSDocument1
{
    PrinterDate date;
    PrinterTime time;
    uint32_t docNum;
    uint32_t docMac;
    QString taxID;
    QString regNumber;
    uint8_t taxType;
    uint8_t workMode;
};

struct FSDocument11
{
    PrinterDate date;
    PrinterTime time;
    uint32_t docNum;
    uint32_t docMac;
    QString taxID;
    QString regNumber;
    uint8_t taxType;
    uint8_t workMode;
    uint8_t reasonCode;
};

struct FSDocument6
{
    PrinterDate date;
    PrinterTime time;
    uint32_t docNum;
    uint32_t docMac;
    QString taxID;
    QString regNumber;
};

struct FSDocument2
{
    PrinterDate date;
    PrinterTime time;
    uint32_t docNum;
    uint32_t docMac;
    uint32_t dayNum;
};

struct FSDocument3
{
    PrinterDate date;
    PrinterTime time;
    uint32_t docNum;
    uint32_t docMac;
    uint8_t operationType;
    uint64_t amount;
};

struct FSDocument21
{
    PrinterDate date;
    PrinterTime time;
    uint32_t docNum;
    uint32_t docMac;
    uint32_t docCount;
    PrinterDate docDate;
    PrinterTime docTime;
};

struct FSFindDocument
{
    uint32_t docNum;            // in, Номер фискального документа: 4 байта
    int resultCode;         // out, Код ошибки: 1 байт
    uint8_t docType;            // out, Тип фискального документа: 1 байт
    uint8_t hasTicket;          // out, Получена ли квитанция из ОФД: 1 байт
    QByteArray docData;         // out, Данные фискального документа
    FSDocument1 document1;
    FSDocument2 document2;
    FSDocument3 document3;
    FSDocument6 document6;
    FSDocument11 document11;
    FSDocument21 document21;
};

struct FSOpenDay{
    int resultCode;         // out, Код ошибки: 1 байт
    uint16_t dayNum;            // out, Номер смены: 2 байта
    uint32_t docNum;            // out, Номер ФД: 4 байта
    uint32_t docMac;            // out, Фискальный признак: 4 байта
};

struct FSReceiptItem{
    uint8_t operation;      // in, Тип операции: 1 байт
    uint64_t quantity;      // in, Количество: 5 байт 0000000000…9999999999
    uint64_t price;         // in, Цена: 5 байт 0000000000…9999999999
    uint64_t discount;      // in, Скидка: 5 байт 0000000000…9999999999
    uint64_t charge;        // in, Надбавка: 5 байт 0000000000…9999999999
    uint8_t department;     // in, Номер отдела: 1 байт
    uint8_t tax;            // in, Налог: 1 байт
    uint64_t barcode;       // in, Штрих-код: 5 байт 000000000000…999999999999
    QString text;           // in, название товара и скидки. Названия товара и
};                           // in, скидки должны заканчиваться нулём

struct FSReadOpenParam{
    uint8_t docNumber;      // in, Порядковый номер отчета о регистрации/перерегистрации: 1 байт
    uint16_t tagId;         // in, Номер тега (Тип Т, TLV параметра): 2 байта (если T=FFFFh2, то читать TLV структуру командой FF3Bh)
    QByteArray data;        // out, TLV структура: X1 байт
};

struct FSReadRegisters{
    uint16_t itemCode;          // in, Код товара (2 байта) 1…9999
    int resultCode;         // out, Код ошибки: 1 байт
    uint8_t operatorNumber;     // out, Порядковый номер оператора (1 байт) 1…30
    uint64_t registerValue[8];  // out, Значения регистров
};

struct FSBufferStatus{
    uint16_t dataSize;      // out, Количество байт в буфере: 2 байта 0 – нет данных
    uint8_t blockSize;      // out, Максимальный размер блока данных: 1 байт
};

struct FSBufferBlock{
    uint16_t offset;
    uint8_t size;
    QByteArray data;
};

struct FSStartWriteBuffer {
    uint16_t size;          // in, Длина данных
    int resultCode;     // out, Код ошибки (1 байт)
    uint16_t blockSize;     // out, Размер блока
};

struct FSWriteBuffer{
    uint16_t offset;
    QByteArray block;
    int resultCode;     // out, Код ошибки (1 байт)
};

struct FSRefiscalization{
    uint8_t code;           // in, Код причины перерегистрации: 1 байт
    int resultCode;     // out, Код ошибки (1 байт)
    uint32_t docNum;        // out, Номер ФД: 4 байта
    uint32_t docMac;        // out, Фискальный признак: 4 байта
};

struct FSPrintCorrection{
    uint64_t total;         // in, Итог чека: 5 байт 0000000000…9999999999
    uint8_t operation;      // in, Тип операции 1 байт
    int resultCode;     // out, Код ошибки (1 байт)
    uint16_t recNum;        // out, Порядковый номер оператора (1 байт) 1…30
    uint32_t docNum;        // out, Номер ФД: 4 байта
    uint32_t docMac;        // out, Фискальный признак: 4 байта
};

struct FSPrintCorrection2{
    uint8_t correctionType; // in, Тип коррекции : 1 байт
    uint8_t paymentType;    // in, Признак расчета: 1байт
    uint64_t total;         // in, Сумма расчёта :5 байт
    uint64_t payment[5];    // in, Сумма по чеку наличными:5 байт
    uint64_t taxAmount[5];  // in, Сумма налога: 5 байт
    uint8_t taxSystem;      // in, Применяемая система налогообложения:1байт
    int resultCode;     // out, Код ошибки (1 байт)
    uint16_t recNum;        // out,
    uint32_t docNum;        // out, Номер ФД: 4 байта
    uint32_t docMac;        // out, Фискальный признак: 4 байта
};

struct FSCommReport{
    uint32_t docNum;            // out, Номер ФД: 4 байта
    uint32_t docMac;            // out, Фискальный признак: 4 байта
    uint32_t docCount;          // out, Количество неподтверждённых документов: 4 байта
    PrinterDate firstDate;      // out, Дата первого неподтверждённого документа: 3 байта ГГ,ММ,ДД
};

struct FSCommStatus{
    uint8_t flags;
    uint8_t readStatus;
    uint16_t docCount;
    uint32_t firstNumber;
    PrinterDate firstDate;
    PrinterTime firstTime;
};

struct FSDocumentInfo{
    uint32_t docNum;    // in, Номер фискального документа: 4 байта
    uint16_t docType;   // out, Тип фискального документа: 2 байта STLV
    uint16_t docSize;   // out, Длина фискального документа: 2 байта
};

struct FSTicket{
    uint32_t docNum;    // in, Номер фискального документа: 4 байта
    QByteArray ticket;  // out, Квитанция: N байт
};

struct FSDocument{
    uint32_t docNum;
    uint32_t docMac;
};

struct FSDayStatus{
    uint8_t status;
    uint16_t dayNumber;
    uint16_t recNumber;
};

struct FSCloseDay{
    uint16_t dayNumber;         // out, Номер только что закрытой смены: 2 байта
    uint32_t docNum;            // out, Номер ФД: 4 байта
    uint32_t docMac;            // out, Фискальный признак: 4 байта
};

struct FSSale{
    uint8_t operation;  // in, Тип операции: 1 байт
    uint64_t quantity;  // in, Количество: 5 байт 0000000000…9999999999
    uint64_t price;     // in, Цена: 5 байт 0000000000…9999999999
    uint64_t discount;  // in, Скидка: 5 байт 0000000000…9999999999
    uint64_t charge;    // in, Надбавка: 5 байт 0000000000…9999999999
    uint64_t taxAmount; // in, Налог: 5 байт 0000000000…9999999999
    uint8_t department; // in, Номер отдела: 1 байт
    uint8_t tax;        // in, Налог: 1 байт
    uint64_t barcode;   // in, Штрих-код: 5 байт 000000000000…999999999999
    QString text;       // in, Текст
};

struct FSSale2{
    uint8_t operation;          // in, Тип операции: 1 байт
    uint64_t quantity;          // in, Количество: 5 байт 0000000000…9999999999
    uint64_t price;             // in, Цена: 5 байт 0000000000…9999999999
    uint64_t amount;            // in, Сумма: 5 байт 0000000000…9999999999
    uint64_t taxAmount;         // in, Налог: 5 байт 0000000000…9999999999
    uint8_t tax;                // in, Налог: 1 байт
    uint8_t department;         // in, Номер отдела: 1 байт
    PaymentType paymentType;    // in,
    PaymentItem paymentItem;    // in,
    QString text;               // in, Текст
};

struct FSCloseReceipt{
    uint64_t payments[16];  // in, Сумма типа оплаты (5 байт)
    uint8_t roundAmount;    // in, Скидка/Надбавка
    uint64_t taxAmount[6];  // in, Сумма налога
    uint8_t taxSystem;      // in, Система налогообложения ( 1 байт)
    QString text;           // in, Текст
    uint64_t change;        // out, Сдача (5 байт)
    uint32_t docNum;        // Номер ФД :4 байта
    uint32_t docMac;        // Фискальный признак: 4 байта
    PrinterDate date;
    PrinterTime time;       // Дата и время: 5 байт DATE_TIME3
};

struct FSDiscountCharge{
    uint64_t discount;  // in, Скидка: 5 байт
    uint64_t charge;    // in, Надбавка: 5 байт
    uint8_t tax;        // in, Налог: 1 байт
    QString text;       // in, Описание скидки или надбавки: 128 байт ASCII
};

struct PrinterParametersFlags {
  bool capJrnNearEndSensor;   // 0 – Весовой датчик контрольной ленты
  bool capRecNearEndSensor;   // 1 – Весовой датчик чековой ленты
  bool capJrnEmptySensor;     // 2 – Оптический датчик контрольной ленты
  bool capRecEmptySensor;     // 3 – Оптический датчик чековой ленты
  bool capCoverSensor;        // 4 – Датчик крышки
  bool capJrnLeverSensor;     // 5 – Рычаг термоголовки контрольной ленты
  bool capRecLeverSensor;     // 6 – Рычаг термоголовки чековой ленты
  bool capSlpNearEndSensor;   // 7 – Верхний датчик подкладного документа
  bool capSlpEmptySensor;     // 8 – Нижний датчик подкладного документа
  bool capPresenter;          // 9 – Презентер поддерживается
  bool capPresenterCommands;  // 10 – Поддержка команд работы с презентером
  bool capEJNearFull;         // 11 – Флаг заполнения ЭКЛЗ
  bool capEJ;                 // 12 – ЭКЛЗ поддерживается
  bool capCutter;             // 13 – Отрезчик поддерживается
  bool capDrawerStateAsPaper; // 14 – Состояние ДЯ как датчик бумаги в презентере
  bool capDrawerSensor;       // 15 – Датчик денежного ящика
  bool capPrsInSensor;        // 16 – Датчик бумаги на входе в презентер
  bool capPrsOutSensor;       // 17 – Датчик бумаги на выходе из презентера
  bool capBillAcceptor;       // 18 – Купюроприемник поддерживается
  bool capTaxKeyPad;          // 19 – Клавиатура НИ поддерживается
  bool capJrnPresent;         // 20 – Контрольная лента поддерживается
  bool capSlpPresent;         // 21 – Подкладной документ поддерживается
  bool capNonfiscalDoc;       // 22 – Поддержка команд нефискального документа
  bool capCashCore;           // 23 – Поддержка протокола Кассового Ядра (cashcore)
  bool capInnLeadingZero;     // 24 – Ведущие нули в ИНН
  bool capRnmLeadingZero;     // 25 – Ведущие нули в РНМ
  bool SwapGraphicsLine;      // 26 – Переворачивать байты при печати линии
  bool capTaxPasswordLock;    // 27 – Блокировка ККТ по неверному паролю налогового инспектора
  bool capProtocol2;          // 28 – Поддержка альтернативного нижнего уровня протокола ККТ
  bool capLFInPrintText;      // 29 – Поддержка переноса строк символом '\n' (код 10) в командах печати строк 12H, 17H, 2FH
  bool capFontInPrintText;  // 30 – Поддержка переноса строк номером шрифта (коды 1…9) в команде печати строк 2FH
  bool capLFInFiscalCommands; // 31 – Поддержка переноса строк символом '\n' (код 10) в фискальных командах 80H…87H, 8AH, 8BH
  bool capFontInFiscalCommands; // 32 – Поддержка переноса строк номером шрифта (коды 1…9) в фискальных командах 80H…87H, 8AH, 8BH
  bool capTopCashierReports;   // 33 – Права "СТАРШИЙ КАССИР" (28) на снятие отчетов: X, операционных регистров, по отделам, по налогам, по кассирам, почасового, по товарам
  bool capSlpInPrintCommands;      // 34 – Поддержка Бит 3 "слип чек" в командах печати: строк 12H, 17H, 2FH,расширенной графики 4DH, C3H, графической линии C5H; поддержка
  bool capGraphicsC4;           // 35 – Поддержка блочной загрузки графики в команде C4H
  bool capCommand6B;            // 36 – Поддержка команды 6BH "Возврат названия ошибоки"
  bool capFlagsGraphicsEx;      // 37 – Поддержка флагов печати для команд печати расширенной графики C3H и печати графической линии C5H
  bool capMFP;                  // 39 – Поддержка МФП
  bool capEJ5;                  // 40 – Поддержка ЭКЛЗ5
  bool capScaleGraphics;        // 41 – Печать графики с масштабированием (команда 4FH)
  bool capGraphics512;          // 42 – Загрузка и печать графики-512 (команды 4DH, 4EH)
  bool capFS;                   // 43 – поддержка фискального накопителя
  bool capEoD;                  // 44 – поддержка режима передачи данных через драйвер (EoD)
};

struct ModelParameters {
  uint64_t flagsValue;
  uint8_t Font1Width;
  uint8_t Font2Width;
  uint8_t GraphicsStartLine;
  uint8_t InnDigits;
  uint8_t RnmDigits;
  uint8_t LongRnmDigits;
  uint8_t LongSerialDigits;
  uint32_t DefTaxPassword;
  uint32_t DefSysPassword;
  uint8_t BluetoothTable;
  uint8_t TaxFieldNumber;
  uint16_t MaxCommandLength;
  uint8_t GraphicsWidthInBytes;
  uint8_t Graphics512WidthInBytes;
  uint16_t Graphics512MaxHeight;
  PrinterParametersFlags flags;
};

class PrinterField
{
private:
    int Id;
    uint16_t row;
    QString value;
    PrinterFieldInfo info;
public:
    PrinterField(){
    }

    PrinterField(PrinterFieldInfo info){
        this->info = info;
    }

    void setInfo(PrinterFieldInfo info){
        this->info = info;
    }

    void setId(int value){
        Id = value;
    }

    int getId(){
        return Id;
    }

    uint16_t getRow(){
        return row;
    }

    PrinterFieldInfo getInfo(){
        return info;
    }

    QString getValue(){
        return value;
    }

    void setValue(QString value){
        this->value = value;
    }

    bool isString()
    {
        return getInfo().type == FIELD_TYPE_STR;
    }

    void setBinary(QByteArray data)
    {
        if (isString())
        {
            while (data.length() > 0)
            {
                int len = data.length();
                if (data.at(len-1) != 0) break;
                data.remove(len-1, 1);
            }
            PrinterCommand command(0);
            value = command.from1251(data);
        } else
        {
            uint64_t ivalue = 0;
            for (uint i = 0; i < info.size; i++)
            {
                ivalue += data[i] << (i*8);
            }
            value = QString::number(ivalue);
        }
    }

    QByteArray getBinary()
    {
        if (isString())
        {
            PrinterCommand command(0);
            return command.to1251(value);
        } else
        {
             QByteArray result;
             uint64_t ivalue = static_cast<uint64_t>(value.toLongLong());
             for (int i = 0; i < info.size; i++)
             {
                 result.append(static_cast<uint8_t> ((ivalue >> i * 8) & 0xFF));
             }

             return result;
        }
     }

};

class PrinterFields
{
private:
    QString modelName;
    QVector<PrinterField> items;
public:
    PrinterFields(){
    }

    int size(){
        return items.size();
    }

    void clear(){
        items.clear();
    }

    bool find(int table, int field, PrinterField& printerField)
    {
        for (int i=0;i<size();i++)
        {
            PrinterField item = get(i);
            PrinterFieldInfo fieldInfo = item.getInfo();
            if ((fieldInfo.table == table)&&(fieldInfo.field == field)){
                printerField = item;
                return true;
            }
        }
        return false;
    }

    void add(PrinterField field){
        items.append(field);
    }

    PrinterField get(int index){
        return items[index];
    }

    void setModelName(QString value){
        modelName = value;
    }

    QString getModelName(){
        return modelName;
    }
};

class PrinterTable
{
private:
    PrinterFields fields;
    PrinterTableInfo info;
public:
    PrinterTable(){
    }

    PrinterTable(PrinterTableInfo info){
        this->info = info;
    }

    PrinterFields getFields(){
        return fields;
    }
    PrinterTableInfo getInfo(){
        return info;
    }
    QString getName(){
        return info.name;
    }
    int getNumber(){
        return info.number;
    }
    int getFieldCount(){
        return info.fieldCount;
    }
    int getRowCount(){
        return info.rowCount;
    }
};

class PrinterTables
{
private:
    QVector<PrinterTable> items;
public:
    PrinterTables(){}

    int size(){
        return items.size();
    }

    void clear(){
        items.clear();
    }

    void add(PrinterTable table){
        items.append(table);
    }
    PrinterTable get(int index){
        return items[index];
    }
};

class CsvTablesReader{
private:
    bool isComment(QString line);
    qlonglong getParamInt(QString line, int index);
    uint getParamUInt(QString line, int index);
    uint8_t getParamUInt8(QString line, int index);
    uint16_t getParamUInt16(QString line, int index);
    QString getParamStr(QString line, int index);
public:
    CsvTablesReader(){}
    void load(QString fileName, PrinterFields fields);
};

#define EVENT_BEFORE 0
#define EVENT_AFTER  1

class IPrinterFilter
{
public:
    virtual ~IPrinterFilter() {}
    virtual void startDump(uint8_t event, StartDumpCommand& data)=0;
    virtual void readDump(uint8_t event, ReadDumpCommand& data)=0;
    virtual void stopDump(uint8_t event, StopDumpCommand& data)=0;
    virtual void fiscalization(uint8_t event, FiscalizationCommand& data)=0;
    virtual void fiscalizationLong(uint8_t event, FiscalizationCommand& data)=0;
    virtual void writeLongSerial(uint8_t event, WriteLongSerialCommand& data)=0;
    virtual void readLongSerial(uint8_t event, ReadLongSerialCommand& data)=0;
    virtual void readShortStatus(uint8_t event, ReadShortStatusCommand& data)=0;
    virtual void readLongStatus(uint8_t event, ReadLongStatusCommand& data)=0;
    virtual void printBold(uint8_t event, PrintBoldCommand& data)=0;
    virtual void beep(uint8_t event, BeepCommand& data)=0;
    virtual void writePortParameters(uint8_t event, PortParametersCommand& data)=0;
    virtual void readPortParameters(uint8_t event, PortParametersCommand& data)=0;
    virtual void resetMemory(uint8_t event)=0;
    virtual void printString(uint8_t event, PrintStringCommand& data)=0;
    virtual void printDocHeader(uint8_t event, PrintDocHeaderCommand& data)=0;
    virtual void startTest(uint8_t event, StartTestCommand& data)=0;
    virtual void readRegister(uint8_t event, ReadCashRegCommand& data)=0;
    virtual void readRegister(uint8_t event, ReadOperRegCommand& data)=0;
    virtual void writeLicense(uint8_t event, LicenseCommand& data)=0;
    virtual void readLicense(uint8_t event, LicenseCommand& data)=0;
    virtual void writeTable(uint8_t event, TableValueCommand& data)=0;
    virtual void readTable(uint8_t event, TableValueCommand& data)=0;
    virtual void writePoint(uint8_t event, PointCommand& command)=0;
    virtual void writeTime(uint8_t event, TimeCommand& data)=0;
    virtual void writeDate(uint8_t event, DateCommand& data)=0;
    virtual void confirmDate(uint8_t event, DateCommand& data)=0;
    virtual void cutPaper(uint8_t event, CutCommand& data)=0;
    virtual void readFont(uint8_t event, ReadFontCommand& data)=0;
    virtual void openDrawer(uint8_t event, OpenDrawerCommand& data)=0;
    virtual void feedPaper(uint8_t event, FeedPaperCommand& data)=0;
    virtual void stopTest(uint8_t event, PasswordCommand& data)=0;
    virtual void printStringFont(uint8_t event, PrintStringFontCommand& data)=0;
    virtual void printXReport(uint8_t event, PasswordCommand& data)=0;
    virtual void printZReport(uint8_t event, PasswordCommand& data)=0;
    virtual void printDepartmentReport(uint8_t event, PasswordCommand& data)=0;
    virtual void printTaxReport(uint8_t event, PasswordCommand& data)=0;
    virtual void printCashIn(uint8_t event, CashCommand& data)=0;
    virtual void printCashOut(uint8_t event, CashCommand& data)=0;
    virtual void printHeader(uint8_t event, PasswordCommand& data)=0;
    virtual void printDocEnd(uint8_t event, PrintDocEndCommand& data)=0;
    virtual void printSale(uint8_t event, ReceiptItemCommand& data)=0;
    virtual void printSale(uint8_t event, FSSale& data)=0;
    virtual void printSale(uint8_t event, FSSale2& data)=0;
    virtual void printBuy(uint8_t event, ReceiptItemCommand& data)=0;
    virtual void printRetSale(uint8_t event, ReceiptItemCommand& data)=0;
    virtual void printRetBuy(uint8_t event, ReceiptItemCommand& data)=0;
    virtual void printTrailer(uint8_t event, PasswordCommand& data)=0;
    virtual void printStorno(uint8_t event, ReceiptItemCommand& data)=0;
    virtual void closeReceipt(uint8_t event, CloseReceiptCommand& data)=0;
    virtual void printDiscount(uint8_t event, AmountAjustCommand& data)=0;
    virtual void printCharge(uint8_t event, AmountAjustCommand& data)=0;
    virtual void cancelReceipt(uint8_t event, PasswordCommand& data)=0;
    virtual void readSubtotal(uint8_t event, ReadSubtotalCommand& data)=0;
    virtual void printDiscountStorno(uint8_t event, AmountAjustCommand& data)=0;
    virtual void printChargeStorno(uint8_t event, AmountAjustCommand& data)=0;
    virtual void printCopy(uint8_t event, PasswordCommand& data)=0;
    virtual void openReceipt(uint8_t event, OpenReceiptCommand& data)=0;
    virtual void continuePrint(uint8_t event, PasswordCommand& data)=0;
    virtual void printBarcode(uint8_t event, PrintBarcodeCommand& data)=0;
    virtual void printGraphics(uint8_t event, PrintGraphicsCommand& data)=0;
    virtual void printGraphics1(uint8_t event, PrintGraphicsCommand& data)=0;
    virtual void printGraphics2(uint8_t event, PrintGraphicsCommand& data)=0;
    virtual void loadGraphics(uint8_t event, LoadGraphicsCommand& data)=0;
    virtual void loadGraphics1(uint8_t event, LoadGraphicsCommand& data)=0;
    virtual void loadGraphics2(uint8_t event, LoadGraphicsCommand& data)=0;
    virtual void printGraphicsLine(uint8_t event, GraphicsLineCommand& data)=0;
    virtual void openDay(uint8_t event, PasswordCommand& data)=0;
    virtual void readDeviceType(uint8_t event, DeviceTypeCommand& data)=0;
    virtual void printScaledGraphics(uint8_t event, ScaledGraphicsCommand& data)=0;
    virtual void softReset(uint8_t event)=0;
    virtual void initTables(uint8_t event, PasswordCommand& data)=0;
    virtual void slipEject(uint8_t event, SlipEjectCommand& data)=0;
    virtual void printRegisters(uint8_t event, PasswordCommand& data)=0;
    virtual void readTableInfo(uint8_t event, TableInfoCommand& data)=0;
    virtual void readFieldInfo(uint8_t event, FieldInfoCommand& data)=0;
    virtual void writeSerial(uint8_t event, WriteSerialCommand& data)=0;
    virtual void resetFM(uint8_t event, int& resultCode)=0;
    virtual void fmReadTotals(uint8_t event, FMReadTotalsCommand& data)=0;
    virtual void fsReadTotals(uint8_t event, FSReadTotalsCommand& data)=0;
    virtual void fsReadDocCount(uint8_t event, int& docCount)=0;

    virtual void fmReadLastDate(uint8_t event, FMReadLastDateCommand& data)=0;
    virtual void fmReadRange(uint8_t event, FMReadRangeCommand& data)=0;
    virtual void fmDatesReport(uint8_t event, FMDatesReportCommand& data)=0;
    virtual void fmDaysReport(uint8_t event, FMDaysReportCommand& data)=0;
    virtual void fmCancelReport(uint8_t event, PasswordCommand& data)=0;
    virtual void fmReadFiscalization(uint8_t event, FMReadFiscalizationCommand& data)=0;
    virtual void slipOpenDocument(uint8_t event, SlipOpenDocumentCommand& data)=0;
    virtual void slipOpenStdDocument(uint8_t event, SlipOpenStdDocumentCommand& data)=0;
    virtual void slipPrintItem(uint8_t event, SlipPrintItemCommand& data)=0;
    virtual void loadGraphics3(uint8_t event, LoadGraphics3Command& data)=0;
    virtual void printGraphics3(uint8_t event, PrintGraphics3Command& data)=0;
    virtual void writeParameter(uint8_t event, int ParamId, QString value)=0;
    virtual void writeTableStr(uint8_t event, int table, int row, int field, QString value)=0;
};

class PrinterFilter: public IPrinterFilter
{
public:
    PrinterFilter(){}
    virtual ~PrinterFilter(){}

    virtual void startDump(uint8_t event, StartDumpCommand& data)
    {
        (void)event;
        (void)data;
    }

    virtual void readDump(uint8_t event, ReadDumpCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void stopDump(uint8_t event, StopDumpCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void fiscalization(uint8_t event, FiscalizationCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void fiscalizationLong(uint8_t event, FiscalizationCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void writeLongSerial(uint8_t event, WriteLongSerialCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void readLongSerial(uint8_t event, ReadLongSerialCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void readShortStatus(uint8_t event, ReadShortStatusCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void readLongStatus(uint8_t event, ReadLongStatusCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printBold(uint8_t event, PrintBoldCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void beep(uint8_t event, BeepCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void writePortParameters(uint8_t event, PortParametersCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void readPortParameters(uint8_t event, PortParametersCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void resetMemory(uint8_t event)    {
        (void)event;
    }

    virtual void printString(uint8_t event, PrintStringCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printDocHeader(uint8_t event, PrintDocHeaderCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void startTest(uint8_t event, StartTestCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void readRegister(uint8_t event, ReadCashRegCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void readRegister(uint8_t event, ReadOperRegCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void writeLicense(uint8_t event, LicenseCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void readLicense(uint8_t event, LicenseCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void writeTable(uint8_t event, TableValueCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void readTable(uint8_t event, TableValueCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void writePoint(uint8_t event, PointCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void writeTime(uint8_t event, TimeCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void writeDate(uint8_t event, DateCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void confirmDate(uint8_t event, DateCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void cutPaper(uint8_t event, CutCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void readFont(uint8_t event, ReadFontCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void openDrawer(uint8_t event, OpenDrawerCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void feedPaper(uint8_t event, FeedPaperCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void stopTest(uint8_t event, PasswordCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printStringFont(uint8_t event, PrintStringFontCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printXReport(uint8_t event, PasswordCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printZReport(uint8_t event, PasswordCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printDepartmentReport(uint8_t event, PasswordCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printTaxReport(uint8_t event, PasswordCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printCashIn(uint8_t event, CashCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printCashOut(uint8_t event, CashCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printHeader(uint8_t event, PasswordCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printDocEnd(uint8_t event, PrintDocEndCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printSale(uint8_t event, ReceiptItemCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printSale(uint8_t event, FSSale& data){
        (void)event;
        (void)data;
    }

    virtual void printSale(uint8_t event, FSSale2& data){
        (void)event;
        (void)data;
    }

    virtual void printBuy(uint8_t event, ReceiptItemCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printRetSale(uint8_t event, ReceiptItemCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printRetBuy(uint8_t event, ReceiptItemCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printTrailer(uint8_t event, PasswordCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printStorno(uint8_t event, ReceiptItemCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void closeReceipt(uint8_t event, CloseReceiptCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printDiscount(uint8_t event, AmountAjustCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printCharge(uint8_t event, AmountAjustCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void cancelReceipt(uint8_t event, PasswordCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void readSubtotal(uint8_t event, ReadSubtotalCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printDiscountStorno(uint8_t event, AmountAjustCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printChargeStorno(uint8_t event, AmountAjustCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printCopy(uint8_t event, PasswordCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void openReceipt(uint8_t event, OpenReceiptCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void continuePrint(uint8_t event, PasswordCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printBarcode(uint8_t event, PrintBarcodeCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printGraphics(uint8_t event, PrintGraphicsCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printGraphics1(uint8_t event, PrintGraphicsCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printGraphics2(uint8_t event, PrintGraphicsCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void loadGraphics(uint8_t event, LoadGraphicsCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void loadGraphics1(uint8_t event, LoadGraphicsCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void loadGraphics2(uint8_t event, LoadGraphicsCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printGraphicsLine(uint8_t event, GraphicsLineCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void openDay(uint8_t event, PasswordCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void readDeviceType(uint8_t event, DeviceTypeCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printScaledGraphics(uint8_t event, ScaledGraphicsCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void softReset(uint8_t event)    {
        (void)event;
    }

    virtual void initTables(uint8_t event, PasswordCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void slipEject(uint8_t event, SlipEjectCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void printRegisters(uint8_t event, PasswordCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void readTableInfo(uint8_t event, TableInfoCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void readFieldInfo(uint8_t event, FieldInfoCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void writeSerial(uint8_t event, WriteSerialCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void resetFM(uint8_t event, int& resultCode)    {
        (void)event;
        (void)resultCode;
    }

    virtual void fmReadTotals(uint8_t event, FMReadTotalsCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void fsReadTotals(uint8_t event, FSReadTotalsCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void fsReadDocCount(uint8_t event, int& docCount)    {
        (void)event;
        (void)docCount;
    }


    virtual void fmReadLastDate(uint8_t event, FMReadLastDateCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void fmReadRange(uint8_t event, FMReadRangeCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void fmDatesReport(uint8_t event, FMDatesReportCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void fmDaysReport(uint8_t event, FMDaysReportCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void fmCancelReport(uint8_t event, PasswordCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void fmReadFiscalization(uint8_t event, FMReadFiscalizationCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void slipOpenDocument(uint8_t event, SlipOpenDocumentCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void slipOpenStdDocument(uint8_t event, SlipOpenStdDocumentCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void slipPrintItem(uint8_t event, SlipPrintItemCommand& data)    {
        (void)event;
        (void)data;
    }

    virtual void loadGraphics3(uint8_t event, LoadGraphics3Command& data)    {
        (void)event;
        (void)data;
    }

    virtual void printGraphics3(uint8_t event, PrintGraphics3Command& data)    {
        (void)event;
        (void)data;
    }

    virtual void writeParameter(uint8_t event, int ParamId, QString value)  {
        (void)event;
        (void)ParamId;
        (void)value;
    }
    virtual void writeTableStr(uint8_t event, int table, int row,
        int field, QString value){
        (void)event;
        (void)table;
        (void)row;
        (void)field;
        (void)value;
    }
};

class ShtrihFiscalPrinter : public QObject {
    Q_OBJECT
public:
    explicit ShtrihFiscalPrinter(QObject* parent, Logger* logger);
    virtual ~ShtrihFiscalPrinter();
    void setTimeout(uint value);
    bool getJournalEnabled();
    void setJournalEnabled(bool value);
    int send(PrinterCommand& command);
    int connectDevice();
    void disconnectDevice();
    bool succeeded(int rc);
    bool failed(int rc);
    bool isShtrihMobile();
    int startDump(StartDumpCommand& data);
    int readDump(ReadDumpCommand& data);
    int stopDump(StopDumpCommand& data);
    int fiscalization(FiscalizationCommand& data);
    int fiscalizationLong(FiscalizationCommand& data);
    int writeLongSerial(WriteLongSerialCommand& data);
    int readLongSerial(ReadLongSerialCommand& data);
    int readShortStatus(ReadShortStatusCommand& data);
    int readLongStatus(ReadLongStatusCommand& data);
    int printBold(PrintBoldCommand& data);
    int beep(BeepCommand& data);
    int writePortParameters(PortParametersCommand& data);
    int readPortParameters(PortParametersCommand& data);
    int resetMemory();
    int printString(PrintStringCommand& data);
    int printDocHeader(PrintDocHeaderCommand& data);
    int startTest(StartTestCommand& data);
    int readRegister(ReadCashRegCommand& data);
    int readRegister(ReadOperRegCommand& data);
    int writeLicense(LicenseCommand& data);
    int readLicense(LicenseCommand& data);
    int writeTable(TableValueCommand& data);
    int readTable(TableValueCommand& data);
    int writePoint(PointCommand& command);
    int writeTime(TimeCommand& data);
    int writeDate(DateCommand& data);
    int confirmDate(DateCommand& data);
    int cutPaper(CutCommand& data);
    int readFont(ReadFontCommand& data);
    int openDrawer(OpenDrawerCommand& data);
    int feedPaper(FeedPaperCommand& data);
    int stopTest(PasswordCommand& data);
    int printStringFont(PrintStringFontCommand& data);
    int printXReport(PasswordCommand& data);
    int printZReport(PasswordCommand& data);
    int printDepartmentReport(PasswordCommand& data);
    int printTaxReport(PasswordCommand& data);
    int printCashierReport(PasswordCommand& data);
    int printHourReport(PasswordCommand& data);
    int printItemsReport(PasswordCommand& data);
    int printCashIn(CashCommand& data);
    int printCashOut(CashCommand& data);
    int printHeader(PasswordCommand& data);
    int printDocEnd(PrintDocEndCommand& data);
    int execute(uint16_t code, ReceiptItemCommand& data);
    int printSale(ReceiptItemCommand& data);
    int printBuy(ReceiptItemCommand& data);
    int printRetSale(ReceiptItemCommand& data);
    int printRetBuy(ReceiptItemCommand& data);
    int printTrailer(PasswordCommand& data);
    int printStorno(ReceiptItemCommand& data);
    int closeReceipt(CloseReceiptCommand& data);
    int closeReceipt2(CloseReceiptCommand2& data);
    int printDiscount(AmountAjustCommand& data);
    int printCharge(AmountAjustCommand& data);
    int printAmountAjustment(uint16_t code, AmountAjustCommand& data);
    int cancelReceipt(PasswordCommand& data);
    int readSubtotal(ReadSubtotalCommand& data);
    int printDiscountStorno(AmountAjustCommand& data);
    int printChargeStorno(AmountAjustCommand& data);
    int printCopy(PasswordCommand& data);
    int openReceipt(OpenReceiptCommand& data);
    int continuePrint(PasswordCommand& data);
    int printBarcode(PrintBarcodeCommand& data);
    int printGraphics(PrintGraphicsCommand& data);
    int printGraphics1(PrintGraphicsCommand& data);
    int printGraphics2(PrintGraphicsCommand& data);
    int loadGraphics(LoadGraphicsCommand& data);
    int loadGraphics1(LoadGraphicsCommand& data);
    int loadGraphics2(LoadGraphicsCommand& data);
    int printGraphicsLine(GraphicsLineCommand& data);
    int openDay(PasswordCommand& data);
    int readDeviceType(DeviceTypeCommand& data);
    int readModelParameters(ModelParameters& data);
    int printScaledGraphics(ScaledGraphicsCommand& data);
    int softReset();
    int initTables(PasswordCommand& data);
    int slipEject(SlipEjectCommand& data);
    int printRegisters(PasswordCommand& data);
    int readTableInfo(TableInfoCommand& data);
    int readFieldInfo(FieldInfoCommand& data);
    int writeSerial(WriteSerialCommand& data);
    int resetFM(int& resultCode);
    int fmReadTotals(FMReadTotalsCommand& data);
    int fsReadTotals(FSReadTotalsCommand& data);
    int fsReadDocCount(uint16_t& docCount);

    int readTotals(PrinterTotals& data);
    int fmReadLastDate(FMReadLastDateCommand& data);
    int fmReadRange(FMReadRangeCommand& data);
    int fmDatesReport(FMDatesReportCommand& data);
    int fmDaysReport(FMDaysReportCommand& data);
    int fmCancelReport(PasswordCommand& data);
    int fmReadFiscalization(FMReadFiscalizationCommand& data);
    int fmReadCorruptedRecords(FMReadCorruptedRecordsCommand& data);
    int readErrorText(ReadErrorTextCommand& data);
    void write(PrinterCommand& command, SlipDocParams data);
    void write(PrinterCommand& command, StdSlipParams data);
    void write(PrinterCommand& command, SlipOperationParams data);
    void write(PrinterCommand& command, SlipOperation data);
    int slipOpenDocument(SlipOpenDocumentCommand& data);
    int slipOpenStdDocument(SlipOpenStdDocumentCommand& data);
    int slipPrintItem(SlipPrintItemCommand& data);
    int loadGraphics3(LoadGraphics3Command& data);
    int printGraphics3(PrintGraphics3Command& data);

    QString getModeText(int value);
    QString getSubmodeText(int value);
    static QString getErrorText2(uint8_t code);
    static QString getErrorText(int code);
    static QString getErrorTextFS(int code);
    static QString getFSErrorText(int code);

    QString getDeviceCodeText(int code);
    uint8_t getTimeoutCode(uint value);
    uint getTimeoutValue(uint8_t value);
    uint getBaudRateValue(uint8_t value);
    uint8_t getBaudRateCode(uint value);
    void setProtocol(PrinterProtocol* protocol);
    int waitForPrinting();
    void checkEcrMode(int mode);
    int printBarcode(PrinterBarcode barcode);
    int loadImage(const QString& fileName);
    int loadImage(QImage& image);
    int printImage(QImage image);
    int printImage(uint imageIndex);
    int printImage(PrinterImage image);
    int printImage(uint16_t startLine, uint16_t endLine);
    int loadImage1(QImage& image);
    int loadImage2(QImage& image);
    int loadImage3(QImage& image);
    int printImage1(QImage& image);
    int printImage2(QImage& image);
    int printImage3(QImage& image);

    void clearImages();
    uint getImageCount();
    uint16_t getStartLine();
    int getMaxGraphicsWidth();
    int getMaxGraphicsHeight();
    void addImage(PrinterImage image);
    QImage alignImage(QImage image, int width);
    QString getVersion();
    int readLineLength(uint8_t font);
    int getLineLength(uint8_t font);
    int printLine(QString text);
    int printTextSplit(QString& text);
    int printText(QString text);
    QStringList splitText(QString text, uint8_t font);
    int resetPrinter();
    bool isDayOpened();
    void setMaxRetryCount(uint32_t value);

    int openFiscalDay();
    void writeFields(QString fileName);
    void writeFields(PrinterFields fields);
    int writeField(PrinterField field);
    uint16_t readDayNumber();
    uint64_t readCashRegister(uint8_t number);
    uint16_t readOperationRegister(uint8_t number);
    int readTableInt(uint8_t table, uint16_t row, uint8_t field);
    QString readTableStr(uint8_t table, uint16_t row, uint8_t field);
    int writeTableStr(uint8_t table, uint16_t row, uint8_t field, QString value);
    int readTable(uint8_t table, uint16_t row, uint8_t field, QString text);
    void check(int resultCode);
    QStringList readHeader();
    QStringList readTrailer();
    QString readPaymentName(uint8_t number);
    bool readTrailerEnabled();
    QStringList getHeader();
    QStringList getTrailer();

    int printEJDepartmentReportOnDates(PrintEJDepartmentReportOnDates& data);
    int printEJDepartmentReportOnDays(PrintEJDepartmentReportOnDays& data);
    int printEJDayReportOnDates(PrintEJDayReportOnDates& data);
    int printEJDayReportOnDays(PrintEJDayReportOnDays& data);
    int printEJDayReport(PrintEJDayReport& data);
    int printEJDayTotal(PrintEJDayTotal& data);
    int printEJDocument(PrintEJDocument& data);
    int stopEJPrint();
    int printEJActivationReport();
    int activateEJ();
    int closeEJArchive();
    int readEJSerial(ReadEJSerial& data);
    int cancelEJDocument();
    int readEJStatus1(ReadEJStatus1& data);
    int readEJStatus2(ReadEJStatus2& data);
    int readEJVersion(ReadEJVersion& data);
    int testEJArchive();
    int initEJArchive();
    int readEJDocumentLine(ReadEJDocumentLine& data);
    EJFlags decodeEJFlags(uint8_t value);
    int readEJJournal(ReadEJJournal& data);
    int readEJDocument(ReadEJDocument& data);
    int readEJReportByDays(ReadEJReportByDays& data);
    int readEJReportByDates(ReadEJReportByDates& data);
    int readEJDepartmentReportByDays(ReadEJDepartmentReportByDays& data);
    int readEJDepartmentReportByDates(ReadEJDepartmentReportByDates& data);
    int readEJDayTotals(ReadEJDayTotals& data);
    int readEJActivation(ReadEJActivation& data);
    int setEJErrorCode(uint8_t code);
    void startFDOThread();
    void stopFDOThread();
    void sendDocuments();
    bool readBlock(QByteArray& block);
    int writeBlock(QByteArray block);
    void sendBlocks();
    bool sendBlock(QByteArray block, QByteArray& result);
    int getPrinterField(uint8_t table, uint16_t row, uint8_t field, PrinterField& printerField);
    void setPollInterval(uint32_t value);
    void lock();
    void unlock();
    int fsReadStatus(FSStatus& status);
    int fsReadSerial(QString& serial);
    int fsReadExpDate(PrinterDate& date);
    int fsReadVersion(FSVersion& version);
    int fsStartFiscalization(uint8_t reportType);
    int fsReset(uint8_t code);
    int fsCancelDocument();
    int fsReadFiscalization(FSReadFiscalization& data);
    int fsFindDocument(FSFindDocument& data);
    int fsOpenDay(FSOpenDay& data);
    int fsWriteTLV(QByteArray& data);
    int fsWriteTLVOperation(QByteArray& data);
    int fsPrintItem(FSReceiptItem& data);
    int fsReadOpenParam(FSReadOpenParam& data);
    int fsInitEEPROM();
    int fsReadRegisters(FSReadRegisters& data);
    int fsReadBufferStatus(FSBufferStatus& data);
    int fsReadBufferBlock(FSBufferBlock& data);
    int fsStartWriteBuffer(FSStartWriteBuffer& data);
    int fsWriteBuffer(FSWriteBuffer& data);
    int fsPrintRefiscalization(FSRefiscalization& data);
    int fsStartCorrectionReceipt();
    int fsPrintCorrection(FSPrintCorrection& data);
    int fsStartCommReport();
    int fsPrintCommReport(FSCommReport& data);
    int fsReadCommStatus(FSCommStatus& data);
    int fsReadDocument(FSDocumentInfo& data);
    int fsReadDocumentTLV(QByteArray& data);
    int fsReadTicket(FSTicket& data);
    int fsStartClose();
    int fsClose(FSDocument& data);
    int fsReadQueueSize(uint16_t& data);
    int fsReadDayStatus(FSDayStatus& data);
    int fsStartOpenDay();
    int fsStartCloseDay();
    int fsCloseDay(FSCloseDay& data);
    int fsPrintSale(FSSale& data);
    int fsCloseReceipt(FSCloseReceipt& data);
    int fsPrintSale2(FSSale2& data);
    int fsPrintCorrection2(FSPrintCorrection2& data);
    int fsDiscountCharge(FSDiscountCharge& data);
    int fsWriteTag(uint16_t tagId, QString tagValue);
    int fsWriteOperationTag(uint16_t tagId, QString tagValue);
    int printTag(uint16_t tagId, QString tagValue);

    QString PrinterDateToStr(PrinterDate date);
    QString PrinterTimeToStr(PrinterTime time);
    int fsPrintFiscalization(FSPrintFiscalization& data);

    bool getFdoThreadEnabled();
    void setFdoThreadEnabled(bool value);
    void setServerParams(ServerParams value);
    DeviceTypeCommand getDeviceType();
    void beforeCloseReceipt();
    int printLines(QStringList lines);

    void jrnPrintAll();
    void jrnPrintCurrentDay();
    void jrnPrintDay(int dayNumber);
    void jrnPrintDoc(int docNumber);
    void jrnPrintDocRange(int N1, int N2);
    QString readParameter(int ParamId);
    int writeParameter(int ParamId, QString value);

    bool canRepeatCommand(uint16_t commandCode);
    uint32_t getDocumentMac(FSFindDocument doc);
    FSDocument1 decodeDocument1(QByteArray data);
    FSDocument2 decodeDocument2(QByteArray data);
    FSDocument3 decodeDocument3(QByteArray data);
    FSDocument6 decodeDocument6(QByteArray data);
    FSDocument11 decodeDocument11(QByteArray data);
    FSDocument21 decodeDocument21(QByteArray data);
    Logger* getLogger();
    int correctDate();
    int readDate(QDateTime& printerDate);
    int writeDateTime(QDateTime dt);
    int writeCurrentDateTime();
    int readDocument(FSDocumentInfo& doc, QByteArray& data);
    int readDocumentTLV(QByteArray& data);
private:
    Logger* logger;
    QMutex* mutex;
    TlvTags tlvTags;
    QThread* thread;
    IPrinterFilter* filter;
    PrinterProtocol* protocol;
    std::vector<PrinterImage> images;
    PrinterFields fields;
    ServerParams serverParams;
    DeviceTypeCommand deviceType;
    JournalPrinter* journal;

    bool stopFlag;
    bool connected;
    bool fdoThreadEnabled;
    uint timeout;
    uint32_t pollInterval;
    uint32_t maxRetryCount;
    uint32_t usrPassword;
    uint32_t sysPassword;
    uint32_t taxPassword;
    bool isFiscalized;
    bool journalEnabled;
    bool capFiscalStorage;
    bool capLoadGraphics1;
    bool capLoadGraphics2;
    bool capLoadGraphics3;
    bool capPrintGraphics1;
    bool capPrintGraphics2;
    bool capPrintGraphics3;
    uint16_t startLine;
    uint8_t numHeaderRow;
    uint8_t numTrailerRow;
    uint8_t numHeaderLines;
    uint8_t numTrailerLines;
    uint16_t dayNumber;
    QString userName;
    QString fiscalID;
    QString regNumber;
    QString serialNumber;
    QString fsSerialNumber;
    QStringList header;
    QStringList trailer;
public:
    uint sleepTimeInMs;
    bool userNameEnabled;
    bool printTagsEnabled;
    int lineLength[10];
    uint32_t validTimeDiffInSecs;
    PrinterAlignment imageAlignment;

    IPrinterFilter* getFilter(){
        return filter;
    }
    void setFilter(IPrinterFilter* value)
    {
        if (filter){
            delete filter;
        }
        filter = value;
    }

    uint32_t getUsrPassword(){
        return usrPassword;
    }

    void setUsrPassword(uint32_t value){
        usrPassword = value;
    }

    uint32_t getSysPassword(){
        return sysPassword;
    }

    void setSysPassword(uint32_t value){
        sysPassword = value;
    }

    uint32_t getTaxPassword(){
        return taxPassword;
    }

    void setTaxPassword(uint32_t value){
        taxPassword = value;
    }

    JournalPrinter* getJournal(){
        return journal;
    }

};

class FDOThread : public QThread
{
    Q_OBJECT

    void run()
    {
        driver->sendDocuments();
    }
public:
    void setParams(ShtrihFiscalPrinter* driver){
        this->driver = driver;
    }
private:
    ShtrihFiscalPrinter* driver;
};

#endif /* SHTRIHFISCALPRINTER_H */
