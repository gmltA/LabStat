import QtQuick 2.0
import QtQuick.Controls 1.0
import "../QML/QuickAndroid"
import "."
//import Material 0.1 as Material

PopupArea {
    id: popupDialog

    property alias title: title.text
    property alias body: body.text

    default property alias data: dialogContent.data

    property string negativeButtonText: "Cancel"
    property string positiveButtonText: "Ok"

    signal accepted()
    signal rejected()

    function close()
    {
        active = false
    }

    anchors.centerIn: parent
    width: parent.width - 96 * dp
    height: content.implicitHeight + buttons.implicitHeight
            + content.anchors.bottomMargin + content.anchors.topMargin + 8 * dp

    enabled: false

    Rectangle {
        id: dialog
        anchors.fill: parent
        radius: 2
        opacity: 0

        color: "white"

        Column {
            id: content

            anchors {
                top: parent.top
                margins: 24 * dp
                bottomMargin: 16 * dp
                horizontalCenter: parent.horizontalCenter
            }

            width: parent.width - anchors.leftMargin - anchors.rightMargin
            height: title.visible ? title.implicitHeight : 0 + body.implicitHeight + 8 * dp
            spacing: 8 * dp

            Text {
                id: title
                width: parent.width

                visible: text != ""

                font.family: "Roboto Medium"
                font.pixelSize: 20 * dp
                color: "#212121"
                text: "Use Google's location service?"
                wrapMode: Text.WordWrap
            }
            Text {
                id: body
                width: parent.width

                font.family: "Roboto"
                font.pixelSize: 14 * dp
                lineHeight: 13 * dp
                lineHeightMode: Text.FixedHeight
                color: "#757575"
                text: "Let Google help apps determine location. This means sending anonymous location data to Google, even when no apps are running."
                wrapMode: Text.Wrap
            }
            Column {
                id: dialogContent
                spacing: 16 * dp
                width: parent.width
            }
        }

        Row {
            id: buttons
            anchors {
                rightMargin: 16 * dp
                leftMargin: 16 * dp
                bottom: parent.bottom
                bottomMargin: 8 * dp
                horizontalCenter: parent.horizontalCenter
            }

            width: parent.width - anchors.rightMargin - anchors.leftMargin
            height: 48 * dp

            layoutDirection: Qt.RightToLeft
            spacing: 8 * dp

            PopupDialogButton {
                caption: positiveButtonText
                onClicked: {
                    accepted()
                    close()
                }
            }

            PopupDialogButton {
                caption: negativeButtonText
                onClicked: {
                    rejected()
                    close()
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
