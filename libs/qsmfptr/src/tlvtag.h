#ifndef TLVTAG_H
#define TLVTAG_H

#include <QMap>
#include <QList>
#include <QString>
#include <QTextCodec>


enum TagType{
  ttByte,
  ttUint16,
  ttUInt32,
  ttVLN,
  ttFVLN,
  ttBitMask,
  ttUnixTime,
  ttString,
  ttSTLV,
  ttByteArray
};

class TlvTag
{
public:
    TlvTag();

    int tag;
    int length;
    QString description;
    TagType tagType;
    QString shortDescription;
    bool fixedLength;
    QByteArray buffer;

    void addInt(int value)
    {
        buffer.append((value >> 0) & 0xFF);
        buffer.append((value >> 8) & 0xFF);
    }

    QByteArray to866(QString value)
    {
        QTextCodec* codec = QTextCodec::codecForName("IBM 866");
        if (codec != nullptr)
        {
            return codec->fromUnicode(value);
        }
        return value.toLocal8Bit();
    }

    QByteArray getData(QString tagValue)
    {
        buffer.clear();

        addInt(tag);
        addInt(tagValue.length());
        buffer.append(to866(tagValue));
        return buffer;
    }

};

class TlvTags
{
    TlvTag tlvTag;
    QMap<int, TlvTag> items;
public:
    TlvTags(){
        createTags();
    }

    void add(int tag, QString description,
        QString shortDescription, TagType tagType,
        int length, bool fixedLength = false)
    {
        TlvTag item;
        item.tag = tag;
        item.length = length;
        item.description = description;
        item.tagType = tagType;
        item.shortDescription = shortDescription;
        item.fixedLength = fixedLength;
        items.insert(tag, item);
    }


    TlvTag* find(int tag)
    {
        tlvTag = items.find(tag).value();
        return &tlvTag;
    }

