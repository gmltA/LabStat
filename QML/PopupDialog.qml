import QtQuick 2.0
import QtQuick.Controls 1.0

Rectangle {
    width: parent.width - 96 * dp
    height: 200 * dp
    radius: 2

    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter
    color: "white"

    Rectangle {
        anchors {
            top: parent.top
            margins: 24 * dp
            bottomMargin: 16 * dp
            horizontalCenter: parent.horizontalCenter
        }

        height: parent.height - (buttons.height + buttons.anchors.bottomMargin + anchors.topMargin + anchors.bottomMargin)
        width: parent.width - anchors.leftMargin * 2

        Column
        {
            spacing: 18 * dp
            anchors.fill: parent
            Text {
                id: title
                width: parent.width

                font.family: "Roboto"
                font.weight: Font.DemiBold
                font.pixelSize: 20
                color: "#212121"
                text: "Use Google's location service?"
                wrapMode: Text.Wrap
            }

            Text {
                id: body
                width: parent.width

                font.family: "Roboto"
                font.pixelSize: 14
                color: "#757575"
                text: "Let Google help apps determine location."
                wrapMode: Text.Wrap
            }
        }
    }

    Rectangle {
        id: buttons

        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8 * dp

        width: parent.width
        height: 48 * dp

        Row
        {
            anchors {
                rightMargin: 16 * dp
                leftMargin: 16 * dp
                horizontalCenter: parent.horizontalCenter
            }

            width: parent.width - anchors.rightMargin - anchors.leftMargin
            height: parent.height

            layoutDirection: Qt.RightToLeft
            spacing: 8 * dp

            Item {
                anchors.verticalCenter: parent.verticalCenter
                height: 36 * dp
                width: childrenRect.width + 16 * dp
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter

                    text: "Button"
                    font.family: "Roboto"
                    font.weight: Font.DemiBold
                    font.pixelSize: 14
                    color: "#212121"
                    font.capitalization: Font.AllUppercase
                }
            }
            Item {
                anchors.verticalCenter: parent.verticalCenter
                height: 36 * dp
                width: childrenRect.width + 16 * dp
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter

                    text: "Button"
                    font.family: "Roboto"
                    font.weight: Font.DemiBold
                    font.pixelSize: 14
                    color: "#212121"
                    font.capitalization: Font.AllUppercase
                }
            }
        }
    }

    MaterialShadow {
        anchors.fill: parent
        z : -10
        depth : 3
        asynchronous: true
    }
}
