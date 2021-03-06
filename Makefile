#############################################################################
# Makefile for building: QmlTest
# Generated by qmake (3.0) (Qt 5.6.3)
# Project:  QmlTest/QmlTest.pro
# Template: app
# Command: /usr/lib/qt5/bin/qmake 'QMAKE_CFLAGS_RELEASE=-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -Wformat -Wformat-security -fmessage-length=0 -march=armv7-a -mfloat-abi=hard -mfpu=neon -mthumb -Wno-psabi' 'QMAKE_CFLAGS_DEBUG=-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -Wformat -Wformat-security -fmessage-length=0 -march=armv7-a -mfloat-abi=hard -mfpu=neon -mthumb -Wno-psabi' 'QMAKE_CXXFLAGS_RELEASE=-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -Wformat -Wformat-security -fmessage-length=0 -march=armv7-a -mfloat-abi=hard -mfpu=neon -mthumb -Wno-psabi' 'QMAKE_CXXFLAGS_DEBUG=-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -Wformat -Wformat-security -fmessage-length=0 -march=armv7-a -mfloat-abi=hard -mfpu=neon -mthumb -Wno-psabi' QMAKE_STRIP=: PREFIX=/usr -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug -o Makefile QmlTest/QmlTest.pro
#############################################################################

MAKEFILE      = Makefile

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_QML_DEBUG -DQT_QUICK_LIB -DQT_GUI_LIB -DQT_BLUETOOTH_LIB -DQT_QML_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB
CFLAGS        = -pipe -O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -Wformat -Wformat-security -fmessage-length=0 -march=armv7-a -mfloat-abi=hard -mfpu=neon -mthumb -Wno-psabi -fPIC -fvisibility=hidden -fvisibility-inlines-hidden -Wall -W -D_REENTRANT -fPIC $(DEFINES)
CXXFLAGS      = -pipe -O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -Wformat -Wformat-security -fmessage-length=0 -march=armv7-a -mfloat-abi=hard -mfpu=neon -mthumb -Wno-psabi -std=gnu++0x -fPIC -fvisibility=hidden -fvisibility-inlines-hidden -Wall -W -D_REENTRANT -fPIC $(DEFINES)
INCPATH       = -IQmlTest -I. -Ilibs/qsmfptr -isystem /usr/include/sailfishapp -isystem /usr/include/mdeclarativecache5 -isystem /usr/include/qt5 -isystem /usr/include/qt5/QtQuick -isystem /usr/include/qt5/QtGui -isystem /usr/include/qt5/QtBluetooth -isystem /usr/include/qt5/QtQml -isystem /usr/include/qt5/QtNetwork -isystem /usr/include/qt5/QtCore -I. -I/usr/share/qt5/mkspecs/linux-g++
QMAKE         = /usr/lib/qt5/bin/qmake
DEL_FILE      = rm -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
COPY          = cp -f
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
INSTALL_FILE  = install -m 644 -p
INSTALL_PROGRAM = install -m 755 -p
INSTALL_DIR   = cp -f -R
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
TAR           = tar -cf
COMPRESS      = gzip -9f
DISTNAME      = QmlTest1.0.0
DISTDIR = /home/src1/QmlTest/.tmp/QmlTest1.0.0
LINK          = g++
LFLAGS        = -Wl,-rpath,/usr/share/QmlTest/lib -Wl,-rpath-link,/usr/lib
LIBS          = $(SUBLIBS) -L/home/src1/QmlTest/../libs/qsmfptr/ -lqsmfptr -lsailfishapp -pie -rdynamic -lmdeclarativecache5 -lQt5Quick -lQt5Gui -lQt5Bluetooth -lQt5Qml -lQt5Network -lQt5Core -lGLESv2 -lpthread 
AR            = ar cqs
RANLIB        = 
SED           = sed
STRIP         = :

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = QmlTest/src/QmlTest.cpp qrc_qmltest.cpp
OBJECTS       = QmlTest.o \
		qrc_qmltest.o
