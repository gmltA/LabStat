import QtQuick 2.0
import "."

View {
    id: root

    property bool stateTwoActive: state == "second"

    signal stateOneClicked
    signal stateTwoClicked

    function toggleState() {
        if (state == "first")
            state = "second"
        else if (state == "second")
            state = "first"
    }

    anchors.right: parent.right
    anchors.rightMargin: 18 * dp
    height: 48 * dp
    width: 48 * dp

    state: "first"

    radius: height / 2
    tintColor: button.pressed ? Qt.darker(Theme.primaryColor, 1.1) : Theme.primaryColor

    Behavior on rotation {
        RotationAnimation {
            duration: 200
            easing.type: Easing.InOutQuad
        }
    }
    Behavior on tintColor {
        ColorAnimation {
            duration: 50
        }
    }

    Text {
        id: buttonText
        anchors.centerIn: parent
        text: "+"
        color: "white"
        font.family: "Roboto Regular"
        font.pixelSize: 24 * dp

        Behavior on font.pixelSize {
            NumberAnimation {
                duration: 200
                easing.type: Easing.InOutQuad
            }
        }
    }
    MouseArea {
        id: button
        anchors.fill: parent
        onClicked: {
            if (root.state == "first")
                stateOneClicked()
            else if (root.state == "second")
                stateTwoClicked()
        }
        onPressAndHold: {
            toggleState()
        }
    }

    states: [
        State {
            name: "first"
        },
        State {
            name: "second"
            PropertyChanges {
                target: root
                rotation: 45
                tintColor: button.pressed ? Qt.darker(Theme.accentColor, 1.1) : Theme.accentColor
            }
            PropertyChanges {
                target: buttonText
                font.pixelSize: 36 * dp
            }
        }
    ]
}

