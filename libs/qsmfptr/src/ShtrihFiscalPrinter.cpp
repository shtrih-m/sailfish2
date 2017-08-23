/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ShtrihFiscalPrinter.cpp
 * Author: V.Kravtsov
 * 
 * Created on June 28, 2016, 8:01 PM
 */

#include <QFile>
#include <QDebug>
#include <QPainter>
#include <QPixmap>
#include <QByteArray>
#include <vector>

#include "ShtrihFiscalPrinter.h"
#include "ServerConnection.h"
#include "fiscalprinter.h"
#include "tlvtag.h"
#include "utils.h"
#include <qzint.h>
#include <tlvlist.h>

bool CsvTablesReader::isComment(QString line) {
    return line.startsWith("//");
}

int CsvTablesReader::getParamInt(QString line, int index)
{
    return getParamStr(line, index).toLongLong();
}

QString CsvTablesReader::getParamStr(QString line, int index)
{
    QString result = "";
    QStringList	list = line.split(",", QString::KeepEmptyParts, Qt::CaseInsensitive);
    if (list.size() > index)
    {
        result = list[index];
    }
    if (result.startsWith("\"")) {
        result = result.replace(0, 1);
    }
    if (result.endsWith("\"")) {
        if (result.length() > 1) {
            result = result.replace(0, result.length() - 1, 1);
        } else {
            result = "";
        }
    }
    return result;
}


void CsvTablesReader::load(QString fileName, PrinterFields fields)
{
    qDebug() << "CsvTablesReader::load(" << fileName << ")";

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QTextStream stream(&file);
    while(!stream.atEnd()) {
        QString line = stream.readLine();
        if (isComment(line))
        {
            line = line.toUpper();
            /// Модель: ЯРУС-01К; №00001000
            QString modelNameTag = "МОДЕЛЬ: ";
            int beginIndex = line.lastIndexOf(modelNameTag);
            if (beginIndex != -1)
            {
                line = line.remove(0, beginIndex + modelNameTag.length()-1);
                int endIndex = line.lastIndexOf(";");
                if (endIndex != -1){
                    line = line.left(endIndex);
                }
                QString modelName = line;
                fields.setModelName(modelName);
            }
        } else
        {
            PrinterFieldInfo fieldInfo;
            fieldInfo.table = getParamInt(line, 0);
            fieldInfo.row = getParamInt(line, 1);
            fieldInfo.field = getParamInt(line, 2);
            fieldInfo.size = getParamInt(line, 3);
            fieldInfo.type = getParamInt(line, 4);
            fieldInfo.min = getParamInt(line, 5);
            fieldInfo.max = getParamInt(line, 6);
            fieldInfo.name = getParamStr(line, 7);
            QString fieldValue = getParamStr(line, 8);

            PrinterField field;
            field.setInfo(fieldInfo);
            field.setValue(fieldValue);
            fields.add(field);
        }
    }
    file.close();
    qDebug() << "CsvTablesReader::load: OK";
}

ShtrihFiscalPrinter::ShtrihFiscalPrinter(QObject* parent)
    : QObject(parent)
{
    mutex = new QMutex(QMutex::RecursionMode::Recursive);
    taxPassword = 0;
    usrPassword = 1;
    sysPassword = 30;
    startLine = 2;
    timeout = 1000;
    maxRetryCount = 10;
    imageAlignment = PrinterAlignment::Center;
    isFiscalized = false;
    capFiscalStorage = false;
    capLoadGraphics1 = false;
    capLoadGraphics2 = false;
    capLoadGraphics3 = false;
    capPrintGraphics1 = false;
    capPrintGraphics2 = false;
    capPrintGraphics3 = false;
    for (int i = 0; i < 10; i++) {
        lineLength[i] = 0;
    }
    numHeaderRow = 11;
    numTrailerRow = 1;
    numHeaderLines = 4;
    numTrailerLines = 3;
    filter = new PrinterFilter();
    stopFlag = false;
    pollInterval = 1000; // 1000 ms
    fdoThreadEnabled = false;
    printTagsEnabled = true;
    userNameEnabled = true;
    userName = "";
    sleepTimeInMs = 1000;
}

ShtrihFiscalPrinter::~ShtrihFiscalPrinter()
{
    delete mutex;
    delete filter;
}

DeviceTypeCommand ShtrihFiscalPrinter::getDeviceType(){
    return deviceType;
}

void ShtrihFiscalPrinter::setFdoThreadEnabled(bool value){
    fdoThreadEnabled = value;
}

bool ShtrihFiscalPrinter::getFdoThreadEnabled(){
    return fdoThreadEnabled;
}

void ShtrihFiscalPrinter::lock(){
    mutex->lock();
}

void ShtrihFiscalPrinter::unlock(){
    mutex->unlock();
}

void ShtrihFiscalPrinter::setPollInterval(int value){
    this->pollInterval = value;
}

void ShtrihFiscalPrinter::setMaxRetryCount(int value)
{
    this->maxRetryCount = value;
}

void ShtrihFiscalPrinter::setTimeout(int value)
{
    this->timeout = value;
}

void ShtrihFiscalPrinter::setProtocol(PrinterProtocol* protocol)
{
    this->protocol = protocol;
}

bool ShtrihFiscalPrinter::succeeded(int rc)
{
    return (rc == 0);
}

bool ShtrihFiscalPrinter::failed(int rc)
{
    return rc != 0;
}

int ShtrihFiscalPrinter::send(PrinterCommand& command)
{
    int rc = 0;
    for (uint32_t i = 0; i < maxRetryCount; i++) {
        if (i != 0) {
            qDebug() << "retry " << i;
        }

        lock();
        try{
            rc = protocol->send(command);
            unlock();
        }
            catch(QException exception)
        {
            unlock();
            exception.raise();
        }
        if (rc == 0) break;

        qDebug() << "ERROR:  " << getErrorText2(rc);

        switch (rc) {
        case 0x50: {
            QSleepThread::msleep(sleepTimeInMs);
            rc = waitForPrinting();
            if (rc != 0)
                return rc;
            break;
        }

        case 0x58: {
            PasswordCommand command;
            rc = continuePrint(command);
            if (rc != 0)
                return rc;
            rc = waitForPrinting();
            if (rc != 0)
                return rc;
            break;
        }

        default:
            return rc;
        }
    }
    return rc;
}

int ShtrihFiscalPrinter::waitForPrinting()
{
    int rc = 0;
    while (true) {
        ReadShortStatusCommand command;
        rc = readShortStatus(command);
        if (rc != 0)
            break;

        //qDebug() << "mode: " << command.mode;
        //qDebug() << "submode: " << command.submode;

        switch (command.submode) {
        case ECR_SUBMODE_IDLE: {
            switch (command.mode) {
            case MODE_FULLREPORT:
            case MODE_EJREPORT:
            case MODE_SLPPRINT:
                QSleepThread::msleep(sleepTimeInMs);
                break;
            }
            return rc;
        }

        case ECR_SUBMODE_PASSIVE:
        case ECR_SUBMODE_ACTIVE: {
            //rc = checkPaper(status);
            return rc;
        }

        case ECR_SUBMODE_REPORT:
        case ECR_SUBMODE_PRINT: {
            QSleepThread::msleep(sleepTimeInMs);
            break;
        }

        default: {
            break;
        }
        }
    }
    return rc;
}

void ShtrihFiscalPrinter::connectDevice()
{
    protocol->connect();

    check(readDeviceType(deviceType));

    int lineNumber = 1;
    LoadGraphicsCommand loadCommand;
    loadCommand.line = lineNumber;
    capLoadGraphics1 = loadGraphics1(loadCommand) == 0;
    capLoadGraphics2 = loadGraphics2(loadCommand) == 0;

    LoadGraphics3Command load3Command;
    load3Command.bufferType = 1;
    load3Command.lineCount = 1;
    load3Command.lineLength = 40;
    load3Command.startLine = 1;
    capLoadGraphics3 = loadGraphics3(load3Command) == 0;

    PrintGraphicsCommand printCommand;
    printCommand.line1 = lineNumber;
    printCommand.line2 = lineNumber + 1;
    capPrintGraphics1 = printGraphics1(printCommand) == 0;
    capPrintGraphics2 = printGraphics2(printCommand) == 0;

    PrintGraphics3Command command;
    command.startLine = lineNumber;
    command.endLine = lineNumber + 1;
    command.vScale = 1;
    command.hScale = 1;
    command.flags = 2;
    capPrintGraphics2 = printGraphics3(command) == 0;


    if (userNameEnabled && (deviceType.model == 19))
    {
        userName = readTable(14, 1, 7);
    }

    if (fdoThreadEnabled)
    {
        startFDOThread();
    }

    qDebug() << "capLoadGraphics1: " << capLoadGraphics1;
    qDebug() << "capLoadGraphics2: " << capLoadGraphics2;
    qDebug() << "capLoadGraphics3: " << capLoadGraphics3;
    qDebug() << "capPrintGraphics1: " << capPrintGraphics1;
    qDebug() << "capPrintGraphics2: " << capPrintGraphics2;
    qDebug() << "capPrintGraphics3: " << capPrintGraphics3;
}

void ShtrihFiscalPrinter::disconnectDevice()
{
    stopFDOThread();
    protocol->disconnect();
}

void ShtrihFiscalPrinter::startFDOThread()
{
    qDebug() << "startFDOThread";

    // read FDO server parameters
    serverParams.address = readTable(15, 1, 1);
    serverParams.port = readTable(15, 1, 2).toInt();
    pollInterval = readTable(15, 1, 3).toInt()*1000;
    serverParams.connectTimeout = 20000;
    serverParams.readTimeout = 100000;
    serverParams.writeTimeout = 20000;
    serverParams.connectRetries = 1;

    qDebug() << "params.address: " << serverParams.address;
    qDebug() << "params.port: " << serverParams.port;
    qDebug() << "params.readTimeout: " << serverParams.readTimeout;
    qDebug() << "params.writeTimeout: " << serverParams.writeTimeout;
    qDebug() << "params.connectTimeout: " << serverParams.connectTimeout;


    stopFlag = false;
    FDOThread* thread= new FDOThread();
    thread->setParams(this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void ShtrihFiscalPrinter::stopFDOThread()
{
    stopFlag = true;
}

void ShtrihFiscalPrinter::sendDocuments()
{
    qDebug() << "sendDocuments";
    while (!stopFlag)
    {
        try{
            QByteArray block;
            QByteArray answer;
            if (readBlock(block))
            {
                if (sendBlock(block, answer)){
                    writeBlock(answer);
                }
            }
        }
        catch(QException exception)
        {
            qDebug() << "sendDocuments: failed";
        }
        QThread::msleep(pollInterval);
    }
    qDebug() << "sendDocuments: OK";
}

void ShtrihFiscalPrinter::sendBlocks()
{
    QByteArray block;
    QByteArray answer;
    int docCount = 0;
    while ((fsReadDocCount(docCount) == 0)&&(docCount > 0))
    {
        qDebug() << "docCount: " << docCount;
        if (readBlock(block))
        {
            if (sendBlock(block, answer))
            {
                writeBlock(answer);
                QSleepThread::msleep(sleepTimeInMs);
            }
        }
    }
}

bool ShtrihFiscalPrinter::sendBlock(QByteArray block, QByteArray& result)
{
    qDebug() << "sendBlock";

    OFDHeader header;
    connection.setParams(serverParams);
    connection.connect();
    connection.write(block);
    QByteArray readBlock;
    if (!connection.read(sizeof(header), result))
    {
        return false;
    }
    memcpy(&header, result.data(), result.length());
    if (header.size > 0)
    {
        if (!connection.read(header.size, readBlock)){
            return false;
        }
        result.append(readBlock);
    }
    connection.disconnect();
    qDebug() << "sendBlock: OK";
    return true;
}

int ShtrihFiscalPrinter::writeBlock(QByteArray block)
{
    qDebug() << "writeBlock, size: " << block.size();

    int rc = 0;

    lock();
    try
    {
        FSStartWriteBuffer startCommand;
        startCommand.size = block.size();
        int rc = fsStartWriteBuffer(startCommand);
        if (failed(rc)) {
            unlock();
            return rc;
        }

        int blockSize = 64;
        if (startCommand.blockSize > 0){
            blockSize = startCommand.blockSize;
        }
        int count = (block.size() + blockSize -1)/blockSize;
        for (int i=0;i<count;i++)
        {
            int bsize = blockSize;
            if (bsize > block.size()){
                bsize = block.size();
            }

            FSWriteBuffer dataBlock;
            dataBlock.block = block.left(bsize);
            dataBlock.offset = i * blockSize;
            rc = fsWriteBuffer(dataBlock);
            if (failed(rc)) return rc;
            block.remove(0, bsize);
        }
        unlock();
    }
    catch(QException exception)
    {
        unlock();
        exception.raise();
    }
    qDebug() << "writeBlock: OK";
    return rc;
}

bool ShtrihFiscalPrinter::readBlock(QByteArray& block)
{
    qDebug() << "readBlock";

    lock();
    try{
        FSBufferStatus bufferStatus;
        check(fsReadBufferStatus(bufferStatus));
        qDebug() << "bufferStatus.dataSize: " << bufferStatus.dataSize;

        if (bufferStatus.dataSize <= 0) {
            unlock();
            return false;
        }

        int blockSize = 64;
        if ((bufferStatus.blockSize > 0)&&(bufferStatus.blockSize < 240)){
            blockSize = bufferStatus.blockSize;
        }
        int bytesToRead = bufferStatus.dataSize;
        int blockCount = (bufferStatus.dataSize + blockSize-1)/blockSize;
        for (int i=0;i<blockCount;i++)
        {
            FSBufferBlock dataBlock;
            dataBlock.offset = i * blockSize;
            dataBlock.size = blockSize;
            if (bytesToRead < blockSize){
                dataBlock.size = bytesToRead;
            }
            check(fsReadBufferBlock(dataBlock));
            block.append(dataBlock.data);
            bytesToRead -= blockSize;
        }
        unlock();
    }
    catch(QException exception)
    {
        unlock();
        exception.raise();
    }
    qDebug() << "readBlock: OK";
    return true;
}



/*****************************************************************************
Запрос дампа
Команда:	01H. Длина сообщения: 6 байт.
Пароль ЦТО или пароль системного администратора, 
если пароль ЦТО не был установлен (4 байта)
Код устройства (1 байт)
    01 – накопитель ФП 1
    02 – накопитель ФП 2
    03 – часы
    04 – энергонезависимая память
    05 – процессор ФП
    06 – память программ ФР
    07 – оперативная память ФР
Ответ:		01H. Длина сообщения: 4 байта.
Код ошибки (1 байт)
Количество блоков данных (2 байта)
*****************************************************************************/

int ShtrihFiscalPrinter::startDump(StartDumpCommand& data)
{
    qDebug() << "startDump";

    PrinterCommand command(0x01);
    command.write(sysPassword, 4);
    command.write(data.deviceCode, 1);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.blockCount = command.readShort();
    }
    return data.resultCode;
}

/*****************************************************************************

Запрос данных
Команда:	02H. Длина сообщения: 5 байт.
Пароль ЦТО или пароль системного администратора, если пароль ЦТО не был установлен (4 байта)
Ответ:		02H. Длина сообщения: 37 байт.
Код ошибки (1 байт)
Код устройства в команде запроса дампа (1 байт):
?	01 – накопитель ФП1
?	02 – накопитель ФП2
?	03 – часы
?	04 – энергонезависимая память
?	05 – процессор ФП
?	06 – память программ ФР
?	07 – оперативная память ФР
Номер блока данных (2 байта)
Блок данных (32 байта)

*****************************************************************************/

int ShtrihFiscalPrinter::readDump(ReadDumpCommand& data)
{
    qDebug() << "readDump";

    PrinterCommand command(0x02);
    command.write(sysPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.deviceCode = (DeviceCode)command.readChar();
        data.blockNumber = command.readShort();
        command.read((char*)data.blockData, sizeof(data.blockData));
    }
    return data.resultCode;
}

/*****************************************************************************
Прерывание выдачи данных
Команда:	03H. Длина сообщения: 5 байт.
Пароль системного администратора (4 байта)
Ответ:		03H. Длина сообщения: 2 байта.
Код ошибки (1 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::stopDump(StopDumpCommand& data)
{
    qDebug() << "stopDump";

    PrinterCommand command(0x03);
    command.write(sysPassword, 4);
    data.resultCode = send(command);
    return data.resultCode;
}

/*****************************************************************************
Фискализация (перерегистрация) с длинным РНМ
Команда:	0DH. Длина сообщения: 22 байта.
    Пароль старый (4 байта)
    Пароль новый (4 байта)
    РНМ (7 байт) 00000000000000…99999999999999
    ИНН (6 байт) 000000000000…999999999999
Ответ:		0DH. Длина сообщения: 9 байт.
    Код ошибки (1 байт)
    Номер фискализации (перерегистрации) (1 байт) 1…16
    Количество оставшихся перерегистраций (1 байт) 0…15
    Номер последней закрытой смены (2 байта) 0000…2100
    Дата фискализации (перерегистрации) (3 байта) ДД-ММ-ГГ
*****************************************************************************/

int ShtrihFiscalPrinter::fiscalizationLong(FiscalizationCommand& data)
{
    qDebug() << "fiscalization";

    PrinterCommand command(0x0D);
    command.write(taxPassword, 4);
    command.write(data.newPassword, 4);
    command.write(data.rnm, 7);
    command.write(data.inn, 6);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.fiscalNumber = command.readChar();
        data.fiscalLeft = command.readChar();
        data.dayNumber = command.readShort();
        data.date = command.readDate();
    }
    return data.resultCode;
}

/*****************************************************************************
Ввод длинного заводского номера
Команда:	0EH. Длина сообщения: 12 байт.
Пароль (4 байта) (пароль «0»)
Заводской номер (7 байт) 00000000000000…99999999999999
Ответ:		0EH. Длина сообщения: 2 байта.
Код ошибки (1 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::writeLongSerial(WriteLongSerialCommand& data)
{
    qDebug() << "addSerial";

    PrinterCommand command(0x0E);
    command.write(taxPassword, 4);
    command.write(data.serial, 7);
    data.resultCode = send(command);
    return data.resultCode;
}

/*****************************************************************************
Запрос длинного заводского номера и длинного РНМ
Команда:	0FH. Длина сообщения: 5 байт.
Пароль оператора (4 байта)
Ответ:		0FH. Длина сообщения: 16 байт.
Код ошибки (1 байт)
Заводской номер (7 байт) 00000000000000…99999999999999
РНМ (7 байт) 00000000000000…99999999999999
*****************************************************************************/

int ShtrihFiscalPrinter::readLongSerial(ReadLongSerialCommand& data)
{
    qDebug() << "readLongSerial";

    PrinterCommand command(0x0F);
    command.write(usrPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.serial = command.read(7);
        data.rnm = command.read(7);
    }
    return data.resultCode;
}

/*****************************************************************************
Короткий запрос состояния ФР
 
Команда:	10H. Длина сообщения: 5 байт.
    Пароль оператора (4 байта)
Ответ:		10H. Длина сообщения: 16 байт.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
    Флаги ФР (2 байта)
    Режим ФР (1 байт)
    Подрежим ФР (1 байт)
    Количество операций в чеке (1 байт) младший байт двухбайтного числа (см. ниже)
    Напряжение резервной батареи (1 байт)
    Напряжение источника питания (1 байт)
    Код ошибки ФП (1 байт)
    Код ошибки ЭКЛЗ (1 байт)
    Количество операций в чеке (1 байт) старший байт двухбайтного числа (см. выше)
    Зарезервировано (3 байта)

*****************************************************************************/

int ShtrihFiscalPrinter::readShortStatus(ReadShortStatusCommand& data)
{
    //qDebug() << "readShortStatus";

    PrinterCommand command(0x10);
    command.write(usrPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
        data.flags = command.readShort();
        data.mode = command.readChar();
        data.submode = command.readChar();
        data.numOperations = command.readChar();
        data.batteryVoltage = command.readChar();
        data.supplyVoltage = command.readChar();
        data.fmError = command.readChar();
        data.ejError = command.readChar();
        data.numOperations += (command.readChar() << 8);
        command.read((char*)data.reserved, sizeof(data.reserved));
        //if command.
    }
    return data.resultCode;
}

/*****************************************************************************
Запрос состояния ФР

Команда:	11H. Длина сообщения: 5 байт.
    Пароль оператора (4 байта)
Ответ:		11H. Длина сообщения: 48 байт.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
    Версия ПО ФР (2 байта)
    Сборка ПО ФР (2 байта)
    Дата ПО ФР (3 байта) ДД-ММ-ГГ
    Номер в зале (1 байт)
    Сквозной номер текущего документа (2 байта)
    Флаги ФР (2 байта)
    Режим ФР (1 байт)
    Подрежим ФР (1 байт)
    Порт ФР (1 байт)
    Версия ПО ФП (2 байта)
    Сборка ПО ФП (2 байта)
    Дата ПО ФП (3 байта) ДД-ММ-ГГ
    Дата (3 байта) ДД-ММ-ГГ
    Время (3 байта) ЧЧ-ММ-СС
    Флаги ФП (1 байт)
    Заводской номер (4 байта)
    Номер последней закрытой смены (2 байта)
    Количество свободных записей в ФП (2 байта)
    Количество перерегистраций (фискализаций) (1 байт)
    Количество оставшихся перерегистраций (фискализаций) (1 байт)
    ИНН (6 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::readLongStatus(ReadLongStatusCommand& data)
{
    qDebug() << "readLongStatus";

    PrinterCommand command(0x11);
    command.write(usrPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
        data.firmwareVersion = command.readStr(2);
        data.firmwareBuild = command.readShort();
        data.firmwareDate = command.readDate();
        data.storeNumber = command.readChar();
        data.documentNumber = command.readShort();

        data.flags = command.readShort();
        data.mode = command.readChar();
        data.submode = command.readChar();
        data.portNumber = command.readChar();
        data.fmFirmwareVersion = command.readChar() + "." + command.readChar();
        data.fmFirmwareBuild = command.readShort();
        data.fmFirmwareDate = command.readDate();
        data.date = command.readDate();
        data.time = command.readTime();
        data.fmFlags = command.readChar();
        data.serialNumber = command.read(4);
        data.dayNumber = command.readShort();
        data.fmFreeRecords = command.readShort();
        data.registrationNumber = command.readChar();
        data.freeRegistration = command.readChar();
        data.fiscalID = command.read(6);
    }
    return data.resultCode;
}

/*****************************************************************************
Печать жирной строки

Команда:	12H. Длина сообщения: 26 байт.
    Пароль оператора (4 байта)
    Флаги (1 байт) Бит 0 – контрольная лента, Бит 1 – чековая лента.
    Печатаемые символы (20 байт)

Ответ:		12H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30

    Примечание: Печатаемые символы – символы в кодовой странице WIN1251.
    Символы с кодами 0…31 не отображаются.
*****************************************************************************/

int ShtrihFiscalPrinter::printBold(PrintBoldCommand& data)
{
    qDebug() << "printBold";

    PrinterCommand command(0x12);
    command.write(usrPassword, 4);
    command.write8(data.flags);
    command.write(data.text, 20);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
    }
    return data.resultCode;
}

/*****************************************************************************

Гудок
Команда:	13H. Длина сообщения: 5 байт.
•	Пароль оператора (4 байта)
Ответ:		13H. Длина сообщения: 3 байта.
•	Код ошибки (1 байт)
•	Порядковый номер оператора (1 байт) 1…30

*****************************************************************************/

int ShtrihFiscalPrinter::beep(BeepCommand& data)
{
    qDebug() << "beep";

    PrinterCommand command(0x13);
    command.write(usrPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
    }
    return data.resultCode;
}

/*****************************************************************************
Установка параметров обмена
Команда:	14H. Длина сообщения: 8 байт.
•	Пароль системного администратора (4 байта)
•	Номер порта (1 байт) 0…255
•	Код скорости обмена (1 байт) 0…6
•	Тайм аут приема байта (1 байт) 0…255
Ответ:		14H. Длина сообщения: 2 байта.
•	Код ошибки (1 байт)

*****************************************************************************/

int ShtrihFiscalPrinter::writePortParameters(PortParametersCommand& data)
{
    qDebug() << "writePortParameters";

    PrinterCommand command(0x14);
    command.write(sysPassword, 4);
    command.write8(data.port);
    command.write8(getBaudRateCode(data.baudRate));
    command.write8(getTimeoutCode(data.timeout));
    data.resultCode = send(command);
    return data.resultCode;
}