DIST          = /usr/share/qt5/mkspecs/features/spec_pre.prf \
		/usr/share/qt5/mkspecs/common/unix.conf \
		/usr/share/qt5/mkspecs/common/linux.conf \
		/usr/share/qt5/mkspecs/common/sanitize.conf \
		/usr/share/qt5/mkspecs/common/gcc-base.conf \
		/usr/share/qt5/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt5/mkspecs/common/g++-base.conf \
		/usr/share/qt5/mkspecs/common/g++-unix.conf \
		/usr/share/qt5/mkspecs/qconfig.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_bluetooth.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_bluetooth_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_compositor.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_compositor_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_concurrent.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_concurrent_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_contacts.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_contacts_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_core.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_core_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_dbus.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_dbus_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_docgallery.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_docgallery_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_feedback.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_feedback_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_gui.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_gui_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_location.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_location_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_multimedia.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_multimedia_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_network.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_network_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_opengl.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_opengl_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_openglextensions.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_openglextensions_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_organizer.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_organizer_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_platformsupport_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_positioning.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_positioning_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_publishsubscribe.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_publishsubscribe_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_qmfclient.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_qmfclient_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_qmfmessageserver.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_qmfmessageserver_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_qml.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_qml_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_qtmultimediaquicktools_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_quick.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_quick_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_quickparticles_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_sensors.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_sensors_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_serviceframework.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_serviceframework_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_sql.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_sql_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_svg.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_svg_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_versit.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_versit_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_versitorganizer.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_versitorganizer_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_waylandclient.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_waylandclient_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_webkit.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_webkit_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_widgets.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_widgets_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_xml.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_xml_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_xmlpatterns.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_xmlpatterns_private.pri \
		/usr/share/qt5/mkspecs/features/qt_functions.prf \
		/usr/share/qt5/mkspecs/features/qt_config.prf \
		/usr/share/qt5/mkspecs/linux-g++/qmake.conf \
		/usr/share/qt5/mkspecs/features/spec_post.prf \
		/usr/share/qt5/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt5/mkspecs/features/default_pre.prf \
		/usr/share/qt5/mkspecs/features/resolve_config.prf \
		/usr/share/qt5/mkspecs/features/default_post.prf \
		/usr/share/qt5/mkspecs/features/sailfishapp_i18n.prf \
		/usr/share/qt5/mkspecs/features/sailfishapp.prf \
		/usr/share/qt5/mkspecs/features/link_pkgconfig.prf \
		/usr/share/qt5/mkspecs/features/qml_debug.prf \
		/usr/share/qt5/mkspecs/features/warn_on.prf \
		/usr/share/qt5/mkspecs/features/qt.prf \
		/usr/share/qt5/mkspecs/features/resources.prf \
		/usr/share/qt5/mkspecs/features/moc.prf \
		/usr/share/qt5/mkspecs/features/unix/opengl.prf \
		/usr/share/qt5/mkspecs/features/unix/thread.prf \
		/usr/share/qt5/mkspecs/features/file_copies.prf \
		/usr/share/qt5/mkspecs/features/testcase_targets.prf \
		/usr/share/qt5/mkspecs/features/exceptions.prf \
		/usr/share/qt5/mkspecs/features/yacc.prf \
		/usr/share/qt5/mkspecs/features/lex.prf \
		QmlTest/QmlTest/QmlTest.pro  QmlTest/src/QmlTest.cpp
QMAKE_TARGET  = QmlTest
DESTDIR       = 
TARGET        = QmlTest


first: all
####### Build rules

