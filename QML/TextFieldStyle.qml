import QtQuick 2.0
import QtQuick.Controls.Styles 1.2

TextFieldStyle {
    font {
            family: "Roboto"
            pixelSize: 16 * dp
    }

    background: Item {
        Rectangle {
            anchors.bottom: parent.bottom
            width: parent.width
            height: control.activeFocus ? 2 * dp : 1 * dp
            color: Qt.rgba(0, 0, 0, 0.57)

            Behavior on height {
                NumberAnimation { duration: 200 }
            }
        }
    }
}