/*****************************************************************************
Чтение параметров обмена
Команда:	15H. Длина сообщения: 6 байт.
    Пароль системного администратора (4 байта)
    Номер порта (1 байт) 0…255
Ответ:		15H. Длина сообщения: 4 байта.
    Код ошибки (1 байт)
    Код скорости обмена (1 байт) 0…6
    Тайм аут приема байта (1 байт) 0…255
*****************************************************************************/

int ShtrihFiscalPrinter::readPortParameters(PortParametersCommand& data)
{
    qDebug() << "readPortParameters";

    PrinterCommand command(0x15);
    command.write(sysPassword, 4);
    command.write8(data.port);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.baudRate = getBaudRateValue(command.readChar());
        data.timeout = getTimeoutValue(command.readChar());
    }
    return data.resultCode;
}

/*****************************************************************************
Технологическое обнуление
Команда:	16H. Длина сообщения: 1 байт.
Ответ:		16H. Длина сообщения: 2 байта.
Код ошибки (1 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::resetMemory()
{
    qDebug() << "resetMemory";
    PrinterCommand command(0x16);
    return send(command);
}

/*****************************************************************************
Печать строки
Команда:	17H. Длина сообщения: 46 байт.
    Пароль оператора (4 байта)
    Флаги (1 байт) Бит 0 – контрольная лента, Бит 1 – чековая лента.
    Печатаемые символы (40 байт)
Ответ:		17H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::printString(PrintStringCommand& data)
{
    qDebug() << "printString";
    filter->printString(EVENT_BEFORE, data);

    PrinterCommand command(0x17);
    command.write(usrPassword, 4);
    command.write8(data.flags);
    command.write(data.text, 40);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
        filter->printString(EVENT_AFTER, data);
    }
    return data.resultCode;
}

/*****************************************************************************
Печать заголовка документа
Команда:	18H. Длина сообщения: 37 байт.
    Пароль оператора (4 байта)
    Наименование документа (30 байт)
    Номер документа (2 байта)
Ответ:		18H. Длина сообщения: 5 байт.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
    Сквозной номер документа (2 байта)
*****************************************************************************/

int ShtrihFiscalPrinter::printDocHeader(PrintDocHeaderCommand& data)
{
    qDebug() << "printDocHeader";

    filter->printDocHeader(EVENT_BEFORE, data);
    PrinterCommand command(0x18);
    command.write(usrPassword, 4);
    command.write(data.text, 30);
    command.write16(data.number);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
        data.documentNumber = command.readShort();

        filter->printDocHeader(EVENT_AFTER, data);
    }
    return data.resultCode;
}

/*****************************************************************************
Тестовый прогон
Команда:	19H. Длина сообщения: 6 байт.
    Пароль оператора (4 байта)
    Период вывода в минутах (1 байт) 1…99
Ответ:		19H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::startTest(StartTestCommand& data)
{
    qDebug() << "startTest";

    PrinterCommand command(0x19);
    command.write(usrPassword, 4);
    command.write8(data.periodInMinutes);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
    }
    return data.resultCode;
}

/*****************************************************************************
Запрос денежного регистра
Команда:	1AH. Длина сообщения: 6 байт.
    Пароль оператора (4 байта)
    Номер регистра (1 байт) 0… 255
Ответ:		1AH. Длина сообщения: 9 байт.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
    Содержимое регистра (6 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::readRegister(ReadCashRegCommand& data)
{
    qDebug() << "read cash register";

    PrinterCommand command(0x1A);
    command.write(usrPassword, 4);
    command.write8(data.number);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
        data.value = command.read(6);
    }
    return data.resultCode;
}

/*****************************************************************************
Запрос операционного регистра
Команда:	1BH. Длина сообщения: 6 байт.
    Пароль оператора (4 байта)
    Номер регистра (1 байт) 0… 255
Ответ:		1BH. Длина сообщения: 5 байт.
Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
    Содержимое регистра (2 байта)
*****************************************************************************/

int ShtrihFiscalPrinter::readRegister(ReadOperRegCommand& data)
{
    qDebug() << "read operation register";

    PrinterCommand command(0x1B);
    command.write(usrPassword, 4);
    command.write8(data.number);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
        data.value = command.readShort();
    }
    return data.resultCode;
}

/*****************************************************************************
Запись лицензии
Команда:	1CH. Длина сообщения: 10 байт.
    Пароль системного администратора (4 байта)
    Лицензия (5 байт) 0000000000…9999999999
Ответ:		1CH. Длина сообщения: 2 байта.
    Код ошибки (1 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::writeLicense(LicenseCommand& data)
{
    qDebug() << "writeLicense";
    PrinterCommand command(0x1C);
    command.write(sysPassword, 4);
    command.write(data.license, 5);
    data.resultCode = send(command);
    return data.resultCode;
}

/*****************************************************************************
Чтение лицензии
Команда:	1DH. Длина сообщения: 5 байт.
    Пароль системного администратора (4 байта)
Ответ:		1DH. Длина сообщения: 7 байт.
    Код ошибки (1 байт)
    Лицензия (5 байт) 0000000000…9999999999
*****************************************************************************/

int ShtrihFiscalPrinter::readLicense(LicenseCommand& data)
{
    qDebug() << "readLicense";
    PrinterCommand command(0x1C);
    command.write(sysPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.license = command.read(5);
    }
    return data.resultCode;
}

/*****************************************************************************
Запись таблицы
Команда:	1EH. Длина сообщения: (9+X) байт.
    Пароль системного администратора (4 байта)
    Таблица (1 байт)
    Ряд (2 байта)
    Поле (1 байт)
    Значение (X байт) до 40 байт
Ответ:		1EH. Длина сообщения: 2 байта.
    Код ошибки (1 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::writeTable(TableValueCommand& data)
{
    qDebug() << "writeTable";

    PrinterCommand command(0x1E);
    command.write(sysPassword, 4);
    command.write8(data.table);
    command.write16(data.row);
    command.write8(data.field);
    command.write(data.value);
    data.resultCode = send(command);
    return data.resultCode;
}

/*****************************************************************************
Чтение таблицы
Команда:	1FH. Длина сообщения: 9 байт.
    Пароль системного администратора (4 байта)
    Таблица (1 байт)
    Ряд (2 байта)
    Поле (1 байт)
Ответ:		1FH. Длина сообщения: (2+X) байт.
    Код ошибки (1 байт)
    Значение (X байт) до 40 байт
*****************************************************************************/

int ShtrihFiscalPrinter::readTable(TableValueCommand& data)
{
    qDebug() << "readTable";

    PrinterCommand command(0x1F);
    command.write(sysPassword, 4);
    command.write8(data.table);
    command.write16(data.row);
    command.write8(data.field);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.value = command.getRxBuffer();
    }
    return data.resultCode;
}

/*****************************************************************************
Запись положения десятичной точки
Команда:	20H. Длина сообщения: 6 байт.
    Пароль системного администратора (4 байта)
    Положение десятичной точки (1 байт) «0»– 0 разряд, «1»– 2 разряд
Ответ:		20H. Длина сообщения: 2 байта.
    Код ошибки (1 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::writePoint(PointCommand& data)
{
    qDebug() << "writePoint";

    PrinterCommand command(0x20);
    command.write(sysPassword, 4);
    command.write(data.point, 1);
    data.resultCode = send(command);
    return data.resultCode;
}

/*****************************************************************************
Программирование времени
Команда:	21H. Длина сообщения: 8 байт.
    Пароль системного администратора (4 байта)
    Время (3 байта) ЧЧ-ММ-СС
Ответ:		21H. Длина сообщения: 2 байта.
    Код ошибки (1 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::writeTime(TimeCommand& data)
{
    qDebug() << "write";

    PrinterCommand command(0x21);
    command.write(sysPassword, 4);
    command.write(data.time);
    data.resultCode = send(command);
    return data.resultCode;
}

/*****************************************************************************
Программирование даты
Команда:	22H. Длина сообщения: 8 байт.
    Пароль системного администратора (4 байта)
    Дата (3 байта) ДД-ММ-ГГ
Ответ:		22H. Длина сообщения: 2 байта.
    Код ошибки (1 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::writeDate(DateCommand& data)
{
    qDebug() << "write";

    PrinterCommand command(0x22);
    command.write(sysPassword, 4);
    command.write(data.date);
    data.resultCode = send(command);
    return data.resultCode;
}

/*****************************************************************************
Подтверждение программирования даты
Команда:	23H. Длина сообщения: 8 байт.
    Пароль системного администратора (4 байта)
    Дата (3 байта) ДД-ММ-ГГ
Ответ:		23H. Длина сообщения: 2 байта.
    Код ошибки (1 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::confirmDate(DateCommand& data)
{
    qDebug() << "confirmDate";

    PrinterCommand command(0x23);
    command.write(sysPassword, 4);
    command.write(data.date);
    data.resultCode = send(command);
    return data.resultCode;
}

/*****************************************************************************
Инициализация таблиц начальными значениями
Команда:	24H. Длина сообщения: 5 байт.
    Пароль системного администратора (4 байта)
Ответ:		24H. Длина сообщения: 2 байта.
    Код ошибки (1 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::initTables(PasswordCommand& data)
{
    qDebug() << "initTables";

    PrinterCommand command(0x24);
    command.write(sysPassword, 4);
    data.resultCode = send(command);
    return data.resultCode;
}

/*****************************************************************************
Отрезка чека
Команда:	25H. Длина сообщения: 6 байт.
    Пароль оператора (4 байта)
    Тип отрезки (1 байт) «0» – полная, «1» – неполная
Ответ:		25H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::cutPaper(CutCommand& data)
{
    qDebug() << "cutPaper";

    filter->cutPaper(EVENT_BEFORE, data);
    PrinterCommand command(0x25);
    command.write(usrPassword, 4);
    command.write8(data.cutType);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();

        filter->cutPaper(EVENT_AFTER, data);
    }
    return data.resultCode;
}

/*****************************************************************************
Прочитать параметры шрифта
Команда:	26H. Длина сообщения: 6 байт.
    Пароль системного администратора (4 байта)
    Номер шрифта (1 байт)
Ответ:		26H. Длина сообщения: 7 байт.
    Код ошибки (1 байт)
    Ширина области печати в точках (2 байта)
    Ширина символа с учетом межсимвольного интервала в точках (1 байт)
    Высота символа с учетом межстрочного интервала в точках (1 байт)
    Количество шрифтов в ФР (1 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::readFont(ReadFontCommand& data)
{
    qDebug() << "readFont";

    PrinterCommand command(0x26);
    command.write(sysPassword, 4);
    command.write8(data.fontNumber);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.printWidth = command.readShort();
        data.charWidth = command.readChar();
        data.charHeight = command.readChar();
        data.fontCount = command.readChar();
    }
    return data.resultCode;
}

/*****************************************************************************
Общее гашение
Команда:	27H. Длина сообщения: 5 байт.
    Пароль системного администратора (4 байта)
Ответ:		27H. Длина сообщения: 2 байта.
    Код ошибки (1 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::softReset()
{
    qDebug() << "softReset";

    PrinterCommand command(0x27);
    command.write(sysPassword, 4);
    return send(command);
}

/*****************************************************************************
Открыть денежный ящик
Команда:	28H. Длина сообщения: 6 байт.
    Пароль оператора (4 байта)
    Номер денежного ящика (1 байт) 0, 1
Ответ:		28H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::openDrawer(OpenDrawerCommand& data)
{
    qDebug() << "openDrawer";

    PrinterCommand command(0x28);
    command.write(usrPassword, 4);
    command.write8(data.number);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
    }
    return data.resultCode;
}

/*****************************************************************************
Протяжка
Команда:	29H. Длина сообщения: 7 байт.
    Пароль оператора (4 байта)
    Флаги (1 байт)
    Бит 0 – контрольная лента,
    Бит 1 – чековая лента,
    Бит 2 – подкладной документ.
    Количество строк (1 байт) 1…255 – максимальное количество строк ограничивается размером буфера печати, но не превышает 255
Ответ:		29H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::feedPaper(FeedPaperCommand& data)
{
    qDebug() << "feedPaper";

    filter->feedPaper(EVENT_BEFORE, data);
    PrinterCommand command(0x29);
    command.write(usrPassword, 4);
    command.write8(data.flags);
    command.write8(data.count);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();

        filter->feedPaper(EVENT_AFTER, data);
    }
    return data.resultCode;
}

/*****************************************************************************
Выброс подкладного документа
Команда:	2AH. Длина сообщения: 6 байт.
    Пароль оператора (4 байта)
    Направление выброса подкладного документа (1 байт) «0» – вниз, «1» – вверх
Ответ:		2AH. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::slipEject(SlipEjectCommand& data)
{
    qDebug() << "slipEject";

    PrinterCommand command(0x29);
    command.write(usrPassword, 4);
    command.write(data.ejectDirection, 1);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
    }
    return data.resultCode;
}

/*****************************************************************************
Прерывание тестового прогона
Команда:	2BH. Длина сообщения: 5 байт.
    Пароль оператора (4 байта)
Ответ:		2BH. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::stopTest(PasswordCommand& data)
{
    qDebug() << "stopTest";

    PrinterCommand command(0x2B);
    command.write(usrPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
    }
    return data.resultCode;
}

/*****************************************************************************
Снятие показаний операционных регистров
Команда:	2СH. Длина сообщения: 5 байт.
    Пароль администратора или системного администратора (4 байта)
Ответ:		2СH. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 29, 30
*****************************************************************************/

int ShtrihFiscalPrinter::printRegisters(PasswordCommand& data)
{
    qDebug() << "printRegisters";
    PrinterCommand command(0x2C);
    command.write(usrPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
    }
    return data.resultCode;
}

/*****************************************************************************
Запрос структуры таблицы
Команда:	2DH. Длина сообщения: 6 байт.
    Пароль системного администратора (4 байта)
    Номер таблицы (1 байт)
Ответ:		2DH. Длина сообщения: 45 байт.
    Код ошибки (1 байт)
    Название таблицы (40 байт)
    Количество рядов (2 байта)
    Количество полей (1 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::readTableInfo(TableInfoCommand& data)
{
    qDebug() << "readTableInfo";
    PrinterCommand command(0x2B);
    command.write(sysPassword, 4);
    command.write(data.table, 1);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.name = command.readStr(40);
        data.rows = command.readShort();
        data.fields = command.readChar();
    }
    return data.resultCode;
}

/*****************************************************************************
Запрос структуры поля
Команда:	2EH. Длина сообщения: 7 байт.
    Пароль системного администратора (4 байта)
    Номер таблицы (1 байт)
    Номер поля (1 байт)
Ответ:		2EH. Длина сообщения: (44+X+X) байт.
    Код ошибки (1 байт)
    Название поля (40 байт)
    Тип поля (1 байт) «0» – BIN, «1» – CHAR
    Количество байт – X (1 байт)
    Минимальное значение поля – для полей типа BIN (X байт)
    Максимальное значение поля – для полей типа BIN (X байт)
*****************************************************************************/

int ShtrihFiscalPrinter::readFieldInfo(FieldInfoCommand& data)
{
    qDebug() << "readFieldInfo";

    PrinterCommand command(0x2E);
    command.write(sysPassword, 4);
    command.write(data.table, 1);
    command.write(data.field, 1);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.name = command.readStr(40);
        data.type = command.readChar();
        data.size = command.readChar();
        data.min = 0;
        data.max = 0;
        if (data.type == 0) {
            data.min = command.read(data.size);
            data.max = command.read(data.size);
        }
    }
    return data.resultCode;
}

/*****************************************************************************
Печать строки данным шрифтом
Команда:	2FH. Длина сообщения: 47 байт.
    Пароль оператора (4 байта)
    Флаги (1 байт) Бит 0 – контрольная лента, Бит 1 – чековая лента.
    Номер шрифта (1 байт) 0…255
    Печатаемые символы (40 байт)
Ответ:		2FH. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::printStringFont(PrintStringFontCommand& data)
{
    qDebug() << "printStringFont";

    filter->printStringFont(EVENT_BEFORE, data);
    PrinterCommand command(0x2F);
    command.write(usrPassword, 4);
    command.write8(data.flags);
    command.write8(data.font);
    command.write(data.text);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();

        filter->printStringFont(EVENT_AFTER, data);
    }
    return data.resultCode;
}

/*****************************************************************************
Суточный отчет без гашения
Команда:	40H. Длина сообщения: 5 байт.
    Пароль администратора или системного администратора (4 байта)
Ответ:		40H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 29, 30
*****************************************************************************/

int ShtrihFiscalPrinter::printXReport(PasswordCommand& data)
{
    qDebug() << "printXReport";
    filter->printXReport(EVENT_BEFORE, data);
    PrinterCommand command(0x40);
    command.write(sysPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
        filter->printXReport(EVENT_AFTER, data);
    }
    return data.resultCode;
}

/*****************************************************************************
Суточный отчет с гашением
Команда:	41H. Длина сообщения: 5 байт.
Пароль администратора или системного администратора (4 байта)
Ответ:		41H. Длина сообщения: 3 байта.
Код ошибки (1 байт)
Порядковый номер оператора (1 байт) 29, 30
*****************************************************************************/

int ShtrihFiscalPrinter::printZReport(PasswordCommand& data)
{
    qDebug() << "printZReport";
    filter->printZReport(EVENT_BEFORE, data);
    PrinterCommand command(0x41);
    command.write(sysPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)){
        data.operatorNumber = command.readChar();
        filter->printZReport(EVENT_AFTER, data);
    }
    return data.resultCode;
}

/*****************************************************************************
Отчёт по секциям
Команда:	42H. Длина сообщения: 5 байт.
    Пароль администратора или системного администратора (4 байта)
Ответ:		42H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 29, 30
*****************************************************************************/

int ShtrihFiscalPrinter::printDepartmentReport(PasswordCommand& data)
{
    qDebug() << "printDepartmentReport";
    PrinterCommand command(0x42);
    command.write(sysPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)){
        data.operatorNumber = command.readChar();
    }
    return data.resultCode;
}

/*****************************************************************************
Отчёт по налогам
Команда:	43H. Длина сообщения: 5 байт.
    Пароль администратора или системного администратора (4 байта)
Ответ:		43H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 29, 30
*****************************************************************************/

int ShtrihFiscalPrinter::printTaxReport(PasswordCommand& data)
{
    qDebug() << "printTaxReport";
    PrinterCommand command(0x43);
    command.write(sysPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)){
        data.operatorNumber = command.readChar();
    }
    return data.resultCode;
}

/*****************************************************************************
Внесение
Команда:	50H. Длина сообщения: 10 байт.
    Пароль оператора (4 байта)
    Сумма (5 байт)
Ответ:		50H. Длина сообщения: 5 байт.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
    Сквозной номер документа (2 байта)
*****************************************************************************/

int ShtrihFiscalPrinter::printCashIn(CashCommand& data)
{
    qDebug() << "printCashIn";

    filter->printCashIn(EVENT_BEFORE, data);
    PrinterCommand command(0x50);
    command.write(usrPassword, 4);
    command.write(data.amount, 5);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
        data.documentNumber = command.readShort();

        filter->printCashIn(EVENT_AFTER, data);
    }
    return data.resultCode;
}

/*****************************************************************************
Выплата
Команда:	51H. Длина сообщения: 10 байт.
    Пароль оператора (4 байта)
    Сумма (5 байт)
Ответ:		51H. Длина сообщения: 5 байт.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
    Сквозной номер документа (2 байта)
*****************************************************************************/

int ShtrihFiscalPrinter::printCashOut(CashCommand& data)
{
    qDebug() << "printCashOut";

    filter->printCashOut(EVENT_BEFORE, data);
    PrinterCommand command(0x51);
    command.write(usrPassword, 4);
    command.write(data.amount, 5);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
        data.documentNumber = command.readShort();

        filter->printCashOut(EVENT_AFTER, data);
    }
    return data.resultCode;
}

/*****************************************************************************
Печать клише
Команда:	52H. Длина сообщения: 5 байт.
    Пароль оператора (4 байта)
Ответ:		52H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::printHeader(PasswordCommand& data)
{
    qDebug() << "printHeader";
    filter->printHeader(EVENT_BEFORE, data);

    PrinterCommand command(0x52);
    command.write(usrPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
        filter->printHeader(EVENT_AFTER, data);
    }
    return data.resultCode;
}

/*****************************************************************************
Конец Документа
Команда:	53H. Длина сообщения: 6 байт.
    Пароль оператора (4 байта)
    Параметр (1 байт)
    0- без рекламного текста
    1 - с рекламным тестом
Ответ:		53H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::printDocEnd(PrintDocEndCommand& data)
{
    qDebug() << "printDocEnd";

    filter->printDocEnd(EVENT_BEFORE, data);
    PrinterCommand command(0x53);
    command.write(usrPassword, 4);
    command.write8(data.printad);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
        filter->printDocEnd(EVENT_AFTER, data);
    }
    return data.resultCode;
}

/*****************************************************************************
Печать рекламного текста
Команда:	54H. Длина сообщения:5 байт.
    Пароль оператора (4 байта)
Ответ:		54H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::printTrailer(PasswordCommand& data)
{
    qDebug() << "printTrailer";
    filter->printTrailer(EVENT_BEFORE, data);
    PrinterCommand command(0x54);
    command.write(usrPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
        filter->printTrailer(EVENT_AFTER, data);
    }
    return data.resultCode;
}

/*****************************************************************************
Ввод заводского номера
Команда:	60H. Длина сообщения: 9 байт.
    Пароль (4 байта) (пароль «0»)
    Заводской номер (4 байта) 00000000…99999999
Ответ:		60H. Длина сообщения: 2 байта.
    Код ошибки (1 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::writeSerial(WriteSerialCommand& data)
{
    qDebug() << "writeSerial";

    PrinterCommand command(0x60);
    command.write(taxPassword, 4);
    command.write(data.serial, 4);
    data.resultCode = send(command);
    return data.resultCode;
}

/*****************************************************************************
Инициализация ФП
Команда:	61H. Длина сообщения: 1 байт.
Ответ:		61H. Длина сообщения: 2 байта.
Код ошибки (1 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::resetFM(int& resultCode)
{
    qDebug() << "resetFM";

    PrinterCommand command(0x61);
    resultCode = send(command);
    return resultCode;
}

/*****************************************************************************
Запрос суммы записей в ФП
Команда:	62H. Длина сообщения: 6 байт.
    Пароль администратора или системного администратора (4 байта)
    Тип запроса (1 байт) «0» – сумма всех записей, «1» – сумма записей после последней перерегистрации
Ответ:		62H. Длина сообщения: 29 байт.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 29, 30
    Сумма сменных итогов продаж (8 байт)
    Сумма сменных итог покупок (6 байт) При отсутствии ФП 2: FFh FFh FFh FFh FFh FFh
    Сумма сменных возвратов продаж (6 байт) При отсутствии ФП 2: FFh FFh FFh FFh FFh FFh
    Сумма сменных возвратов покупок (6 байт) При отсутствии ФП 2: FFh FFh FFh FFh FFh FFh
*****************************************************************************/

int ShtrihFiscalPrinter::fmReadTotals(FMReadTotalsCommand& data)
{
    qDebug() << "fmReadTotals";

    PrinterCommand command(0x62);
    command.write(sysPassword, 4);
    command.write(data.type, 1);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
        data.saleTotal = command.read(8);
        data.buyTotal = command.read(6);
        data.retSaleTotal = command.read(6);
        data.retBuyTotal = command.read(6);
    }
    return data.resultCode;
}

/*****************************************************************************
Запрос количества ФД на которые нет квитанции FF3FH
Код команды FF3Fh . Длина сообщения: 6 байт.
    Пароль системного администратора: 4 байта
Ответ:	    FF3Fh Длина сообщения: 3 байт.
    Код ошибки: 1 байт
    Количество неподтверждённых ФД : 2 байта
*****************************************************************************/

int ShtrihFiscalPrinter::fsReadDocCount(int& docCount)
{
    //qDebug() << "fsReadDocCount";

    PrinterCommand command(0xFF3F);
    command.write(sysPassword, 4);
    int resultCode = send(command);
    if (succeeded(resultCode)) {
        docCount = command.read(2);
    }
    return resultCode;
}

int ShtrihFiscalPrinter::fsReadTotals(FSReadTotalsCommand& data)
{
    qDebug() << "fsReadTotals";

    PrinterCommand command(0xFE);
    command.write(0xF4, 1);
    command.write(0, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.saleTotal = command.read(8);
        data.retSaleTotal = command.read(8);
        data.buyTotal = command.read(8);
        data.retBuyTotal = command.read(8);
    }
    return data.resultCode;
}

