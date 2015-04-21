import QtQuick 2.0

Item {
    id: button

    property alias caption: text.text

    anchors.verticalCenter: parent.verticalCenter

    height: 36 * dp
    width: text.width + 16 * dp

    signal clicked()

    Text {
        id: text
        anchors.centerIn: parent
        maximumLineCount: 1

        text: "Button"
        font.family: "Roboto Medium"
        font.pixelSize: 14
        color: "#00BCD4"
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