$(TARGET): /home/src1/QmlTest/../libs/qsmfptr/libqsmfptr.a $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: QmlTest/QmlTest.pro /usr/share/qt5/mkspecs/linux-g++/qmake.conf /usr/share/qt5/mkspecs/features/spec_pre.prf \
		/usr/share/qt5/mkspecs/common/unix.conf \
		/usr/share/qt5/mkspecs/common/linux.conf \
		/usr/share/qt5/mkspecs/common/sanitize.conf \
		/usr/share/qt5/mkspecs/common/gcc-base.conf \
		/usr/share/qt5/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt5/mkspecs/common/g++-base.conf \
		/usr/share/qt5/mkspecs/common/g++-unix.conf \
		/usr/share/qt5/mkspecs/qconfig.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_bluetooth.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_bluetooth_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_compositor.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_compositor_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_concurrent.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_concurrent_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_contacts.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_contacts_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_core.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_core_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_dbus.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_dbus_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_docgallery.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_docgallery_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_feedback.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_feedback_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_gui.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_gui_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_location.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_location_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_multimedia.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_multimedia_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_network.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_network_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_opengl.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_opengl_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_openglextensions.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_openglextensions_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_organizer.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_organizer_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_platformsupport_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_positioning.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_positioning_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_publishsubscribe.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_publishsubscribe_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_qmfclient.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_qmfclient_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_qmfmessageserver.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_qmfmessageserver_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_qml.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_qml_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_qtmultimediaquicktools_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_quick.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_quick_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_quickparticles_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_sensors.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_sensors_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_serviceframework.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_serviceframework_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_sql.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_sql_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_svg.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_svg_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_versit.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_versit_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_versitorganizer.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_versitorganizer_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_waylandclient.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_waylandclient_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_webkit.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_webkit_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_widgets.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_widgets_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_xml.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_xml_private.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_xmlpatterns.pri \
		/usr/share/qt5/mkspecs/modules/qt_lib_xmlpatterns_private.pri \
		/usr/share/qt5/mkspecs/features/qt_functions.prf \
		/usr/share/qt5/mkspecs/features/qt_config.prf \
		/usr/share/qt5/mkspecs/linux-g++/qmake.conf \
		/usr/share/qt5/mkspecs/features/spec_post.prf \
		/usr/share/qt5/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt5/mkspecs/features/default_pre.prf \
		/usr/share/qt5/mkspecs/features/resolve_config.prf \
		/usr/share/qt5/mkspecs/features/default_post.prf \
		/usr/share/qt5/mkspecs/features/sailfishapp_i18n.prf \
		/usr/share/qt5/mkspecs/features/sailfishapp.prf \
		/usr/share/qt5/mkspecs/features/link_pkgconfig.prf \
		/usr/share/qt5/mkspecs/features/qml_debug.prf \
		/usr/share/qt5/mkspecs/features/warn_on.prf \
		/usr/share/qt5/mkspecs/features/qt.prf \
		/usr/share/qt5/mkspecs/features/resources.prf \
		/usr/share/qt5/mkspecs/features/moc.prf \
		/usr/share/qt5/mkspecs/features/unix/opengl.prf \
		/usr/share/qt5/mkspecs/features/unix/thread.prf \
		/usr/share/qt5/mkspecs/features/file_copies.prf \
		/usr/share/qt5/mkspecs/features/testcase_targets.prf \
		/usr/share/qt5/mkspecs/features/exceptions.prf \
		/usr/share/qt5/mkspecs/features/yacc.prf \
		/usr/share/qt5/mkspecs/features/lex.prf \
		QmlTest/QmlTest.pro \
		QmlTest/qmltest.qrc \
		/usr/lib/libQt5Quick.prl \
		/usr/lib/libQt5Gui.prl \
		/usr/lib/libQt5Bluetooth.prl \
		/usr/lib/libQt5Qml.prl \
		/usr/lib/libQt5Network.prl \
		/usr/lib/libQt5Core.prl
	$(QMAKE) 'QMAKE_CFLAGS_RELEASE=-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -Wformat -Wformat-security -fmessage-length=0 -march=armv7-a -mfloat-abi=hard -mfpu=neon -mthumb -Wno-psabi' 'QMAKE_CFLAGS_DEBUG=-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -Wformat -Wformat-security -fmessage-length=0 -march=armv7-a -mfloat-abi=hard -mfpu=neon -mthumb -Wno-psabi' 'QMAKE_CXXFLAGS_RELEASE=-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -Wformat -Wformat-security -fmessage-length=0 -march=armv7-a -mfloat-abi=hard -mfpu=neon -mthumb -Wno-psabi' 'QMAKE_CXXFLAGS_DEBUG=-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -Wformat -Wformat-security -fmessage-length=0 -march=armv7-a -mfloat-abi=hard -mfpu=neon -mthumb -Wno-psabi' QMAKE_STRIP=: PREFIX=/usr -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug -o Makefile QmlTest/QmlTest.pro