int ShtrihFiscalPrinter::readTotals(PrinterTotals& data){
    qDebug() << "readTotals";

    int rc = 0;
    if (capFiscalStorage)
    {
        FSReadTotalsCommand command;
        rc = fsReadTotals(command);
        if (succeeded(rc)){
            data.saleTotal = command.saleTotal;
            data.retSaleTotal = command.retSaleTotal;
            data.buyTotal = command.buyTotal;
            data.retBuyTotal = command.retBuyTotal;
        }
    } else
    {
        if (isFiscalized){
            FMReadTotalsCommand command;
            command.type = data.type;
            rc = fmReadTotals(command);
            if (succeeded(rc)){
                data.saleTotal = command.saleTotal;
                data.retSaleTotal = command.retSaleTotal;
                data.buyTotal = command.buyTotal;
                data.retBuyTotal = command.retBuyTotal;
            }
        } else
        {
            data.saleTotal = 0;
            data.retSaleTotal = 0;
            data.buyTotal = 0;
            data.retBuyTotal = 0;
        }
    }
    data.resultCode = rc;
    return data.resultCode;
}

/*****************************************************************************
Запрос даты последней записи в ФП
Команда:	63H. Длина сообщения: 5 байт.
    Пароль администратора или системного администратора (4 байта)
Ответ:		63H. Длина сообщения: 7 байт.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 29, 30
    Тип последней записи (1 байт) «0» – фискализация (перерегистрация), «1» – сменный итог.
    Дата (3 байта) ДД-ММ-ГГ
*****************************************************************************/

int ShtrihFiscalPrinter::fmReadLastDate(FMReadLastDateCommand& data)
{
    qDebug() << "fmReadLastDate";

    PrinterCommand command(0x63);
    command.write(sysPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
        data.type = command.readChar();
        data.date = command.readDate();
    }
    return data.resultCode;
}

/*****************************************************************************
Запрос диапазона дат и смен
Команда:	64H. Длина сообщения: 5 байт.
    Пароль налогового инспектора (4 байта)
Ответ:		64H. Длина сообщения: 12 байт.
    Код ошибки (1 байт)
    Дата первой смены (3 байта) ДД-ММ-ГГ
    Дата последней смены (3 байта) ДД-ММ-ГГ
    Номер первой смены (2 байта) 0000…2100
    Номер последней смены (2 байта) 0000…2100
*****************************************************************************/

int ShtrihFiscalPrinter::fmReadRange(FMReadRangeCommand& data)
{
    qDebug() << "fmReadRange";

    PrinterCommand command(0x64);
    command.write(taxPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.firstDayDate = command.readDate();
        data.lastDayDate = command.readDate();
        data.firstDayNumber = command.readShort();
        data.lastDayNumber = command.readShort();
    }
    return data.resultCode;
}

/*****************************************************************************
Фискализация (перерегистрация)
Команда:	65H. Длина сообщения: 20 байт.
    Пароль старый (4 байта)
    Пароль новый (4 байта)
    РНМ (5 байт) 0000000000…9999999999
    ИНН (6 байт) 000000000000…999999999999
Ответ:		65H. Длина сообщения: 9 байт.
    Код ошибки (1 байт)
    Номер фискализации (перерегистрации) (1 байт) 1…16
    Количество оставшихся перерегистраций (1 байт) 0…15
    Номер последней закрытой смены (2 байта) 0000…2100
    Дата фискализации (перерегистрации) (3 байта) ДД-ММ-ГГ
*****************************************************************************/

int ShtrihFiscalPrinter::fiscalization(FiscalizationCommand& data)
{
    qDebug() << "fiscalization";

    PrinterCommand command(0x65);
    command.write(taxPassword, 4);
    command.write(data.newPassword, 4);
    command.write(data.rnm, 5);
    command.write(data.inn, 6);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.fiscalNumber = command.readChar();
        data.fiscalLeft = command.readChar();
        data.dayNumber = command.readShort();
        data.date = command.readDate();
    }
    return data.resultCode;
}

/*****************************************************************************
Фискальный отчет по диапазону дат
Команда:	66H. Длина сообщения: 12 байт.
    Пароль налогового инспектора (4 байта)
    Тип отчета (1 байт) «0» – короткий, «1» – полный
    Дата первой смены (3 байта) ДД-ММ-ГГ
    Дата последней смены (3 байта) ДД-ММ-ГГ
Ответ:		66H. Длина сообщения: 12 байт.
    Код ошибки (1 байт)
    Дата первой смены (3 байта) ДД-ММ-ГГ
    Дата последней смены (3 байта) ДД-ММ-ГГ
    Номер первой смены (2 байта) 0000…2100
    Номер последней смены (2 байта) 0000…2100
*****************************************************************************/

int ShtrihFiscalPrinter::fmDatesReport(FMDatesReportCommand& data)
{
    qDebug() << "fmDatesReport";

    PrinterCommand command(0x66);
    command.write(taxPassword, 4);
    command.write(data.reportType, 1);
    command.write(data.firstDate);
    command.write(data.lastDate);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.firstDayDate = command.readDate();
        data.lastDayDate = command.readDate();
        data.firstDayNumber = command.readShort();
        data.lastDayNumber = command.readShort();
    }
    return data.resultCode;
}

/*****************************************************************************
Фискальный отчет по диапазону смен
Команда:	67H. Длина сообщения: 10 байт.
    Пароль налогового инспектора (4 байта)
    Тип отчета (1 байт) «0» – короткий, «1» – полный
    Номер первой смены (2 байта) 0000…2100
    Номер последней смены (2 байта) 0000…2100
Ответ:		67H. Длина сообщения: 12 байт.
    Код ошибки (1 байт)
    Дата первой смены (3 байта) ДД-ММ-ГГ
    Дата последней смены (3 байта) ДД-ММ-ГГ
    Номер первой смены (2 байта) 0000…2100
    Номер последней смены (2 байта) 0000…2100
*****************************************************************************/

int ShtrihFiscalPrinter::fmDaysReport(FMDaysReportCommand& data)
{
    qDebug() << "fmDaysReport";

    PrinterCommand command(0x67);
    command.write(taxPassword, 4);
    command.write(data.reportType, 1);
    command.write(data.firstDay, 2);
    command.write(data.lastDay, 2);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.firstDayDate = command.readDate();
        data.lastDayDate = command.readDate();
        data.firstDayNumber = command.readShort();
        data.lastDayNumber = command.readShort();
    }
    return data.resultCode;
}

/*****************************************************************************
Прерывание полного отчета
Команда:	68H. Длина сообщения: 5 байт.
    Пароль налогового инспектора (4 байта)
Ответ:		68H. Длина сообщения: 2 байта.
    Код ошибки (1 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::fmCancelReport(PasswordCommand& data)
{
    qDebug() << "fmCancelReport";
    PrinterCommand command(0x68);
    command.write(taxPassword, 4);
    data.resultCode = send(command);
    return data.resultCode;
}

/*****************************************************************************
Чтение параметров фискализации (перерегистрации)
Команда:	69H. Длина сообщения: 6 байт.
    Пароль налогового инспектора, при котором была проведена данная фискализация (4 байта)
    Номер фискализации (перерегистрации) (1 байт) 1…16
Ответ:		69H. Длина сообщения: 22 байта.
    Код ошибки (1 байт)
    Пароль (4 байта)
    РНМ (5 байт) 0000000000…9999999999
    ИНН (6 байт) 000000000000…999999999999
    Номер смены перед фискализацией (перерегистрацией) (2 байта) 0000…2100
    Дата фискализации (перерегистрации) (3 байта) ДД-ММ-ГГ
*****************************************************************************/

int ShtrihFiscalPrinter::fmReadFiscalization(FMReadFiscalizationCommand& data)
{
    qDebug() << "fmReadFiscalization";

    PrinterCommand command(0x0D);
    command.write(taxPassword, 4);
    command.write(data.fiscalNumber, 1);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.rnm = command.read(5);
        data.inn = command.read(6);
        data.dayNumber = command.readShort();
        data.date = command.readDate();
    }
    return data.resultCode;
}

/*****************************************************************************
Открыть фискальный подкладной документ
Команда:	70H. Длина сообщения: 26 байт.
    Пароль оператора (4 байта)
    Тип документа (1 байт) «0» – продажа, «1» – покупка, «2» – возврат продажи, «3» – возврат покупки
    Дублирование печати (извещение, квитанция) (1 байт) «0» – колонки, «1» – блоки строк
    Количество дублей (1 байт) 0…5
    Смещение между оригиналом и 1-ым дублем печати (1 байт) *
    Смещение между 1-ым и 2-ым дублями печати (1 байт) *
    Смещение между 2-ым и 3-им дублями печати (1 байт) *
    Смещение между 3-им и 4-ым дублями печати (1 байт) *
    Смещение между 4-ым и 5-ым дублями печати (1 байт) *
    Номер шрифта клише (1 байт)
    Номер шрифта заголовка документа (1 байт)
    Номер шрифта номера ЭКЛЗ (1 байт)
    Номер шрифта значения КПК и номера КПК (1 байт)
    Номер строки клише (1 байт)
    Номер строки заголовка документа (1 байт)
    Номер строки номера ЭКЛЗ (1 байт)
    Номер строки признака повтора документа (1 байт)
    Смещение клише в строке (1 байт)
    Смещение заголовка документа в строке (1 байт)
    Смещение номера ЭКЛЗ в строке (1 байт)
    Смещение КПК и номера КПК в строке (1 байт)
    Смещение признака повтора документа в строке (1 байт)
Ответ:		70H. Длина сообщения: 5 байт.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
    Сквозной номер документа (2 байта)
    *– Для колонок величина смещения задаётся в символах, для блоков строк – в строках.
*****************************************************************************/

void ShtrihFiscalPrinter::write(PrinterCommand& command, SlipDocParams data)
{
    command.write8(data.docType); // Тип документа (1 байт) "0" - продажа, "1" - покупка, "2" - возврат продажи, "3" - возврат покупки
    command.write8(data.copyType); // Дублирование печати (извещение, квитанция) (1 байт) "0" - колонки, "1" - блоки строк
    command.write8(data.copyCount); // Количество дублей (1 байт) 0…5
    command.write8(data.copy1Offset); // Смещение между оригиналом и 1-ым дублем печати (1 байт) *
    command.write8(data.copy2Offset); // Смещение между 1-ым и 2-ым дублями печати (1 байт) *
    command.write8(data.copy3Offset); // Смещение между 2-ым и 3-им дублями печати (1 байт) *
    command.write8(data.copy4Offset); // Смещение между 3-им и 4-ым дублями печати (1 байт) *
    command.write8(data.copy5Offset); // Смещение между 4-ым и 5-ым дублями печати (1 байт) *
    command.write8(data.headerFont); // Номер шрифта клише (1 байт)
    command.write8(data.docHeaderFont); // Номер шрифта заголовка документа (1 байт)
    command.write8(data.ejSerialFont); // Номер шрифта номера ЭКЛЗ (1 байт)
    command.write8(data.ejCRCFont); // Номер шрифта значения КПК и номера КПК (1 байт)
    command.write8(data.headerLine); // Номер строки клише (1 байт)
    command.write8(data.docHeaderLine); // Номер строки заголовка документа (1 байт)
    command.write8(data.ejSErialLine); // Номер строки номера ЭКЛЗ (1 байт)
    command.write8(data.dupSignLine); // Номер строки признака повтора документа (1 байт)
    command.write8(data.headerLineOffset); // Смещение клише в строке (1 байт)
    command.write8(data.docHeaderLineOffset); // Смещение заголовка документа в строке (1 байт)
    command.write8(data.ejSerialLineOffset); // Смещение номера ЭКЛЗ в строке (1 байт)
    command.write8(data.ejCRCLineOffset); // Смещение КПК и номера КПК в строке (1 байт)
    command.write8(data.ejDupSignLineOffset); // Смещение признака повтора документа в строке (1 байт)
}

int ShtrihFiscalPrinter::slipOpenDocument(SlipOpenDocumentCommand& data)
{
    qDebug() << "fmReadFiscalization";

    PrinterCommand command(0x70);
    command.write(usrPassword, 4);
    write(command, data.params);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
        data.docNumber = command.readShort();
    }
    return data.resultCode;
}

/*****************************************************************************
Открыть стандартный фискальный подкладной документ
Команда:	71H. Длина сообщения: 13 байт.
    Пароль оператора (4 байта)
    Тип документа (1 байт) «0» – продажа, «1» – покупка, «2» – возврат продажи, «3» – возврат покупки
    Дублирование печати (извещение, квитанция) (1 байт) «0» – колонки, «1» – блоки строк
    Количество дублей (1 байт) 0…5
    Смещение между оригиналом и 1-ым дублем печати (1 байт) *
    Смещение между 1-ым и 2-ым дублями печати (1 байт) *
    Смещение между 2-ым и 3-им дублями печати (1 байт) *
    Смещение между 3-им и 4-ым дублями печати (1 байт) *
    Смещение между 4-ым и 5-ым дублями печати (1 байт) *
Ответ:		71H. Длина сообщения: 5 байт.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
    Сквозной номер документа (2 байта)
    *– Для колонок величина смещения задаётся в символах, для блоков строк – в строках.
*****************************************************************************/

void ShtrihFiscalPrinter::write(PrinterCommand& command, StdSlipParams data)
{
    command.write8(data.docType); // Тип документа (1 байт) "0" - продажа, "1" - покупка, "2" - возврат продажи, "3" - возврат покупки
    command.write8(data.copyType); // Дублирование печати (извещение, квитанция) (1 байт) "0" - колонки, "1" - блоки строк
    command.write8(data.copyCount); // Количество дублей (1 байт) 0…5
    command.write8(data.copy1Offset); // Смещение между оригиналом и 1-ым дублем печати (1 байт) *
    command.write8(data.copy2Offset); // Смещение между 1-ым и 2-ым дублями печати (1 байт) *
    command.write8(data.copy3Offset); // Смещение между 2-ым и 3-им дублями печати (1 байт) *
    command.write8(data.copy4Offset); // Смещение между 3-им и 4-ым дублями печати (1 байт) *
    command.write8(data.copy5Offset); // Смещение между 4-ым и 5-ым дублями печати (1 байт) *
}

int ShtrihFiscalPrinter::slipOpenStdDocument(SlipOpenStdDocumentCommand& data)
{
    qDebug() << "slipOpenStdDocument";

    PrinterCommand command(0x71);
    command.write(usrPassword, 4);
    write(command, data.params);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
        data.docNumber = command.readShort();
    }
    return data.resultCode;
}

/*****************************************************************************
Формирование операции на подкладном документе
Команда:	72H. Длина сообщения: 82 байта.
    Пароль оператора (4 байта)
    Формат целого количества (1 байт) «0» – без цифр после запятой, «1» – с цифрами после запятой
    Количество строк в операции (1 байт) 1…3
    Номер текстовой строки в операции (1 байт) 0…3, «0» – не печатать
    Номер строки произведения количества на цену в операции (1 байт) 0…3, «0» – не печатать
    Номер строки суммы в операции (1 байт) 1…3
    Номер строки отдела в операции (1 байт) 1…3
    Номер шрифта текстовой строки (1 байт)
    Номер шрифта количества (1 байт)
    Номер шрифта знака умножения количества на цену (1 байт)
    Номер шрифта цены (1 байт)
    Номер шрифта суммы (1 байт)
    Номер шрифта отдела (1 байт)
    Количество символов поля текстовой строки (1 байт)
    Количество символов поля количества (1 байт)
    Количество символов поля цены (1 байт)
    Количество символов поля суммы (1 байт)
    Количество символов поля отдела (1 байт)
    Смещение поля текстовой строки в строке (1 байт)
    Смещение поля произведения количества на цену в строке (1 байт)
    Смещение поля суммы в строке (1 байт)
    Смещение поля отдела в строке (1 байт)
    Номер строки ПД с первой строкой блока операции (1 байт)
    Количество (5 байт)
    Цена (5 байт)
    Отдел (1 байт) 0…16
    Налог 1 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 2 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 3 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 4 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Текст (40 байт)
Ответ:		72H. Длина сообщения: 3 байт.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

void ShtrihFiscalPrinter::write(PrinterCommand& command, SlipOperationParams data)
{
    command.write8(data.quantityFormat); // Формат целого количества (1 байт) "0" - без цифр после запятой, "1" - с цифрами после запятой
    command.write8(data.lineCount); // Количество строк в операции (1 байт) 1…3
    command.write8(data.textLine); // Номер текстовой строки в операции (1 байт) 0…3, "0" - не печатать
    command.write8(data.quantityLine); // Номер строки произведения количества на цену в операции (1 байт) 0…3, "0" - не печатать
    command.write8(data.summLine); // Номер строки суммы в операции (1 байт) 1…3
    command.write8(data.departmentLine); // Номер строки отдела в операции (1 байт) 1…3
    command.write8(data.textFont); // Номер шрифта текстовой строки (1 байт)
    command.write8(data.quantityFont); // Номер шрифта количества (1 байт)
    command.write8(data.multSignFont); // Номер шрифта знака умножения количества на цену (1 байт)
    command.write8(data.priceFont); // Номер шрифта цены (1 байт)
    command.write8(data.sumFont); // Номер шрифта суммы (1 байт)
    command.write8(data.departmentFont); // Номер шрифта отдела (1 байт)
    command.write8(data.textWidth); // Количество символов поля текстовой строки (1 байт)
    command.write8(data.quantityWidth); // Количество символов поля количества (1 байт)
    command.write8(data.priceWidth); // Количество символов поля цены (1 байт)
    command.write8(data.sumWidth); // Количество символов поля суммы (1 байт)
    command.write8(data.departmentWidth); // Количество символов поля отдела (1 байт)
    command.write8(data.textOffset); // Смещение поля текстовой строки в строке (1 байт)
    command.write8(data.quantityOffset); // Смещение поля произведения количества на цену в строке (1 байт)
    command.write8(data.sumOffset); // Смещение поля суммы в строке (1 байт)
    command.write8(data.departmentOffset); // Смещение поля отдела в строке (1 байт)
};

void ShtrihFiscalPrinter::write(PrinterCommand& command, SlipOperation data)
{
    command.write8(data.lineNumber); // Номер строки ПД с первой строкой
    command.write(data.quantity, 5); // Количество (5 байт)
    command.write(data.price, 5); // Цена (5 байт)
    command.write8(data.department); // Отдел (1 байт) 0…16
    command.write8(data.tax1); // Налог 1 (1 байт) "0" - нет, "1"…"4" - налоговая группа
    command.write8(data.tax2); // Налог 1 (1 байт) "0" - нет, "1"…"4" - налоговая группа
    command.write8(data.tax3); // Налог 1 (1 байт) "0" - нет, "1"…"4" - налоговая группа
    command.write8(data.tax4); // Налог 1 (1 байт) "0" - нет, "1"…"4" - налоговая группа
    command.write(data.text); // Текст (40 байт)
};

int ShtrihFiscalPrinter::slipPrintItem(SlipPrintItemCommand& data)
{
    qDebug() << "slipPrintItem";

    PrinterCommand command(0x72);
    command.write(usrPassword, 4);
    write(command, data.params);
    write(command, data.item);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
    }
    return data.resultCode;
}

/*****************************************************************************
Формирование стандартной операции на подкладном документе
Команда:	73H. Длина сообщения: 61 байт.
    Пароль оператора (4 байта)
    Номер строки ПД с первой строкой блока операции (1 байт)
    Количество (5 байт)
    Цена (5 байт)
    Отдел (1 байт) 0…16
    Налог 1 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 2 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 3 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 4 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Текст (40 байт)
Ответ:		73H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

/*****************************************************************************
Формирование скидки/надбавки на подкладном документе
Команда:	74H. Длина сообщения: 68 байт.
Пароль оператора (4 байта)
Количество строк в операции (1 байт) 1…2
Номер текстовой строки в операции (1 байт) 0…2, «0» – не печатать
Номер строки названия операции в операции (1 байт) 1…2
Номер строки суммы в операции (1 байт) 1…2
Номер шрифта текстовой строки (1 байт)
Номер шрифта названия операции (1 байт)
Номер шрифта суммы (1 байт)
Количество символов поля текстовой строки (1 байт)
Количество символов поля суммы (1 байт)
Смещение поля текстовой строки в строке (1 байт)
Смещение поля названия операции в строке (1 байт)
Смещение поля суммы в строке (1 байт)
Тип операции (1 байт) «0» – скидка, «1» – надбавка
Номер строки ПД с первой строкой блока скидки/надбавки (1 байт)
Сумма (5 байт)
Налог 1 (1 байт) «0» – нет, «1»…«4» – налоговая группа
Налог 2 (1 байт) «0» – нет, «1»…«4» – налоговая группа
Налог 3 (1 байт) «0» – нет, «1»…«4» – налоговая группа
Налог 4 (1 байт) «0» – нет, «1»…«4» – налоговая группа
Текст (40 байт)
Ответ:		74H. Длина сообщения: 3 байта.
Код ошибки (1 байт)
Порядковый номер оператора (1 байт) 1…30
Формирование стандартной скидки/надбавки на подкладном документе
Команда:	75H. Длина сообщения: 56 байт.
Пароль оператора (4 байта)
Тип операции (1 байт) «0» – скидка, «1» – надбавка
Номер строки ПД с первой строкой блока скидки/надбавки (1 байт)
Сумма (5 байт)
Налог 1 (1 байт) «0» – нет, «1»…«4» – налоговая группа
Налог 2 (1 байт) «0» – нет, «1»…«4» – налоговая группа
Налог 3 (1 байт) «0» – нет, «1»…«4» – налоговая группа
Налог 4 (1 байт) «0» – нет, «1»…«4» – налоговая группа
Текст (40 байт)
Ответ:		75H. Длина сообщения: 3 байт.
Код ошибки (1 байт)
Порядковый номер оператора (1 байт) 1…30
Формирование закрытия чека на подкладном документе
Команда:	76H. Длина сообщения: 182 байта.
Пароль оператора (4 байта)
Количество строк в операции (1 байт) 1…17
Номер строки итога в операции (1 байт) 1…17
Номер текстовой строки в операции (1 байт) 0…17, «0» – не печатать
Номер строки наличных в операции (1 байт) 0…17, «0» – не печатать
Номер строки типа оплаты 2 в операции (1 байт) 0…17, «0» – не печатать
Номер строки типа оплаты 3 в операции (1 байт) 0…17, «0» – не печатать
Номер строки типа оплаты 4 в операции (1 байт) 0…17, «0» – не печатать
Номер строки сдачи в операции (1 байт) 0…17, «0» – не печатать
Номер строки оборота по налогу А в операции (1 байт) 0…17, «0» – не печатать
Номер строки оборота по налогу Б в операции (1 байт) 0…17, «0» – не печатать
Номер строки оборота по налогу В в операции (1 байт) 0…17, «0» – не печатать
Номер строки оборота по налогу Г в операции (1 байт) 0…17, «0» – не печатать
Номер строки суммы по налогу А в операции (1 байт) 0…17, «0» – не печатать
Номер строки суммы по налогу Б в операции (1 байт) 0…17, «0» – не печатать
Номер строки суммы по налогу В в операции (1 байт) 0…17, «0» – не печатать
Номер строки суммы по налогу Г в операции (1 байт) 0…17, «0» – не печатать
Номер строки суммы до начисления скидки в операции (1 байт) 0…17, «0» – не печатать
Номер строки суммы скидки в операции (1 байт) 0…17, «0» – не печатать
Номер шрифта текстовой строки (1 байт)
Номер шрифта «ИТОГ» (1 байт)
Номер шрифта суммы итога (1 байт)
Номер шрифта «НАЛИЧНЫМИ» (1 байт)
Номер шрифта суммы наличных (1 байт)
Номер шрифта названия типа оплаты 2 (1 байт)
Номер шрифта суммы типа оплаты 2 (1 байт)
Номер шрифта названия типа оплаты 3 (1 байт)
Номер шрифта суммы типа оплаты 3 (1 байт)
Номер шрифта названия типа оплаты 4 (1 байт)
Номер шрифта суммы типа оплаты 4 (1 байт)
Номер шрифта «СДАЧА» (1 байт)
Номер шрифта суммы сдачи (1 байт)
Номер шрифта названия налога А (1 байт)
Номер шрифта оборота налога А (1 байт)
Номер шрифта ставки налога А (1 байт)
Номер шрифта суммы налога А (1 байт)
Номер шрифта названия налога Б (1 байт)
Номер шрифта оборота налога Б (1 байт)
Номер шрифта ставки налога Б (1 байт)
Номер шрифта суммы налога Б (1 байт)
Номер шрифта названия налога В (1 байт)
Номер шрифта оборота налога В (1 байт)
Номер шрифта ставки налога В (1 байт)
Номер шрифта суммы налога В (1 байт)
Номер шрифта названия налога Г (1 байт)
Номер шрифта оборота налога Г (1 байт)
Номер шрифта ставки налога Г (1 байт)
Номер шрифта суммы налога Г (1 байт)
Номер шрифта «ВСЕГО» (1 байт)
Номер шрифта суммы до начисления скидки (1 байт)
Номер шрифта «СКИДКА ХХ.ХХ %» (1 байт)
Номер шрифта суммы скидки на чек (1 байт)
Количество символов поля текстовой строки (1 байт)
Количество символов поля суммы итога (1 байт)
Количество символов поля суммы наличных (1 байт)
Количество символов поля суммы типа оплаты 2 (1 байт)
Количество символов поля суммы типа оплаты 3 (1 байт)
Количество символов поля суммы типа оплаты 4 (1 байт)
Количество символов поля суммы сдачи (1 байт)
Количество символов поля названия налога А (1 байт)
Количество символов поля оборота налога А (1 байт)
Количество символов поля ставки налога А (1 байт)
Количество символов поля суммы налога А (1 байт)
Количество символов поля названия налога Б (1 байт)
Количество символов поля оборота налога Б (1 байт)
Количество символов поля ставки налога Б (1 байт)
Количество символов поля суммы налога Б (1 байт)
Количество символов поля названия налога В (1 байт)
Количество символов поля оборота налога В (1 байт)
Количество символов поля ставки налога В (1 байт)
Количество символов поля суммы налога В (1 байт)
Количество символов поля названия налога Г (1 байт)
Количество символов поля оборота налога Г (1 байт)
Количество символов поля ставки налога Г (1 байт)
Количество символов поля суммы налога Г (1 байт)
Количество символов поля суммы до начисления скидки (1 байт)
Количество символов поля процентной скидки на чек (1 байт)
Количество символов поля суммы скидки на чек (1 байт)
Смещение поля текстовой строки в строке (1 байт)
Смещение поля «ИТОГ» в строке (1 байт)
Смещение поля суммы итога в строке (1 байт)
Смещение поля «НАЛИЧНЫМИ» в строке (1 байт)
Смещение поля суммы наличных в строке (1 байт)
Смещение поля названия типа оплаты 2 в строке (1 байт)
Смещение поля суммы типа оплаты 2 в строке (1 байт)
Смещение поля названия типа оплаты 3 в строке (1 байт)
Смещение поля суммы типа оплаты 3 в строке (1 байт)
Смещение поля названия типа оплаты 4 в строке (1 байт)
Смещение поля суммы типа оплаты 4 в строке (1 байт)
Смещение поля «СДАЧА» в строке (1 байт)
Смещение поля суммы сдачи в строке (1 байт)
Смещение поля названия налога А в строке (1 байт)
Смещение поля оборота налога А в строке (1 байт)
Смещение поля ставки налога А в строке (1 байт)
Смещение поля суммы налога А в строке (1 байт)
Смещение поля названия налога Б в строке (1 байт)
Смещение поля оборота налога Б в строке (1 байт)
Смещение поля ставки налога Б в строке (1 байт)
Смещение поля суммы налога Б в строке (1 байт)
Смещение поля названия налога В в строке (1 байт)
Смещение поля оборота налога В в строке (1 байт)
Смещение поля ставки налога В в строке (1 байт)
Смещение поля суммы налога В в строке (1 байт)
Смещение поля названия налога Г в строке (1 байт)
Смещение поля оборота налога Г в строке (1 байт)
Смещение поля ставки налога Г в строке (1 байт)
Смещение поля суммы налога Г в строке (1 байт)
Смещение поля «ВСЕГО» в строке (1 байт)
Смещение поля суммы до начисления скидки в строке (1 байт)
Смещение поля «СКИДКА ХХ.ХХ %» в строке (1 байт)
Смещение поля суммы скидки в строке (1 байт)
Номер строки ПД с первой строкой блока операции (1 байт)
Сумма наличных (5 байт)
Сумма типа оплаты 2 (5 байт)
Сумма типа оплаты 3 (5 байт)
Сумма типа оплаты 4 (5 байт)
Скидка в % на чек от 0 до 99,99 % (2 байта) 0000…9999
Налог 1 (1 байт) «0» – нет, «1»…«4» – налоговая группа
Налог 2 (1 байт) «0» – нет, «1»…«4» – налоговая группа
Налог 3 (1 байт) «0» – нет, «1»…«4» – налоговая группа
Налог 4 (1 байт) «0» – нет, «1»…«4» – налоговая группа
Текст (40 байт)
Ответ:		76H. Длина сообщения: 8 байт.
Код ошибки (1 байт)
Порядковый номер оператора (1 байт) 1…30
Сдача (5 байт) 0000000000…9999999999
Формирование стандартного закрытия чека на подкладном документе
Команда:	77H. Длина сообщения: 72 байта.
Пароль оператора (4 байта)
Номер строки ПД с первой строкой блока операции (1 байт)
Сумма наличных (5 байт)
Сумма типа оплаты 2 (5 байт)
Сумма типа оплаты 3 (5 байт)
Сумма типа оплаты 4 (5 байт)
Скидка в % на чек от 0 до 99,99 % (2 байта) 0000…9999
Налог 1 (1 байт) «0» – нет, «1»…«4» – налоговая группа
Налог 2 (1 байт) «0» – нет, «1»…«4» – налоговая группа
Налог 3 (1 байт) «0» – нет, «1»…«4» – налоговая группа
Налог 4 (1 байт) «0» – нет, «1»…«4» – налоговая группа
Текст (40 байт)
Ответ:		77H. Длина сообщения: 8 байт.
Код ошибки (1 байт)
Порядковый номер оператора (1 байт) 1…30
Сдача (5 байт) 0000000000…9999999999
Конфигурация подкладного документа
Команда:	78H. Длина сообщения: 209 байт.
Пароль оператора (4 байта)
Ширина подкладного документа в шагах (2 байта)*
Длина подкладного документа в шагах (2 байта)*
Ориентация печати – поворот в градусах по часовой стрелке (1 байт) «0» – 0º, «1» – 90º, «2» – 180º, «3» – 270º
Межстрочный интервал между 1-ой и 2-ой строками в шагах (1 байт)*
Межстрочный интервал между 2-ой и 3-ей строками в шагах (1 байт)*
аналогично для строк 3…199 в шагах (1 байт)*
Межстрочный интервал между 199-ой и 200-ой строками в шагах (1 байт)*
Ответ:		78H. Длина сообщения: 3 байта.
Код ошибки (1 байт)
Порядковый номер оператора (1 байт) 1…30
*   размер шага зависит от печатающего механизма конкретного фискального регистратора. Шаг по горизонтали не равен шагу по вертикали: эти параметры печатающего механизма указываются в инструкции по эксплуатации на ФР.
Установка стандартной конфигурации подкладного документа
Команда:	79H. Длина сообщения: 5 байт.
Пароль оператора (4 байта)
Ответ:		79H. Длина сообщения: 3 байта.
Код ошибки (1 байт)
Порядковый номер оператора (1 байт) 1…30
Заполнение буфера подкладного документа нефискальной информацией
Команда:	7AH. Длина сообщения: (6 + X) байт.
Пароль оператора (4 байта)
Номер строки (1 байт) 1…200
Печатаемая информация (X байт) символ с кодом 27 и следующий за ним символ не помещаются в буфер подкладного документа, а задают тип шрифта следующих символов; не более 250 байт
Ответ:		7AH. Длина сообщения: 3 байта.
Код ошибки (1 байт)
Порядковый номер оператора (1 байт) 1…30
Очистка строки буфера подкладного документа от нефискальной информации
Команда:	7BH. Длина сообщения: 6 байт.
Пароль оператора (4 байта)
Номер строки (1 байт) 1…200
Ответ:		7BH. Длина сообщения: 3 байта.
Код ошибки (1 байт)
Порядковый номер оператора (1 байт) 1…30
Очистка всего буфера подкладного документа от нефискальной информации
Команда:	7CH. Длина сообщения: 5 байт.
Пароль оператора (4 байта)
Ответ:		7CH. Длина сообщения: 3 байта.
Код ошибки (1 байт)
Порядковый номер оператора (1 байт) 1…30
Печать подкладного документа
Команда:	7DH. Длина сообщения: 7 байт.
Пароль оператора (4 байта)
Очистка нефискальной информации (1 байт) «0» – есть, «1» – нет
Тип печатаемой информации (1 байт) «0» – только нефискальная информация, «1» – только фискальная информация, «2» – вся информация
Ответ:		7DH. Длина сообщения: 3 байта.
Код ошибки (1 байт)
Порядковый номер оператора (1 байт) 1…30
Общая конфигурация подкладного документа
Команда:	7EH. Длина сообщения: 11 байт.
Пароль оператора (4 байта)
Ширина подкладного документа в шагах (2 байта)*
Длина подкладного документа в шагах (2 байта)*
Ориентация печати (1 байт) «0» – 0º; «1» – 90º; «2» – 180º; «3» – 270º
Межстрочный интервал между строками в шагах (1 байт)*
Ответ:		7EH. Длина сообщения: 3 байта.
Код ошибки (1 байт)
Порядковый номер оператора (1 байт) 1…30
*   размер шага зависит от печатающего механизма конкретного фискального регистратора. Шаг по горизонтали не равен шагу по вертикали: эти параметры печатающего механизма указываются в инструкции по эксплуатации на ФР.

*/

