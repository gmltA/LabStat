TEMPLATE = app

QT += qml quick concurrent xml

android{
    QT += androidextras
}

SOURCES += main.cpp \
    googleauthclient.cpp \
    GoogleDrive/apirequest.cpp \
    GoogleDrive/driveapi.cpp \
    GoogleDrive/drivefile.cpp \
    synchandler.cpp \
    googledesktopauthclient.cpp \
    datasheet.cpp \
    drivesyncprocessor.cpp \
    GoogleDrive/sheetsapirequest.cpp \
    subjectdata.cpp \
    subjecthandler.cpp

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
    googleauthclient.h \
    GoogleDrive/apirequest.h \
    GoogleDrive/driveapi.h \
    interface.datastore.h \
    interface.dataitem.h \
    GoogleDrive/drivefile.h \
    synchandler.h \
    interface.authclient.h \
    googledesktopauthclient.h \
    datasheet.h \
    drivesyncprocessor.h \
    GoogleDrive/sheetsapirequest.h \
    subjectdata.h \
    subjecthandler.h
