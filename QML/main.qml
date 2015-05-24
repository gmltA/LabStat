import QtQuick 2.4
import QtQuick.Controls.Styles 1.3
import QtQuick.Layouts 1.1

import QtQuick.Controls 1.2
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
            title: "Change Text"
            text: "Add new subject to track stats for"

            z: 5

            onAccepted: {
                SubjectHandler.addSubject(value)
            }
        }
    }

    Component {
        id: dialogBuilder

        PopupDialog {
            title: "Add sync processor"
            text: "Data can be used by processors differently. For Google Drive it should include Folder / File name"
            z: 5
            ComboBox {
                id: processorSelector
                width: parent.width
                model: SubjectHandler.getAvailableProcessorTypes()
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
                SubjectHandler.attachProcessor(processorSelector.currentIndex, text.text)
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

        defaultTitle: "Application"

        icon.onMenuClicked: {
            drawer.show()
        }
    }

    NavigationDrawer {
        id: drawer

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
                        dialog.showing = true
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
                            dialog.showing = true
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

        onModelChanged: {
            var title = actionBar.defaultTitle
            if (stats.model)
                title = model.getGroupId().toString()

            actionBar.title = title
        }
    }
}