/usr/share/qt5/mkspecs/features/spec_pre.prf:
/usr/share/qt5/mkspecs/common/unix.conf:
/usr/share/qt5/mkspecs/common/linux.conf:
/usr/share/qt5/mkspecs/common/sanitize.conf:
/usr/share/qt5/mkspecs/common/gcc-base.conf:
/usr/share/qt5/mkspecs/common/gcc-base-unix.conf:
/usr/share/qt5/mkspecs/common/g++-base.conf:
/usr/share/qt5/mkspecs/common/g++-unix.conf:
/usr/share/qt5/mkspecs/qconfig.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_bluetooth.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_bluetooth_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_compositor.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_compositor_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_concurrent.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_concurrent_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_contacts.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_contacts_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_core.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_core_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_dbus.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_dbus_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_docgallery.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_docgallery_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_feedback.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_feedback_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_gui.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_gui_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_location.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_location_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_multimedia.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_multimedia_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_network.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_network_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_opengl.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_opengl_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_openglextensions.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_openglextensions_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_organizer.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_organizer_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_platformsupport_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_positioning.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_positioning_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_publishsubscribe.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_publishsubscribe_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_qmfclient.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_qmfclient_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_qmfmessageserver.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_qmfmessageserver_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_qml.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_qml_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_qtmultimediaquicktools_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_quick.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_quick_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_quickparticles_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_sensors.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_sensors_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_serviceframework.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_serviceframework_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_sql.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_sql_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_svg.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_svg_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_versit.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_versit_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_versitorganizer.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_versitorganizer_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_waylandclient.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_waylandclient_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_webkit.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_webkit_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_widgets.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_widgets_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_xml.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_xml_private.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_xmlpatterns.pri:
/usr/share/qt5/mkspecs/modules/qt_lib_xmlpatterns_private.pri:
/usr/share/qt5/mkspecs/features/qt_functions.prf:
/usr/share/qt5/mkspecs/features/qt_config.prf:
/usr/share/qt5/mkspecs/linux-g++/qmake.conf:
/usr/share/qt5/mkspecs/features/spec_post.prf:
/usr/share/qt5/mkspecs/features/exclusive_builds.prf:
/usr/share/qt5/mkspecs/features/default_pre.prf:
/usr/share/qt5/mkspecs/features/resolve_config.prf:
/usr/share/qt5/mkspecs/features/default_post.prf:
/usr/share/qt5/mkspecs/features/sailfishapp_i18n.prf:
/usr/share/qt5/mkspecs/features/sailfishapp.prf:
/usr/share/qt5/mkspecs/features/link_pkgconfig.prf:
/usr/share/qt5/mkspecs/features/qml_debug.prf:
/usr/share/qt5/mkspecs/features/warn_on.prf:
/usr/share/qt5/mkspecs/features/qt.prf:
/usr/share/qt5/mkspecs/features/resources.prf:
/usr/share/qt5/mkspecs/features/moc.prf:
/usr/share/qt5/mkspecs/features/unix/opengl.prf:
/usr/share/qt5/mkspecs/features/unix/thread.prf:
/usr/share/qt5/mkspecs/features/file_copies.prf:
/usr/share/qt5/mkspecs/features/testcase_targets.prf:
/usr/share/qt5/mkspecs/features/exceptions.prf:
/usr/share/qt5/mkspecs/features/yacc.prf:
/usr/share/qt5/mkspecs/features/lex.prf:
QmlTest/QmlTest.pro:
QmlTest/qmltest.qrc:
/usr/lib/libQt5Quick.prl:
/usr/lib/libQt5Gui.prl:
/usr/lib/libQt5Bluetooth.prl:
/usr/lib/libQt5Qml.prl:
/usr/lib/libQt5Network.prl:
/usr/lib/libQt5Core.prl:
qmake: FORCE
	@$(QMAKE) 'QMAKE_CFLAGS_RELEASE=-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -Wformat -Wformat-security -fmessage-length=0 -march=armv7-a -mfloat-abi=hard -mfpu=neon -mthumb -Wno-psabi' 'QMAKE_CFLAGS_DEBUG=-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -Wformat -Wformat-security -fmessage-length=0 -march=armv7-a -mfloat-abi=hard -mfpu=neon -mthumb -Wno-psabi' 'QMAKE_CXXFLAGS_RELEASE=-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -Wformat -Wformat-security -fmessage-length=0 -march=armv7-a -mfloat-abi=hard -mfpu=neon -mthumb -Wno-psabi' 'QMAKE_CXXFLAGS_DEBUG=-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -Wformat -Wformat-security -fmessage-length=0 -march=armv7-a -mfloat-abi=hard -mfpu=neon -mthumb -Wno-psabi' QMAKE_STRIP=: PREFIX=/usr -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug -o Makefile QmlTest/QmlTest.pro

qmake_all: FORCE


all: Makefile $(TARGET)

dist: distdir FORCE
	(cd `dirname $(DISTDIR)` && $(TAR) $(DISTNAME).tar $(DISTNAME) && $(COMPRESS) $(DISTNAME).tar) && $(MOVE) `dirname $(DISTDIR)`/$(DISTNAME).tar.gz . && $(DEL_FILE) -r $(DISTDIR)

