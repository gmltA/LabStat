import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.2

import QtQuick.Controls 1.2
import "../QML/NavigationDrawer"

ApplicationWindow {
    title: "Navigation Drawer"
    id: mainWindow
    width: 320
    height: 480
    visible: true

    readonly property real dp: mainWindow.width / 320

    FontLoader { id: materialIcons; source: "qrc:/fonts/Material-Design-Icons.ttf" }

    NavigationDrawer {
        id: drawer

        color: "white"
        //anchors.top: navigationBar.bottom
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        position: Qt.LeftEdge

        Flickable {
            anchors.fill: parent
            contentHeight: drawerMenu.height
            contentWidth: parent.width

            Column {
                id: drawerMenu
                anchors.left: parent.left
                anchors.right: parent.right

                Rectangle {
                    height: 56 * dp
                    anchors.left: parent.left
                    anchors.right: parent.right
                    Column {
                        anchors.left: parent.left
                        anchors.leftMargin: 16 * dp
                        anchors.verticalCenter: parent.verticalCenter
                        Text {
                            text: "Alex gmlt.A"

                            font.pointSize: 14
                            font.weight: Font.Black
                            color: Qt.rgba(0,0,0,0.54)
                        }
                        Text {
                            text: "Text"

                            font.pointSize: 14
                            color: Qt.rgba(0,0,0,0.54)
                        }
                    }
                }

                NavigationDrawerListItem {
                    icon: ""
                    caption: "Item"
                    NavigationDrawerItem {
                        caption: "Test"
                    }
                }
                NavigationDrawerItem {
                    caption: "Test"
                }
            }
        }
    }
}
