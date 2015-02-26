import QtQuick 2.0
import QtGraphicalEffects 1.0

Rectangle {
    anchors.fill: parent
    Rectangle {
        id: bg
        height: parent.height
        width: parent.width
    }

    layer.enabled: true

    layer.effect: DropShadow {
        radius: 10
        samples: 16
        source: bg
        color: Qt.rgba(0, 0, 0, 0.8)
        transparentBorder: true
    }
}

