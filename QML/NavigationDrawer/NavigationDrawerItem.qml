import QtQuick 2.3
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.2
import "../." // Singletons import

Button {
    id: root
    height: 48 * dp
    anchors.left: parent ? parent.left : undefined
    anchors.right: parent ? parent.right : undefined

    property alias icon: leftIcon.text
    property alias secondaryAction: secondaryActionArea
    property alias rightIcon: rightIcon.text
    property alias rightIconItem: rightIcon

    property alias caption: captionItem.text
    property alias captionItem: captionItem
    property alias color: captionItem.color
    property alias iconColor: leftIcon.color
    property var extraData

    signal triggered(var extraData);

    Connections {
        target: root
        onClicked: {
            triggered(extraData)
        }
    }

    Text {
        id: leftIcon
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 16 * dp

        font.family: materialIcons.name
        font.pointSize: 28

        color: Theme.iconColor
        text: ""
    }

    Text {
        id: captionItem
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 72 * dp

        font.pointSize: 14
        font.family: "Roboto Regular"

        color: Theme.textColor
        text: "Caption"
    }

    Item {
        height: root.height
        width: height
        anchors.right: parent.right
        anchors.rightMargin: 16 * dp

        Text {
            id: rightIcon
            anchors.centerIn: parent

            font.family: materialIcons.name
            font.pointSize: 28

            color: Theme.iconColor
            text: ""
        }

        MouseArea {
            id: secondaryActionArea
            anchors.fill: parent
            enabled: false
        }
    }

    style: ButtonStyle {
        background: Rectangle {
            implicitWidth: 100
            implicitHeight: 25
            color: control.pressed ? "#E6E6E6" : "white"
        }
    }
}

