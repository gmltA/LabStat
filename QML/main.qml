import QtQuick 2.1
import QtQuick.Controls.Styles 1.2

import QtQuick.Controls 1.2
import "../QML/QuickAndroid"
import "../QML/NavigationDrawer"
import SyncHandler 1.0

ApplicationWindow {
    title: "LabStat"
    id: mainWindow
    width: 360
    height: 640
    visible: true

    // for QMLScene debug only!
    readonly property real dp: mainWindow.width / 320

    FontLoader {
        id: materialIcons
        source: "qrc:/fonts/Material-Design-Icons.ttf"
    }

    ActionBar {
        id: actionBar
        z: 1
    }

    NavigationDrawer {
        id: drawer

        color: "white"
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        position: Qt.LeftEdge

        function togglePage()
        {
            menuPage.visible = !menuPage.visible;
            syncPage.visible = !syncPage.visible;
            if (syncPage.visible)
                header.buttonIcon = "";
            else
                header.buttonIcon = "";
        }

        Flickable {
            anchors.fill: parent
            contentHeight: drawerMenu.height
            contentWidth: parent.width

            Connections {
                target: SyncHandler
                onProcessorAddCalled: {
                    var component = Qt.createComponent("NavigationDrawer/NavigationDrawerSyncItem.qml");
                    var listItem = component.createObject(syncProcessors);

                    //listItem.icon = processorData['online'] === 1 ? "" : "";
                    listItem.caption = processorData['title'];
                    listItem.processorId = processorData['id'];
                }
            }

            Column {
                id: drawerMenu
                anchors.left: parent.left
                anchors.right: parent.right

                NavigationDrawerHeader {
                    id: header
                    mainText: "Alex gmlt.A"
                    secondaryText: "Just a text"

                    buttonArea.onClicked: {
                        drawer.togglePage();
                    }
                }

                NavigationDrawerDivider {
                }

                Column {
                    id: menuPage
                    anchors.left: parent.left
                    anchors.right: parent.right

                    NavigationDrawerListItem {
                        id: groupList
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
                Column {
                    id: syncPage
                    visible: false
                    anchors.left: parent.left
                    anchors.right: parent.right

                    Column {
                        id: syncProcessors
                        anchors.left: parent.left
                        anchors.right: parent.right
                    }

                    NavigationDrawerDivider {
                    }

                    NavigationDrawerItem {
                        id: addProcessorItem
                        icon: ""
                        caption: "Add sync processor"
                    }
                }
            }
        }
    }
}
