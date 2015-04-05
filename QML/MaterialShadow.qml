import QtQuick 2.0

Item {
    id : shadow

    property int depth : 1;
    property bool asynchronous: false

    Loader {
        id : bottomShadow
        asynchronous: shadow.asynchronous
        anchors.fill: parent
        property string color : "#000000"
        property int verticalOffset: 0
        property int horizontalOffset: 0
        property real blur: 0

        sourceComponent: Shadow {
                color: bottomShadow.color
                verticalOffset: bottomShadow.verticalOffset
                horizontalOffset: bottomShadow.horizontalOffset
                blur: bottomShadow.blur
            }
    }

    Loader {
        id : topShadow
        asynchronous: shadow.asynchronous
        anchors.fill: parent
        property string color : "#000000"
        property int verticalOffset: 0
        property int horizontalOffset: 0
        property real blur: 0

        sourceComponent: Shadow {
                color: topShadow.color
                verticalOffset: topShadow.verticalOffset
                horizontalOffset: topShadow.horizontalOffset
                blur: topShadow.blur
            }
    }

    // Reference: http://www.google.com/design/spec/layout/layout-principles.html#layout-principles-dimensionality

    states: [
        State {
            name: "D1"
            when : depth === 1

            PropertyChanges {
                target: topShadow
                verticalOffset : 1 * dp
                horizontalOffset: 0
                opacity: 0.12
                blur : 1.5 * dp
            }

            PropertyChanges {
                target: bottomShadow
                verticalOffset : 1 * dp
                horizontalOffset: 0
                blur : 1 * dp
                opacity: 0.24
            }

        },
        State {
            name: "D2"
            when : depth === 2

            PropertyChanges {
                target: topShadow
                verticalOffset : 3 * dp
                horizontalOffset: 0
                blur : 3 * dp
                opacity: 0.16
            }

            PropertyChanges {
                target: bottomShadow
                verticalOffset : 3 * dp
                horizontalOffset: 0
                blur : 3 * dp
                opacity: 0.23
            }
        },
        State {
            name: "D3"
            when : depth === 3

            PropertyChanges {
                target: topShadow
                verticalOffset : 10 * dp
                horizontalOffset: 0
                blur : 10 * dp
                opacity: 0.19
            }

            PropertyChanges {
                target: bottomShadow
                verticalOffset : 6 * dp
                horizontalOffset: 0
                blur : 3 * dp
                opacity: 0.23
            }
        },

        State {
            name: "D4"
            when : depth === 4

            PropertyChanges {
                target: topShadow
                verticalOffset : 14 * dp
                horizontalOffset: 0
                blur : 14 * dp
                opacity: 0.25
            }

            PropertyChanges {
                target: bottomShadow
                verticalOffset : 10 * dp
                horizontalOffset: 0
                blur : 5 * dp
                opacity: 0.22
            }
        },
        State {
            name: "D5"
            when : depth === 5

            PropertyChanges {
                target: topShadow
                verticalOffset : 19 * dp
                horizontalOffset: 0
                blur : 19 * dp
                opacity: 0.30
            }

            PropertyChanges {
                target: bottomShadow
                verticalOffset : 15 * dp
                horizontalOffset: 0
                blur : 6 * dp
                opacity: 0.22

            }
        }
    ]
}
