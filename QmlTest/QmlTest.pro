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
TARGET = QmlTest

CONFIG += sailfishapp
QT += bluetooth
QT += core

SOURCES += \
    src/QmlTest.cpp

OTHER_FILES += qml/QmlTest.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    qml/pages/SecondPage.qml \
    rpm/QmlTest.changes.in \
    rpm/QmlTest.spec \
    rpm/QmlTest.yaml \
    translations/*.ts \
    QmlTest.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 256x256

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n
CONFIG += c++11
# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/QmlTest-de.ts


unix:!macx: LIBS += -L$$OUT_PWD/../libs/qsmfptr/ -lqsmfptr

INCLUDEPATH += $$PWD/../libs/qsmfptr
DEPENDPATH += $$PWD/../libs/qsmfptr

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../libs/qsmfptr/libqsmfptr.a

RESOURCES += \
    qmltest.qrc