/*****************************************************************************
Продажа
Команда:	80H. Длина сообщения: 60 байт.
    Пароль оператора (4 байта)
    Количество (5 байт) 0000000000…9999999999
    Цена (5 байт) 0000000000…9999999999
    Номер отдела (1 байт) 0…16
    Налог 1 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 2 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 3 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 4 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Текст (40 байт)
Ответ:		80H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::printLine(QString& text)
{
    qDebug() << "printLine " << text;
    PrintStringCommand command;
    command.flags = SMFP_STATION_REC;
    command.text = text;
    return printString(command);
}

int ShtrihFiscalPrinter::readLineLength(int font)
{
    int len = 40;
    ReadFontCommand command;
    command.fontNumber = font;
    int rc = readFont(command);
    if ((rc == 0) && (command.charWidth != 0)) {
        len = command.printWidth / command.charWidth;
    }
    return len;
}

int ShtrihFiscalPrinter::getLineLength(int font)
{
    int len = 40;
    if ((font >= 1) && (font <= 10)) {
        if (lineLength[font - 1] == 0) {
            lineLength[font - 1] = readLineLength(font);
        }
        len = lineLength[font - 1];
    }
    qDebug() << "getLineLength: " << len;
    return len;
}

QStringList ShtrihFiscalPrinter::splitText(QString text, int font)
{
    qDebug() << "splitText";
    int lineLength = getLineLength(font);
    QStringList list = text.split(QRegExp("\n|\r\n|\r"));
    int index = 0;
    while (index < list.length()) {
        if (list.at(index).length() > lineLength) {
            QString s = list.at(index);
            list.removeAt(index);
            list.insert(index, s.left(lineLength));
            list.insert(index + 1, s.right(s.length() - lineLength));
        }
        index++;
    }
    return list;
}

int ShtrihFiscalPrinter::printText(QString& text)
{
    int rc = 0;
    QStringList lines = splitText(text, 1);

    if (lines.length() <= 1)
        return 0;
    while (lines.length() > 1) {
        rc = printLine(lines[0]);
        if (rc != 0)
            break;
        lines.removeAt(0);
    }
    text = lines[0];
    return rc;
}

int ShtrihFiscalPrinter::execute(int code, ReceiptItemCommand& data)
{
    data.resultCode = printText(data.text);
    if (failed(data.resultCode)) {
        return data.resultCode;
    }

    data.text = data.text.left(40);

    PrinterCommand command(code);
    command.write(usrPassword, 4);
    command.write(data.quantity, 5);
    command.write(data.price, 5);
    command.write(data.department, 1);
    command.write(data.tax1, 1);
    command.write(data.tax2, 1);
    command.write(data.tax3, 1);
    command.write(data.tax4, 1);
    command.write(data.text, 40);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
    }
    return data.resultCode;
}

int ShtrihFiscalPrinter::printSale(ReceiptItemCommand& data)
{
    qDebug() << "printSale";

    filter->printSale(EVENT_BEFORE, data);

    // Open fiscal day for fiscal storage
    int rc = openFiscalDay();
    if (failed(rc)) return rc;

    execute(0x80, data);
    if (succeeded(data.resultCode)){
        filter->printSale(EVENT_AFTER, data);
    }
    return data.resultCode;
}

/*****************************************************************************
Покупка
Команда:	81H. Длина сообщения: 60 байт.
    Пароль оператора (4 байта)
    Количество (5 байт) 0000000000…9999999999
    Цена (5 байт) 0000000000…9999999999
    Номер отдела (1 байт) 0…16
    Налог 1 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 2 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 3 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 4 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Текст (40 байт)
Ответ:		80H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::printBuy(ReceiptItemCommand& data)
{
    qDebug() << "printBuy";

    filter->printBuy(EVENT_BEFORE, data);
    // Open fiscal day for fiscal storage
    int rc = openFiscalDay();
    if (failed(rc)) return rc;

    execute(0x81, data);
    if (succeeded(data.resultCode)){
        filter->printBuy(EVENT_AFTER, data);
    }
    return data.resultCode;
}

/*****************************************************************************
Возврат продажи
Команда:	82H. Длина сообщения: 60 байт.
    Пароль оператора (4 байта)
    Количество (5 байт) 0000000000…9999999999
    Цена (5 байт) 0000000000…9999999999
    Номер отдела (1 байт) 0…16
    Налог 1 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 2 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 3 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 4 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Текст (40 байт)
Ответ:		80H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::printRetSale(ReceiptItemCommand& data)
{
    qDebug() << "printRetSale";

    filter->printRetSale(EVENT_BEFORE, data);
    // Open fiscal day for fiscal storage
    int rc = openFiscalDay();
    if (failed(rc)) return rc;

    execute(0x82, data);
    if (succeeded(data.resultCode)){
        filter->printRetSale(EVENT_AFTER, data);
    }
    return data.resultCode;
}

/*****************************************************************************
Возврат покупки
Команда:	83H. Длина сообщения: 60 байт.
    Пароль оператора (4 байта)
    Количество (5 байт) 0000000000…9999999999
    Цена (5 байт) 0000000000…9999999999
    Номер отдела (1 байт) 0…16
    Налог 1 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 2 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 3 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 4 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Текст (40 байт)
Ответ:		80H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::printRetBuy(ReceiptItemCommand& data)
{
    qDebug() << "printRetBuy";

    filter->printRetBuy(EVENT_BEFORE, data);
    // Open fiscal day for fiscal storage
    int rc = openFiscalDay();
    if (failed(rc)) return rc;

    execute(0x83, data);
    if (succeeded(data.resultCode)){
        filter->printRetBuy(EVENT_AFTER, data);
    }
    return data.resultCode;
}

/*****************************************************************************
Сторно
Команда:	84H. Длина сообщения: 60 байт.
    Пароль оператора (4 байта)
    Количество (5 байт) 0000000000…9999999999
    Цена (5 байт) 0000000000…9999999999
    Номер отдела (1 байт) 0…16
    Налог 1 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 2 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 3 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 4 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Текст (40 байт)
Ответ:		80H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::printStorno(ReceiptItemCommand& data)
{
    qDebug() << "printStorno";
    filter->printStorno(EVENT_BEFORE, data);
    execute(0x84, data);
    if (succeeded(data.resultCode)){
        filter->printStorno(EVENT_AFTER, data);
    }
    return data.resultCode;
}

/*****************************************************************************
Закрытие чека
Команда:	85H. Длина сообщения: 71 байт.
    Пароль оператора (4 байта)
    Сумма наличных (5 байт) 0000000000…9999999999
    Сумма типа оплаты 2 (5 байт) 0000000000…9999999999
    Сумма типа оплаты 3 (5 байт) 0000000000…9999999999
    Сумма типа оплаты 4 (5 байт) 0000000000…9999999999
    Скидка/Надбавка(в случае отрицательного значения) в % на чек от 0 до 99,99 % (2 байта со знаком) -9999…9999
    Налог 1 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 2 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 3 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 4 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Текст (40 байт)
Ответ:		85H. Длина сообщения: 8 байт.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
    Сдача (5 байт) 0000000000…9999999999
*****************************************************************************/

void ShtrihFiscalPrinter::beforeCloseReceipt()
{
    if (userNameEnabled && (deviceType.model == 19))
    {
        fsWriteTag(1048, userName);
    }
}

int ShtrihFiscalPrinter::closeReceipt(CloseReceiptCommand& data)
{
    qDebug() << "closeReceipt";

    beforeCloseReceipt();
    filter->closeReceipt(EVENT_BEFORE, data);
    PrinterCommand command(0x85);
    command.write(usrPassword, 4);
    command.write(data.amount1, 5);
    command.write(data.amount2, 5);
    command.write(data.amount3, 5);
    command.write(data.amount4, 5);
    command.write(data.discount, 2);
    command.write(data.tax1, 1);
    command.write(data.tax2, 1);
    command.write(data.tax3, 1);
    command.write(data.tax4, 1);
    command.write(data.text, 40);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
        data.change = command.read(5);
        filter->closeReceipt(EVENT_AFTER, data);
    }
    return data.resultCode;
}

int getNextDocNumber(int value)
{
    int result = value + 1;
    if (result > 9999) result = 9999;
    return result;
}

int ShtrihFiscalPrinter::closeReceipt2(CloseReceiptCommand2& data)
{
    qDebug() << "closeReceipt2";

    ReadLongStatusCommand status;
    int rc = readLongStatus(status);
    if (failed(rc)) return rc;

    ReadOperRegCommand command;
    command.number = 148 + ((status.mode & 0xF0) >> 4);
    rc = readRegister(command);
    if (failed(rc)) return rc;
    data.receiptNumber = getNextDocNumber(command.value);
    data.documentNumber = getNextDocNumber(status.documentNumber);

    return closeReceipt(data.closeCommand);
}

/*****************************************************************************
Скидка
Команда:	86H. Длина сообщения: 54 байт.
    Пароль оператора (4 байта)
    Сумма (5 байт) 0000000000…9999999999
    Налог 1 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 2 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 3 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 4 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Текст (40 байт)
Ответ:		86H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::printDiscount(AmountAjustCommand& data)
{
    qDebug() << "printDiscount";
    filter->printDiscount(EVENT_BEFORE, data);
    printAmountAjustment(0x86, data);
    if (succeeded(data.resultCode)){
        filter->printDiscount(EVENT_AFTER, data);
    }
    return data.resultCode;
}

/*****************************************************************************
Надбавка
Команда:	87H. Длина сообщения: 54 байт.
    Пароль оператора (4 байта)
    Сумма (5 байт) 0000000000…9999999999
    Налог 1 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 2 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 3 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 4 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Текст (40 байт)
Ответ:		87H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::printCharge(AmountAjustCommand& data)
{
    qDebug() << "printCharge";
    filter->printCharge(EVENT_BEFORE, data);
    printAmountAjustment(0x87, data);
    if (succeeded(data.resultCode)){
        filter->printCharge(EVENT_AFTER, data);
    }
    return data.resultCode;
}

int ShtrihFiscalPrinter::printAmountAjustment(int code, AmountAjustCommand& data)
{
    PrinterCommand command(code);
    command.write(usrPassword, 4);
    command.write(data.amount, 5);
    command.write(data.tax1, 1);
    command.write(data.tax2, 1);
    command.write(data.tax3, 1);
    command.write(data.tax4, 1);
    command.write(data.text, 40);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
    }
    return data.resultCode;
}

/*****************************************************************************
Аннулирование чека
Команда:	88H. Длина сообщения: 5 байт.
    Пароль оператора (4 байта)
Ответ:		88H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::cancelReceipt(PasswordCommand& data)
{
    qDebug() << "cancelReceipt";
    filter->cancelReceipt(EVENT_BEFORE, data);
    PrinterCommand command(0x88);
    command.write(usrPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)){
        data.operatorNumber = command.readChar();
        filter->cancelReceipt(EVENT_AFTER, data);
    }
    return data.resultCode;
}

/*****************************************************************************
Подытог чека
Команда:	89H. Длина сообщения: 5 байт.
    Пароль оператора (4 байта)
Ответ:		89H. Длина сообщения: 8 байт.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
    Подытог чека (5 байт) 0000000000…9999999999
*****************************************************************************/

int ShtrihFiscalPrinter::readSubtotal(ReadSubtotalCommand& data)
{
    qDebug() << "readSubtotal";

    PrinterCommand command(0x89);
    command.write(usrPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
        data.amount = command.read(5);
    }
    return data.resultCode;
}

/*****************************************************************************
Сторно скидки
Команда:	8AH. Длина сообщения: 54 байта.
    Пароль оператора (4 байта)
    Сумма (5 байт) 0000000000…9999999999
    Налог 1 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 2 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 3 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 4 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Текст (40 байт)
Ответ:		8AH. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::printDiscountStorno(AmountAjustCommand& data)
{
    qDebug() << "printDiscountStorno";
    filter->printDiscountStorno(EVENT_BEFORE, data);
    printAmountAjustment(0x8A, data);
    if (succeeded(data.resultCode)){
        filter->printDiscountStorno(EVENT_AFTER, data);
    }
    return data.resultCode;
}

/*****************************************************************************
Сторно надбавки
Команда:	8BH. Длина сообщения: 54 байта.
    Пароль оператора (4 байта)
    Сумма (5 байт) 0000000000…9999999999
    Налог 1 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 2 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 3 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Налог 4 (1 байт) «0» – нет, «1»…«4» – налоговая группа
    Текст (40 байт)
Ответ:		8BH. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::printChargeStorno(AmountAjustCommand& data)
{
    qDebug() << "printChargeStorno";
    filter->printChargeStorno(EVENT_BEFORE, data);
    printAmountAjustment(0x8B, data);
    if (succeeded(data.resultCode)){
        filter->printChargeStorno(EVENT_AFTER, data);
    }
    return data.resultCode;
}

/*****************************************************************************
Повтор документа
Команда:	8CH. Длина сообщения: 5 байт.
    Пароль оператора (4 байта)
Ответ:		8CH. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::printCopy(PasswordCommand& data)
{
    qDebug() << "printCopy";
    PrinterCommand command(0x8C);
    command.write(usrPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)){
        data.operatorNumber = command.readChar();
    }
    return data.resultCode;
}

/*****************************************************************************
Открыть чек
Команда:	8DH. Длина сообщения: 6 байт.
    Пароль оператора (4 байта)
    Тип документа (1 байт):  0 – продажа;
    1 – покупка;
    2 – возврат продажи;
    3 – возврат покупки
Ответ:		8DH. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::openFiscalDay()
{
    qDebug() << "openFiscalDay";

    int rc = 0;
    if (!isDayOpened())
    {
         PasswordCommand command;
         rc = openDay(command);
         if (succeeded(rc)){
             rc = waitForPrinting();
         }
    }
    return rc;
}

int ShtrihFiscalPrinter::openReceipt(OpenReceiptCommand& data)
{
    qDebug() << "openReceipt";

    filter->openReceipt(EVENT_BEFORE, data);
    // Open fiscal day for fiscal storage
    int rc = openFiscalDay();
    if (failed(rc)) return rc;

    PrinterCommand command(0x8D);
    command.write(usrPassword, 4);
    command.write8(data.receiptType);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
        filter->openReceipt(EVENT_AFTER, data);
    }
    return data.resultCode;
}

/*****************************************************************************
Отчет ЭКЛЗ по отделам в заданном диапазоне дат
Команда: A0H. Длина сообщения: 13 байт.
    Пароль системного администратора (4 байта)
    Тип отчета (1 байт) «0» – короткий, «1» – полный
    Номер отдела (1 байт) 1…16
    Дата первой смены (3 байта) ДД-ММ-ГГ
    Дата последней смены (3 байта) ДД-ММ-ГГ
Ответ: A0H. Длина сообщения: 2 байта.
    Код ошибки (1 байт)

Примечание: Время выполнения команды – до 150 секунд.
*****************************************************************************/

