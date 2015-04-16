import QtQuick 2.0
import "../QML/QuickAndroid"

Rectangle {
    id: root

    property alias title: titleText.text
    anchors.top: parent.top
    width: parent.width
    height: 48 * dp
    color: "#00BCD4"

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
        anchors.fill: parent
        z : -1
        depth : 1
        asynchronous: true
    }
}

