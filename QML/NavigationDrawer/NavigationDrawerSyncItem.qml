import QtQuick 2.0
import SyncHandler 1.0

Item {
    id: root

    property alias caption: item.caption
    property int processorId

    function syncStopped(stoppedProcessorId)
    {
        // bug: double-call from one processor
        if (stoppedProcessorId === processorId)
            state = "";
    }

    height: 48 * dp
    anchors.left: parent.left
    anchors.right: parent.right

    Connections {
        target: item
        onClicked: {
            SyncHandler.sync(processorId);
            state = "syncing";
        }
    }

    Connections {
        target: SyncHandler
        onSyncStopped: {
            syncStopped(processorId);
        }
    }

    NavigationDrawerItem {
        id: item
        icon: ""
        rightIcon: ""
        rightIconItem.state: "hidden"
        caption: "Item"

        SequentialAnimation on rightIconItem.rotation {
            id: iconAnim

            running: false
            alwaysRunToEnd: true
            loops: Animation.Infinite

            NumberAnimation { from: 0; to: 360; duration: 500; easing.type: Easing.InOutQuad }
            PauseAnimation { duration: 250 }
        }

        rightIconItem.states: [
            State {
                name: "hidden"
                PropertyChanges {
                    target: item.rightIconItem;
                    visible: false
                    font.pointSize: 1
                }
            },
            State {
                name: "visible"
                PropertyChanges {
                    target: item.rightIconItem;
                    visible: true
                }
            }
        ]
        rightIconItem.transitions: [
            Transition {
                from: "hidden"; to: "visible";
                NumberAnimation {
                    properties: "font.pointSize"
                    easing.type: Easing.InOutQuad
                }
            },
            Transition {
                from: "visible"; to: "hidden";
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

    states: [
        State {
            name: "syncing"

            PropertyChanges {
                target: item;
                enabled: false
            }

            PropertyChanges {
                target: iconAnim;
                running: true
            }

            PropertyChanges {
                target: item.rightIconItem
                state: "visible"
            }
        }
    ]
}