    void createTags()
    {
      add(1000, "наименование документа", "НАИМ. ДОК.",	ttString, 0);
      add(1001, "признак автоматического режима", "ПРИЗН. АВТОМ. РЕЖ.", ttByte, 1);
      add(1002, "признак автономного режима", "ПРИЗН. АВТОНОМН. РЕЖ.", ttByte, 1);
      add(1005, "адрес оператора перевода", "АДР. ОПЕР. ПЕРЕВОДА", ttString, 256);
      add(1008, "телефон или электронный адрес покупателя", "ТЕЛ. ИЛИ EMAIL ПОКУПАТЕЛЯ", ttString, 64);
      add(1009, "адрес расчетов", "АДР.РАСЧЕТОВ", ttString, 256);
      // Старые теги
      add(1010, "размер вознаграждения банковского агента", "РАЗМЕР ВОЗНАГР. БАНК. АГЕНТА", ttVLN, 8);
      add(1011, "размер вознаграждения платежного агента", "РАЗМЕР ВОЗНАГР. ПЛАТ. АГЕНТА", ttVLN, 8);

      add(1012, "дата, время", "ДАТА, ВРЕМЯ", ttUnixTime, 4);
      add(1013, "заводской номер ККТ", "ЗАВ. НОМЕР ККТ", ttString, 20);
      add(1016, "ИНН оператора перевода", "ИНН ОПЕРАТОРА ПЕРЕВОДА", ttString, 12, true);
      add(1017, "ИНН ОФД", "ИНН ОФД", ttString, 12, true);
      add(1018, "ИНН пользователя", "ИНН ПОЛЬЗ.", ttString, 12, true);
      add(1020, "сумма расчета, указанного в чеке (БСО)", "СУММА РАСЧЕТА В ЧЕКЕ(БСО)", ttVLN, 6);
      add(1021, "кассир", "КАССИР", ttString, 64);
      add(1022, "код ответа ОФД", "КОД ОТВЕРА ОФД", ttByte, 1);
      add(1023, "количество предмета расчета", "КОЛ-ВО ПРЕДМ. РАСЧЕТА", ttFVLN, 8);
      add(1026, "наименование оператора перевода", "НАИМЕН. ОПЕР. ПЕРЕВОДА", ttString, 64);
      add(1030, "наименование предмета расчета", "НАИМЕН. ПРЕДМ. РАСЧЕТА", ttString, 128);
      add(1031, "сумма по чеку (БСО) наличными", "СУММА ПО ЧЕКУ НАЛ (БСО)", ttVLN, 6);
      add(1036, "номер автомата", "НОМЕР АВТОМАТА", ttString, 20);
      add(1037, "регистрационный номер ККТ", "РЕГ. НОМЕР ККТ", ttString, 20, true);
      add(1038, "номер смены", "НОМЕР СМЕНЫ", ttUInt32, 4);
      add(1040, "номер ФД", "НОМЕР ФД", ttUInt32, 4);
      add(1041, "номер ФН", "НОМЕР ФН", ttString, 16, true);
      add(1042, "номер чека за смену", "НОМЕР ЧЕКА ЗА СМЕНУ", ttUInt32, 4);
      add(1043, "стоимость предмета расчета", "СТОИМ. ПРЕДМ. РАСЧЕТА", ttVLN, 6);
      add(1044, "операция платежного агента", "ОПЕРАЦИЯ ПЛАТ. АГЕНТА", ttString, 24);
      // Старый тег
      add(1045, "операция банковского субагента", "ОПЕРАЦИЯ БАНК. СУБАГЕНТА", ttString, 24);

      add(1046, "наименование ОФД", "НАИМЕН. ОФД", ttString, 256);
      add(1048, "наименование пользователя", "НАИМЕН. ПОЛЬЗ.", ttString, 256);
      add(1050, "признак исчерпания ресурса ФН", "ПРИЗН. ИСЧЕРП. РЕСУРСА ФН", ttByte, 1);
      add(1051, "признак необходимости срочной замены ФН", "ПРИЗН. НЕОБХ. СРОЧН. ЗАМЕНЫ ФН", ttByte, 1);
      add(1052, "признак переполнения памяти ФН", "ПРИЗН. ПЕРЕПОЛН. ПАМЯТИ ФН", ttByte, 1);
      add(1053, "признак превышения времени ожидания ответа ОФД", "ПРИЗН ПРЕВЫШ. ВРЕМЕНИ ОЖИД. ОТВ. ОФД", ttByte, 1);
      add(1054, "признак расчета", "ПРИЗН. РАСЧЕТА", ttByte, 1);
      add(1055, "применяемая система налогообложения", "ПРИМЕН. СИСТ. НАЛОГООБЛОЖЕНИЯ", ttByte, 1);
      add(1056, "признак шифрования", "ПРИЗН. ШИФРОВАНИЯ", ttByte, 1);
      add(1057, "признак платежного агента", "ПРИЗН. ПЛАТ. АГЕНТА", ttByte, 1);
      add(1059, "предмет расчета", "ПРЕДМ. РАСЧЕТА", ttSTLV, 1024);
      add(1060, "адрес сайта ФНС", "АДР. САЙТА ФНС", ttString, 256);
      add(1062, "системы налогообложения", "СИСТЕМЫ НАЛОГООБЛ.", ttByte, 1);
      add(1068, "сообщение оператора для ФН", "СООБЩ. ОПЕРАТОРА ДЛЯ ФН", ttSTLV, 9);
      add(1073, "телефон платежного агента", "ТЕЛ. ПЛАТ. АГЕНТА", ttString, 19);
      add(1074, "телефон оператора по приему платежей", "ТЕЛ ОПЕР. ПО ПРИЕМУ ПЛАТЕЖЕЙ", ttString, 19);
      add(1075, "телефон оператора перевода", "ТЕЛ. ОПЕР. ПЕРЕВОДА", ttString, 19);
      add(1077, "ФП документа", "ФП ДОКУМЕНТА", ttByteArray, 6);
      add(1078, "ФП оператора", "ФП ОПЕРАТОРА", ttByteArray, 16);
      add(1079, "цена за единицу предмета расчета", "ЦЕНА ЗА ЕД. ПРЕДМ. РАСЧ.", ttVLN, 6);
      add(1080, "штриховой код EAN 13", "ШК EAN 13", ttString, 16);
      add(1081, "сумма по чеку (БСО) электронными", "СУММА ПО ЧЕКУ ЭЛЕКТРОННЫМИ(БСО)", ttVLN, 6);
      // Старые теги
      add(1082, "телефон банковского субагента", "ТЕЛ. БАНК. СУБАГЕНТА", ttString, 19);
      add(1083, "телефон платежного субагента", "ТЕЛ. ПЛАТ. СУБАГЕНТА", ttString, 19);


      add(1084, "дополнительный реквизит пользователя", "ДОП. РЕКВИЗИТ ПОЛЬЗОВ.", ttSTLV, 320);
      add(1085, "наименование дополнительного реквизита пользователя", "НАИМЕН. ДОП. РЕКВИЗИТ. ПОЛЬЗОВ.", ttString, 64);
      add(1086, "значение дополнительного реквизита пользователя", "ЗНАЧ. ДОП. РЕКВИЗИТ. ПОЛЬЗОВ.", ttString, 256);
      add(1097, "количество непереданных ФД", "КОЛ-ВО НЕПЕРЕДАННЫХ ФД", ttUInt32, 4);
      add(1098, "дата и время первого из непереданных ФД", "ДАТА И ВРЕМЯ ПЕРВОГО НЕПЕРЕДАНН. ФД", ttUnixTime, 4);
      add(1101, "код причины перерегистрации", "КОД ПРИЧИНЫ ПЕРЕРЕГИСТР.", ttByte, 1);
      add(1102, "сумма НДС чека по ставке 18%", "СУММА НДС ЧЕКА 18%", ttVLN, 6);
      add(1103, "сумма НДС чека по ставке 10%", "СУММА НДС ЧЕКА 10%", ttVLN, 6);
      add(1104, "сумма расчета по чеку с НДС по ставке 0%", "СУММА РАСЧ. ПО ЧЕКУ 0%", ttVLN, 6);
      add(1105, "сумма расчета по чеку без НДС", "СУММА РАСЧ. ПО ЧЕКУ БЕЗ НДС", ttVLN, 6);
      add(1106, "сумма НДС чека по расч. ставке 18/118", "СУММА НДС ЧЕКА ПО РАСЧ. СТАВКЕ 18/118", ttVLN, 6);
      add(1107, "сумма НДС чека по расч. ставке 10/110", "СУММА НДС ЧЕКА ПО РАСЧ. СТАВКЕ 10/110", ttVLN, 6);
      add(1108, "признак ККТ для расчетов только в Интернет", "ПРИЗН. ККТ ДЛЯ РАСЧ. ТОЛЬКО В ИНТЕРНЕТ", ttByte, 1);
      add(1109, "признак расчетов за услуги", "ПРИЗН. РАСЧ. ЗА УСЛУГИ", ttByte, 1);
      add(1110, "признак АС БСО", "ПРИЗН. АС БСО", ttByte, 1);
      add(1111, "общее количество ФД за смену", "ОБЩ. КЛ-ВО ФД ЗА СМЕНУ", ttUInt32, 4);
      add(1116, "номер первого непереданного документа", "НОМЕР ПЕРВОГО НЕПЕРЕДАНН. ДОК-ТА", ttUInt32, 4);
      add(1117, "адрес электронной почты отправителя чека", "АДР. ЭЛ. ПОЧТЫ ОТПРАВ. ЧЕКА", ttString, 64);
      add(1118, "количество кассовых чеков (БСО) за смену", "КОЛ-ВО КАССОВЫХ ЧЕКОВ ЗА СМЕНУ(БСО)", ttUInt32, 4);
      // старый тег
      add(1119, "телефон оператора по приему платежей", "ТЕЛ. ОПЕР. ПО ПРИЕМУ ПЛАТ.", ttString, 19);

      add(1126, "признак проведения лотереи", "ПРИЗН. ПРОВЕДЕНИЯ ЛОТЕРЕИ", ttByte, 1);
      add(1129, "счетчики операций «приход»", "СЧЕТЧИКИ ОПЕР. ПРИХОД", ttSTLV, 116);
      add(1130, "счетчики операций «возврат прихода»", "СЧЕТЧИКИ ОПЕР. ВОЗВР. ПРИХОДА", ttSTLV, 116);
      add(1131, "счетчики операций «расход»", "СЧЕТЧИКИ ОПЕР. РАСХОД", ttSTLV, 116);
      add(1132, "счетчики операций «возврат расхода»", "СЧЕТЧИКИ ОПЕР. ВОЗВР. РАСХОДА", ttSTLV, 116);
      add(1133, "счетчики операций по чекам коррекции", "СЧЕТЧИКИ ОПЕР ПО ЧЕКАМ КОРР.", ttSTLV, 216);
      add(1134, "количество чеков (БСО) со всеми признаками расчетов", "КОЛ-ВО ЧЕКОВ БСО СО ВСЕМИ ПРИЗН. РАСЧ.", ttUInt32, 4);
      add(1135, "количество чеков по признаку расчетов", "КОЛ-ВО ЧЕКОВ ПО ПРИЗН. РАСЧ.", ttUInt32, 4);
      add(1136, "итоговая сумма в чеках (БСО) наличными", "ИТОГ. СУММ. В ЧЕКАХ БСО НАЛ.", ttVLN, 8);
      add(1138, "итоговая сумма в чеках (БСО) электронными", "ИТОГ СУММА В ЧЕКАХ БСО ЭЛЕКТР.", ttVLN, 8);
      add(1139, "сумма НДС по ставке 18%", "СУММА НДС 18%", ttVLN, 8);
      add(1140, "сумма НДС по ставке 10%", "СУММА НДС 10%", ttVLN, 8);
      add(1141, "сумма НДС по расч. ставке 18/118", "СУММА НДС ПО РАСЧ. СТАВКЕ 18/118", ttVLN, 8);
      add(1142, "сумма НДС по расч. ставке 10/110", "СУММА НДС ПО РАСЧ. СТАВКЕ 10/110", ttVLN, 8);
      add(1143, "сумма расчетов с НДС по ставке 0%", "СУММА РАСЧ. С НДС 0%", ttVLN, 8);
      add(1144, "количество чеков коррекции", "КОЛ-ВО ЧЕКОВ ОПЕРАЦИИ", ttUInt32, 4);
      add(1145, "счетчики коррекций «приход»", "СЧЕТЧ. КОРРЕКЦИЙ ПРИХОД", ttSTLV, 100);
      add(1146, "счетчики коррекций «расход»", "СЧЕТЧ. КОРРЕКЦИЙ РАСХОД", ttSTLV, 100);
      add(1148, "количество самостоятельных корректировок", "КОЛ-ВО САМОСТ. КОРРЕКТИРОВОК", ttUInt32, 4);
      add(1149, "количество корректировок по предписанию", "КОЛ-ВО КОРРЕКТИРОВОК ПО ПРЕДПИС.", ttUInt32, 4);
      add(1151, "сумма коррекций НДС по ставке 18%", "СУММА КОРРЕКЦИЙ НДС 18%", ttVLN, 8);
      add(1152, "сумма коррекций НДС по ставке 10%", "СУММА КОРРЕКЦИЙ НДС 10%", ttVLN, 8);
      add(1153, "сумма коррекций НДС по расч. ставке 18/118", "СУММА КОРРЕКЦИЙ НДС ПО РАСЧ. СТ. 18/110", ttVLN, 8);
      add(1154, "сумма коррекций НДС расч. ставке 10/110", "СУММА КОРРЕКЦИЙ НДС ПО РАС. СТ. 10/110", ttVLN, 8);
      add(1155, "сумма коррекций с НДС по ставке 0%", "СУММА КОРРЕКЦИЙ НДС 0%", ttVLN, 8);
      add(1157, "счетчики итогов ФН", "СЧЕТЧИКИ ИТОГОВ ФН", ttSTLV, 708);
      add(1158, "счетчики итогов непереданных ФД", "СЧЕТЧ ИТОГОВ НЕПЕРЕД. ФД", ttSTLV, 708);
      add(1162, "код товарной номенклатуры", "КОД ТОВАРН. НОМЕНКЛ.", ttByteArray, 32);
      add(1171, "телефон поставщика", "ТЕЛ. ПОСТАВЩИКА", ttString, 19);
      add(1173, "тип коррекции", "ТИП КОРРЕКЦИИ", ttByte, 1);
      add(1174, "основание для коррекции", "ОСНОВАНИЕ ДЛЯ КООРЕКЦИИ", ttSTLV, 292);
      add(1177, "наименование основания для коррекции", "НАИМЕН. ОСН. ДЛЯ КОРРЕКЦ", ttString, 256);
      add(1178, "дата документа основания для коррекции", "ДАТА ДОК-ТА ОСН. ДЛЯ КОРРЕКЦ", ttUnixTime, 4);
      add(1179, "номер документа основания для коррекции", "НОМЕР ДОК-ТА ОСН. ДЛЯ КОРРЕКЦ", ttString, 32);
      add(1183, "сумма расчетов без НДС", "СУММА РАСЧ. БЕЗ НДС", ttVLN, 8);
      add(1184, "сумма коррекций без НДС", "СУММА КОРРЕКЦ. БЕЗ НДС", ttVLN, 8);
      add(1187, "место расчетов", "МЕСТО РАСЧЕТОВ", ttString, 256);
      add(1188, "версия ККТ", "ВЕРСИЯ ККТ", ttString, 8);
      add(1189, "версия ФФД ККТ", "ВЕРСИЯ ФФД ККТ", ttByte, 1);
      add(1190, "версия ФФД ФН", "ВЕРСИЯ ФФД ФН", ttByte, 1);
      add(1191, "дополнительный реквизит предмета расчета", "ДОП. РЕКВ. ПРЕДМ. РАСЧЕТА", ttString, 64);
      add(1192, "дополнительный реквизит чека (БСО)", "ДОП. РЕКВ. ЧЕКА БСО", ttString, 16);
      add(1193, "признак проведения азартных игр", "ПРИЗН. ПРОВЕД. АЗАРТН. ИГР", ttByte, 1);
      add(1194, "счетчики итогов смены", "СЧЕТЧИКИ ИТОГОВ СМЕНЫ", ttSTLV, 708);
      add(1196, "QR-код", "QR-КОД", ttString, 0);
      add(1197, "единица измерения предмета расчета", "ЕД. ИЗМЕР. ПРЕДМЕТА РАСЧ.", ttString, 16);
      add(1198, "размер НДС за единицу предмета расчета", "РАЗМ. НДС ЗА ЕД. ПРЕДМ. РАСЧ.", ttVLN, 6);
      add(1199, "ставка НДС ", "СТАВКА НДС", ttByte, 1);
      add(1200, "сумма НДС за предмет расчета", "СУММА НДС ЗА ПРЕДМ. РАСЧ.", ttVLN, 6);
      add(1201, "общая итоговая сумма в чеках (БСО)", "ОБЩ. ИТОГ. СУММА В ЧЕКАХ БСО", ttVLN, 8);
      add(1203, "ИНН кассира", "ИНН КАССИРА", ttString, 12, true);
      add(1205, "коды причин изменения сведений о ККТ", "КОДЫ ПРИЧИНЫ ИЗМ. СВЕД. О ККТ", ttBitMask, 4);
      add(1206, "сообщение оператора", "СООБЩ. ОПЕР.", ttBitMask, 1);
      add(1207, "признак торговли подакцизными товарами", "ПРИЗН. ТОРГОВЛИ ПОДАКЦИЗН. ТОВАРАМИ", ttByte, 1);
      add(1208, "сайт чеков", "САЙТ ЧЕКОВ", ttString, 256);
      add(1209, "версия ФФД", "ВЕРСИЯ ФФД", ttByte, 1);
      add(1212, "признак предмета расчета", "ПРИЗН. ПРЕДМЕТА РАСЧ.", ttByte, 1);
      add(1213, "ресурс ключей ФП", "РЕСУРС КЛЮЧЕЙ ФП", ttUint16, 2);
      add(1214, "признак способа расчета", "ПРИЗН. СПОСОБА РАСЧ.", ttByte, 1);
      add(1215, "сумма по чеку (БСО) предоплатой (зачетом аванса))", "СУММА ПО ЧЕКУ БСО ПРЕДОПЛ.", ttVLN, 6);
      add(1216, "сумма по чеку (БСО) постоплатой (в кредит)", "СУММА ПО ЧЕКУ БСО ПОСТОПЛ.", ttVLN, 6);
      add(1217, "сумма по чеку (БСО) встречным предоставлением", "СУММА ПО ЧЕКУ БСО ВСТРЕЧН. ПРЕДОСТ.", ttVLN, 6);
      add(1218, "итоговая сумма в чеках (БСО) предоплатами (авансами)", "ИТОГ. СУММА В ЧЕКАХ БСО ПРЕДОПЛ.", ttVLN, 8);
      add(1219, "итоговая сумма в чеках (БСО) постоплатами (кредитами)", "ИТОГ. СУММА В ЧЕКАХ БСО ПОСТОПЛ.", ttVLN, 8);
      add(1220, "итоговая сумма в чеках (БСО) встречными предоставлениями", "ИТОГ. СУММА В ЧЕКАХ БСО ВСТРЕЧН. ПРЕДОСТ.", ttVLN, 8);
      add(1221, "признак установки принтера в автомате", "ПРИЗН. УСТАНОВКИ ПРИНТЕРА В АВТОМАТЕ", ttByte, 1);
      add(1222, "признак агента по предмету расчета", "ПРИЗН. АГ. ПО ПРЕДМ. РАСЧ", ttByte, 1);
      add(1223, "данные агента", "ДАННЫЕ АГЕНТА", ttSTLV, 512);
      add(1224, "данные поставщика", "ДАННЫЕ ПОСТАВЩИКА", ttSTLV, 512);
      add(1225, "наименование поставщика", "НАИМЕН. ПОСТАВЩИКА", ttString, 256);
      add(1226, "ИНН поставщика", "ИНН ПОСТАВЩИКА", ttString, 12, true);
    }

};

#endif // TLVTAG_H