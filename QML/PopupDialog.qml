import QtQuick 2.0
import QtQuick.Controls 1.0
import "."
//import Material 0.1 as Material

PopupBase {
    id: dialog

    overlayLayer: "dialogOverlayLayer"
    overlayColor: Qt.rgba(0, 0, 0, 0.3)

    opacity: showing ? 1 : 0
    visible: opacity > 0

    width: Math.max(minimumWidth,
                    content.contentWidth + 2 * contentMargins)

    height: Math.min(350 * dp,
                     headerView.height + 32 * dp +
                     content.contentHeight +
                     content.topMargin +
                     content.bottomMargin +
                     buttonContainer.height)

    property int contentMargins: 16 * dp

    property int minimumWidth: 270 * dp

    property alias title: titleLabel.text
    property alias text: textLabel.text

    property alias negativeButton: negativeButton

    property alias positiveButton: positiveButton

    property string negativeButtonText: "Cancel"
    property string positiveButtonText: "Ok"
    property alias positiveButtonEnabled: positiveButton.enabled

    property bool hasActions: true

    default property alias dialogContent: column.data

    signal accepted()
    signal rejected()

    anchors {
        centerIn: parent
        verticalCenterOffset: showing ? 0 : -(dialog.height/3)

        Behavior on verticalCenterOffset {
            NumberAnimation {
                duration: 150
                easing.type: Easing.OutQuad
            }
        }
    }

    Behavior on opacity {
        NumberAnimation {
            duration: 150
            easing.type: Easing.OutQuad
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
            closeKeyPressed(event)
        }
    }

    Keys.onReleased: {
        if (event.key === Qt.Key_Back) {
            closeKeyPressed(event)
        }
    }

    function closeKeyPressed(event) {
        if (dialog.showing) {
            if (dialog.dismissOnTap) {
                dialog.close()
            }
            event.accepted = true
        }
    }

    function show() {
        open()
    }

    View {
        id: dialogContainer

        anchors.fill: parent
        elevation: 5
        radius: dp * (2)

        MouseArea {
            anchors.fill: parent
            propagateComposedEvents: false

            onClicked: {
                mouse.accepted = false
            }
        }

        Item {
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                topMargin: dp * (8)
            }

            clip: true
            height: headerView.height + dp * (32)

            View {
                backgroundColor: "white"
                elevation: content.atYBeginning ? 0 : 1
                fullWidth: true

                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                }

                height: headerView.height + dp * (16)
            }
        }


        Column {
            id: headerView

            spacing: dp * (16)

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top

                leftMargin: dp * (16)
                rightMargin: dp * (16)
                topMargin: dp * (16)
            }

            Label {
                id: titleLabel

                width: parent.width
                wrapMode: Text.Wrap
                style: "title"
                visible: text != ""
            }

            Label {
                id: textLabel

                width: parent.width
                wrapMode: Text.Wrap
                style: "dialog"
                visible: text != ""
            }
        }

        Rectangle {
            anchors.fill: content
        }

        Flickable {
            id: content

            contentWidth: column.implicitWidth
            contentHeight: column.height
            clip: true

            anchors {
                left: parent.left
                right: parent.right
                top: headerView.bottom
                topMargin: dp * (8)
                bottomMargin: dp * (-8)
                bottom: buttonContainer.top
            }

            interactive: contentHeight + dp * (8) > height
            bottomMargin: hasActions ? 0 : dp * (8)

            Rectangle {
                Column {
                    id: column
                    anchors {
                        left: parent.left
                        margins: contentMargins
                    }

                    width: content.width - 2 * contentMargins
                    spacing: dp * (16)
                }
            }
        }

        Item {
            id: buttonContainer

            anchors {
                bottomMargin: dp * (8)
                bottom: parent.bottom
                right: parent.right
                left: parent.left
            }

            height: hasActions ? buttonView.height + dp * (8) : 0
            clip: true

            View {
                id: buttonView

                height: hasActions ? positiveButton.implicitHeight + dp * (8) : 0
                visible: hasActions

                backgroundColor: "white"
                elevation: content.atYEnd ? 0 : 1
                fullWidth: true
                elevationInverted: true

                anchors {
                    bottom: parent.bottom
                    right: parent.right
                    left: parent.left
                }

                PopupDialogButton {
                    id: negativeButton

                    text: negativeButtonText
                    textColor: Theme.accentColor
                    context: "dialog"

                    anchors {
                        top: parent.top
                        right: positiveButton.left
                        topMargin: dp * (8)
                        rightMargin: dp * (8)
                    }

                    onClicked: {
                        rejected();
                        close();
                    }
                }

                PopupDialogButton {
                    id: positiveButton

                    text: positiveButtonText
                    textColor: Theme.accentColor
                    context: "dialog"

                    anchors {
                        top: parent.top
                        topMargin: dp * (8)
                        rightMargin: dp * (16)
                        right: parent.right
                    }

                    onClicked: {
                        accepted();
                        close()
                    }
                }
            }
        }
    }
}