int ShtrihFiscalPrinter::printEJDepartmentReportOnDates(PrintEJDepartmentReportOnDates& data)
{
    qDebug() << "printEJDepartmentReportOnDates";
    PrinterCommand command(0xA0);
    command.write(sysPassword, 4);
    command.write(data.reportType, 1);
    command.write(data.department, 1);
    command.write(data.date1);
    command.write(data.date2);
    data.resultCode = send(command);
    return data.resultCode;
}

/*****************************************************************************
Отчет ЭКЛЗ по отделам в заданном диапазоне номеров смен
Команда: A1H. Длина сообщения: 11 байт.
    Пароль системного администратора (4 байта)
    Тип отчета (1 байт) «0» – короткий, «1» – полный
    Номер отдела (1 байт) 1…16
    Номер первой смены (2 байта) 0000…2100
    Номер последней смены (2 байта) 0000…2100
Ответ: A1H. Длина сообщения: 2 байта.
    Код ошибки (1 байт)

Примечание: Время выполнения команды – до 150 секунд.
*****************************************************************************/

int ShtrihFiscalPrinter::printEJDepartmentReportOnDays(PrintEJDepartmentReportOnDays& data)
{
    qDebug() << "printEJDepartmentReportOnDays";
    PrinterCommand command(0xA1);
    command.write(sysPassword, 4);
    command.write(data.reportType, 1);
    command.write(data.department, 1);
    command.write(data.day1, 2);
    command.write(data.day2, 2);
    data.resultCode = send(command);
    return data.resultCode;
}

/*****************************************************************************
Отчет ЭКЛЗ по закрытиям смен в заданном диапазоне дат
Команда: A2H. Длина сообщения: 12 байт.
    Пароль системного администратора (4 байта)
    Тип отчета (1 байт) «0» – короткий, «1» – полный
    Дата первой смены (3 байта) ДД-ММ-ГГ
    Дата последней смены (3 байта) ДД-ММ-ГГ
Ответ: A2H. Длина сообщения: 2 байта.
    Код ошибки (1 байт)

Примечание: Время выполнения команды – до 100 секунд.
*****************************************************************************/

int ShtrihFiscalPrinter::printEJDayReportOnDates(PrintEJDayReportOnDates& data)
{
    qDebug() << "printEJDayReportOnDates";
    PrinterCommand command(0xA1);
    command.write(sysPassword, 4);
    command.write(data.reportType, 1);
    command.write(data.date1);
    command.write(data.date2);
    data.resultCode = send(command);
    return data.resultCode;
}

/*****************************************************************************
Отчет ЭКЛЗ по закрытиям смен в заданном диапазоне номеров смен
Команда: A3H. Длина сообщения: 10 байт.
    Пароль системного администратора (4 байта)
    Тип отчета (1 байт) «0» – короткий, «1» – полный
    Номер первой смены (2 байта) 0000…2100
    Номер последней смены (2 байта) 0000…2100
Ответ: A3H. Длина сообщения: 2 байта.
    Код ошибки (1 байт)

Примечание: Время выполнения команды – до 100 секунд.
*****************************************************************************/

int ShtrihFiscalPrinter::printEJDayReportOnDays(PrintEJDayReportOnDays& data)
{
    qDebug() << "printEJDayReportOnDays";
    PrinterCommand command(0xA3);
    command.write(sysPassword, 4);
    command.write(data.reportType, 1);
    command.write(data.day1, 2);
    command.write(data.day2, 2);
    data.resultCode = send(command);
    return data.resultCode;
}

/*****************************************************************************
Итоги смены по номеру смены ЭКЛЗ
Команда: A4H. Длина сообщения: 7 байт.
    Пароль системного администратора (4 байта)
    Номер смены (2 байта) 0000…2100
Ответ: A4H. Длина сообщения: 2 байта.
    Код ошибки (1 байт)

Примечание: Время выполнения команды – до 40 секунд.
*****************************************************************************/

int ShtrihFiscalPrinter::printEJDayTotal(PrintEJDayTotal& data)
{
    qDebug() << "printEJDayTotal";
    PrinterCommand command(0xA4);
    command.write(sysPassword, 4);
    command.write(data.day, 2);
    data.resultCode = send(command);
    return data.resultCode;
}

/*****************************************************************************
Платежный документ из ЭКЛЗ по номеру КПК
Команда: A5H. Длина сообщения: 9 байт.
    Пароль системного администратора (4 байта)
    Номер КПК (4 байта) 00000000…99999999
Ответ: A5H. Длина сообщения: 2 байта.
    Код ошибки (1 байт)

Примечание: Время выполнения команды – до 40 секунд.
*****************************************************************************/

int ShtrihFiscalPrinter::printEJDocument(PrintEJDocument& data)
{
    qDebug() << "printEJDocument";
    PrinterCommand command(0xA5);
    command.write(sysPassword, 4);
    command.write(data.macNumber, 4);
    data.resultCode = send(command);
    return data.resultCode;
}

/*****************************************************************************
Контрольная лента из ЭКЛЗ по номеру смены
Команда: A6H. Длина сообщения: 7 байт.
    Пароль системного администратора (4 байта)
    Номер смены (2 байта) 0000…2100
Ответ: A6H. Длина сообщения: 2 байта.
    Код ошибки (1 байт)

Примечание: Время выполнения команды – до 40 секунд.
*****************************************************************************/

int ShtrihFiscalPrinter::printEJDayReport(PrintEJDayReport& data)
{
    qDebug() << "printEJDayReport";
    PrinterCommand command(0xA6);
    command.write(sysPassword, 4);
    command.write(data.day, 2);
    data.resultCode = send(command);
    return data.resultCode;
}

/*****************************************************************************
Прерывание полного отчета ЭКЛЗ или контрольной ленты
ЭКЛЗ или печати платежного документа ЭКЛЗ

Команда: A7H. Длина сообщения: 5 байт.
    Пароль системного администратора (4 байта)
Ответ: A7H. Длина сообщения: 2 байта.
    Код ошибки (1 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::stopEJPrint()
{
    qDebug() << "stopEJPrint";
    PrinterCommand command(0xA7);
    command.write(sysPassword, 4);
    return send(command);
}

/*****************************************************************************
Итог активизации ЭКЛЗ
Команда: A8H. Длина сообщения: 5 байт.
    Пароль системного администратора (4 байта)
Ответ: A8H. Длина сообщения: 2 байта.
    Код ошибки (1 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::printEJActivationReport()
{
    qDebug() << "printEJActivationReport";
    PrinterCommand command(0xA8);
    command.write(sysPassword, 4);
    return send(command);
}

/*****************************************************************************
Активизация ЭКЛЗ
Команда: A9H. Длина сообщения: 5 байт.
    Пароль системного администратора (4 байта)
Ответ: A9H. Длина сообщения: 2 байта.
    Код ошибки (1 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::activateEJ()
{
    qDebug() << "activateEJ";
    PrinterCommand command(0xA9);
    command.write(sysPassword, 4);
    return send(command);
}

/*****************************************************************************
Закрытие архива ЭКЛЗ
Команда: AAH. Длина сообщения: 5 байт.
    Пароль системного администратора (4 байта)
Ответ: AAH. Длина сообщения: 2 байта.
    Код ошибки (1 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::closeEJArchive()
{
    qDebug() << "closeEJArchive";
    PrinterCommand command(0xAA);
    command.write(sysPassword, 4);
    return send(command);
}

/*****************************************************************************
Запрос регистрационного номера ЭКЛЗ
    Команда: ABH. Длина сообщения: 5 байт.
    Пароль системного администратора (4 байта)
Ответ: ABH. Длина сообщения: 7 байт.
    Код ошибки (1 байт)
    Номер ЭКЛЗ (5 байт) 0000000000…9999999999
*****************************************************************************/

int ShtrihFiscalPrinter::readEJSerial(ReadEJSerial& data)
{
    qDebug() << "readEJSerial";
    PrinterCommand command(0xAB);
    command.write(sysPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)){
        data.serial = command.read(5);
    }
    return data.resultCode;
}

/*****************************************************************************
Прекращение ЭКЛЗ
Команда: ACH. Длина сообщения: 5 байт.
    Пароль системного администратора (4 байта)
Ответ: ACH. Длина сообщения: 2 байта.
    Код ошибки (1 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::cancelEJDocument()
{
    qDebug() << "cancelEJDocument";
    PrinterCommand command(0xAC);
    command.write(sysPassword, 4);
    return send(command);
}

/*****************************************************************************
Запрос состояния по коду 1 ЭКЛЗ
Команда: ADH. Длина сообщения: 5 байт.
    Пароль системного администратора (4 байта)
Ответ: ADH. Длина сообщения: 22 байта.
    Код ошибки (1 байт)
    Итог документа последнего КПК (5 байт) 0000000000…9999999999
    Дата последнего КПК (3 байта) ДД-ММ-ГГ
    Время последнего КПК (2 байта) ЧЧ-ММ
    Номер последнего КПК (4 байта) 00000000…99999999
    Номер ЭКЛЗ (5 байт) 0000000000…9999999999
    Флаги ЭКЛЗ (см. описание ЭКЛЗ) (1 байт)
*****************************************************************************/

EJFlags ShtrihFiscalPrinter::decodeEJFlags(uint8_t value){
       EJFlags flags;
       flags.docType = value & 0x03;
       flags.isArchiveOpened = testBit(value, 2);
       flags.isActivated = testBit(value, 3);
       flags.isReportMode = testBit(value, 4);
       flags.isDocOpened = testBit(value, 5);
       flags.isDayOpened = testBit(value, 6);
       flags.isFatalError = testBit(value, 7);
       return flags;
}

int ShtrihFiscalPrinter::readEJStatus1(ReadEJStatus1& data)
{
    qDebug() << "readEJStatus1";
    PrinterCommand command(0xAD);
    command.write(sysPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)){
        data.docAmount = command.read(5);
        data.docDate = command.readDate();
        data.docTime = command.readTime2();
        data.macNumber = command.read(4);
        data.serial = command.read(5);
        data.flags = decodeEJFlags(command.read(1));
    }
    return data.resultCode;
}

/*****************************************************************************
Запрос состояния по коду 2 ЭКЛЗ
Команда: AEH. Длина сообщения: 5 байт.
    Пароль системного администратора (4 байта)
Ответ: AEH. Длина сообщения: 28 байт.
    Код ошибки (1 байт)
    Номер смены (2 байта) 0000…2100
    Итог продаж (6 байт) 000000000000…999999999999
    Итог покупок (6 байт) 000000000000…999999999999
    Итог возвратов продаж (6 байт) 000000000000…999999999999
    Итог возвратов покупок (6 байт) 000000000000…999999999999
*****************************************************************************/

int ShtrihFiscalPrinter::readEJStatus2(ReadEJStatus2& data)
{
    qDebug() << "readEJStatus2";
    PrinterCommand command(0xAE);
    command.write(sysPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)){
        data.dayNumber = command.read(2);
        data.saleTotal = command.read(6);
        data.buyTotal = command.read(6);
        data.retSaleTotal = command.read(6);
        data.retBuyTotal = command.read(6);
    }
    return data.resultCode;
}

/*****************************************************************************
Тест целостности архива ЭКЛЗ
Команда: AFH. Длина сообщения: 5 байт.
    Пароль системного администратора (4 байта)
Ответ: AFH. Длина сообщения: 2 байта.
    Код ошибки (1 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::testEJArchive()
{
    qDebug() << "testEJArchive";
    PrinterCommand command(0xAF);
    command.write(sysPassword, 4);
    return send(command);
}

/*****************************************************************************
Запрос версии ЭКЛЗ
Команда: B1H. Длина сообщения: 5 байт.
    Пароль системного администратора (4 байта)
Ответ: B1H. Длина сообщения: 20 байт.
    Код ошибки (1 байт)
    Строка символов в кодировке WIN1251 (18 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::readEJVersion(ReadEJVersion& data)
{
    qDebug() << "readEJVersion";
    PrinterCommand command(0xB1);
    command.write(sysPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)){
        data.version = command.readStr();
    }
    return data.resultCode;
}

/*****************************************************************************
Инициализация архива ЭКЛЗ
Команда: B2H. Длина сообщения: 5 байт.
    Пароль системного администратора (4 байта)
Ответ: B2H. Длина сообщения: 2 байта.
    Код ошибки (1 байт)
Примечание: Команда работает только с отладочным комплектом ЭКЛЗ. Время
выполнения команды – до 120 секунд.
*****************************************************************************/

int ShtrihFiscalPrinter::initEJArchive()
{
    qDebug() << "initEJArchive";
    PrinterCommand command(0xB2);
    command.write(sysPassword, 4);
    return send(command);
}

/*****************************************************************************
Запрос данных отчѐта ЭКЛЗ
Команда: B3H. Длина сообщения: 5 байт.
    Пароль системного администратора (4 байта)
Ответ: B3H. Длина сообщения: (2+Х) байт.
    Код ошибки (1 байт)
    Строка или фрагмент отчета (см. спецификацию ЭКЛЗ) (X байт)
*****************************************************************************/

int ShtrihFiscalPrinter::readEJDocumentLine(ReadEJDocumentLine& data)
{
    qDebug() << "readEJData";
    PrinterCommand command(0xB3);
    command.write(sysPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)){
        data.text = command.readStr();
    }
    return data.resultCode;
}

/*****************************************************************************
Запрос контрольной ленты ЭКЛЗ
Команда: B4H. Длина сообщения: 7 байт.
    Пароль системного администратора (4 байта)
    Номер смены (2 байта) 0000…2100
Ответ: B4H. Длина сообщения: 18 байт.
    Код ошибки (1 байт)
    Тип ККМ – строка символов в кодировке WIN1251 (16 байт)
    Примечание: Время выполнения команды – до 40 секунд.
*****************************************************************************/

int ShtrihFiscalPrinter::readEJJournal(ReadEJJournal& data)
{
    qDebug() << "readEJJournal";
    PrinterCommand command(0xB4);
    command.write(sysPassword, 4);
    command.write(data.dayNumber, 2);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)){
        data.text = command.readStr();
    }
    return data.resultCode;
}


/*****************************************************************************
    Запрос документа ЭКЛЗ

    Команда: B5H. Длина сообщения: 9 байт.
        Пароль системного администратора (4 байта)
        Номер КПК (4 байта) 00000000…99999999

    Ответ: B5H. Длина сообщения: 18 байт.
        Код ошибки (1 байт)
        Тип ККМ – строка символов в кодировке WIN1251 (16 байт)

        Примечание: Время выполнения команды – до 40 секунд.

*****************************************************************************/

int ShtrihFiscalPrinter::readEJDocument(ReadEJDocument& data)
{
    qDebug() << "readEJDocument";
    PrinterCommand command(0xB5);
    command.write(sysPassword, 4);
    command.write(data.macNumber, 2);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)){
        data.text = command.readStr();
    }
    return data.resultCode;
}

/*****************************************************************************
    Запрос отчѐта ЭКЛЗ по отделам в заданном диапазоне дат

    Команда: B6H. Длина сообщения: 13 байт.
        Пароль системного администратора (4 байта)
        Тип отчета (1 байт) «0» – короткий, «1» – полный
        Номер отдела (1 байт) 1…16
        Дата первой смены (3 байта) ДД-ММ-ГГ
        Дата последней смены (3 байта) ДД-ММ-ГГ

    Ответ: B6H. Длина сообщения: 18 байт.
        Код ошибки (1 байт)
        Тип ККМ – строка символов в кодировке WIN1251 (16 байт)

        Примечание: Время выполнения команды – до 150 секунд.
*****************************************************************************/

int ShtrihFiscalPrinter::readEJDepartmentReportByDates(ReadEJDepartmentReportByDates& data)
{
    qDebug() << "readEJDepartmentReportByDates";
    PrinterCommand command(0xB6);
    command.write(sysPassword, 4);
    command.write(data.reportType, 1);
    command.write(data.department, 1);
    command.write(data.date1);
    command.write(data.date2);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)){
        data.text = command.readStr();
    }
    return data.resultCode;
}

/*****************************************************************************
    Запрос отчѐта ЭКЛЗ по отделам в заданном диапазоне
    номеров смен

    Команда: B7H. Длина сообщения: 11 байт.
        Пароль системного администратора (4 байта)
        Тип отчета (1 байт) «0» – короткий, «1» – полный
        Номер отдела (1 байт) 1…16
        Номер первой смены (2 байта) 0000…2100
        Номер последней смены (2 байта) 0000…2100
    Ответ: B7H. Длина сообщения: 18 байт.
        Код ошибки (1 байт)
        Тип ККМ – строка символов в кодировке WIN1251 (16 байт)

        Примечание: Время выполнения команды – до 150 секунд.

*****************************************************************************/

int ShtrihFiscalPrinter::readEJDepartmentReportByDays(ReadEJDepartmentReportByDays& data)
{
    qDebug() << "readEJDepartmentReportByDays";
    PrinterCommand command(0xB7);
    command.write(sysPassword, 4);
    command.write(data.reportType, 1);
    command.write(data.department, 1);
    command.write(data.day1, 2);
    command.write(data.day2, 2);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)){
        data.text = command.readStr();
    }
    return data.resultCode;
}

/*****************************************************************************
    Запрос отчѐта ЭКЛЗ по закрытиям смен в заданном
    диапазоне дат

    Команда: B8H. Длина сообщения: 12 байт.
        Пароль системного администратора (4 байта)
        Тип отчета (1 байт) «0» – короткий, «1» – полный
        Дата первой смены (3 байта) ДД-ММ-ГГ
        Дата последней смены (3 байта) ДД-ММ-ГГ

    Ответ: B8H. Длина сообщения: 18 байт.
        Код ошибки (1 байт)
        Тип ККМ – строка символов в кодировке WIN1251 (16 байт)

        Примечание: Время выполнения команды – до 100 секунд.
*****************************************************************************/

int ShtrihFiscalPrinter::readEJReportByDates(ReadEJReportByDates& data)
{
    qDebug() << "readEJReportByDates";
    PrinterCommand command(0xB8);
    command.write(sysPassword, 4);
    command.write(data.reportType, 1);
    command.write(data.date1);
    command.write(data.date2);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)){
        data.text = command.readStr();
    }
    return data.resultCode;
}

/*****************************************************************************
    Запрос отчѐта ЭКЛЗ по закрытиям смен в заданном
    диапазоне номеров смен

    Команда: B9H. Длина сообщения: 10 байт.
        Пароль системного администратора (4 байта)
        Тип отчета (1 байт) «0» – короткий, «1» – полный
        Номер первой смены (2 байта) 0000…2100
        Номер последней смены (2 байта) 0000…2100

    Ответ: B9H. Длина сообщения: 18 байт.
        Код ошибки (1 байт)
        Тип ККМ – строка символов в кодировке WIN1251 (16 байт)

        Примечание: Время выполнения команды – до 100 секунд.
*****************************************************************************/

int ShtrihFiscalPrinter::readEJReportByDays(ReadEJReportByDays& data)
{
    qDebug() << "readEJReportByDays";
    PrinterCommand command(0xB9);
    command.write(sysPassword, 4);
    command.write(data.reportType, 1);
    command.write(data.day1, 2);
    command.write(data.day2, 2);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)){
        data.text = command.readStr();
    }
    return data.resultCode;
}

/*****************************************************************************

    Запрос в ЭКЛЗ итогов смены по номеру смены

    Команда: BAH. Длина сообщения: 7 байт.
        Пароль системного администратора (4 байта)
        Номер смены (2 байта) 0000…2100

    Ответ: BAH. Длина сообщения: 18 байт.
        Код ошибки (1 байт)
        Тип ККМ – строка символов в кодировке WIN1251 (16 байт)

    Примечание: Время выполнения команды – до 40 секунд.

*****************************************************************************/

int ShtrihFiscalPrinter::readEJDayTotals(ReadEJDayTotals& data)
{
    qDebug() << "readEJDayTotals";
    PrinterCommand command(0xBA);
    command.write(sysPassword, 4);
    command.write(data.day, 2);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)){
        data.text = command.readStr();
    }
    return data.resultCode;
}

/*****************************************************************************
    Запрос итога активизации ЭКЛЗ
    Команда: BBH. Длина сообщения: 5 байт.
        Пароль системного администратора (4 байта)
    Ответ: BBH. Длина сообщения: 18 байт.
        Код ошибки (1 байт)
        Тип ККМ – строка символов в кодировке WIN1251 (16 байт)
*****************************************************************************/

int ShtrihFiscalPrinter::readEJActivation(ReadEJActivation& data)
{
    qDebug() << "readEJActivation";
    PrinterCommand command(0xBB);
    command.write(sysPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)){
        data.text = command.readStr();
    }
    return data.resultCode;
}

/*****************************************************************************
    Вернуть ошибку ЭКЛЗ
    Команда: BCH. Длина сообщения: 6 байт.
        Пароль системного администратора (4 байта)
        Код ошибки (1 байт)
    Ответ: BCH. Длина сообщения: 2 байта.
        Код ошибки (1 байт)

        Примечание: Команда работает только с отладочным
*****************************************************************************/

int ShtrihFiscalPrinter::setEJErrorCode(uint8_t code)
{
    qDebug() << "setEJErrorCode";
    PrinterCommand command(0xBC);
    command.write(sysPassword, 4);
    command.write(code, 1);
    return send(command);
}

/*****************************************************************************
Продолжение печати
Команда:	B0H. Длина сообщения: 5 байт.
    Пароль оператора, администратора или системного администратора (4 байта)
Ответ:		B0H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::continuePrint(PasswordCommand& data)
{
    qDebug() << "continuePrint";
    PrinterCommand command(0xB0);
    command.write(usrPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)){
        data.operatorNumber = command.readChar();
    }
    return data.resultCode;
}

/*****************************************************************************
Загрузка графики
Команда: 	C0H. Длина сообщения: 46 байт.
    Пароль оператора (4 байта)
    Номер линии (1 байт) 0…199
    Графическая информация (40 байт)
Ответ:		C0H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::loadGraphics1(LoadGraphicsCommand& data)
{
    qDebug() << "loadGraphics1(" << data.line << ")";

    PrinterCommand command(0xC0);
    command.write(usrPassword, 4);
    command.write8(data.line);
    command.write(data.data, 40);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
    }
    return data.resultCode;
}

/*****************************************************************************
Печать графики
Команда:	C1H. Длина сообщения: 7 байт.
    Пароль оператора (4 байта)
    Начальная линия (1 байт) 1…200
    Конечная линия (1 байт) 1…200
Ответ:		С1H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::printGraphics1(PrintGraphicsCommand& data)
{
    qDebug() << "printGraphics1";

    PrinterCommand command(0xC1);
    command.write(usrPassword, 4);
    command.write8(data.line1);
    command.write8(data.line2);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
    }
    return data.resultCode;
}

/*****************************************************************************
Печать штрих-кода
Команда:	C2H. Длина сообщения: 10 байт.
    Пароль оператора (4 байта)
    Штрих-код (5 байт) 000000000000…999999999999
Ответ:		С2H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::printBarcode(PrintBarcodeCommand& data)
{
    qDebug() << "printBarcode";

    PrinterCommand command(0xC2);
    command.write(usrPassword, 4);
    command.write(data.barcode, 5);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
    }
    return data.resultCode;
}

/*****************************************************************************
Печать расширенной графики
Команда:	C3H. Длина сообщения: 9 байт.
    Пароль оператора (4 байта)
    Начальная линия (2 байта) 1…1200
    Конечная линия (2 байта) 1…1200
Ответ:		C3H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::printGraphics2(PrintGraphicsCommand& data)
{
    qDebug() << "printGraphics2";

    PrinterCommand command(0xC3);
    command.write(usrPassword, 4);
    command.write16(data.line1);
    command.write16(data.line2);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
    }
    return data.resultCode;
}

/*****************************************************************************
Загрузка расширенной графики
Команда: 	C4H. Длина сообщения: 47 байт.
    Пароль оператора (4 байта)
    Номер линии (2 байта) 0…1199
    Графическая информация (40 байт)
Ответ:		С4H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::loadGraphics2(LoadGraphicsCommand& data)
{
    qDebug() << "loadGraphics2";

    PrinterCommand command(0xC4);
    command.write(usrPassword, 4);
    command.write16(data.line);
    command.write(data.data, 40);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
    }
    return data.resultCode;
}

/*****************************************************************************
Печать линии
Команда: 	C5H. Длина сообщения: X + 7 байт.
    Пароль оператора (4 байта)
    Количество повторов (2 байта)
    Графическая информация (X байт)
Ответ:		C5H. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::printGraphicsLine(GraphicsLineCommand& data)
{
    qDebug() << "printGraphicsLine";

    PrinterCommand command(0xC5);
    command.write(usrPassword, 4);
    command.write16(data.height);
    command.write(data.data);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
    }
    return data.resultCode;
}

/*****************************************************************************
Открыть смену
Команда:	E0H. Длина сообщения: 5байт.
    Пароль оператора (4 байта)
Ответ:		E0H. Длина сообщения: 2 байта.
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::openDay(PasswordCommand& data)
{
    qDebug() << "openDay";

    PrinterCommand command(0xE0);
    command.write(usrPassword, 4);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)){
        data.operatorNumber = command.readChar();
    }
    return data.resultCode;
}

/*****************************************************************************
Получить тип устройства
Команда:	FCH. Длина сообщения: 1 байт.
Ответ:		FCH. Длина сообщения: (8+X) байт.
    Код ошибки (1 байт)
    Тип устройства (1 байт) 0…255
    Подтип устройства (1 байт) 0…255
    Версия протокола для данного устройства (1 байт) 0…255
    Подверсия протокола для данного устройства (1 байт) 0…255
    Модель устройства (1 байт) 0…255
    Язык устройства (1 байт) 0…255 русский – 0; английский – 1;
    Название устройства – строка символов в кодировке WIN1251. Количество байт, отводимое под название устройства, определяется в каждом конкретном случае самостоятельно разработчиками устройства (X байт)
*****************************************************************************/

