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
    SQLite/sqlitesyncprocessor.cpp

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
    QML/QuickAndroid/InverseMouseArea.qml \
    QML/QuickAndroid/MaterialShadow.qml \
    QML/QuickAndroid/PopupArea.qml \
    QML/QuickAndroid/AnimationLoader.qml \
    QML/QuickAndroid/anim/ActivityEnter.qml \
    QML/QuickAndroid/anim/ActivityExit.qml \
    QML/QuickAndroid/anim/FastFadeIn.qml \
    QML/QuickAndroid/anim/FastFadeOut.qml \
    QML/QuickAndroid/anim/GrowFadeIn.qml \
    QML/QuickAndroid/anim/ShrinkFadeOut.qml \
    QML/ActionBar.qml \
    QML/TabbedListView.qml \
    QML/MaterialCheckBox.qml \
    QML/Palette.qml \
    QML/qmldir \
    QML/Theme.qml

DISTFILES += \ android-sources/AndroidManifest.xml \
    android-sources/project.properties \
    android-sources/src/org/qtproject/labstat/GoogleAuthClient.java

HEADERS += \
    interface.datastore.h \
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
    SQLite/sqlitesyncprocessor.h
