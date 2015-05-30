import QtQuick 2.0
import QtQuick.Controls 1.2
import "."

Button {
    id: button

    property color backgroundColor: elevation > 0 ? "white" : "transparent"

    property string context: "default" // or "dialog" or "snackbar"

    property int elevation

    property color textColor: Theme.textColor

    style: ButtonStyle {}
}
