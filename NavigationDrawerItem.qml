import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.2

Button {
    id: root
    opacity: parent.opacity
    height: 48 * dp
    anchors.left: parent.left
    anchors.right: parent.right

    property alias icon: icon.text
    property alias caption: caption.text

    Text {
        id: caption
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 72 * dp

        font.pointSize: 14
        font.family: "Roboto"
        text: "Caption"
    }

    Text {
        id: icon
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 16 * dp

        font.family: materialIcons.name
        font.pointSize: 28
        text: ""
    }

    style: ButtonStyle {
        background: Rectangle {
            implicitWidth: 100
            implicitHeight: 25
            color: control.pressed ? "#E6E6E6" : "white"
        }
    }
}

