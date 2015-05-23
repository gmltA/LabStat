import QtQuick 2.4
import QtQuick.Controls 1.2

PopupDialog {
    property alias input: text

    title: ""
    body: ""
    active: true
    z: 6

    TextInput {
        id: text
        width: parent.width
        height: 24 * dp
        font.pixelSize: 14 * dp
        maximumLength: 20
        Rectangle {
            anchors.bottom: parent.bottom
            width: parent.width
            height: 1 * dp
            color: Qt.rgba(0, 0, 0, 0.57)
        }
    }
}

