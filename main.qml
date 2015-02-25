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

                    NavigationDrawerItem
                    {
                        caption: "Item"
                        icon: ""
                    }
                }
            }



          //  YourContentItem {
            //    ....
            //}
        }
    //}
}
