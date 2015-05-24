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
                        text: "Test note" /* note */
                        font.family: "Roboto Regular"
                        font.pixelSize: 12 * dp
                        color: Theme.subTextColor
                        visible: false
                    }
                }

                CheckBox {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 10 * dp

                    z: 1

                    checked: attendence

                    onClicked: {
                        attendence = checked
                    }

                    style: MaterialCheckBox {
                        color: Theme.accentColor
                        uncheckedColor: Theme.iconColor
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onPressAndHold: {
                        if (personDelegateElement.state == "")
                            personDelegateElement.state = "expanded"
                    }

                    onClicked: {
                        personDelegateElement.state = personDelegateElement.state == "" ? "expanded" : ""
                        /*
                        if (personDelegateElement.state == "expanded")
                            personDelegateElement.state = ""*/
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

                Grid {
                    id: statControls
                    columns: 2
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.rightMargin: 16 * dp
                    anchors.leftMargin: 16 * dp

                    verticalItemAlignment : Grid.AlignVCenter
                    Text {
                        text: "Лабораторная #1"
                        font.family: "Roboto Regular"
                        font.pixelSize: 12 * dp
                    }

                    CheckBox {
                        style: MaterialCheckBox {
                            color: Theme.accentColor
                            uncheckedColor: Theme.iconColor
                        }
                    }

                    Text {
                        text: "Лабораторная #2"
                        font.family: "Roboto Regular"
                        font.pixelSize: 12 * dp
                    }

                    CheckBox {
                        style: MaterialCheckBox {
                            color: Theme.accentColor
                            uncheckedColor: Theme.iconColor
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

    Rectangle {
        id: tabContainer

        // BUG: should be parent.width, but this property assigns after element init
        //      and this causes buggy tab highlight behaviour
        width: mainWindow.width
        height: 48 * dp
        y: Math.min(0, Math.max(-height, content.currentItem ? content.currentItem.scrollDiff : 0))
        z: 1

        color: Theme.primaryColor

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

        onCurrentIndexChanged: {
            dateTabs.currentIndex = currentIndex
        }
    }

    states: [
        State {
            name: "hidden"
            when: !root.model
            PropertyChanges {
                target: root
                visible: false
            }
            PropertyChanges {
                target: tabContainer
                y: -height
            }
        }
    ]
}
