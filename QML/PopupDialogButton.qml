import QtQuick 2.0

Item {
    id: button

    property alias caption: text.text

    anchors.verticalCenter: parent.verticalCenter

    height: 36 * dp
    width: text.width + 16 * dp

    Text {
        id: text
        anchors.centerIn: parent

        text: "Button"
        font.family: "Roboto Medium"
        font.pixelSize: 14
        color: "#00BCD4"
        font.capitalization: Font.AllUppercase
    }
}
