import QtQuick 2.0
import QtQuick.Controls.Styles 1.3

CheckBoxStyle {
    id: style

    property color uncheckedColor
    property color color

    indicator: Item {

        implicitHeight: 54 * dp
        implicitWidth: 54 * dp

        Rectangle {
            id: rect
            anchors.centerIn: parent

            width: 24 * dp
            height: 24 * dp
            radius: 2 * dp
            border.width: 2 * dp
            border.color: style.uncheckedColor

            Behavior on border.color {
                SequentialAnimation {
                    PauseAnimation {
                        duration: 50
                    }
                    ColorAnimation {
                        easing.type: Easing.InOutQuad
                        duration: 50
                    }
                }
            }

            Behavior on border.width {
                NumberAnimation {
                    easing.type: Easing.InOutQuad
                    duration: 100
                }
            }

            state: parent.state

            transitions: [
                Transition {
                    from: "checked"
                    to: ""
                    PauseAnimation {
                        duration: 100
                    }
                }
            ]

            states: [
                State {
                    name: "checked"
                    PropertyChanges {
                        target: rect
                        border.width: 20 * dp
                        border.color: style.color
                    }
                }
            ]
        }

        Text {
            id: check
            anchors.centerIn: parent

            text: ""
            color: "white"
            font.pixelSize: 1 * dp
            font.family: materialIcons.name

            Behavior on font.pixelSize {
                NumberAnimation {
                    easing.type: Easing.InOutQuad
                    duration: 100
                }
            }

            state: parent.state

            transitions: [
                Transition {
                    from: ""
                    to: "checked"
                    PauseAnimation {
                        duration: 100
                    }
                }
            ]

            states: [
                State {
                    name: "checked"
                    PropertyChanges {
                        target: check
                        font.pixelSize: 24 * dp
                    }
                }
            ]
        }

        states: [
            State {
                name: "checked"
                when: control.checked
                PropertyChanges {
                    target: rect
                    border.width: 20 * dp
                }
                PropertyChanges {
                    target: check
                    font.pixelSize: 24 * dp
                }
            }
        ]
    }
}
