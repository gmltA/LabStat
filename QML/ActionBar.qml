import QtQuick 2.0
import "."

View  {
    id: root

    property string defaultTitle
    property alias title: titleText.text
    property alias icon: drawerIcon

    anchors.top: parent.top
    width: parent.width
    height: 48 * dp
    tintColor: Theme.primaryColor

    MenuBackIcon {
        id: drawerIcon
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