distdir: FORCE
	@test -d $(DISTDIR) || mkdir -p $(DISTDIR)
	$(COPY_FILE) --parents $(DIST) $(DISTDIR)/
	$(COPY_FILE) --parents QmlTest/qmltest.qrc $(DISTDIR)/
	$(COPY_FILE) --parents QmlTest/src/QmlTest.cpp $(DISTDIR)/
	$(COPY_FILE) --parents translations/QmlTest-de.ts $(DISTDIR)/


clean: compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


distclean: clean 
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


####### Sub-libraries

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

check: first

benchmark: first

compiler_rcc_make_all: qrc_qmltest.cpp
compiler_rcc_clean:
	-$(DEL_FILE) qrc_qmltest.cpp
qrc_qmltest.cpp: QmlTest/qmltest.qrc \
		QmlTest/translations/ShtrihFiscalPrinter_ru.qm \
		QmlTest/translations/QmlTest.qm \
		QmlTest/translations/QmlTest-de.qm
	/usr/lib/qt5/bin/rcc -name qmltest QmlTest/qmltest.qrc -o qrc_qmltest.cpp

compiler_moc_header_make_all:
compiler_moc_header_clean:
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_rcc_clean 

####### Compile

QmlTest.o: QmlTest/src/QmlTest.cpp libs/qsmfptr/src/printertest.h \
		libs/qsmfptr/src/shtrihfiscalprinter.h \
		libs/qsmfptr/src/tlvtag.h \
		libs/qsmfptr/src/printercommand.h \
		libs/qsmfptr/src/printertypes.h \
		libs/qsmfptr/src/fiscalprinter.h \
		libs/qsmfptr/src/serverconnection.h \
		libs/qsmfptr/src/logger.h \
		libs/qsmfptr/src/JournalPrinter.h \
		libs/qsmfptr/src/Logger.h \
		libs/qsmfptr/src/bluetoothport2.h \
		libs/qsmfptr/src/lib/bluetooth.h \
		libs/qsmfptr/src/printerprotocol1.h \
		libs/qsmfptr/src/printerprotocol2.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o QmlTest.o QmlTest/src/QmlTest.cpp

qrc_qmltest.o: qrc_qmltest.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qrc_qmltest.o qrc_qmltest.cpp

####### Install

install_qm: first FORCE
	@test -d $(INSTALL_ROOT)/usr/share/QmlTest/translations || mkdir -p $(INSTALL_ROOT)/usr/share/QmlTest/translations
	lupdate -noobsolete /home/src1/QmlTest/QmlTest/src /home/src1/QmlTest/QmlTest/qml -ts /home/src1/QmlTest/QmlTest/translations/QmlTest.ts /home/src1/QmlTest/QmlTest/translations/QmlTest-de.ts && mkdir -p translations && [ "/home/src1/QmlTest" != "/home/src1/QmlTest/QmlTest" -a 1 -eq 1 ] && cp -af /home/src1/QmlTest/QmlTest/translations/QmlTest-de.ts /home/src1/QmlTest/translations || : ; [ 1 -eq 1 ] && lrelease -nounfinished /home/src1/QmlTest/translations/QmlTest-de.ts || :
	-$(INSTALL_FILE) /home/src1/QmlTest/translations/QmlTest-de.qm $(INSTALL_ROOT)/usr/share/QmlTest/translations/

uninstall_qm: FORCE
	-$(DEL_FILE) -r $(INSTALL_ROOT)/usr/share/QmlTest/translations/QmlTest-de.qm
	-$(DEL_DIR) $(INSTALL_ROOT)/usr/share/QmlTest/translations/ 


install_qml: first FORCE
	@test -d $(INSTALL_ROOT)/usr/share/QmlTest || mkdir -p $(INSTALL_ROOT)/usr/share/QmlTest
	-$(INSTALL_DIR) /home/src1/QmlTest/QmlTest/qml $(INSTALL_ROOT)/usr/share/QmlTest/

uninstall_qml: FORCE
	-$(DEL_FILE) -r $(INSTALL_ROOT)/usr/share/QmlTest/qml
	-$(DEL_DIR) $(INSTALL_ROOT)/usr/share/QmlTest/ 


