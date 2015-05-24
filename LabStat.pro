TEMPLATE = app

QT += qml sql quick concurrent xml

android{
    QT += androidextras
}

SOURCES += main.cpp \
    synchandler.cpp \
    datasheet.cpp \
    subjectdata.cpp \
    subjecthandler.cpp \
    student.cpp \
    GoogleDrive/drivesyncprocessor.cpp \
    GoogleDrive/googleauthclient.cpp \
    GoogleDrive/googledesktopauthclient.cpp \
    GoogleDrive/API/apirequest.cpp \
    GoogleDrive/API/driveapi.cpp \
    GoogleDrive/API/drivefile.cpp \
    GoogleDrive/API/sheetsapirequest.cpp \
    SQLite/sqlitesyncprocessor.cpp \
    studentlistmodel.cpp \
    timetablemodel.cpp \
    stattableentry.cpp \
    timetableentry.cpp \
    appdatastorage.cpp

RESOURCES += qml.qrc

QML_IMPORT_PATH =

include(deployment.pri)

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sources

DISTFILES += \
    QML/NavigationDrawer/NavigationDrawer.qml \
    QML/NavigationDrawer/NavigationDrawerItem.qml \
    QML/NavigationDrawer/NavigationDrawerListItem.qml \
    QML/main.qml \
    QML/MenuBackIcon.qml \
    QML/NavigationDrawer/NavigationDrawerSubheader.qml \
    QML/NavigationDrawer/NavigationDrawerDivider.qml \
    QML/NavigationDrawer/NavigationDrawerHeader.qml \
    QML/NavigationDrawer/NavigationDrawerSyncItem.qml \
    QML/PopupDialog.qml \
    QML/PopupDialogButton.qml \
    QML/ActionBar.qml \
    QML/TabbedListView.qml \
    QML/MaterialCheckBox.qml \
    QML/Palette.qml \
    QML/qmldir \
    QML/Theme.qml \
    QML/InputPopupDialog.qml \
    QML/View.qml \
    QML/Label.qml \
    QML/PopupBase.qml

DISTFILES += \ android-sources/AndroidManifest.xml \
    android-sources/project.properties \
    android-sources/src/org/qtproject/labstat/GoogleAuthClient.java

HEADERS += \
    interface.dataitem.h \
    synchandler.h \
    interface.authclient.h \
    datasheet.h \
    subjectdata.h \
    subjecthandler.h \
    student.h \
    GoogleDrive/drivesyncprocessor.h \
    GoogleDrive/googleauthclient.h \
    GoogleDrive/googledesktopauthclient.h \
    GoogleDrive/interface.authclient.h \
    GoogleDrive/API/apirequest.h \
    GoogleDrive/API/driveapi.h \
    GoogleDrive/API/drivefile.h \
    GoogleDrive/API/sheetsapirequest.h \
    SQLite/sqlitesyncprocessor.h \
    studentlistmodel.h \
    timetablemodel.h \
    stattableentry.h \
    timetableentry.h \
    appdatastorage.h \
    interface.syncprocessor.h
