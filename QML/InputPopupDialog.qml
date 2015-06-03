import QtQuick 2.4
import QtQuick.Controls 1.2

PopupDialog {
    id: inputDialog

    hasActions: true

    positiveButtonEnabled: textField.acceptableInput

    property alias textField: textField

    property alias validator: textField.validator
    property alias inputMask: textField.inputMask
    property alias inputMethodHints: textField.inputMethodHints

    property alias placeholderText: textField.placeholderText
    property alias value: textField.text

    onOpened: {
        value = ""
        textField.forceActiveFocus()
    }

    TextField {
        id: textField

        anchors {
            left: parent.left
            right: parent.right
        }

        style: TextFieldStyle {
        }
    }
}
