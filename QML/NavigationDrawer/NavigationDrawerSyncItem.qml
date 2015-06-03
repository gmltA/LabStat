import QtQuick 2.0
import SubjectHandler 1.0
import "../."

Item {
    id: root

    property alias caption: item.caption
    property alias icon: item.icon
    property int processorId
    property bool online

    signal initCompleted(bool success)

    function processorAdded(processorData) {
        if (processorId === processorData['id']) {
            if (processorData['result'])
                state = ""

            initCompleted(processorData['result'])
        }
    }

    function syncStopped(stoppedProcessorId) {
        if (stoppedProcessorId === processorId)
            state = ""
    }

    height: 48 * dp
    anchors.left: parent ? parent.left : undefined
    anchors.right: parent ? parent.right : undefined

    state: "inactive"

    Connections {
        target: item
        onClicked: {
            SubjectHandler.sync(processorId)
            state = "syncing"
        }
    }

    Connections {
        target: SubjectHandler
        onSyncStopped: {
            syncStopped(processorId)
        }
        onProcessorAdded: {
            processorAdded(processorData)
        }
    }

    Rectangle {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: {
            if (flickable.visibleArea.xPosition > 0)
                return parent.left
            else
                return parent.right
        }
        width: (1 - item.opacity) * 2 * 40 * dp
        height: width
        radius: height / 2
        color: Theme.primaryDarkColor
        opacity: 0.3
    }
    Rectangle {
        id: deleteCircle
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: {
            if (flickable.visibleArea.xPosition > 0)
                return parent.left
            else
                return parent.right
        }
        width: ((1 - item.opacity) * 50 - 20) * dp
        height: width
        radius: height / 2
        color: Theme.primaryDarkColor
        Behavior on color {
            ColorAnimation {
                duration: 200
                easing.type: Easing.InOutQuad
            }
        }

        states: [
            State {
                name: "active"
                when: Math.abs(flickable.visibleArea.xPosition) > 0.3
                PropertyChanges {
                    target: deleteCircle
                    color: Theme.accentColor
                }
            }
        ]
    }

    Flickable {
        id: flickable
        anchors.fill: parent
        clip: true
        flickableDirection: Flickable.HorizontalFlick

        onDragEnded: {
            if (Math.abs(flickable.visibleArea.xPosition) > 0.3)
                SubjectHandler.deleteProcessor(processorId)
        }

        NavigationDrawerItem {
            id: item
            icon: online ? "" : ""
            rightIcon: ""
            rightIconItem.state: "hidden"
            caption: "Item"

            width: root.width - 1
            enabled: !flickable.flicking
            opacity: {
                if (flickable.dragging)
                    return Math.min(1, Math.max(0, 0.3 - Math.abs(flickable.visibleArea.xPosition)))
                else
                    return 1
            }

            Behavior on opacity {
                NumberAnimation {
                    duration: 70
                    easing.type: Easing.InOutQuad
                }
            }

            SequentialAnimation on rightIconItem.rotation {
                id: iconAnim

                running: false
                alwaysRunToEnd: true
                loops: Animation.Infinite

                NumberAnimation {
                    from: 0
                    to: 360
                    duration: 500
                    easing.type: Easing.InOutQuad
                }
                PauseAnimation {
                    duration: 250
                }
            }

            rightIconItem.states: [
                State {
                    name: "hidden"
                    PropertyChanges {
                        target: item.rightIconItem
                        visible: false
                        font.pointSize: 1
                    }
                },
                State {
                    name: "visible"
                    PropertyChanges {
                        target: item.rightIconItem
                        visible: true
                    }
                }
            ]
            rightIconItem.transitions: [
                Transition {
                    from: "hidden"
                    to: "visible"
                    NumberAnimation {
                        properties: "font.pointSize"
                        easing.type: Easing.InOutQuad
                    }
                },
                Transition {
                    from: "visible"
                    to: "hidden"
                    PropertyAnimation {
                        properties: "visible"
                        easing.type: Easing.InOutQuad
                    }
                    NumberAnimation {
                        properties: "font.pointSize"
                        easing.type: Easing.InOutQuad
                    }
                }
            ]
        }
    }

    states: [
        State {
            name: "inactive"

            PropertyChanges {
                target: item
                enabled: false
                rightIcon: ""
            }

            PropertyChanges {
                target: item.rightIconItem
                state: "visible"
            }
        },
        State {
            name: "syncing"

            PropertyChanges {
                target: item
                enabled: false
                color: Theme.primaryColor
                iconColor: Theme.primaryColor
            }

            PropertyChanges {
                target: iconAnim
                running: true
            }

            PropertyChanges {
                target: item.rightIconItem
                state: "visible"
            }
        }
    ]
}
