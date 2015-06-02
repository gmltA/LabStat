import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

import "."

Rectangle {
    id: root

    property var model

    Component {
        id: pageDelegate
        Item {
            property alias scrollStop: pupils.scrollStop
            property alias scrollDiff: pupils.scrollDiff
            width: root.width
            height: content.height

            ListView {
                id: pupils
                property int scrollStop: 0
                property int scrollDiff: 0

                anchors.fill: parent
                model: students
                delegate: personDelegate
                displayMarginBeginning: 48 * dp
                onContentYChanged: {
                    scrollDiff = scrollStop - contentY
                }

                onMovementStarted: {
                    scrollStop += scrollDiff
                }

                onMovementEnded: {
                    scrollStop = contentY
                    scrollDiff = Math.min(0, Math.max(-tabContainer.height,
                                                      scrollDiff))
                }
            }
        }
    }

    Component {
        id: tabDelegate
        Item {
            id: wrapper
            width: dateTabs.width / 3
            height: dateTabs.height
            state: wrapper.ListView.isCurrentItem ? "current" : ""

            property int subgroup: group

            Row {
                id: dateTime
                anchors.centerIn: parent
                height: parent.height
                spacing: 5 * dp

                Behavior on anchors.verticalCenterOffset {
                    NumberAnimation {
                        duration: 100
                        easing.type: Easing.InOutQuad
                    }
                }

                Text {
                    id: dateText
                    anchors.verticalCenter: parent.verticalCenter

                    text: Qt.formatDateTime(date, "dd.MM")
                    font.family: "Roboto Regular"
                    font.pixelSize: 15 * dp
                    color: "white"
                }
                Text {
                    id: timeText
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: 2 * dp

                    text: Qt.formatDateTime(time, "hh:mm")
                    font.family: "Roboto Condensed Light"
                    font.pixelSize: 12 * dp
                    color: "white"
                }

                transform: Scale {
                    id: titleScale
                    origin.x: width / 2
                    origin.y: height / 2
                    xScale: 0.6
                    yScale: 0.6
                    Behavior on xScale {
                        NumberAnimation {
                            duration: 100
                        }
                    }
                    Behavior on yScale {
                        NumberAnimation {
                            duration: 100
                        }
                    }
                }
            }

            Text {
                id: groupText
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: highlight.top

                text: group == 1 ? "1 подгруппа" : group == 2 ? "2 подгруппа" : "Группа целиком"
                color: "white"
                font.pixelSize: 12 * dp
                font.family: "Roboto Condensed"

                transform: Scale {
                    id: groupScale
                    origin.x: width / 2
                    origin.y: 0
                    xScale: 0
                    yScale: 0
                    Behavior on xScale {
                        NumberAnimation {
                            duration: 100
                        }
                    }
                    Behavior on yScale {
                        NumberAnimation {
                            duration: 100
                        }
                    }
                }
            }

            Rectangle {
                id: highlight
                anchors.bottom: parent.bottom
                width: parent.width
                color: "white"

                Behavior on height {
                    NumberAnimation {
                        duration: 100
                    }
                }
            }

            states: [
                State {
                    name: "current"
                    PropertyChanges {
                        target: dateTime
                        anchors.verticalCenterOffset: -5 * dp
                    }
                    PropertyChanges {
                        target: titleScale
                        xScale: 1
                        yScale: 1
                    }
                    PropertyChanges {
                        target: highlight
                        height: 2 * dp
                    }
                    PropertyChanges {
                        target: groupScale
                        xScale: 1
                        yScale: 1
                    }
                }
            ]
        }
    }

    Component {
        id: personDelegate
        Item {
            id: personDelegateElement

            property var dialog: undefined

            function updateNote() {
                note = editNoteDialog.value
            }

            function disconnectUpdate() {
                editNoteDialog.accepted.disconnect(updateNote)
            }

            height: personHeader.height + personStats.height
            width: root.width

            Item {
                id: personHeader
                anchors.top: parent.top

                height: 72 * dp
                width: parent.width

                Column {
                    anchors.left: parent.left
                    anchors.leftMargin: 16 * dp
                    anchors.verticalCenter: parent.verticalCenter

                    spacing: 8 * dp
                    Text {
                        text: name + " " + surname
                        font.family: "Roboto Medium"
                        font.pixelSize: 14 * dp
                        color: Theme.textColor
                    }
                    Text {
                        id: noteField
                        text: note
                        font.family: "Roboto Regular"
                        font.pixelSize: 12 * dp
                        color: Theme.subTextColor
                        visible: false
                    }
                }

                MouseArea {
                    anchors.fill: parent

                    onPressAndHold: {
                        personDelegateElement.state = "expanded"

                        editNoteDialog.open()
                        editNoteDialog.accepted.connect(updateNote)
                        editNoteDialog.closed.connect(disconnectUpdate)
                    }

                    onClicked: {
                        personDelegateElement.state = personDelegateElement.state == "" ? "expanded" : ""
                    }
                }

                CheckBox {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 10 * dp

                    checked: attendence

                    onClicked: {
                        attendence = checked
                    }

                    style: MaterialCheckBox {
                        color: Theme.accentColor
                        uncheckedColor: Theme.iconColor
                    }
                }
            }
            Item {
                id: personStats
                anchors.top: personHeader.bottom
                width: parent.width
                height: 0
                clip: true

                Behavior on height {
                    NumberAnimation {
                        easing.type: "InOutCubic"
                        duration: 200
                    }
                }


                Column {
                    id: statControls

                    anchors.left: parent.left
                    anchors.right: parent.right
                    Repeater {
                        id: statRepeater
                        model: labWorks

                        delegate: Item {
                            width: parent ? parent.width : 0
                            height: 48 * dp
                            anchors.rightMargin: 16 * dp
                            anchors.leftMargin: 16 * dp
                            anchors.left: parent ? parent.left : undefined
                            anchors.right: parent ? parent.right : undefined

                            Label {
                                anchors.verticalCenter: parent.verticalCenter
                                text: "Лабораторная работа #" + (index + 1)
                                style: "body1"
                            }

                            CheckBox {
                                anchors.right: parent.right
                                checked: modelData
                                style: MaterialCheckBox {
                                    color: Theme.accentColor
                                    uncheckedColor: Theme.iconColor
                                }
                                onClicked: {
                                    var temp = labWorks
                                    temp[index] = checked
                                    labWorks = temp
                                }
                            }
                        }
                    }
                }
            }

            Rectangle {
                anchors.bottom: parent.bottom

                width: parent.width
                height: 1 * dp
                color: Theme.dividerColor
            }

            states: [
                State {
                    name: "expanded"
                    PropertyChanges {
                        target: personStats
                        height: statControls.height
                    }
                    PropertyChanges {
                        target: noteField
                        visible: noteField.text != "" ? true : false
                    }
                }
            ]
        }
    }

    ListView {
        id: content

        anchors.bottom: parent.bottom

        width: parent.width
        height: parent.height - dateTabs.height

        orientation: ListView.Horizontal

        highlightRangeMode: ItemView.StrictlyEnforceRange
        boundsBehavior: Flickable.StopAtBounds
        snapMode: ListView.SnapOneItem
        model: root.model

        delegate: pageDelegate

        // HACK! positionViewAtIndex is broken during onModelChanged function
        Timer {
            id: timer
            interval: 100
            repeat: false
            onTriggered: {
                content.positionViewAtIndex(model.getClosestEntryIndex(), ListView.SnapPosition)
            }
        }

        onModelChanged: {
            if (model)
                timer.start()
        }

        onCurrentIndexChanged: {
            dateTabs.currentIndex = currentIndex
        }
    }

    View {
        id: tabContainer

        // BUG: should be parent.width, but this property assigns after element init
        //      and this causes buggy tab highlight behaviour
        width: mainWindow.width
        height: 48 * dp
        y: Math.min(0, Math.max(-height, content.currentItem ? content.currentItem.scrollDiff : 0))
        elevation: 1

        tintColor: Theme.primaryColor

        Behavior on y {
            NumberAnimation {
                duration: 100
                easing.type: Easing.OutCubic
            }
        }

        ListView {
            id: dateTabs
            interactive: false

            width: parent.width
            height: parent.height

            orientation: ListView.Horizontal
            highlightRangeMode: ItemView.StrictlyEnforceRange
            boundsBehavior: Flickable.StopAtBounds
            snapMode: ListView.SnapOneItem
            model: root.model
            delegate: tabDelegate
            preferredHighlightBegin: parent.width / 3
            preferredHighlightEnd: (parent.width / 3) * 2
        }
    }

    InputPopupDialog {
        id: editNoteDialog
        text: "Modify sutdent's note"
    }

    states: [
        State {
            name: "hidden"
            when: !root.model
            PropertyChanges {
                target: root
                opacity: 0
            }
            PropertyChanges {
                target: tabContainer
                y: -height
            }
        }
    ]
}
