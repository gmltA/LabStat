import QtQuick 2.0
import "."

pragma Singleton

QtObject {
    property color primaryColor: Palette.colors["indigo"]["500"]
    property color primaryDarkColor: Palette.colors["indigo"]["700"]
    property color accentColor: Palette.colors["pink"]["A200"]
    property color backgroundColor: "#f3f3f3"
    property color tabHighlightColor: accentColor

    readonly property color textColor: shade(0.87)
    readonly property color subTextColor: shade(0.54)
    readonly property color iconColor: subTextColor
    readonly property color hintColor: shade(0.26)
    readonly property color dividerColor: shade(0.12)

    function shade(alpha) {
		return Qt.rgba(0,0,0,alpha)
    }
}