import QtQuick 2.0

Item {
    id: button

    property alias caption: text.text

    anchors.verticalCenter: parent.verticalCenter

    height: 36 * dp
    width: text.width + 16 * dp

    Text {
        id: text
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        text: "Button"
        font.family: "Roboto"
        font.weight: Font.DemiBold
        font.pixelSize: 14
        color: "#212121"
        font.capitalization: Font.AllUppercase
    }
}
