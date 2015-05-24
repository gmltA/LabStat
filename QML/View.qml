import QtQuick 2.0
import QtGraphicalEffects 1.0

/*!
   \qmltype View
   \inqmlmodule Material 0.1
   \brief Provides a base view component, with support for Material Design elevation,
   background colors, and tinting.
 */
Item {
    id: item
    width: 100
    height: 62

    property int elevation: 0
    property real radius: 0

    property string style: "default"

    property color backgroundColor: elevation > 0 ? "white" : "transparent"
    property color tintColor: "transparent"

    property alias border: rect.border

    property bool fullWidth
    property bool fullHeight

    property alias clipContent: rect.clip

    default property alias data: rect.data

    property bool elevationInverted: false

    property var topShadow: [
        {
            "opacity": 0,
            "offset": dp * (0),
            "blur": dp * (0)
        },

        {
            "opacity": 0.12,
            "offset": dp * (1),
            "blur": dp * (1.5)
        },

        {
            "opacity": 0.16,
            "offset": dp * (3),
            "blur": dp * (3)
        },

        {
            "opacity": 0.19,
            "offset": dp * (10),
            "blur": dp * (10)
        },

        {
            "opacity": 0.25,
            "offset": dp * (14),
            "blur": dp * (14)
        },

        {
            "opacity": 0.30,
            "offset": dp * (19),
            "blur": dp * (19)
        }
    ]

    property var bottomShadow: [
        {
            "opacity": 0,
            "offset": dp * (0),
            "blur": dp * (0)
        },

        {
            "opacity": 0.24,
            "offset": dp * (1),
            "blur": dp * (1)
        },

        {
            "opacity": 0.23,
            "offset": dp * (3),
            "blur": dp * (3)
        },

        {
            "opacity": 0.23,
            "offset": dp * (6),
            "blur": dp * (3)
        },

        {
            "opacity": 0.22,
            "offset": dp * (10),
            "blur": dp * (5)
        },

        {
            "opacity": 0.22,
            "offset": dp * (15),
            "blur": dp * (6)
        }
    ]

    RectangularGlow {
        property var elevationInfo: bottomShadow[Math.min(elevation, 5)]
        property real horizontalShadowOffset: elevationInfo.offset * Math.sin((2 * Math.PI) * (parent.rotation / 360.0))
        property real verticalShadowOffset: elevationInfo.offset * Math.cos((2 * Math.PI) * (parent.rotation / 360.0))

        anchors.centerIn: parent
        width: parent.width + (fullWidth ? dp * (10) : 0)
        height: parent.height + (fullHeight ? dp * (20) : 0)
        anchors.horizontalCenterOffset: horizontalShadowOffset * (elevationInverted ? -1 : 1)
        anchors.verticalCenterOffset: verticalShadowOffset * (elevationInverted ? -1 : 1)
        glowRadius: elevationInfo.blur
        opacity: elevationInfo.opacity
        spread: 0.05
        color: "black"
        cornerRadius: item.radius + glowRadius * 2.5
        //visible: parent.opacity == 1
    }

    RectangularGlow {
        property var elevationInfo: topShadow[Math.min(elevation, 5)]
        property real horizontalShadowOffset: elevationInfo.offset * Math.sin((2 * Math.PI) * (parent.rotation / 360.0))
        property real verticalShadowOffset: elevationInfo.offset * Math.cos((2 * Math.PI) * (parent.rotation / 360.0))

        anchors.centerIn: parent
        width: parent.width + (fullWidth ? dp * (10) : 0)
        height: parent.height + (fullHeight ? dp * (20) : 0)
        anchors.horizontalCenterOffset: horizontalShadowOffset * (elevationInverted ? -1 : 1)
        anchors.verticalCenterOffset: verticalShadowOffset * (elevationInverted ? -1 : 1)
        glowRadius: elevationInfo.blur
        opacity: elevationInfo.opacity
        spread: 0.05
        color: "black"
        cornerRadius: item.radius + glowRadius * 2.5
        //visible: parent.opacity == 1
    }

    Rectangle {
        id: rect
        anchors.fill: parent
        color: Qt.tint(backgroundColor, tintColor)
        radius: item.radius
        antialiasing: parent.rotation || radius > 0 ? true : false
        clip: true

        Behavior on color {
            ColorAnimation { duration: 200 }
        }
    }
}
