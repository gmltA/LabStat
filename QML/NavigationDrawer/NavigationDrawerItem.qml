import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.2

Button {
    id: root
    height: 48 * dp
    anchors.left: parent.left
    anchors.right: parent.right

    property alias icon: leftIcon.text
    property alias rightIcon: rightIcon.text
    property alias rightIconItem: rightIcon
    property alias caption: captionItem.text
    property alias captionItem: captionItem

    Text {
        id: leftIcon
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 16 * dp

        font.family: materialIcons.name
        font.pointSize: 28

        color: Qt.rgba(0,0,0,0.87)
        text: ""
    }

    Text {
        id: captionItem
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 72 * dp

        font.pointSize: 14
        font.family: "Roboto Regular"

        color: Qt.rgba(0,0,0,0.87)
        text: "Caption"
    }

    Text {
        id: rightIcon
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 16 * dp

        font.family: materialIcons.name
        font.pointSize: 28

        color: Qt.rgba(0,0,0,0.87)
        text: ""
    }

    style: ButtonStyle {
        background: Rectangle {
            implicitWidth: 100
            implicitHeight: 25
            color: control.pressed ? "#E6E6E6" : "white"
        }
    }
}

