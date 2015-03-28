import QtQuick 2.1
import QtQuick.Controls.Styles 1.2

import QtQuick.Controls 1.2
import "../QML/NavigationDrawer"

ApplicationWindow {
    title: "Navigation Drawer"
    id: mainWindow
    width: 320
    height: 480
    visible: true

    // for QMLScene debug only!
    readonly property real dp: mainWindow.width / 320

    FontLoader { id: materialIcons; source: "qrc:/fonts/Material-Design-Icons.ttf" }

    Rectangle {
        anchors.top: parent.top
        width: parent.width
        height: 48 * dp
        color: "blue"

        MenuBackIcon {
            anchors.left: parent.left
            anchors.leftMargin: 16 * dp
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            anchors.left: parent.left
            anchors.leftMargin: 72 * dp
            anchors.verticalCenter: parent.verticalCenter

            text: "Application"
            font.pointSize: 20
            color: "white"
        }

        Rectangle {
            height: 4 * dp
            width: parent.width
            z: 1
            anchors.bottomMargin: -4*dp
            anchors.bottom: parent.bottom
            gradient: Gradient{
                GradientStop { position: 1; color: "#00000000"}
                GradientStop { position: 0; color: "#2c000000"}
            }
        }

    }

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

                NavigationDrawerHeader {
                    mainText: "Alex gmlt.A"
                    secondaryText: "Just a text"

                    buttonArea.onClicked: {
                        driveSync.test()
                    }
                }

                NavigationDrawerDivider {
                }

                NavigationDrawerListItem {
                    icon: ""
                    caption: "Groups"
                    NavigationDrawerItem {
                        icon: ""
                        caption: "250501"
                    }
                }
                NavigationDrawerItem {
                    icon: ""
                    caption: "Test"
                }

                NavigationDrawerDivider {
                }

                NavigationDrawerItem {
                    icon: ""
                    caption: "Settings"
                }
                NavigationDrawerItem {
                    icon: ""
                    caption: "Help"
                }
            }
        }
    }
}
