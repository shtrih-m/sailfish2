#ifndef UTILS_H
#define UTILS_H

#include <QThread>

bool testBit(uint64_t value, int bit);

class StringUtils {
public:
    static QString intToHex(long long value, int len)
    {
        QByteArray ba;
        for (int i = 0; i < len; i++) {
            ba.insert(0, (value >> i * 8) & 0xFF);
        }
        return ba.toHex().toUpper();
    }

    static QString dataToHex(QByteArray ba)
    {
        QString result;
        for (int i = 0; i < ba.length(); i++) {
            if (i != 0) {
                result.append(" ");
            }
            QByteArray b;
            b.append(ba.at(i));
            result.append(b.toHex());
        }
        return result.toUpper();
    }
};

class QSleepThread : private QThread {
public:
    static void msleep(uint32_t msec)
    {
        QThread::msleep(msec);
    }
};

#endif // UTILS_H
