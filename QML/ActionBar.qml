import QtQuick 2.0
import "../QML/QuickAndroid"
import "."

Rectangle {
    id: root

    property alias title: titleText.text
    property alias elevation: shadow.depth
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

    MaterialShadow {
        id: shadow
        anchors.fill: parent
        z: -1
        depth: 1
        asynchronous: true
    }
}