int ShtrihFiscalPrinter::readDeviceType(DeviceTypeCommand& data)
{
    qDebug() << "readDeviceType";

    PrinterCommand command(0xFC);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.deviceType1 = command.readChar();
        data.deviceType2 = command.readChar();
        data.protocolVersion1 = command.readChar();
        data.protocolVersion2 = command.readChar();
        data.model = command.readChar();
        data.language = command.readChar();
        data.name = command.readStr();
    }
    return data.resultCode;
}

/*****************************************************************************
Печать графики с масштабированием1
Команда:	4FH. Длина сообщения: 9 байт.
    Пароль оператора (4 байта)
    Начальная линия (1 байт) 1…200
    Конечная линия (1 байт) 1…200
    Масштабирование точки по вертикали (1 байт) «0» – нет
    Масштабирование точки по горизонтали (1 байт) «0» – нет
Ответ:		4FH. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1...30
*****************************************************************************/

int ShtrihFiscalPrinter::printScaledGraphics(ScaledGraphicsCommand& data)
{
    qDebug() << "printScaledGraphics";

    PrinterCommand command(0x4F);
    command.write(usrPassword, 4);
    command.write8(data.line1);
    command.write8(data.line2);
    command.write8(data.vscale);
    command.write8(data.hscale);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
    }
    return data.resultCode;
}

/*****************************************************************************
Загрузка графики-5121
Команда: 	4EH. Длина сообщения: 11+X2 байт.
    Пароль оператора (4 байта)
    Длина линии L (1 байт) 1…40 для T = 0; 1…643 для T = 1
    Номер начальной линии (2 байта) 1…12004 для T = 0; 1…6005 для T = 1
    Количество последующих линий N6 (2 байта) 1…12004 для T = 0; 1…6005 для T = 1
    Тип графического буфера T (1 байт) 0 – для команд [расширенной] графики; 1 – для команд графики-512
    Графическая информация (X2 = N * L байт)
Ответ:		4EH. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::loadGraphics3(LoadGraphics3Command& data)
{
    qDebug() << "loadGraphics3" << data.startLine;

    PrinterCommand command(0x4E);
    command.write(usrPassword, 4);
    command.write8(data.lineLength);
    command.write16(data.startLine);
    command.write16(data.lineCount);
    command.write8(data.bufferType);
    command.write(data.data, data.lineLength);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
    }
    return data.resultCode;
}

/*****************************************************************************
Печать графики-512 с масштабированием1
Команда:	4DH. Длина сообщения: 12 байт.
    Пароль оператора (4 байта)
    Начальная линия (2 байта) 1…600
    Конечная линия (2 байта) 1…600
    Коэффициент масштабирования точки по вертикали (1 байт) 1…255
    Коэффициент масштабирования точки по горизонтали (1 байт) 1…6
    Флаги (1 байт) Бит 0 – контрольная лента2, Бит 1 – чековая лента, Бит 23 – подкладной документ, Бит 34 – слип чек; Бит 75 – отложенная печать графики
Ответ:		4DH. Длина сообщения: 3 байта.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
*****************************************************************************/

int ShtrihFiscalPrinter::printGraphics3(PrintGraphics3Command& data)
{
    qDebug() << "printGraphics3" << data.startLine;

    PrinterCommand command(0x4D);
    command.write(usrPassword, 4);
    command.write16(data.startLine);
    command.write16(data.endLine);
    command.write8(data.vScale);
    command.write8(data.hScale);
    command.write8(data.flags);
    data.resultCode = send(command);
    if (succeeded(data.resultCode)) {
        data.operatorNumber = command.readChar();
    }
    return data.resultCode;
}

int ShtrihFiscalPrinter::getBaudRateCode(int value)
{
    switch (value) {
    case 2400:
        return 0;
    case 4800:
        return 1;
    case 9600:
        return 2;
    case 19200:
        return 3;
    case 38400:
        return 4;
    case 57600:
        return 5;
    case 115200:
        return 6;
    default:
        throw new TextException("Invalid baudrate value");
    }
}

int ShtrihFiscalPrinter::getBaudRateValue(int value)
{
    switch (value) {
    case 0:
        return 2400;
    case 1:
        return 4800;
    case 2:
        return 9600;
    case 3:
        return 19200;
    case 4:
        return 38400;
    case 5:
        return 57600;
    case 6:
        return 115200;
    default:
        throw new TextException("Invalid baudrate code");
    }
}

int ShtrihFiscalPrinter::getTimeoutCode(int value)
{
    if ((value >= 0) && (value <= 150)) {
        return value;
    }
    if ((value > 150) && (value <= 15000)) {
        return ((value / 150) + 149);
    }
    return ((value / 15000) + 248);
}

int ShtrihFiscalPrinter::getTimeoutValue(int value)
{
    if ((value >= 0) && (value <= 150)) {
        return value;
    }
    if ((value > 150) && (value <= 249)) {
        return (value - 149) * 150;
    }
    return (value - 248) * 15000;
}

QString ShtrihFiscalPrinter::getErrorText2(int code)
{
    QByteArray ba;
    ba.append(code);
    QString result = "0x" + ba.toHex() + ", " + getErrorText(code);
    return result;
}

QString ShtrihFiscalPrinter::getFSErrorText(int code)
{
    switch (code){
      case 0x01: return tr("FS: Unknown command or invalid format");
      case 0x02: return tr("FS: Incorrect status");
      case 0x03: return tr("FS: Error, read extended error code");
      case 0x04: return tr("FS: CRC error, read extended error code");
      case 0x05: return tr("FS: Device expired");
      case 0x06: return tr("FS: Archive overflow error");
      case 0x07: return tr("FS: Invalid date/time value");
      case 0x08: return tr("FS: No data available");
      case 0x09: return tr("FS: Invalid parameter value");
      case 0x10: return tr("FS: TLV size too large");
      case 0x11: return tr("FS: No transport connection ");
      case 0x12: return tr("FS: Cryptographic coprocessor resource exhausted");
      case 0x14: return tr("FS: Documents resource exhausted");
      case 0x15: return tr("FS: Maximum send time limit reached");
      case 0x16: return tr("FS: Fiscal day last for 24 hours");
      case 0x17: return tr("FS: Invalid time range betseen two operations");
      case 0x20: return tr("FS: Server message can not be received");
  default:
    return tr("Unknown error");
    }
}

QString ShtrihFiscalPrinter::getErrorTextFS(int code)
{
    if ((code >= 0)&&(code <= 0x1F)){
        return getFSErrorText(code);
    } else{
        return getErrorText(code);
    }
}

QString ShtrihFiscalPrinter::getErrorText(int code)
{
    switch (code) {
    // 0x00
    case 0x00:
        return tr("No errors");
    case 0x01:
        return tr("FM1, FM2 or RTC error");
    case 0x02:
        return tr("FM1 missing");
    case 0x03:
        return tr("FM2 missing");
    case 0x04:
        return tr("Incorrect parameters in FM command");
    case 0x05:
        return tr("No data requested available");
    case 0x06:
        return tr("FM is in data output mode");
    case 0x07:
        return tr("Invalid FM command parameters");
    case 0x08:
        return tr("Command is not supported by FM");
    case 0x09:
        return tr("Invalid command length");
    case 0x0A:
        return tr("Data format is not BCD");
    case 0x0B:
        return tr("FM memory cell failure");
    // 0x10
    case 0x11:
        return tr("License in not entered");
    case 0x12:
        return tr("Serial number is already entered");
    case 0x13:
        return tr("Current date is less than last FM record date");
    case 0x14:
        return tr("FM day totals area overflow");
    case 0x15:
        return tr("Day is already opened");
    case 0x16:
        return tr("Day is not opened");
    case 0x17:
        return tr("First day number is larger than last day number");
    case 0x18:
        return tr("First day date is larger than last day date");
    case 0x19:
        return tr("No FM data available");
    case 0x1A:
        return tr("FM fiscal area overflow");
    case 0x1B:
        return tr("Serial number is not assigned");
    case 0x1C:
        return tr("There is corrupted record in the defined range");
    case 0x1D:
        return tr("Last day sum record is corrupted");
    case 0x1E:
        return tr("FM fiscal area overflow");
    case 0x1F:
        return tr("Registers memory is missing");
    // 0x20
    case 0x20:
        return tr("Cash register overflow after add");
    case 0x21:
        return tr("Subtract summ is larger then cash register value");
    case 0x22:
        return tr("Invalid date");
    case 0x23:
        return tr("Activation record is not found");
    case 0x24:
        return tr("Activation area overflow");
    case 0x25:
        return tr("Activation with requested number is not found");
    case 0x26:
        return tr("More than 3 corrupted records in fiscal memory");
    case 0x27:
        return tr("Fiscal memory CRC corrupted");
    case 0x28:
        return tr("FM: Overflow on reset count");
    case 0x29:
        return tr("Fiscal memory was replaced");
    case 0x2B:
        return tr("Unable to cancel previous command");
    case 0x2C:
        return tr("ECR is zero out (re-clearing is not available)");
    case 0x2D:
        return tr("Department receipt sum is less than void sum");
    case 0x2F:
        return tr("EJ module not answered");
    // 0x30
    case 0x30:
        return tr("EJ module returns NAK");
    case 0x31:
        return tr("EJ module: format error");
    case 0x32:
        return tr("EJ module: CRC error");
    case 0x33:
        return tr("Incorrect command parameters");
    case 0x34:
        return tr("No data");
    case 0x35:
        return tr("Incorrect command parameters for this settings");
    case 0x36:
        return tr("Incorrect command parameters for this ECR implementation");
    case 0x37:
        return tr("Command is not supported");
    case 0x38:
        return tr("PROM error");
    case 0x39:
        return tr("Internal software error");
    case 0x3A:
        return tr("Day charge sum overflow");
    case 0x3B:
        return tr("Day sum overflow");
    case 0x3C:
        return tr("EKLZ: Invalid registration number");
    case 0x3D:
        return tr("Day is closed. Command is invalid");
    case 0x3E:
        return tr("Day Department sum overflow");
    case 0x3F:
        return tr("Day discount sum overflow");
    // 0x40
    case 0x40:
        return tr("Discount range overflow");
    case 0x41:
        return tr("Cash pay range overflow");
    case 0x42:
        return tr("Pay type 2 range overflow");
    case 0x43:
        return tr("Pay type 3 range overflow");
    case 0x44:
        return tr("Pay type 4 range overflow");
    case 0x45:
        return tr("All payment types sum is less than receipt sum");
    case 0x46:
        return tr("No cash in ECR");
    case 0x47:
        return tr("Day tax sum overflow");
    case 0x48:
        return tr("Receipt sum overflow");
    case 0x49:
        return tr("Command is invalid in opened receipt of this type");
    case 0x4A:
        return tr("Receipt is opened. Command is invalid");
    case 0x4B:
        return tr("Receipt buffer overflow");
    case 0x4C:
        return tr("Day total tax sum overflow");
    case 0x4D:
        return tr("Cashless sum is larger than receipt sum");
    case 0x4E:
        return tr("24 hours over");
    case 0x4F:
        return tr("Invalid password");
    // 0x50
    case 0x50:
        return tr("Previous command is printing now");
    case 0x51:
        return tr("Day cash sum overflow");
    case 0x52:
        return tr("Day pay type 2 sum overflow");
    case 0x53:
        return tr("Day pay type 3 sum overflow");
    case 0x54:
        return tr("Day pay type 4 sum overflow");
    case 0x55:
        return tr("Receipt is closed. Command is invalid");
    case 0x56:
        return tr("There is no document to repeat");
    case 0x57:
        return tr("Closed day count in EJ does not correspont to FM day count");
    case 0x58:
        return tr("Waiting for continue print command");
    case 0x59:
        return tr("Document is opened by another operator");
    case 0x5A:
        return tr("Discount sum is larger than receipt sum");
    case 0x5B:
        return tr("Charge range overflow");
    case 0x5C:
        return tr("Low supply voltage, 24v");
    case 0x5D:
        return tr("Table is undefined");
    case 0x5E:
        return tr("Invalid command");
    case 0x5F:
        return tr("Negative receipt sum");
    // 0x60
    case 0x60:
        return tr("Multiplication overflow");
    case 0x61:
        return tr("Price range overflow");
    case 0x62:
        return tr("Quantity range overflow");
    case 0x63:
        return tr("Department range overflow");
    case 0x64:
        return tr("FM is missing");
    case 0x65:
        return tr("Insufficient cash in Department");
    case 0x66:
        return tr("Department cash overflow");
    case 0x67:
        return tr("FM connection error");
    case 0x68:
        return tr("Insufficient tax sum");
    case 0x69:
        return tr("Tax sum overflow");
    case 0x6A:
        return tr("Supply error when I2C active");
    case 0x6B:
        return tr("No receipt paper");
    case 0x6C:
        return tr("No journal paper");
    case 0x6D:
        return tr("Insufficient tax sum");
    case 0x6E:
        return tr("Tax sum overflow");
    case 0x6F:
        return tr("Day cash out overflow");
    // 0x70
    case 0x70:
        return tr("FM overflow");
    case 0x71:
        return tr("Cutter failure");
    case 0x72:
        return tr("Command is not supported in this submode");
    case 0x73:
        return tr("Command is not supported in this mode");
    case 0x74:
        return tr("RAM failure");
    case 0x75:
        return tr("Supply failure");
    case 0x76:
        return tr("Printer failure: no impulse from tachometer generator");
    case 0x77:
        return tr("Printer failure: no signal from sensors");
    case 0x78:
        return tr("Software replaced");
    case 0x79:
        return tr("FM replaced");
    case 0x7A:
        return tr("Field is not editable");
    case 0x7B:
        return tr("Hardware failure");
    case 0x7C:
        return tr("Date does not match");
    case 0x7D:
        return tr("Invalid date format");
    case 0x7E:
        return tr("Invalid value in length field");
    case 0x7F:
        return tr("Receipt total sum range overflow");
    // 0x80
    case 0x80:
        return tr("FM connection error");
    case 0x81:
        return tr("FM connection error");
    case 0x82:
        return tr("FM connection error");
    case 0x83:
        return tr("FM connection error");
    case 0x84:
        return tr("Cash sum overflow");
    case 0x85:
        return tr("Day sale sum overflow");
    case 0x86:
        return tr("Day buy sum overflow");
    case 0x87:
        return tr("Day return sale sum overflow");
    case 0x88:
        return tr("Day return buy sum overflow");
    case 0x89:
        return tr("Day cash in sum overflow");
    case 0x8A:
        return tr("Receipt charge sum overflow");
    case 0x8B:
        return tr("Receipt discount sum overflow");
    case 0x8C:
        return tr("Negative receipt charge sum");
    case 0x8D:
        return tr("Negative receipt discount sum");
    case 0x8E:
        return tr("Zero receipt sum");
    case 0x8F:
        return tr("ECR is not fiscal");
    // 0x90
    case 0x90:
        return tr("Field size is larger than settings value");
    case 0x91:
        return tr("Out of printing field area for this font settings");
    case 0x92:
        return tr("Field overlay");
    case 0x93:
        return tr("RAM recovery successful");
    case 0x94:
        return tr("Receipt operation count overflow");
    case 0x95:
        return tr("EJ: Unknown error");
    // 0xA0
    case 0xA0:
        return tr("EJ connection error");
    case 0xA1:
        return tr("EJ is missing");
    case 0xA2:
        return tr("EJ: Invalid parameter or command format");
    case 0xA3:
        return tr("Invalid EJ state");
    case 0xA4:
        return tr("EJ failure");
    case 0xA5:
        return tr("EJ cryptoprocessor failure");
    case 0xA6:
        return tr("EJ Time limit exceeded");
    case 0xA7:
        return tr("EJ overflow");
    case 0xA8:
        return tr("EJ: invalid date and time");
    case 0xA9:
        return tr("EJ: no requested data available");
    case 0xAA:
        return tr("EJ overflow (negative document sum)");
    // 0xB0
    case 0xB0:
        return tr("EJ: Quantity parameter overflow");
    case 0xB1:
        return tr("EJ: Sum parameter overflow");
    case 0xB2:
        return tr("EJ: Already activated");
    case 0xB3:
        return tr("Incorrect date or time");
    // 0xC0
    case 0xC0:
        return tr("Date and time control(confirm date and time)");
    case 0xC1:
        return tr("EJ: Z-report is not interruptable");
    case 0xC2:
        return tr("Exceeding supply voltage");
    case 0xC3:
        return tr("Receipt sum and EJ sum mismatch");
    case 0xC4:
        return tr("Day numbers mismatch");
    case 0xC5:
        return tr("Slip buffer is empty");
    case 0xC6:
        return tr("Slip is missing");
    case 0xC7:
        return tr("Field is not editable in this mode");
    case 0xC8:
        return tr("No impulses from tachometer sensor");
    case 0xC9:
        return tr("Print head overheated");
    case 0xCA:
        return tr("Temperature is out of range");
    case 0xCB:
        return tr("Overflow of long document number");
    // 0xD0
    case 0xD0:
        return tr("Day journal from EJ module is not printed");
    case 0xD1:
        return tr("Buffer is empty");
    case 0xD2:
        return tr("Invalid cash amount for rounding settings");
    // 0xE0
    case 0xE0:
        return tr("Cash acceptor connection error");
    case 0xE1:
        return tr("Cash acceptor is busy");
    case 0xE2:
        return tr("Receipt sum does not correspond to cash acceptor sum");
    case 0xE3:
        return tr("Cash acceptor error");
    case 0xE4:
        return tr("Cash acceptor sum is not zero");
    // 0xF0
    case 0xF0:
        return tr("Error sending to FM");
    case 0xF1:
        return tr("Error receiving from FM");
    case 0xF2:
        return tr("Receive timeout");
    case 0xF3:
        return tr("Buffer overflow");
    case 0xF4:
        return tr("No requested lines");
    case 0xF5:
        return tr("Receive frame overflow");
    default:
        return tr("Unknown error");
    }
}

QString ShtrihFiscalPrinter::getDeviceCodeText(DeviceCode code)
{
    switch (code) {
    case FMStorage1:
        return tr("FM storage 1");
    case FMStorage2:
        return tr("FM storage 2");
    case RTC:
        return tr("Real time clock");
    case NVRAM:
        return tr("Nonvolative memory");
    case FMCPU:
        return tr("FM processor");
    case FPCPU:
        return tr("FP program memory");
    case FPRAM:
        return tr("FP random access memory");
    case SYSIMAGE:
        return tr("Filesystem image");
    case ULINUX_IMAGE:
        return tr("uLinux image");
    case APP_FILE:
        return tr("Executable file");
    case APP_MEMORY:
        return tr("Program memory");
    default:
        return tr("Unknown device");
    }
}

QString ShtrihFiscalPrinter::getModeText(int value)
{
    switch (value) {
    case MODE_DUMPMODE:
        return tr("Data output mode");
    case MODE_24NOTOVER:
        return tr("Day is opened, 24 hours are not over");
    case MODE_24OVER:
        return tr("Day is opened, 24 hours are over");
    case MODE_CLOSED:
        return tr("Day is closed");
    case MODE_LOCKED:
        return tr("Locked by incorrect tax assessor password");
    case MODE_WAITDATE:
        return tr("Wait for date confirmation");
    case MODE_POINTPOS:
        return tr("Decimal point position mode");
    case MODE_REC:
        return tr("Opened document");
    case MODE_TECH:
        return tr("Technical zero mode");
    case MODE_TEST:
        return tr("Test print mode");
    case MODE_FULLREPORT:
        return tr("Printing full fiscal report");
    case MODE_EJREPORT:
        return tr("Printing EJ report");
    case MODE_SLP:
        return tr("Slip document opened");
    case MODE_SLPPRINT:
        return tr("Waiting for slip");
    case MODE_SLPREADY:
        return tr("Fiscal slip is ready");
    default:
        return tr("Unknown mode");
    }
}

QString ShtrihFiscalPrinter::getSubmodeText(int value)
{
    switch (value) {
    case ECR_SUBMODE_IDLE:
        return tr("Paper is present");
    case ECR_SUBMODE_PASSIVE:
        return tr("Out of paper: passive");
    case ECR_SUBMODE_ACTIVE:
        return tr("Out of paper: active");
    case ECR_SUBMODE_AFTER:
        return tr("After active paper out");
    case ECR_SUBMODE_REPORT:
        return tr("Printing reports mode");
    case ECR_SUBMODE_PRINT:
        return tr("Printing mode");
    default:
        return tr("Unknown advanced mode");
    }
}

int ShtrihFiscalPrinter::printBarcode(PrinterBarcode barcode)
{
    qDebug() << "printBarcode";

    using namespace Zint;
    Zint::QZint encoder;
    encoder.setText(barcode.text);
    encoder.setSymbol(barcode.type);
    encoder.setWidth(1);
    encoder.setHeight(1);
    encoder.encode();

    zint_symbol* zsymbol = encoder.getZintSymbol();
    qDebug() << "zsymbol->rows: " << zsymbol->rows;

    QPixmap pixmap(QSize(zsymbol->rows, zsymbol->rows));
    QPainter painter(&pixmap);
    QRectF paintRect(0, 0, zsymbol->rows, zsymbol->rows);
    encoder.render(painter, paintRect);

    QImage image = pixmap.toImage();
    qDebug() << "image.width: " << image.width()
             << "image.height: " << image.height();

    return printImage1(image);
}

int ShtrihFiscalPrinter::loadImage(const QString& fileName)
{
    QImage image(fileName);
    return loadImage(image);
}

int ShtrihFiscalPrinter::getMaxGraphicsWidth()
{
    return 320;
}

int ShtrihFiscalPrinter::getMaxGraphicsHeight()
{
    return 200;
}

QImage getMonohromeImage(QImage pImage, int threshold)
{
    QImage result = pImage;
    result.fill(255);
    for (int x = 0; x < pImage.width(); x++)
        for (int y = 0; y < pImage.height(); y++)
            result.setPixel(x, y, qGray(pImage.pixel(x, y)) > threshold ? qRgb(255, 255, 255) : qRgb(0, 0, 0));

    return result;
}

int ShtrihFiscalPrinter::loadGraphics(LoadGraphicsCommand& data)
{
    if (capLoadGraphics1) {
        return loadGraphics1(data);
    }
    if (capLoadGraphics2) {
        return loadGraphics2(data);
    }
    return SMFPTR_E_NOGRAPHICS;
}

int ShtrihFiscalPrinter::printGraphics(PrintGraphicsCommand& data)
{
    if (capPrintGraphics1) {
        return printGraphics1(data);
    }
    if (capPrintGraphics2) {
        return printGraphics2(data);
    }
    return SMFPTR_E_NOGRAPHICS;
}

int ShtrihFiscalPrinter::loadImage(QImage& image)
{
    qDebug() << "loadImage";

    if (capLoadGraphics3) {
        return loadImage3(image);
    }
    if (capLoadGraphics2) {
        return loadImage2(image);
    }
    if (capLoadGraphics1) {
        return loadImage1(image);
    }
    return SMFPTR_E_NOGRAPHICS;
}

int ShtrihFiscalPrinter::getStartLine()
{
    return startLine;
}

