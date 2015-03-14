import QtQuick 2.0
import QtQuick.Layouts 1.1

Rectangle {
    id: header

    property alias mainText: mainTextItem.text
    property alias secondaryText: secondaryTextItem.text
    property alias buttonArea: area

    height: 56 * dp
    width: parent.width

    color: "#84BAC4"
    Image {
        id: headerBG
        height: parent.height
        width: parent.width
        fillMode: Image.PreserveAspectCrop
        source: "qrc:/img/bg.jpg"
        verticalAlignment: Image.AlignVCenter
        clip: true
    }

    Rectangle {
        height: parent.height
        width: parent.width
        anchors.verticalCenter: parent.verticalCenter
        color: Qt.rgba(0,0,0,0)

        // todo: should we replace Layout with just Column or even simple Rectangle?
        ColumnLayout {
            anchors.left: parent.left
            anchors.leftMargin: 16 * dp
            anchors.right: parent.left
            anchors.rightMargin: 16 * dp
            anchors.verticalCenter: parent.verticalCenter

            Layout.fillWidth: true

            Text {
                id: mainTextItem
                text: "Main drawer title"

                font.pointSize: 14
                font.weight: Font.Black
                color: "white"
                //color: Qt.rgba(0,0,0,0.54)
            }
            Text {
                id: secondaryTextItem
                text: "Drawer subtitle"

                font.pointSize: 14
                color: "white"
                //color: Qt.rgba(0,0,0,0.54)
            }
        }
        Rectangle {
            height: parent.parent.height
            width: parent.height
            color: area.pressed ? Qt.rgba(1,1,1,0.5) : Qt.rgba(0,0,0,0)

            anchors.right: parent.right
            Text {
                anchors.centerIn: parent
                text: ""
                font.pointSize: 14
                font.family: materialIcons.name

                color: "white"
                //color: Qt.rgba(0,0,0,0.87)
            }
            MouseArea {
                id: area
                anchors.fill: parent
            }
        }
    }
}

