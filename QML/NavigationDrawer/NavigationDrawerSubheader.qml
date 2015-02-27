import QtQuick 2.0

Rectangle {
    id: divider
    height: 48 * dp
    anchors.left: parent.left
    anchors.right: parent.right

    property alias caption: title.text

    Text {
        id: title
        anchors.left: parent.left
        anchors.leftMargin: 16 * dp
        anchors.verticalCenter: parent.verticalCenter
        text: "Subheader"

        font.pointSize: 14
        font.weight: Font.Black
        color: Qt.rgba(0,0,0,0.54)
    }
}

