TEMPLATE = app

QT += qml quick androidextras

SOURCES += main.cpp \
    googleauthclient.cpp \
    GoogleDrive/apirequest.cpp \
    GoogleDrive/driveapi.cpp \
    GoogleDrive/drivefile.cpp

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
    QML/SoftShadow.qml \
    QML/NavigationDrawer/NavigationDrawerSubheader.qml \
    QML/NavigationDrawer/NavigationDrawerDivider.qml \
    QML/NavigationDrawer/NavigationDrawerHeader.qml

DISTFILES += \ android-sources/AndroidManifest.xml \
    android-sources/project.properties \
    android-sources/src/org/qtproject/labstat/GoogleAuthClient.java

HEADERS += \
    googleauthclient.h \
    GoogleDrive/apirequest.h \
    GoogleDrive/driveapi.h \
    interface.datastore.h \
    interface.dataitem.h \
    GoogleDrive/drivefile.h
