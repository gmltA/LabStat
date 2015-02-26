TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp

RESOURCES += qml.qrc

macx {
    QMAKE_MAC_SDK = macosx10.9
} android {
    QT += androidextras
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    QML/NavigationDrawer/NavigationDrawer.qml \
    QML/NavigationDrawer/NavigationDrawerItem.qml \
    QML/NavigationDrawer/NavigationDrawerListItem.qml \
    QML/main.qml \
    QML/MenuBackIcon.qml \
    QML/SoftShadow.qml
