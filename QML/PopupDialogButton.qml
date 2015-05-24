import QtQuick 2.0
import "."

Item {
    id: button

    property alias caption: text.text
    property alias color: text.color

    anchors.verticalCenter: parent.verticalCenter

    implicitHeight: 36 * dp
    implicitWidth: text.width + 16 * dp

    signal clicked()

    Text {
        id: text
        anchors.centerIn: parent
        maximumLineCount: 1

        text: "Button"
        font.family: "Roboto Medium"
        font.pixelSize: 14 * dp
        color: Theme.accentColor
        font.capitalization: Font.AllUppercase
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            parent.clicked()
        }
    }
}