QImage ShtrihFiscalPrinter::alignImage(QImage image, int width)
{
    switch (imageAlignment) {
    case PrinterAlignment::Right: {
        qDebug() << "PrinterAlignment::Right";

        int xoffset = width - image.width();
        QImage img(width, image.height(), QImage::Format_MonoLSB);
        QPainter painter(&img);
        painter.translate(xoffset, 0);
        painter.drawImage(xoffset, 0, image);
        return img;
    }
    case PrinterAlignment::Center: {
        qDebug() << "PrinterAlignment::Center";

        int xoffset = (width - image.width()) / 2;
        QImage img(image.width() + xoffset, image.height(), QImage::Format_MonoLSB);
        QPainter painter(&img);
        painter.translate(xoffset, 0);
        painter.drawImage(xoffset, 0, image);
        return img;
    }
    default: {
        return image;
    }
    }
}

int ShtrihFiscalPrinter::loadImage1(QImage& image)
{
    qDebug() << "loadImage1";

    image = image.convertToFormat(QImage::Format_MonoLSB, Qt::MonoOnly);
    //image = alignImage(image, 320);

    if (image.width() > 320) {
        return SMFPTR_E_LARGEWIDTH;
    }
    if (image.height() > 255) {
        return SMFPTR_E_LARGEHEIGHT;
    }

    int rc = 0;
    int startLine = getStartLine();
    LoadGraphicsCommand command;
    for (int i = 0; i < image.height(); i++) {
        const char* buf = (const char*)image.scanLine(i);
        QByteArray ba(buf, image.bytesPerLine());
        command.line = i + startLine;
        command.data = ba;
        rc = loadGraphics1(command);
        if (failed(rc))
            return rc;
    }
    PrinterImage pimage;
    pimage.startLine = startLine;
    pimage.height = image.height();
    addImage(pimage);
    return rc;
}

int ShtrihFiscalPrinter::loadImage2(QImage& image)
{
    qDebug() << "loadImage2";

    image = image.convertToFormat(QImage::Format_MonoLSB, Qt::MonoOnly);
    image = alignImage(image, 320);

    if (image.width() > 320) {
        return SMFPTR_E_LARGEWIDTH;
    }
    if (image.height() > 65535) {
        return SMFPTR_E_LARGEHEIGHT;
    }

    int rc = 0;
    int startLine = getStartLine();
    LoadGraphicsCommand command;
    for (int i = 0; i < image.height(); i++) {
        const char* buf = (const char*)image.scanLine(i);
        QByteArray ba(buf, image.bytesPerLine());
        command.line = i + startLine;
        command.data = ba;
        rc = loadGraphics2(command);
        if (failed(rc))
            return rc;
    }
    PrinterImage pimage;
    pimage.startLine = startLine;
    pimage.height = image.height();
    addImage(pimage);
    return rc;
}

int ShtrihFiscalPrinter::loadImage3(QImage& image)
{
    qDebug() << "loadImage3";

    image = image.convertToFormat(QImage::Format_MonoLSB, Qt::MonoOnly);
    // image = alignImage(img, 512);
    // qDebug() << "alignImage: " << image.width() << ", " << image.height();

    int rc = 0;
    if (image.bytesPerLine() > 512) {
        return SMFPTR_E_LARGEWIDTH;
    }

    int linesPerCommand = 240 / image.bytesPerLine();
    int count = (image.height() + linesPerCommand - 1) / linesPerCommand;
    int startLine = getStartLine();
    int row = 0;
    for (int i = 0; i < count; i++) {
        int rowCount = 0;
        QByteArray ba;
        for (int j = 0; j < linesPerCommand; j++) {

            QByteArray scanLine((const char*)image.scanLine(row), image.bytesPerLine());
            ba.append(scanLine);
            row++;
            rowCount++;
            if (row >= image.height()) {
                break;
            }
        }
        LoadGraphics3Command command;
        command.lineLength = image.bytesPerLine();
        command.startLine = startLine;
        command.lineCount = rowCount;
        command.bufferType = 1;
        command.data = ba;
        rc = loadGraphics3(command);
        if (failed(rc))
            return rc;
        startLine = startLine + rowCount;
    }
    PrinterImage pimage;
    pimage.startLine = startLine;
    pimage.height = image.height();
    addImage(pimage);
    return rc;
}

void ShtrihFiscalPrinter::addImage(PrinterImage image)
{
    images.push_back(image);
    startLine += image.height;
}

void ShtrihFiscalPrinter::clearImages()
{
    startLine = 2;
    images.clear();
}

int ShtrihFiscalPrinter::getImageCount()
{
    return images.size();
}

int ShtrihFiscalPrinter::printImage(QImage image)
{
    qDebug() << "printImage";

    if ((capLoadGraphics3) && (capPrintGraphics3)) {
        return printImage3(image);
    };
    if ((capLoadGraphics2) && (capPrintGraphics2)) {
        return printImage2(image);
    }
    if ((capLoadGraphics1) && (capPrintGraphics1)) {
        return printImage1(image);
    }
    return SMFPTR_E_NOGRAPHICS;
}

int ShtrihFiscalPrinter::printImage1(QImage& image)
{
    qDebug() << "printImage1";

    int rc = loadImage1(image);
    if (failed(rc))
        return rc;

    PrinterImage pimage = images.at(images.size() - 1);
    PrintGraphicsCommand command;
    command.line1 = pimage.startLine;
    command.line2 = pimage.startLine + pimage.height;
    rc = printGraphics1(command);
    return rc;
}

int ShtrihFiscalPrinter::printImage2(QImage& image)
{
    qDebug() << "printImage2";
    int rc = loadImage2(image);
    if (failed(rc))
        return rc;

    PrinterImage pimage = images.at(images.size() - 1);
    PrintGraphicsCommand command;
    command.line1 = pimage.startLine;
    command.line2 = pimage.startLine + pimage.height;
    rc = printGraphics2(command);
    return rc;
}

int ShtrihFiscalPrinter::printImage3(QImage& image)
{
    qDebug() << "printImage3";

    int rc = loadImage3(image);
    if (failed(rc))
        return rc;

    PrinterImage pimage = images.at(images.size() - 1);
    PrintGraphics3Command command;
    command.startLine = pimage.startLine;
    command.endLine = pimage.startLine + pimage.height;
    command.vScale = 1;
    command.hScale = 1;
    command.flags = 2;
    rc = printGraphics3(command);
    return rc;
}

int ShtrihFiscalPrinter::printImage(int imageIndex)
{
    return printImage(images.at(imageIndex));
}

int ShtrihFiscalPrinter::printImage(PrinterImage image)
{
    return printImage(image.startLine, image.startLine + image.height);
}

QString ShtrihFiscalPrinter::getVersion()
{
    return "1.3";
}

int ShtrihFiscalPrinter::printImage(int startLine, int endLine)
{
    PrintGraphicsCommand command;
    command.line1 = startLine;
    command.line2 = endLine;
    return printGraphics(command);
}

bool ShtrihFiscalPrinter::isDayOpened()
{
    ReadShortStatusCommand command;
    int rc = readShortStatus(command);
    if (rc != 0) return false;
    int mode = command.mode & 0x0F;
    if ((mode == MODE_REC) || (mode == MODE_24NOTOVER) || (mode == MODE_24OVER)) {
        return true;
    }
    return false;
}

int ShtrihFiscalPrinter::resetPrinter()
{
    ReadShortStatusCommand command;
    int rc = readShortStatus(command);
    if (rc != 0)
        return rc;
    if ((command.mode & 0x0F) == MODE_REC) {
        PasswordCommand command;
        rc = cancelReceipt(command);
        if (rc != 0)
            return rc;
        rc = waitForPrinting();
    }
    return rc;
}

void ShtrihFiscalPrinter::writeFields(QString fileName)
{
    PrinterFields fields;
    CsvTablesReader reader;
    reader.load(fileName, fields);
    writeFields(fields);
}

void ShtrihFiscalPrinter::writeFields(PrinterFields fields)
{
    for (int i=0;i<fields.size();i++){
        PrinterField field = fields.get(i);
        writeField(field);
    }
}

int ShtrihFiscalPrinter::writeField(PrinterField field)
{
    TableValueCommand command;
    command.table = field.getInfo().table;
    command.field = field.getInfo().field;
    command.row = field.getRow();
    command.value = field.getBinary();
    return writeTable(command);
}

void ShtrihFiscalPrinter::check(int resultCode)
{
    if (failed(resultCode)){
        throw new DeviceException(resultCode, getErrorText2(resultCode));
    }
}

uint64_t ShtrihFiscalPrinter::readCashRegister(uint8_t number)
{
    ReadCashRegCommand command;
    command.number = number;
    check(readRegister(command));
    return command.value;
}

uint16_t ShtrihFiscalPrinter::readOperationRegister(uint8_t number)
{
    ReadOperRegCommand command;
    command.number = number;
    check(readRegister(command));
    return command.value;
}

uint16_t ShtrihFiscalPrinter::readDayNumber()
{
    ReadLongStatusCommand command;
    check(readLongStatus(command));
    return command.dayNumber;
}

PrinterField ShtrihFiscalPrinter::getPrinterField(int table, int row, int field)
{
    PrinterField printerField;
    if (fields.find(table, field, printerField))
    {
        return printerField;
    }

    FieldInfoCommand command;
    command.table = table;
    command.field = field;
    check(readFieldInfo(command));

    PrinterFieldInfo fieldInfo;
    fieldInfo.field = field;
    fieldInfo.max = command.max;
    fieldInfo.min = command.min;
    fieldInfo.name = command.name;
    fieldInfo.row = row;
    fieldInfo.size = command.size;
    fieldInfo.table = table;
    fieldInfo.type = command.type;

    qDebug() << "fieldInfo.size" << fieldInfo.size;

    printerField.setInfo(fieldInfo);
    fields.add(printerField);
    return printerField;
}

QString ShtrihFiscalPrinter::readTable(int table, int row, int field)
{
    qDebug() << "readTable(" << table << ", " << row << ", " << field << ")";

    PrinterField printerField;
    printerField = getPrinterField(table, row, field);

    TableValueCommand command;
    command.table = table;
    command.row = row;
    command.field = field;
    check(readTable(command));
    printerField.setBinary(command.value);
    QString result = printerField.getValue();
    qDebug() << "readTable:" << result;
    return result;
}

QStringList ShtrihFiscalPrinter::readHeader(){

    QStringList header;
    for (int i = 0; i < numHeaderLines; i++)
    {
        header.append(readTable(SMFP_TABLE_TEXT, i + numHeaderRow, 1));
    }
    return header;
}

QStringList ShtrihFiscalPrinter::readTrailer(){
    QStringList trailer;
    for (int i = 0; i < numTrailerLines; i++)
    {
        trailer.append(readTable(SMFP_TABLE_TEXT, i + numTrailerRow, 1));
    }
    return trailer;
}

QString ShtrihFiscalPrinter::readPaymentName(int number)
{
    return readTable(SMFP_TABLE_PAYTYPE, number, 1);
}

QString ShtrihFiscalPrinter::PrinterDateToStr(PrinterDate date)
{
    QString s;
    return s.sprintf("%02d/%02d/%04d", date.day, date.month, 2000 + date.year);
}

