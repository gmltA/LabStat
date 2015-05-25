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
    tintColor: Theme.primaryColor
    Behavior on rotation {
        RotationAnimation {
            duration: 200
            easing.type: Easing.InOutQuad
        }
    }
    Behavior on tintColor {
        ColorAnimation {
            duration: 200
        }
    }

    Text {
        id: buttonText
        anchors.centerIn: parent
        text: "+"
        color: "white"
        font.family: "Roboto Regular"
        font.pixelSize: 24 * dp
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
            }
            PropertyChanges {
                target: root
                tintColor: Theme.accentColor
            }
        }
    ]
}

