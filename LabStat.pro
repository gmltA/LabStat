TEMPLATE = app

QT += qml quick androidextras

SOURCES += main.cpp \
    googleauthclient.cpp

RESOURCES += qml.qrc

QML_IMPORT_PATH =

include(deployment.pri)

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

HEADERS += \
    googleauthclient.h
