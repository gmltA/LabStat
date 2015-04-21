import QtQuick 2.4
import QtQuick.Controls 1.2

import "../QML/QuickAndroid"

Rectangle {
    id: root

    property var tabsModel
    property var contentModel

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
                model: contentModel
                delegate: person
                displayMarginBeginning: 48 * dp
                onContentYChanged: {
                    scrollDiff = scrollStop - contentY
                }

                onMovementStarted: {
                    scrollStop += scrollDiff
                }

                onMovementEnded: {
                    scrollStop = contentY
                    scrollDiff = Math.min(0, Math.max(-tabContainer.height, scrollDiff))
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

            Text {
                id: title

                anchors.centerIn: parent

                text: itemName
                font.pixelSize: 10 * dp
                Behavior on font.pixelSize {
                    NumberAnimation {
                        duration: 100
                    }
                }
            }

            Rectangle {
                id: highlight
                anchors.bottom: parent.bottom
                width: parent.width
                color: "#00BCD4"

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
                        target: title
                        font.pixelSize: 14 * dp
                    }
                    PropertyChanges {
                        target: highlight
                        height: 4 * dp
                    }
                }
            ]
        }
    }

    Component {
        id: person
        Item {
            width: root.width
            height: 48 * dp
            Rectangle {
                anchors.bottom: parent.bottom

                width: parent.width
                height: 1 * dp
                color: Qt.rgba(0, 0, 0, 0.57)
            }
            Text {
                anchors.left: parent.left
                anchors.leftMargin: 10 * dp
                anchors.verticalCenter: parent.verticalCenter
                text: itemName
            }
            CheckBox {
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10 * dp
            }
        }
    }

    Rectangle {
        id: tabContainer

        // BUG: should be parent.width, but this property assigns after element init
        //      and this causes buggy tab highlight behaviour
        width: mainWindow.width
        height: 48 * dp
        y: Math.min(0, Math.max(-height, content.currentItem.scrollDiff))
        z: 1

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
            model: tabsModel
            delegate: tabDelegate
            preferredHighlightBegin: parent.width / 3
            preferredHighlightEnd: (parent.width / 3) * 2
            Rectangle {
                anchors.centerIn: parent
                width: tabContainer.height
                height: tabContainer.width
                rotation: 90
                gradient: Gradient {
                    GradientStop {
                        position: 0.0
                        color: "white"
                    }
                    GradientStop {
                        position: 0.5
                        color: "transparent"
                    }
                    GradientStop {
                        position: 1.0
                        color: "white"
                    }
                }
            }
        }
        MaterialShadow {
            anchors.fill: parent
            z: -1
            depth: 1
            asynchronous: true
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
        model: tabsModel

        delegate: pageDelegate

        onCurrentIndexChanged: {
            dateTabs.currentIndex = currentIndex
        }
    }

    states: [
        State {
            name: "hidden"
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