install_target: first FORCE
	@test -d $(INSTALL_ROOT)/usr/bin || mkdir -p $(INSTALL_ROOT)/usr/bin
	-$(INSTALL_PROGRAM) $(QMAKE_TARGET) $(INSTALL_ROOT)/usr/bin/$(QMAKE_TARGET)

uninstall_target: FORCE
	-$(DEL_FILE) $(INSTALL_ROOT)/usr/bin/$(QMAKE_TARGET)
	-$(DEL_DIR) $(INSTALL_ROOT)/usr/bin/ 


install_desktop: first FORCE
	@test -d $(INSTALL_ROOT)/usr/share/applications || mkdir -p $(INSTALL_ROOT)/usr/share/applications
	-$(INSTALL_PROGRAM) /home/src1/QmlTest/QmlTest/QmlTest.desktop $(INSTALL_ROOT)/usr/share/applications/

uninstall_desktop: FORCE
	-$(DEL_FILE) -r $(INSTALL_ROOT)/usr/share/applications/QmlTest.desktop
	-$(DEL_DIR) $(INSTALL_ROOT)/usr/share/applications/ 


install_icon86x86: first FORCE
	@test -d $(INSTALL_ROOT)/usr/share/icons/hicolor/86x86/apps || mkdir -p $(INSTALL_ROOT)/usr/share/icons/hicolor/86x86/apps
	-$(INSTALL_PROGRAM) /home/src1/QmlTest/QmlTest/icons/86x86/QmlTest.png $(INSTALL_ROOT)/usr/share/icons/hicolor/86x86/apps/

uninstall_icon86x86: FORCE
	-$(DEL_FILE) -r $(INSTALL_ROOT)/usr/share/icons/hicolor/86x86/apps/QmlTest.png
	-$(DEL_DIR) $(INSTALL_ROOT)/usr/share/icons/hicolor/86x86/apps/ 


install_icon108x108: first FORCE
	@test -d $(INSTALL_ROOT)/usr/share/icons/hicolor/108x108/apps || mkdir -p $(INSTALL_ROOT)/usr/share/icons/hicolor/108x108/apps
	-$(INSTALL_PROGRAM) /home/src1/QmlTest/QmlTest/icons/108x108/QmlTest.png $(INSTALL_ROOT)/usr/share/icons/hicolor/108x108/apps/

uninstall_icon108x108: FORCE
	-$(DEL_FILE) -r $(INSTALL_ROOT)/usr/share/icons/hicolor/108x108/apps/QmlTest.png
	-$(DEL_DIR) $(INSTALL_ROOT)/usr/share/icons/hicolor/108x108/apps/ 


install_icon128x128: first FORCE
	@test -d $(INSTALL_ROOT)/usr/share/icons/hicolor/128x128/apps || mkdir -p $(INSTALL_ROOT)/usr/share/icons/hicolor/128x128/apps
	-$(INSTALL_PROGRAM) /home/src1/QmlTest/QmlTest/icons/128x128/QmlTest.png $(INSTALL_ROOT)/usr/share/icons/hicolor/128x128/apps/

uninstall_icon128x128: FORCE
	-$(DEL_FILE) -r $(INSTALL_ROOT)/usr/share/icons/hicolor/128x128/apps/QmlTest.png
	-$(DEL_DIR) $(INSTALL_ROOT)/usr/share/icons/hicolor/128x128/apps/ 


install_icon256x256: first FORCE
	@test -d $(INSTALL_ROOT)/usr/share/icons/hicolor/256x256/apps || mkdir -p $(INSTALL_ROOT)/usr/share/icons/hicolor/256x256/apps
	-$(INSTALL_PROGRAM) /home/src1/QmlTest/QmlTest/icons/256x256/QmlTest.png $(INSTALL_ROOT)/usr/share/icons/hicolor/256x256/apps/

uninstall_icon256x256: FORCE
	-$(DEL_FILE) -r $(INSTALL_ROOT)/usr/share/icons/hicolor/256x256/apps/QmlTest.png
	-$(DEL_DIR) $(INSTALL_ROOT)/usr/share/icons/hicolor/256x256/apps/ 


install: install_qm install_qml install_target install_desktop install_icon86x86 install_icon108x108 install_icon128x128 install_icon256x256  FORCE

uninstall: uninstall_qm uninstall_qml uninstall_target uninstall_desktop uninstall_icon86x86 uninstall_icon108x108 uninstall_icon128x128 uninstall_icon256x256  FORCE

FORCE:

