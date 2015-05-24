import QtQuick 2.2

Item {
    id: root

    signal menuClicked
    signal backClicked

    width: 24 * dp
    height: 24 * dp

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (root.state == "menu")
                root.menuClicked()
            else
                root.backClicked()
        }
    }

    Rectangle {
        id: bar1
        x: 2 * dp
        y: 5 * dp
        width: 20 * dp
        height: 2 * dp
        antialiasing: true
    }

    Rectangle {
        id: bar2
        x: 2 * dp
        y: 10 * dp
        width: 20 * dp
        height: 2 * dp
        antialiasing: true
    }

    Rectangle {
        id: bar3
        x: 2 * dp
        y: 15 * dp
        width: 20 * dp
        height: 2 * dp
        antialiasing: true
    }

    property int animationDuration: 350

    state: "menu"
    states: [
        State {
            name: "menu"
        },

        State {
            name: "back"
            PropertyChanges {
                target: root
                rotation: 180
            }
            PropertyChanges {
                target: bar1
                rotation: 45
                width: 13 * dp
                x: 9.5 * dp
                y: 8 * dp
            }
            PropertyChanges {
                target: bar2
                width: 17 * dp
                x: 3 * dp
                y: 12 * dp
            }
            PropertyChanges {
                target: bar3
                rotation: -45
                width: 13 * dp
                x: 9.5 * dp
                y: 16 * dp
            }
        }
    ]

    transitions: [
        Transition {
            RotationAnimation {
                target: root
                direction: RotationAnimation.Clockwise
                duration: animationDuration
                easing.type: Easing.InOutQuad
            }
            PropertyAnimation {
                target: bar1
                properties: "rotation, width, x, y"
                duration: animationDuration
                easing.type: Easing.InOutQuad
            }
            PropertyAnimation {
                target: bar2
                properties: "rotation, width, x, y"
                duration: animationDuration
                easing.type: Easing.InOutQuad
            }
            PropertyAnimation {
                target: bar3
                properties: "rotation, width, x, y"
                duration: animationDuration
                easing.type: Easing.InOutQuad
            }
        }
    ]
}
