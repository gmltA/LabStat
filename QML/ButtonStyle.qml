import QtQuick 2.0
import QtQuick.Controls.Styles 1.2
import "."

ButtonStyle {
    id: style

    padding {
        left: 0
        right: 0
        top: 0
        bottom: 0
    }

    property int controlElevation: control.hasOwnProperty("elevation") ? control.elevation : 1

    property color controlBackground: control.hasOwnProperty("backgroundColor")
            ? control.backgroundColor : controlElevation == 0 ? "transparent" : "white"

    property string context: control.hasOwnProperty("context") ? control.context : "default"

    background: View {
        id: background

        implicitHeight: 36 * dp

        radius: 2 * dp

        backgroundColor: controlBackground

        elevation: {
            var elevation = controlElevation

            if (elevation > 0 && (control.focus))
                elevation++;

            if(!control.enabled)
                elevation = 0;

            return elevation;
        }

        tintColor: control.focus || control.hovered
           ? Qt.rgba(0,0,0, elevation > 0 ? 0.03 : 0.05)
           : "transparent"
    }
    label: Item {
        implicitHeight: Math.max(36 * dp, label.height + 16 * dp)
        implicitWidth: context == "dialog"
                ? Math.max(64 * dp, label.width + 16 * dp)
                : context == "snackbar" ? label.width + 16 * dp
                                        : Math.max(88 * dp, label.width + 32 * dp)

        Label {
            id: label
            anchors.centerIn: parent
            text: control.text
            style: "button"
            color: control.enabled ? control.hasOwnProperty("textColor")
                                     ? control.textColor : Theme.textColor
                    : control.darkBackground ? Qt.rgba(1, 1, 1, 0.30)
                                             : Qt.rgba(0, 0, 0, 0.26)
        }
    }
}
