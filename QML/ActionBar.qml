import QtQuick 2.0
import "."

Rectangle {
    id: root

    property alias title: titleText.text
    anchors.top: parent.top
    width: parent.width
    height: 48 * dp
    color: Theme.primaryColor

    MenuBackIcon {
        anchors.left: parent.left
        anchors.leftMargin: 16 * dp
        anchors.verticalCenter: parent.verticalCenter
    }

    Text {
        id: titleText
        anchors.left: parent.left
        anchors.leftMargin: 72 * dp
        anchors.verticalCenter: parent.verticalCenter

        text: "Application"
        font.pointSize: 20
        color: "white"
    }
}

