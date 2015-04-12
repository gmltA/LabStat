import QtQuick 2.0
import QtQuick.Controls 1.0
import "../QML/QuickAndroid"

PopupArea {
    id: popupDialog
    anchors.centerIn: parent
    width: parent.width - 96 * dp
    height: content.height + buttons.height + (8 + 16 + 24) * dp

    enabled: false

    Rectangle {
        id: dialog
        anchors.fill: parent
        radius: 2
        opacity: 0

        color: "white"

        Rectangle {
            anchors {
                top: parent.top
                margins: 24 * dp
                bottomMargin: 16 * dp
                horizontalCenter: parent.horizontalCenter
            }

            height: parent.height - (buttons.height + buttons.anchors.bottomMargin
                                     + anchors.topMargin + anchors.bottomMargin)
            width: parent.width - anchors.leftMargin * 2

            Column {
                id: content
                width: parent.width
                height: title.implicitHeight + body.implicitHeight + 8 * dp
                spacing: 8 * dp

                Text {
                    id: title
                    width: parent.width

                    font.family: "Roboto Medium"
                    font.pixelSize: 20
                    color: "#212121"
                    text: "Use Google's location service?"
                    wrapMode: Text.WordWrap
                }

                Text {
                    id: body
                    width: parent.width

                    font.family: "Roboto"
                    font.pixelSize: 14
                    lineHeight: 13 * dp
                    lineHeightMode: Text.FixedHeight
                    color: "#757575"
                    text: "Let Google help apps determine location. This means sending anonymous location data to Google, even when no apps are running."
                    wrapMode: Text.Wrap
                }
            }
        }

        Rectangle {
            id: buttons

            anchors.bottom: parent.bottom
            anchors.bottomMargin: 8 * dp

            width: parent.width
            height: 48 * dp

            Row {
                anchors {
                    rightMargin: 16 * dp
                    leftMargin: 16 * dp
                    horizontalCenter: parent.horizontalCenter
                }

                width: parent.width - anchors.rightMargin - anchors.leftMargin
                height: parent.height

                layoutDirection: Qt.RightToLeft
                spacing: 8 * dp

                PopupDialogButton {
                    caption: "AGREE"
                }

                PopupDialogButton {
                    caption: "DISAGREE"
                }
            }
        }

        MaterialShadow {
            anchors.fill: parent
            z: -10
            depth: 3
            asynchronous: true
            visible: dialog.opacity === 1
        }
    }

    AnimationLoader {
        id: enterAnimation
        transition: fromNulltoActive
        source: Qt.resolvedUrl("QuickAndroid/anim/GrowFadeIn.qml")
        target: dialog
    }

    AnimationLoader {
        id: exitAnimation
        transition: fromActiveToNull
        source: Qt.resolvedUrl("QuickAndroid/anim/ShrinkFadeOut.qml")
        target: dialog
    }

    states: [
        State {
            name: "Active"
            when: popupDialog.active

            PropertyChanges {
                target: dialog
                opacity: 1.0
            }

            PropertyChanges {
                target: popupDialog
                enabled: true
            }
        }
    ]

    transitions: [
        Transition {
            id: fromNulltoActive
            from: ""
            to: "Active"
        },
        Transition {
            id: fromActiveToNull
            from: "Active"
            to: ""
        }
    ]
}
