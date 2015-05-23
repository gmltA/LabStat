import QtQuick 2.4
import QtQuick.Controls.Styles 1.3
import QtQuick.Layouts 1.1

import QtQuick.Controls 1.2
import "../QML/QuickAndroid"
import "../QML/NavigationDrawer"
import "."
import SubjectHandler 1.0

ApplicationWindow {
    title: "LabStat"
    id: mainWindow
    width: 360
    height: 640
    visible: true

    // for QMLScene debug only!
    property real dp: mainWindow.width / 320

    function groupItemClicked(groupId) {
        SubjectHandler.loadGroupData(groupId)
    }

    FontLoader {
        id: materialIcons
        source: "qrc:/fonts/Material-Design-Icons.ttf"
    }

    Component {
        id: addSubjectDialogBuilder

        InputPopupDialog {
            body: "Add new subject to track stats for"

            onAccepted: {
                SubjectHandler.addSubject(input.text)
            }
        }
    }

    Component {
        id: dialogBuilder

        PopupDialog {
            title: "Add sync processor"
            active: true
            z: 6
            ComboBox {
                id: processorSelector
                width: parent.width
                model: ["Google Drive", "SQLite storage"]
                currentIndex: 1
                onCurrentIndexChanged: {
                    if (currentIndex == 0)
                        text.forceActiveFocus()
                    else if (parent)
                        parent.forceActiveFocus()
                }
            }

            TextInput {
                id: text
                visible: processorSelector.currentIndex == 0
                width: parent.width
                height: 24 * dp
                font.pixelSize: 14 * dp
                maximumLength: 20
                Rectangle {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 1 * dp
                    color: Qt.rgba(0, 0, 0, 0.57)
                }
            }
            onAccepted: {
                if (processorSelector.currentIndex == 0)
                    SubjectHandler.attachDrive(text.text)
                else if (processorSelector.currentIndex == 1)
                    SubjectHandler.attachSQLite(text.text)
            }
        }
    }

    Connections {
        target: SubjectHandler
        onSubjectListChanged: {
            header.model = subjectListModel
        }
        onProcessorListChanged: {
            var model = []
            processors.forEach(function(processor) {
                model.push({"title": processor['title'], "id": processor['id'], "state": ""})
            })
            syncProcessors.processorsModel = model
        }
        onProcessorAddCalled: {
            var model = syncProcessors.processorsModel
            model.push({"title": processorData['title'], "id": processorData['id'], "state": "inactive"})
            syncProcessors.processorsModel =  model
        }
        onGroupListChanged: {
            groupList.listModel = []
            groups.forEach(function(group) {
                groupList.addItem("", group, group, groupItemClicked)
            })
        }
        onGroupDataLoaded: {
            stats.model = timeTable
        }
    }

    Connections {
        target: header
        onHeaderContentScrolled: {
            SubjectHandler.setCurrentSubject(index)
        }
    }

    ActionBar {
        id: actionBar
        elevation: stats.state === "" ? 0 : 1

        z: 1
    }

    NavigationDrawer {
        id: drawer

        color: "white"
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        position: Qt.LeftEdge

        function togglePage() {
            menuPage.visible = !menuPage.visible
            syncPage.visible = !syncPage.visible
            if (syncPage.visible)
                header.buttonIcon = ""
            else
                header.buttonIcon = ""
        }

        Flickable {
            anchors.fill: parent
            contentHeight: drawerMenu.height
            contentWidth: parent.width

            boundsBehavior: Flickable.StopAtBounds

            Column {
                id: drawerMenu
                anchors.left: parent.left
                anchors.right: parent.right

                NavigationDrawerHeader {
                    id: header
                    mainText: "Alex gmlt.A"

                    buttonArea.onClicked: {
                        drawer.togglePage()
                    }

                    actionButton.onClicked: {
                        var p = header.parent
                        while (p.parent)
                            p = p.parent

                        var dialog = addSubjectDialogBuilder.createObject(p)
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

                        property var processorsModel: []

                        anchors.left: parent.left
                        anchors.right: parent.right
                        Repeater {
                            model: syncProcessors.processorsModel
                            NavigationDrawerSyncItem {
                                processorId: syncProcessors.processorsModel[index].id
                                caption: syncProcessors.processorsModel[index].title
                                state: syncProcessors.processorsModel[index].state

                                onInitCompleted: {
                                    if (success)
                                        syncProcessors.processorsModel[index].state = ""
                                    else
                                        syncProcessors.processorsModel.splice(index, 1);

                                }
                            }
                        }
                    }

                    NavigationDrawerDivider {
                    }

                    NavigationDrawerItem {
                        id: addProcessorItem
                        icon: ""
                        caption: "Add sync processor"
                        onClicked: {
                            var p = addProcessorItem.parent
                            while (p.parent)
                                p = p.parent

                            var dialog = dialogBuilder.createObject(p)
                            //dialog.active = true
                        }
                    }
                }
            }
        }
    }

    Text {
        anchors.centerIn: parent
        font.pointSize: 24
        color: Qt.rgba(0, 0, 0, 0.1)
        text: "Select group\nto display stats"
        horizontalAlignment: Text.AlignHCenter
        style: Text.Sunken
        styleColor: "#AAAAAA"
    }

    TabbedListView {
        id: stats
        height: parent.height - actionBar.height
        width: parent.width
        anchors.top: actionBar.bottom
    }
}