QString ShtrihFiscalPrinter::PrinterTimeToStr(PrinterTime time)
{
    QString s;
    return s.sprintf("%02d:%02d:%02d", time.hour, time.min, time.sec);
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

int ShtrihFiscalPrinter::fsReadStatus(FSStatus& status)
{
    qDebug() << "fsReadStatus";
    PrinterCommand command(0xFF01);
    command.write(sysPassword, 4);
    int rc = send(command);
    if (rc == 0){
        int v = command.read8();
        status.mode.value = v;
        status.mode.isConfigured = testBit(v, 0);
        status.mode.isFiscalOpened = testBit(v, 1);
        status.mode.isFiscalClosed = testBit(v, 2);
        status.mode.isDataSended = testBit(v, 3);

        status.docType = command.read8();
        status.isDocReceived = command.read8() > 0;
        status.isDayOpened = command.read8() > 0;

        v = command.read8();
        status.flags.value = v;
        status.flags.cpLockIn3Days = testBit(v, 0);
        status.flags.cpLockIn30Days = testBit(v, 1);
        status.flags.fsMemoryOverflow = testBit(v, 2);
        status.flags.fdoAnswerTimeout = testBit(v, 3);

        status.date = command.readDate2();
        status.time = command.readTime2();
        status.fsSerial = command.readStr(16).trimmed();
        status.docNumber = command.read32();
    }
    return rc;
}

/***************************************************************************
* Запрос номера ФН
* Код команды FF02h . Длина сообщения: 6 байт.
* Пароль системного администратора: 4 байт
* Ответ:	FF02 Длина сообщения: 17 байт.
* Код ошибки (1 байт)
* Номер ФН (16 байт) ASCII
***************************************************************************/

int ShtrihFiscalPrinter::fsReadSerial(QString& serial){
    qDebug() << "fsReadSerial";
    PrinterCommand command(0xFF02);
    command.write(sysPassword, 4);
    int rc = send(command);
    if (rc == 0){
        serial = command.readStr(16).trimmed();
    }
    return rc;
}

/***************************************************************************
Запрос срока действия ФН
Код команды FF03h . Длина сообщения: 6 байт.
Пароль системного администратора: 4 байта
Ответ: FF03h Длина сообщения: 4 байт.
Код ошибки: 1 байт
Срок действия: 3 байта ГГ,ММ,ДД
***************************************************************************/

int ShtrihFiscalPrinter::fsReadExpDate(PrinterDate& date){
    qDebug() << "fsReadExpDate";
    PrinterCommand command(0xFF03);
    command.write(sysPassword, 4);
    int rc = send(command);
    if (rc == 0){
        date = command.readDate2();
    }
    return rc;
}

/***************************************************************************
Запрос версии ФН
Код команды FF04h . Длина сообщения: 6 байт.
Пароль системного администратора: 4 байта
Ответ: FF04h Длина сообщения: 18 байт.
Код ошибки: 1 байт
Строка версии программного обеспечения ФН:16 байт ASCII
Тип программного обеспечения ФН: 1 байт
0 – отладочная версия
1 – серийная версия
***************************************************************************/

int ShtrihFiscalPrinter::fsReadVersion(FSVersion& version){
    qDebug() << "fsReadVersion";
    PrinterCommand command(0xFF04);
    command.write(sysPassword, 4);
    int rc = send(command);
    if (rc == 0){
        version.text = command.readStr(16).trimmed();
        version.isRelease = command.read8() > 0;
    }
    return rc;
}

/***************************************************************************
Начать отчет о регистрации ККТ
Код команды FF05h. Длина сообщения: 7 байт.
    Пароль системного администратора: 4 байта
    Тип отчета: 1 байт
    00 – Отчет о регистрации КТТ
    01 – Отчет об изменении параметров регистрации ККТ, в связи с заменой ФН
    02 – Отчет об изменении параметров регистрации ККТ без замены ФН
Ответ: FF05h Длина сообщения: 1 байт.
    Код ошибки: 1 байт
***************************************************************************/

int ShtrihFiscalPrinter::fsStartFiscalization(int reportType){
    qDebug() << "fsStartFiscalization";
    PrinterCommand command(0xFF05);
    command.write(sysPassword, 4);
    command.write(reportType, 1);
    return send(command);
}

/***************************************************************************

Сформировать отчёт о регистрации ККТ

Код команды FF06h . Длина сообщения: 40 байт.
    Пароль системного администратора: 4 байта
    ИНН : 12 байт ASCII
    Регистрационный номер ККТ: 20 байт ASCII
    Код налогообложения: 1 байт
    Режим работы: 1 байт

Ответ: FF06h Длина сообщения: 9 байт.
    Код ошибки: 1 байт
    Номер ФД: 4 байта
    Фискальный признак: 4 байта

***************************************************************************/

int ShtrihFiscalPrinter::fsPrintFiscalization(FSPrintFiscalization& data){
    qDebug() << "fsPrintFiscalization";
    PrinterCommand command(0xFF06);
    command.write(sysPassword, 4);
    command.writeStr(data.INN, 12);
    command.writeStr(data.RNM, 20);
    command.write8(data.taxMode);
    command.write8(data.workMode);
    int rc = send(command);
    if (rc == 0){
        data.docNum = command.read32();
        data.docMac = command.read32();
    }
    return rc;
}

/***************************************************************************

Сброс состояния ФН

Код команды FF07h . Длина сообщения: 7 байт.
    Пароль системного администратора: 4 байта
    Код запроса: 1 байт

Ответ: FF07h Длина сообщения: 1 байт.
    Код ошибки: 1 байт

***************************************************************************/

int ShtrihFiscalPrinter::fsReset(int code){
    qDebug() << "fsReset";
    PrinterCommand command(0xFF07);
    command.write(sysPassword, 4);
    command.write8(code);
    return send(command);
}

/***************************************************************************
Отменить документ в ФН
Код команды FF08h . Длина сообщения: 6 байт.
Пароль системного администратора: 4 байта
Ответ: FF08h Длина сообщения: 1 байт.
Код ошибки: 1 байт
***************************************************************************/

int ShtrihFiscalPrinter::fsCancelDocument(){
    qDebug() << "fsCancelDocument";
    PrinterCommand command(0xFF08);
    command.write(sysPassword, 4);
    return send(command);
}

/***************************************************************************

Запрос итогов последней фискализации (перерегистрации) FF09H

Код команды FF09h . Длина сообщения: 6 байт.
    Пароль системного администратора: 4 байта

Ответ:    FF09h Длина сообщения: 48 байт.
    Код ошибки : 1 байт
    Дата и время:  5 байт DATE_TIME
    ИНН : 12 байт ASCII
    Регистрационный номер ККT: 20 байт ASCII
    Код налогообложения: 1 байт
    Режим работы: 1 байт
    Номер ФД: 4 байта
    Фискальный признак: 4 байта

***************************************************************************/

int ShtrihFiscalPrinter::fsReadFiscalization(FSReadFiscalization& data){
    qDebug() << "fsReadFiscalization";
    PrinterCommand command(0xFF09);
    command.write(sysPassword, 4);
    int resultCode = send(command);
    if (resultCode == 0)
    {
        data.date = command.readDate2();
        data.time = command.readTime2();
        data.inn = command.readStr(12).trimmed();
        data.rnm = command.readStr(20).trimmed();
        data.taxSystem = command.read8();
        data.workMode = command.read8();
        data.regCode = command.read8();
        data.docNum = command.read32();
        data.docMac = command.read32();
    }
    return resultCode;
}


/***************************************************************************
Найти фискальный документ по номеру
Код команды FF0Ah . Длина сообщения: 10 байт.
Пароль системного администратора: 4 байта
Номер фискального документа: 4 байта
Ответ: FF0Аh Длина сообщения 3+N байт.
Код ошибки: 1 байт
Тип фискального документа: 1 байт
Получена ли квитанция из ОФД: 1 байт
1- да
0 -нет
Данные фискального документа в зависимости от типа документ: N байт
***************************************************************************/

int ShtrihFiscalPrinter::fsFindDocument(FSFindDocument& data){
    qDebug() << "fsFindDocument";
    PrinterCommand command(0xFF0A);
    command.write(sysPassword, 4);
    command.write32(data.docNum);
    data.resultCode = send(command);
    if (data.resultCode == 0)
    {
        data.docType = command.read8();
        data.hasTicket = command.read8();
        data.ticket = command.readBytes();
    }
    return data.resultCode;
}

/***************************************************************************
 * Открыть смену в ФН
Код команды FF0Bh . Длина сообщения: 6 байт.
Пароль системного администратора: 4 байт
Ответ: FF0Bh Длина сообщения: 11 байт.
Код ошибки: 1 байт
Номер новой открытой смены: 2 байта
Номер ФД :4 байта
Фискальный признак: 4 байта
***************************************************************************/

int ShtrihFiscalPrinter::fsOpenDay(FSOpenDay& data){
    qDebug() << "fsOpenDay";
    PrinterCommand command(0xFF0B);
    command.write(sysPassword, 4);
    data.resultCode = send(command);
    if (data.resultCode == 0)
    {
        data.dayNum = command.read16();
        data.docNum = command.read32();
        data.docMac = command.read32();
    }
    return data.resultCode;
}

/***************************************************************************
Передать произвольную TLV структуру
Код команды FF0Ch . Длина сообщения: 6+N байт.
    Пароль системного администратора: 4 байта
    TLV Структура: N байт (мах 250 байт)
Ответ: FF0Сh Длина сообщения: 1 байт.
    Код ошибки: 1 байт
***************************************************************************/

int ShtrihFiscalPrinter::fsWriteTLV(QByteArray& data){
    qDebug() << "fsWriteTLV";
    PrinterCommand command(0xFF0C);
    command.write(sysPassword, 4);
    command.write(data);
    return send(command);
}

/***************************************************************************
 *
Операция со скидками и надбавками
Код команды FF0Dh . Длина сообщения: 254 байт.
    Пароль системного администратора: 4 байта
    Тип операции: 1 байт
    1 – Приход,
    2 – Возврат прихода,
    3 – Расход,
    4 – Возврат расхода
    Количество: 5 байт 0000000000…9999999999
    Цена: 5 байт 0000000000…9999999999
    Скидка: 5 байт 0000000000…9999999999
    Надбавка: 5 байт 0000000000…9999999999
    Номер отдела: 1 байт
    0…16 – режим свободной продажи, 255 – режим продажи по коду товара
    Налог: 1 байт
    Бит 1 «0» – нет, «1» – 1 налоговая группа
    Бит 2 «0» – нет, «1» – 2 налоговая группа
    Бит 3 «0» – нет, «1» – 3 налоговая группа
    Бит 4 «0» – нет, «1» – 4 налоговая группа
    Штрих-код: 5 байт 000000000000…999999999999
    Текст: Текст: 220 байта строка - название товара и скидки
    Примечание: если строка начинается символами \\ то она передаётся на сервер
    ОФД но не печатается на кассе. Названия товара и скидки должны заканчиваться
    нулём (Нуль терминированные строки).
Ответ: FF0Dh Длина сообщения: 1 байт.
    Код ошибки: 1 байт
***************************************************************************/

int ShtrihFiscalPrinter::fsPrintItem(FSReceiptItem& data){
    qDebug() << "fsPrintItem";
    PrinterCommand command(0xFF0D);
    command.write(sysPassword, 4);
    command.write8(data.operation);
    command.write(data.quantity, 5);
    command.write(data.price, 5);
    command.write(data.discount, 5);
    command.write(data.charge, 5);
    command.write(data.department, 1);
    command.write(data.tax, 1);
    command.write(data.barcode, 5);
    command.write(data.text);
    return send(command);
}

/***************************************************************************
Инициализация EEPROM
Команда: FF16H. Длина сообщения: 2 байта.
Ответ: FF16H. Длина сообщения: 3 байта.
Код ошибки (1 байт)
Примечание: для Белоруссии.
***************************************************************************/

int ShtrihFiscalPrinter::fsInitEEPROM(){
    qDebug() << "fsInitEEPROM";
    PrinterCommand command(0xFF16);
    return send(command);
}

/***************************************************************************
Запрос денежных регистров базы товаров
Команда: FF1AH. Длина сообщения: 8 байт.
    Пароль оператора (4 байта)
    Код товара (2 байта) 1…9999
Ответ: FF1AH. Длина сообщения: 52 байт.
    Код ошибки (1 байт)
    Порядковый номер оператора (1 байт) 1…30
    Содержимое регистра продаж по данному товару в чеке (6 байт)
    Содержимое регистра покупок по данному товару в чеке (6 байт)
    Содержимое регистра возврата продаж по данному товару в чеке (6 байт)
    Содержимое регистра возврата покупок по данному товару в чеке (6 байт)
    Содержимое регистра продаж по данному товару за смену (6 байт)
    Содержимое регистра покупок по данному товару за смену (6 байт)
    Содержимое регистра возврата продаж по данному товару за смену(6 байт)
    Содержимое регистра возврата покупок по данному товару за смену(6 байт)

***************************************************************************/

int ShtrihFiscalPrinter::fsReadRegisters(FSReadRegisters& data){
    qDebug() << "fsReadRegisters";
    PrinterCommand command(0xFF1A);
    command.write(sysPassword, 4);
    command.write(data.itemCode, 2);
    data.resultCode = send(command);
    if (data.resultCode == 0)
    {
        data.operatorNumber = command.read8();
        for (int i=0;i<8;i++){
            data.registerValue[i] = command.read(6);
        }
    }
    return data.resultCode;
}

/***************************************************************************

Запросить о наличие данных в буфере
Код команды FF30h . Длина сообщения: 6 байт.
    Пароль системного администратора: 4 байта
Ответ: FF30h Длина сообщения: 4 байта.
    Код ошибки:1 байт
    Количество байт в буфере: 2 байта 0 – нет данных
    Максимальный размер блока данных: 1 байт

***************************************************************************/

int ShtrihFiscalPrinter::fsReadBufferStatus(FSBufferStatus& data){
    qDebug() << "fsReadBufferStatus";
    PrinterCommand command(0xFF30);
    command.write(sysPassword, 4);
    int resultCode = send(command);
    if (resultCode == 0)
    {
        data.dataSize = command.read16();
        data.blockSize = command.read8();
    }
    return resultCode;
}

/***************************************************************************

Прочитать блок данных из буфера
Код команды FF31h . Длина сообщения: 9 байт.
    Пароль системного администратора: 4 байта
    Начальное смещение: 2 байта
    Количество запрашиваемых данных :1 байт
Ответ: FF31h Длина сообщения: 1+N байт.
    Код ошибки: 1 байт
    Данные : N байт

***************************************************************************/

int ShtrihFiscalPrinter::fsReadBufferBlock(FSBufferBlock& data){
    qDebug() << "fsReadBufferBlock";
    PrinterCommand command(0xFF31);
    command.write(sysPassword, 4);
    command.write16(data.offset);
    command.write8(data.size);
    int resultCode = send(command);
    if (resultCode == 0)
    {
        data.data = command.readBytes();
    }
    return resultCode;
}

/***************************************************************************

Начать запись данных в буфер
Код команды FF32h . Длина сообщения: 8 байт.
    Пароль системного администратора: 4 байта
    Размер данных: 2 байта
Ответ: FF32h Длина сообщения: 2 байта.
    Код ошибки: 1 байт
    Максимальный размер блок данных: 1 байт

***************************************************************************/

int ShtrihFiscalPrinter::fsStartWriteBuffer(FSStartWriteBuffer& data){
    qDebug() << "fsStartWriteBuffer";
    PrinterCommand command(0xFF32);
    command.write(sysPassword, 4);
    command.write16(data.size);
    data.resultCode = send(command);
    if (data.resultCode == 0)
    {
        data.blockSize = command.read8();
    }
    return data.resultCode;
}

/***************************************************************************
Записать блок данных в буфер FF33H
Код команды FF33h . Длина сообщения: 9+N байт.
•	Пароль системного администратора: (4 байта)
•	Начальное смещение: (2 байта)
•	Размер данных  (1 байт)
•	Данные для записи ( N байт)
         Ответ:	    FF33h Длина сообщения: 1 байт.
•	Код ошибки: 1 байт
***************************************************************************/

int ShtrihFiscalPrinter::fsWriteBuffer(FSWriteBuffer& data)
{
    qDebug() << "fsWriteBuffer";
    PrinterCommand command(0xFF33);
    command.write(sysPassword, 4);
    command.write16(data.offset);
    command.write8(data.block.size());
    command.write(data.block);
    data.resultCode = send(command);
    return data.resultCode;
};

/***************************************************************************

Сформировать отчёт о перерегистрации ККТ
Код команды FF34h . Длина сообщения: 7 байт.
    Пароль системного администратора: 4 байта
    Код причины перерегистрации: 1 байт
Ответ: FF34h Длина сообщения: 9 байт.
    Код ошибки: 1 байт
    Номер ФД: 4 байта
    Фискальный признак: 4 байта

***************************************************************************/

int ShtrihFiscalPrinter::fsPrintRefiscalization(FSRefiscalization& data)
{
    qDebug() << "fsPrintRefiscalization";
    PrinterCommand command(0xFF34);
    command.write(sysPassword, 4);
    command.write8(data.code);
    data.resultCode = send(command);
    if (data.resultCode == 0){
        data.docNum = command.read32();
        data.docMac = command.read32();
    }
    return data.resultCode;
};

/***************************************************************************
Начать формирование чека коррекции
Код команды FF35h . Длина сообщения: 6 байт.
    Пароль системного администратора: 4 байта
Ответ: FF35h Длина сообщения: 1 байт.
    Код ошибки: 1 байт
***************************************************************************/

int ShtrihFiscalPrinter::fsStartCorrectionReceipt()
{
    qDebug() << "fsStartCorrectionReceipt";
    PrinterCommand command(0xFF35);
    command.write(sysPassword, 4);
    return send(command);
}

/***************************************************************************

Сформировать чек коррекции FF36H
Код команды FF36h . Длина сообщения: 12 байт.
    Пароль системного администратора: 4 байта
    Итог чека: 5 байт 0000000000…9999999999
    Тип операции 1 байт
Ответ: FF36h Длина сообщения: 11 байт.
    Код ошибки: 1 байт
    Номер чека: 2 байта
    Номер ФД: 4 байта
    Фискальный признак: 4 байт

***************************************************************************/

int ShtrihFiscalPrinter::fsPrintCorrection(FSPrintCorrection& data)
{
    qDebug() << "fsPrintCorrection";
    PrinterCommand command(0xFF36);
    command.write(sysPassword, 4);
    command.write(data.total, 5);
    command.write8(data.operation);
    data.resultCode = send(command);
    if (data.resultCode == 0){
        data.recNum = command.read32();
        data.docNum = command.read32();
        data.docMac = command.read32();
    }
    return data.resultCode;
};


/***************************************************************************
Начать формирование отчёта о состоянии расчётов
    Код команды FF37h . Длина сообщения: 6 байт.
    Пароль системного администратора: 4 байта
Ответ: FF37h Длина сообщения: 1 байт.
    Код ошибки: 1 байт
***************************************************************************/

int ShtrihFiscalPrinter::fsStartCommReport()
{
    qDebug() << "fsStartCommReport";
    PrinterCommand command(0xFF37);
    command.write(sysPassword, 4);
    return send(command);
}

/***************************************************************************
 *
Сформировать отчёт о состоянии расчётов
    Код команды FF38h . Длина сообщения: 6 байт.
    Пароль системного администратора: 4 байта
Ответ: FF38h Длина сообщения: 16 байт.
    Код ошибки: 1 байт
    Номер ФД: 4 байта
    Фискальный признак: 4 байта
    Количество неподтверждённых документов: 4 байта
    Дата первого неподтверждённого документа: 3 байта ГГ,ММ,ДД

***************************************************************************/

int ShtrihFiscalPrinter::fsPrintCommReport(FSCommReport& data)
{
    qDebug() << "fsPrintCommReport";
    PrinterCommand command(0xFF38);
    command.write(sysPassword, 4);
    int resultCode = send(command);
    if (resultCode == 0){
        data.docNum = command.read32();
        data.docMac = command.read32();
        data.docCount = command.read32();
        data.firstDate = command.readDate();
    }
    return resultCode;
}

/***************************************************************************
 *
Получить статус информационного обмена
Код команды FF39h . Длина сообщения: 6 байт.
    Пароль системного администратора: 4 байта
Ответ: FF39h Длина сообщения: 14 байт.
    Код ошибки: 1 байт
    Статус информационного обмена: 1 байт
    (0 – нет, 1 – да)
    Бит 0 – транспортное соединение установлено
    Бит 1 – есть сообщение для передачи в ОФД
    Бит 2 – ожидание ответного сообщения (квитанции) от ОФД
    Бит 3 – есть команда от ОФД
    Бит 4 – изменились настройки соединения с ОФД
    Бит 5 – ожидание ответа на команду от ОФД
    Состояние чтения сообщения: 1 байт 1 – да, 0 -нет
    Количество сообщений для ОФД: 2 байта
    Номер документа для ОФД первого в очереди: 4 байта
    Дата и время документа для ОФД первого в очереди: 5 байт

***************************************************************************/

int ShtrihFiscalPrinter::fsReadCommStatus(FSCommStatus& data)
{
    qDebug() << "fsReadCommStatus";
    PrinterCommand command(0xFF39);
    command.write(sysPassword, 4);
    int resultCode = send(command);
    if (resultCode == 0){
        data.flags = command.read8();
        data.readStatus = command.read8();
        data.docCount = command.read16();
        data.firstNumber = command.read32();
        data.firstDate = command.readDate();
        data.firstTime = command.readTime2();
    }
    return resultCode;
}

/***************************************************************************
 *
Запросить фискальный документ в TLV формате

    Код команды FF3Аh . Длина сообщения: 10 байт.
    Пароль системного администратора: 4 байта
    Номер фискального документа: 4 байта
Ответ: FF3Аh Длина сообщения: 5 байт.
    Код ошибки: 1 байт
    Тип фискального документа: 2 байта STLV
    Длина фискального документа: 2 байта

***************************************************************************/

int ShtrihFiscalPrinter::fsReadDocument(FSDocumentInfo& data)
{
    qDebug() << "fsReadDocument";
    PrinterCommand command(0xFF3A);
    command.write(sysPassword, 4);
    command.write(data.docNum, 4);
    int resultCode = send(command);
    if (resultCode == 0){
        data.docType = command.read16();
        data.docSize = command.read16();
    }
    return resultCode;
}

/***************************************************************************

Чтение TLV фискального документа
Код команды FF3Bh . Длина сообщения: 6 байт.
    Пароль системного администратора: 4 байта
Ответ: FF3Bh Длина сообщения: 1+N байт.
    Код ошибки:1 байт
    TLV структура: N байт
***************************************************************************/

int ShtrihFiscalPrinter::fsReadDocumentTLV(QByteArray& data)
{
    qDebug() << "fsReadDocumentTLV";
    PrinterCommand command(0xFF3B);
    command.write(sysPassword, 4);
    int resultCode = send(command);
    if (resultCode == 0){
        data = command.readBytes();
    }
    return resultCode;
}

/***************************************************************************
 *
Запрос квитанции о получении данных в ОФД по номеру документа

    Код команды FF3Сh . Длина сообщения: 11 байт.
    Пароль системного администратора: 4 байта
    Номер фискального документа: 4 байта

Ответ: FF3Сh Длина сообщения: 1+N байт.
    Код ошибки: 1 байт
    Квитанция: N байт

***************************************************************************/

int ShtrihFiscalPrinter::fsReadTicket(FSTicket& data)
{
    qDebug() << "fsReadTicket";
    PrinterCommand command(0xFF3C);
    command.write(sysPassword, 4);
    command.write(data.docNum, 4);
    int resultCode = send(command);
    if (resultCode == 0){
        data.ticket = command.readBytes();
    }
    return resultCode;
}

/***************************************************************************
Начать закрытие фискального режима
Код команды FF3Dh . Длина сообщения: 6 байт.
    Пароль системного администратора: 4 байта
Ответ: FF3Dh Длина сообщения: 1 байт.
    Код ошибки: 1 байт
***************************************************************************/

int ShtrihFiscalPrinter::fsStartClose()
{
    qDebug() << "fsStartClose";
    PrinterCommand command(0xFF3D);
    command.write(sysPassword, 4);
    return send(command);
}

/***************************************************************************
Закрыть фискальный режим
Код команды FF3Eh . Длина сообщения: 6 байт.
    Пароль системного администратора: 4 байта
Ответ: FF3Eh Длина сообщения: 9 байт.
    Код ошибки: 1 байт
    Номер ФД : 4 байта
    Фискальный признак: 4 байта
***************************************************************************/

int ShtrihFiscalPrinter::fsClose(FSDocument& data)
{
    qDebug() << "fsClose";
    PrinterCommand command(0xFF3E);
    command.write(sysPassword, 4);
    int resultCode = send(command);
    if (resultCode == 0){
        data.docNum = command.read32();
        data.docMac = command.read32();
    }
    return resultCode;
}

/***************************************************************************
Запрос количества ФД на которые нет квитанции
Код команды FF3Fh . Длина сообщения: 6 байт.
    Пароль системного администратора: 4 байта
Ответ: FF3Fh Длина сообщения: 3 байт.
    Код ошибки: 1 байт
    Количество неподтверждённых ФД : 2 байта
***************************************************************************/

int ShtrihFiscalPrinter::fsReadQueueSize(uint16_t& data)
{
    qDebug() << "fsReadQueueSize";
    PrinterCommand command(0xFF3F);
    command.write(sysPassword, 4);
    int resultCode = send(command);
    if (resultCode == 0){
        data = command.read16();
    }
    return resultCode;
}

/***************************************************************************
 *
Запрос параметров текущей смены
Код команды FF40h . Длина сообщения: 6 байт.
    Пароль системного администратора: 4 байта
Ответ: FF40h Длина сообщения: 6 байт.
    Код ошибки: 1 байт
    Состояние смены: 1 байт
    Номер смены : 2 байта
    Номер чека: 2 байта

***************************************************************************/

int ShtrihFiscalPrinter::fsReadDayStatus(FSDayStatus& data)
{
    qDebug() << "fsReadDayStatus";
    PrinterCommand command(0xFF40);
    command.write(sysPassword, 4);
    int resultCode = send(command);
    if (resultCode == 0){
        data.status = command.read8();
        data.dayNumber = command.read16();
        data.recNumber = command.read16();
    }
    return resultCode;
}

/***************************************************************************
Начать открытие смены
Код команды FF41h . Длина сообщения: 6 байт.
    Пароль системного администратора: 4 байта
Ответ: FF41h Длина сообщения: 1 байт.
    Код ошибки: 1 байт
***************************************************************************/

int ShtrihFiscalPrinter::fsStartOpenDay()
{
    qDebug() << "fsStartOpenDay";
    PrinterCommand command(0xFF41);
    command.write(sysPassword, 4);
    return send(command);
}

/***************************************************************************
 *
Начать закрытие смены

Код команды FF42h . Длина сообщения: 6 байт.
    Пароль системного администратора: 4 байта
Ответ: FF42h Длина сообщения: 1 байт.
    Код ошибки: 1 байт

***************************************************************************/

int ShtrihFiscalPrinter::fsStartCloseDay()
{
    qDebug() << "fsStartCloseDay";
    PrinterCommand command(0xFF42);
    command.write(sysPassword, 4);
    return send(command);
}


/***************************************************************************
Закрыть смену в ФН
Код команды FF43h . Длина сообщения: 6 байт.
    Пароль системного администратора: 4 байт
Ответ: FF43h Длина сообщения: 11 байт.
    Код ошибки: 1 байт
    Номер только что закрытой смены: 2 байта
    Номер ФД :4 байта
    Фискальный признак: 4 байта
***************************************************************************/

int ShtrihFiscalPrinter::fsCloseDay(FSCloseDay& data)
{
    qDebug() << "fsCloseDay";
    PrinterCommand command(0xFF43);
    command.write(sysPassword, 4);
    int resultCode = send(command);
    if (resultCode == 0){
        data.dayNumber = command.read16();
        data.docNum = command.read32();
        data.docMac = command.read32();
    }
    return resultCode;
}

/***************************************************************************
 *
Операция со скидками, надбавками и налогом

Код команды FF44h . Длина сообщения: 254 байт.
    Пароль системного администратора: 4 байта
    Тип операции: 1 байт
    1 – Приход,
    2 – Возврат прихода,
    3 – Расход,
    4 – Возврат расхода
    Количество: 5 байт 0000000000…9999999999
    Цена: 5 байт 0000000000…9999999999
    Скидка: 5 байт 0000000000…9999999999
    Надбавка: 5 байт 0000000000…9999999999
    Налог: 5 байт 0000000000…9999999999
    Номер отдела: 1 байт
    0…16 – режим свободной продажи, 255 – режим продажи по коду товара
    Налог: 1 байт
    Бит 1 «0» – нет, «1» – 1 налоговая группа
    Бит 2 «0» – нет, «1» – 2 налоговая группа
    Бит 3 «0» – нет, «1» – 3 налоговая группа
    Бит 4 «0» – нет, «1» – 4 налоговая группа
    Штрих-код: 5 байт 000000000000…999999999999
    Текст: 215 байта строка - название товара и скидки
    Примечание: если строка начинается символами \\ то она передаётся на сервер
    ОФД но не печатается на кассе. Названия товара и скидки должны заканчиваться
    нулём (Нуль терминированные строки).
    Примечание: налог является справочной информацией и передаётся извне в случае,
    когда касса не может его рассчитать сама.

Ответ: FF44h Длина сообщения: 1 байт.
    Код ошибки: 1 байт

***************************************************************************/

int ShtrihFiscalPrinter::fsPrintSale(FSSale& data)
{
    qDebug() << "fsPrintSale";
    PrinterCommand command(0xFF44);
    command.write(sysPassword, 4);
    command.write8(data.operation);
    command.write(data.quantity, 5);
    command.write(data.price, 5);
    command.write(data.discount, 5);
    command.write(data.charge, 5);
    command.write(data.taxAmount, 5);
    command.write(data.department, 1);
    command.write(data.tax, 1);
    command.write(data.barcode, 5);
    command.write(data.text);
    return send(command);
}

/***************************************************************************

Закрытие чека расширенное вариант V2 FF45H
    Код команды FF45H. Длина сообщения: 182 байт.
    Пароль системного администратора: 4 байта
    Сумма наличных (5 байт)
    Сумма типа оплаты 2 (5 байт)
    Сумма типа оплаты 3 (5 байт)
    Сумма типа оплаты 4 (5 байт)
    Сумма типа оплаты 5 (5 байт)
    Сумма типа оплаты 6 (5 байт)
    Сумма типа оплаты 7 (5 байт)
    Сумма типа оплаты 8 (5 байт)
    Сумма типа оплаты 9 (5 байт)
    Сумма типа оплаты 10 (5 байт)
    Сумма типа оплаты 11 (5 байт)
    Сумма типа оплаты 12 (5 байт)
    Сумма типа оплаты 13 (5 байт)
    Сумма типа оплаты 14 (5 байт) (предоплата)
    Сумма типа оплаты 15 (5 байт) (постоплата)
    Сумма типа оплаты 16 (5 байт) (встречное представление)
    Округление до рубля в копейках (1 байт)
    Налог 1 (5 байт) (НДС 18%)
    Налог 2 (5 байт) (НДС 10%)
    Оборот по налогу 3 (5 байт) (НДС 0%)
    Оборот по налогу 4 (5 байт) (Без НДС)
    Налог 5 (5 байт) (НДС расч. 18/118)
    Налог 6 (5 байт) (НДС расч. 10/110)
    Система налогообложения(1 байт)
    Текст (0-64 байт)

Ответ: FF45h Длина сообщения: 6 байт.
    Код ошибки: 1 байт
    Сдача ( 5 байт) 0000000000…9999999999

***************************************************************************/

int ShtrihFiscalPrinter::fsCloseReceipt(FSCloseReceipt& data)
{
    qDebug() << "fsCloseReceipt";
    PrinterCommand command(0xFF45);
    command.write(sysPassword, 4);
    for (int i=0;i<16;i++){
        command.write(data.payments[i], 5);
    }
    command.write(data.roundAmount, 1);
    for (int i=0;i<6;i++){
        command.write8(data.taxAmount[i]);
    }
    command.write8(data.taxSystem);
    command.write(data.text);
    int resultCode = send(command);
    if (resultCode == 0){
        data.change = command.read(5);
    }
    return resultCode;
}

/***************************************************************************

Операция V2

Операция V2 FF46H
Код команды FF46h . Длина сообщения:  160 байта.
    Пароль: 4 байта
    Тип операции: 1 байт
    1 – Приход,
    2 – Возврат прихода,
    3 – Расход,
    4 – Возврат расхода
    Количество: 6 байт ( 6 знаков после запятой )
    Цена:             5 байт
    Сумма операции 5 байт *
    Налог:           5 байт **
    Налоговая ставка:  1 байт
    Номер отдела: 1 байт
    0…16 – режим свободной продажи, 255 – режим продажи по коду товара
    Признак способа расчёта : 1 байт
    Признак предмета расчёта: 1 байт
    Наименование товара: 0-128 байт ASCII

Ответ: FF46h Длина сообщения: 1 байт.
    Код ошибки: 1 байт

***************************************************************************/

int ShtrihFiscalPrinter::fsPrintSale2(FSSale2& data)
{
    qDebug() << "fsPrintSale2";
    PrinterCommand command(0xFF46);
    command.write(sysPassword, 4);
    command.write8(data.operation);
    command.write(data.quantity, 6);
    command.write(data.price, 5);
    command.write(data.taxAmount, 5);
    command.write(data.tax, 1);
    command.write(data.department, 1);
    command.write(data.paymentType, 1);
    command.write(data.paymentMode, 1);
    command.write(data.text);
    return send(command);
}

/***************************************************************************

Сформировать чек коррекции V2 FF4AH

Код команды FF4Ah . Длина сообщения: 69 байт.
Пароль системного администратора: 4 байта
Тип коррекции :1 байт
Признак расчета:1байт
Сумма расчёта :5 байт
Сумма по чеку наличными:5 байт
Сумма по чеку электронными:5 байт
Сумма по чеку предоплатой:5 байт
Сумма по чеку постоплатой:5 байт
Сумма по чеку встречным представлением:5 байт
Сумма НДС 18%:5 байт
Сумма НДС 10%:5 байт
Сумма расчёта по ставке 0%:5 байт
Сумма расчёта по чеку без НДС:5 байт
Сумма расчёта по расч. ставке 18/118:5 байт
Сумма расчёта по расч. ставке 10/110:5 байт
Применяемая система налогообложения:1байт


Ответ:	    FF4Ah Длина сообщения: 11 байт.
Код ошибки: 1 байт
Номер чека: 2 байта
Номер ФД: 4 байта
Фискальный признак: 4 байт
***************************************************************************/

int ShtrihFiscalPrinter::fsPrintCorrection2(FSPrintCorrection2& data)
{
    qDebug() << "fsPrintCorrection2";
    PrinterCommand command(0xFF4A);
    command.write(sysPassword, 4);
    command.write(data.correctionType, 1);
    command.write(data.paymentType, 1);
    command.write(data.total, 5);
    for (int i=0;i<5;i++){
        command.write(data.payment[i], 5);
    }
    for (int i=0;i<6;i++){
        command.write(data.taxAmount[i], 5);
    }
    command.write(data.taxSystem, 1);
    int resultCode = send(command);
    if (resultCode == 0){
        data.recNum = command.read32();
        data.docNum = command.read32();
        data.docMac = command.read32();
    }
    return resultCode;
};

/***************************************************************************

Скидка, надбавка на чек для Роснефти FF4BH
Код команды FF4BH. Длина сообщения: 145 байт.
Пароль системного администратора: 4 байта
Скидка:      5 байт
Надбавка:    5 байт
Налог:  1 байт
Описание скидки или надбавки: 128 байт ASCII
Ответ:    FF4Bh Длина сообщения: 1 байт.
Код ошибки: 1 байт
***************************************************************************/

int ShtrihFiscalPrinter::fsDiscountCharge(FSDiscountCharge& data)
{
    qDebug() << "fsDiscountCharge";
    PrinterCommand command(0xFF4B);
    command.write(sysPassword, 4);
    command.write(data.discount, 5);
    command.write(data.charge, 5);
    command.write(data.tax, 1);
    command.write(data.text);
    return send(command);
};


int ShtrihFiscalPrinter::fsWriteTag(uint16_t tagId, QString tagValue){
    qDebug() << "fsWriteTag";

    TLVList list;
    list.add(tagId, tagValue);
    QByteArray ba = list.getData();

    int rc = fsWriteTLV(ba);
    if ((rc == 0) && printTagsEnabled)
    {
        printTag(tagId, tagValue);
    }
}

int ShtrihFiscalPrinter::printTag(uint16_t tagId, QString tagValue)
{
    int rc = 0;
    TlvTag* tlvTag = tlvTags.find(tagId);
    if (tlvTag != NULL)
    {
        QString text = tlvTag->shortDescription;
        text.append(": " + tagValue);

        PrintStringCommand command;
        command.text = text;
        command.flags = SMFP_STATION_REC;
        rc = printString(command);
    }
    return rc;
}

void ShtrihFiscalPrinter::setServerParams(ServerParams value){
    serverParams = value;
}

void ShtrihFiscalPrinter::journalPrintCurrentDay()
{
    //JournalPrinter journal(journalFileName, printer);
    //journal.printCurrentDay();
}

void ShtrihFiscalPrinter::journalPrintDay(int dayNumber)
{}

void ShtrihFiscalPrinter::journalPrintDoc(int docNumber)
{}

void ShtrihFiscalPrinter::journalPrintDocRange(int N1, int N2)
{}



