#-------------------------------------------------
#
# Project created by ARASHz4
#
#-------------------------------------------------

QT += core gui network svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

!android{
    TARGET = 'Local Messenger'
}

android{
    TARGET = 'LocalMessenger'
}

TEMPLATE = app

CONFIG += c++11

win32{
    RC_FILE = Windows/WinRes.rc
    CONFIG -= embed_manifest_exe
}

mac{
    QMAKE_INFO_PLIST = Mac/Info.plist
    ICON = Mac/AppIcon.icns
}

android{
    QT += androidextras
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

TRANSLATIONS = Translate/English.ts \
               Translate/Persian.ts

SOURCES += main.cpp\
    slsettings.cpp \
    options.cpp \
    about.cpp \
    messengerwindow.cpp \
    connection.cpp \
    QSingleApp/qtlocalpeer.cpp \
    QSingleApp/qtlockedfile.cpp \
    QSingleApp/qtlockedfile_unix.cpp \
    QSingleApp/qtlockedfile_win.cpp \
    QSingleApp/qtsingleapplication.cpp \
    QSingleApp/qtsinglecoreapplication.cpp

HEADERS += \
    slsettings.h \
    options.h \
    about.h \
    messengerwindow.h \
    connection.h \
    QSingleApp/qtlocalpeer.h \
    QSingleApp/qtlockedfile.h \
    QSingleApp/qtsingleapplication.h \
    QSingleApp/qtsinglecoreapplication.h

FORMS   += \
    options.ui \
    about.ui \
    messengerwindow.ui \
    connection.ui

RESOURCES += \
    resource.qrc

DISTFILES += \
    Linux/LocalMessenger.desktop \
    Linux/icons/hicolor/128x128/apps/LocalMessenger.png \
    Linux/icons/hicolor/24x24/apps/LocalMessenger.png \
    Linux/icons/hicolor/256x256/apps/LocalMessenger.png \
    Linux/icons/hicolor/32x32/apps/LocalMessenger.png \
    Linux/icons/hicolor/48x48/apps/LocalMessenger.png \
    Mac/Info.plist \
    Mac/AppIcon.icns \
    Windows/WinRes.rc \
    Windows/WinDetails.h \
    Windows/manifest \
    Windows/AppIcon.ico \
    Translate/English.ts \
    Translate/Persian.ts \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/src/com/arashz4/localmessenger/NotificationClient.java
