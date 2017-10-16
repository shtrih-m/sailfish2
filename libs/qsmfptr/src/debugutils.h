#ifndef DEBUGUTILS_H
#define DEBUGUTILS_H

#include <stdio.h>
#include <math.h>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include "utils.h"

class DebugUtils
{
public:
    DebugUtils();

    static void writeTx(QByteArray ba){
        write("-> ", ba);
    }

    static void writeRx(QByteArray ba){
        write("<- ", ba);
    }

    static void write(QString prefix, QByteArray ba)
    {
        int blockSize = 20;
        int count = (ba.size()+blockSize-1)/blockSize;
        for (int i=0;i<count;i++)
        {
            int len = std::min(blockSize, ba.size()-i*blockSize);
            QByteArray block = ba.mid(i*blockSize, len);
            qDebug() << (prefix + StringUtils::dataToHex(block));
        }
    }

};

#endif // DEBUGUTILS_H
