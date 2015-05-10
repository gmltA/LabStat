import QtQuick 2.0
import QtQuick.Layouts 1.1
import "../."

Rectangle {
    id: header

    property alias mainText: mainTextItem.text
    property alias secondaryText: secondaryTextItem.text
    property alias buttonIcon: icon.text
    property alias buttonArea: area
    property alias actionButton: button
    property var model

    signal headerContentScrolled(int index)

    height: 144 * dp
    width: parent.width

    Image {
        id: headerBG
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        source: "qrc:/img/bg.jpg"
        verticalAlignment: Image.AlignVCenter
    }

    Rectangle {
        anchors.bottom: parent.bottom
        width: parent.width
        height: 56 * dp
        gradient: Gradient{
            GradientStop { position: 0 ; color: "#00000000"}
            GradientStop { position: 1 ; color: "#2c000000"}
        }
    }

    Rectangle {
        anchors.right: parent.right
        anchors.rightMargin: 18 * dp
        height: 48 * dp
        width: 48 * dp
        z: 1
        radius: height / 2
        color: Theme.accentColor
        Text {
            anchors.centerIn: parent
            text: "+"
            color: "white"
            font.family: "Roboto Regular"
            font.pixelSize: 14 * dp
        }
        MouseArea {
            id: button
            anchors.fill: parent
        }
    }

    ListView {
        id: headerList
        anchors.fill: parent

        visible: header.model && header.model.length !== 0

        orientation: ListView.Horizontal
        highlightRangeMode: ListView.StrictlyEnforceRange
        boundsBehavior: Flickable.StopAtBounds
        snapMode: ListView.SnapOneItem

        onCurrentIndexChanged: {
            header.headerContentScrolled(currentIndex)
        }

        model: header.model
        delegate: Component {
            Item {
                width: headerList.width
                height: headerList.height
                Text {
                    anchors.left: parent.left
                    anchors.leftMargin: 24 * dp
                    anchors.verticalCenter: parent.verticalCenter

                    text: title
                    color: "white"
                    font.pointSize: 24
                    font.family: "Roboto Light"
                }
            }
        }
    }

    Item {
        height: 56 * dp
        width: parent.width
        anchors.bottom: parent.bottom

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
                font.family: "Roboto Medium"
                color: "white"
            }
            Text {
                id: secondaryTextItem

                visible: text != ""

                font.pointSize: 14
                font.family: "Roboto Regular"
                color: "white"
            }
        }
        Rectangle {
            height: parent.height
            width: parent.height
            color: area.pressed ? Qt.rgba(1,1,1,0.5) : Qt.rgba(0,0,0,0)

            anchors.right: parent.right
            Text {
                id: icon
                anchors.centerIn: parent
                text: ""
                font.pointSize: 14
                font.family: materialIcons.name

                color: "white"
            }
            MouseArea {
                id: area
                anchors.fill: parent
            }
        }
    }
}

