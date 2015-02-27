import QtQuick 2.0

Rectangle {
    id: header

    property alias mainText: mainTextItem.text
    property alias secondaryText: secondaryTextItem.text

    height: 56 * dp
    anchors.left: parent.left
    anchors.right: parent.right
    color: "#E3ECF5"

    Column {
        anchors.left: parent.left
        anchors.leftMargin: 16 * dp
        anchors.verticalCenter: parent.verticalCenter
        Text {
            id: mainTextItem
            text: "Main drawer title"

            font.pointSize: 14
            font.weight: Font.Black
            color: Qt.rgba(0,0,0,0.54)
        }
        Text {
            id: secondaryTextItem
            text: "Drawer subtitle"

            font.pointSize: 14
            color: Qt.rgba(0,0,0,0.54)
        }
    }
}

