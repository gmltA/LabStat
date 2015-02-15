import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.3

ApplicationWindow {
    title: qsTr("Hello World")
    width: 640
    height: 480
    visible: true

    menuBar: MenuBar {
        Menu {
            MenuItem {
                text: "Something"
            }
            MenuItem {
                text: "Something else"
            }
        }
    }

    toolBar: ToolBar {
        RowLayout {
            anchors.fill: parent
            ToolButton {
                id: mainButton
                //anchors.left: parent.left
                text: "Work"
            }
            ToolButton {
                id: settingsButton
                //anchors.left: mainButton.right
                text: "Settings"
            }
        }
    }


    Item {
        width: parent.width
        height: parent.height
        id: wtf
        Button{
            text:"lol"
            onClicked: stackView.pop()
        }
    }

    StackView {
            id: stackView
            anchors.fill: parent
            // Implements back key navigation
            focus: true
            Keys.onReleased: if (event.key === Qt.Key_Back && stackView.depth > 1) {
                                 stackView.pop();
                                 event.accepted = true;
                             }
            initialItem: Item {
                        width: parent.width
                        height: parent.height
                        Button{
                            text: "Test"
                            onClicked: stackView.push(wtf)
                        }
                    }
    }

    /*MainForm {
        anchors.fill: parent

        button1.onClicked: messageDialog.show(qsTr("Button 1 pressed"))
        button2.onClicked: messageDialog.show(qsTr("Button 2 pressed"))
        button3.onClicked: messageDialog.show(qsTr("Button 3 pressed"))
    }*/

    MessageDialog {
        id: messageDialog
        title: qsTr("May I have your attention, please?")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }
}
