import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.2

import QtQuick.Controls 1.2
//import QtWebKit 3.0
//import "NavigationDrawer.qml" as MyModule

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
                contentHeight: mennu.height
                contentWidth: parent.width

                Column {
                    id: mennu
                    anchors.left: parent.left
                    anchors.right: parent.right

                    Rectangle {
                        height: 56 * dp
                        anchors.left: parent.left
                        anchors.right: parent.right
                    }

                    Button {
                        id: button1
                        opacity: parent.opacity
                        height: 48 * dp
                        anchors.left: parent.left
                        anchors.right: parent.right

                        Text {
                            id: caption
                            anchors.left: parent.left
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.leftMargin: 72 * dp

                            font.pointSize: 14
                            font.family: "Roboto"
                            text: "Groups"
                        }

                        Text {
                            id: icon
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.rightMargin: 16 * dp

                            font.family: materialIcons.name
                            font.pointSize: 28
                            text: ""
                        }

                        style: ButtonStyle {
                            background: Rectangle {
                                implicitWidth: 100
                                implicitHeight: 25
                                color: control.pressed ? "#E6E6E6" : "white"
                            }
                        }

                        onClicked: {
                            if (b1_subMenu.height > 0)
                            {
                                b1_subMenu.height = 0;
                                b1_subMenu.opacity = 0;
                                icon.text = "";
                                caption.font.bold = false;

                            }
                            else
                            {
                                b1_subMenu.height = 100;
                                b1_subMenu.opacity = 1;
                                icon.text = "";
                                caption.font.bold = true;
                            }
                        }
                    }

                    Rectangle {
                        id: b1_subMenu
                        width: parent.width
                        height: 100
                        color: "#D9D9D9"

                        Rectangle {
                            height: 4 * dp
                            width: parent.width
                            transformOrigin: Item.TopLeft
                            gradient: Gradient{
                                GradientStop { position: 1; color: "#00000000"}
                                GradientStop { position: 0; color: "#2c000000"}
                            }
                        }

                        Rectangle {
                            height: 4 * dp
                            width: parent.width
                            anchors.bottom: parent.bottom
                            gradient: Gradient {
                                GradientStop { position: 0; color: "#00000000"}
                                GradientStop { position: 1; color: "#2c000000"}
                            }
                        }

                        Behavior on height {
                            NumberAnimation {
                                duration: 300
                                easing.type: Easing.OutCubic
                            }
                        }

                        Behavior on opacity {
                            NumberAnimation {
                                duration: 300
                                easing.type: Easing.OutCubic
                            }
                        }
                    }
                }
            }



          //  YourContentItem {
            //    ....
            //}
        }
    //}
}
