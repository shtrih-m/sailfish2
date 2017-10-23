# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application


TEMPLATE = lib
TARGET = qsmfptr

CONFIG += c++11
CONFIG += staticlib
QT += network
QT += bluetooth

INCLUDEPATH += $$PWD\src

TRANSLATIONS = translations/ShtrihFiscalPrinter_ru.ts

DEFINES += NO_PNG ZINT_VERSION="2.4.2"

HEADERS += \
    src/fiscalprinter.h \
    src/MemStream.h \
    src/PrinterCommand.h \
    src/printerprotocol1.h \
    src/PrinterTypes.h \
    src/ShtrihFiscalPrinter.h \
    src/SocketPort.h \
    src/utils.h \
    src/bluetoothport.h \
    src/printertest.h \
    src/printerprotocol2.h \
    src/textfilter.h \
    src/bluetoothport2.h \
    src/serverconnection.h \
    src/tlvlist.h \
    src/tlvtag.h \
    src/journalprinter.h \
    src/xinputstream.h \
    src/debugutils.h

SOURCES += \
    src/MemStream.cpp \
    src/PrinterCommand.cpp \
    src/printerprotocol1.cpp \
    src/printerprotocol2.cpp \
    src/ShtrihFiscalPrinter.cpp \
    src/SocketPort.cpp \
    src/bluetoothport.cpp \
    src/PrinterTest.cpp \
    src/textfilter.cpp \
    src/bluetoothport2.cpp \
    src/utils.cpp \
    src/serverconnection.cpp \
    src/tlvlist.cpp \
    src/tlvtag.cpp \
    src/journalprinter.cpp \
    src/xinputstream.cpp \
    src/debugutils.cpp

HEADERS +=  src/zint/aztec.h \
            src/zint/code49.h \
            src/zint/common.h \
            src/zint/composite.h \
            src/zint/dmatrix.h \
            src/zint/font.h \
            src/zint/gs1.h \
            src/zint/large.h \
            src/zint/maxicode.h \
            src/zint/maxipng.h \
            src/zint/pdf417.h \
            src/zint/reedsol.h \
            src/zint/rss.h \
            src/zint/sjis.h \
            src/zint/zint.h \
            src/zint/gridmtx.h \
            src/zint/qzint.h

SOURCES += src/zint/png.c \
           src/zint/2of5.c \
           src/zint/auspost.c \
           src/zint/aztec.c \
           src/zint/code.c \
           src/zint/code128.c \
           src/zint/code16k.c \
           src/zint/code49.c \
           src/zint/common.c \
           src/zint/composite.c \
           src/zint/dmatrix.c \
           src/zint/gs1.c \
           src/zint/imail.c \
           src/zint/large.c \
           src/zint/library.c \
           src/zint/maxicode.c \
           src/zint/medical.c \
           src/zint/pdf417.c \
           src/zint/plessey.c \
           src/zint/postal.c \
           src/zint/ps.c \
           src/zint/reedsol.c \
           src/zint/rss.c \
           src/zint/svg.c \
           src/zint/telepen.c \
           src/zint/upcean.c \
           src/zint/qr.c \
           src/zint/dllversion.c \
           src/zint/code1.c \
           src/zint/gridmtx.c \
           src/zint/render.c \
           src/zint/qzint.cpp

OTHER_FILES += rpm/qsmfptr.spec \
    translations/*.ts \
    rpm/qsmfptr.yaml \
    *.ts

INCLUDEPATH += $$PWD/src
INCLUDEPATH += $$PWD/src/lib
INCLUDEPATH += $$PWD/src/zint

DEPENDPATH += $$PWD/src


DISTFILES +=

RESOURCES += \
    qsmfptr.qrc

