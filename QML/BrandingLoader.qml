import QtQuick 2.4
import QtQuick.Controls 1.2 as Controls
import "."
import SubjectHandler 1.0

Controls.ApplicationWindow {
    title: "LabStat"
    id: loader
    width: 360
    height: 640
    visible: true

    // for QMLScene debug only!
    //property real dp: loader.width / 320

    Item {
        id: intro
        anchors.fill: parent
        Rectangle {
            id: bg
            anchors.fill: parent
            color: "black"
        }

        Label {
            anchors.centerIn: parent
            anchors.verticalCenterOffset: 50 * dp
            text: "LabStat"
            color: "black"
            style: "display3"
        }

        Label {
            id: bsuir
            anchors.centerIn: parent
            anchors.verticalCenterOffset: 100 * dp
            text: "BSUIR"
            color: Theme.accentColor
            style: "display1"
            opacity: 0
        }

        Item {
            id: grid

            property color gridColor: "white"
            property real gridThickness: 2
            property real zoom: 1

            opacity: 0
            anchors.centerIn: parent
            height: parent.height
            width: loader.width
            Column {
                id: gridColumn
                anchors.centerIn: parent
                height: 50 * dp
                spacing: 50 * dp
                Repeater {
                    model: 2
                    Rectangle {
                        width: loader.width
                        height: grid.gridThickness
                        color: grid.gridColor
                    }
                }
            }
            Row {
                id: gridRow
                anchors.centerIn: parent
                clip: true
                spacing: 100 * dp
                Repeater {
                    model: 2
                    Rectangle {
                        width: grid.gridThickness
                        height: grid.height
                        color: grid.gridColor
                    }
                }
            }

            transform: Scale {
                id: scale
                origin.x: grid.width / 2
                origin.y: grid.height / 2
                xScale: grid.zoom
                yScale: grid.zoom
            }
        }

        Item {
            id: letter
            property real zoom: 1
            anchors.centerIn: parent

            width: horizontalLine.width
            height: verticalLine.height

            Rectangle {
                id: verticalLine
                height: 60 * dp
                width:  10 * dp
                color: Theme.primaryColor
            }

            Rectangle {
                id: horizontalLine
                anchors.top: verticalLine.bottom
                height: 10 * dp
                width: 43 * dp
                color: Theme.primaryColor
            }

            transform: Scale {
                origin.x: 0
                origin.y: letter.height
                xScale: letter.zoom
                yScale: letter.zoom
            }

            transformOrigin: Item.BottomLeft

            state: "hidden"
            states: [
                State {
                    name: "check"
                    ParentChange {
                        target: letter;
                        parent: grid;
                    }
                    PropertyChanges {
                        target: letter
                        rotation: -45
                        anchors.horizontalCenterOffset: 10 * dp
                    }
                    PropertyChanges {
                        target: verticalLine
                        height: 25 * dp
                    }
                    PropertyChanges {
                        target: horizontalLine
                        width: 71 * dp
                    }
                },
                State {
                    name: "check_parent"
                    PropertyChanges {
                        target: letter
                        rotation: -45
                        anchors.horizontalCenterOffset: -15 * dp
                        anchors.verticalCenterOffset: 14 * dp
                    }
                    PropertyChanges {
                        target: verticalLine
                        height: 25 * dp
                    }
                    PropertyChanges {
                        target: horizontalLine
                        width: 71 * dp
                    }
                },
                State {
                    name: "hidden"
                    ParentChange {
                        target: letter;
                        parent: grid;
                    }
                    PropertyChanges {
                        target: letter
                        rotation: -45
                        anchors.horizontalCenterOffset: 10 * dp
                    }
                    PropertyChanges {
                        target: verticalLine
                        height: 0
                    }
                    PropertyChanges {
                        target: horizontalLine
                        width: 0
                    }
                }
            ]
            transitions: [
                Transition {
                    from: "hidden"
                    to: "check"
                    NumberAnimation {
                        properties: "width, height, rotation";
                        duration: 2000
                        easing.type: Easing.InOutQuad
                    }
                },
                Transition {
                    from: "check_parent"
                    to: ""
                    NumberAnimation {
                        properties: "width, height, rotation";
                        duration: 1000
                        easing.type: Easing.InOutQuad
                    }
                }
            ]
        }

        Rectangle {
            id: blackout
            anchors.fill: parent
            gradient: Gradient {
                GradientStop { position: 0.2; color: "black" }
                GradientStop { position: 0.5; color: "transparent" }
                GradientStop { position: 0.8; color: "black" }
            }
        }

        Rectangle {
            id: popup
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 48 * dp
            width: parent.width - 48 * dp * 2
            height: 48 * dp
            radius: 10 * dp
            color: Qt.rgba(255,255,255,0.5)
            visible: false
            Label {
                anchors.centerIn: parent
                style: "dialog"
                text: "Tap to dismiss"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    pageLoader.visible = true
                    intro.visible = false
                    //close()
                }
            }

            SequentialAnimation on opacity {
                id: flickAnim

                running: true
                alwaysRunToEnd: true
                loops: Animation.Infinite

                NumberAnimation {
                    from: 1
                    to: 0.3
                    duration: 500
                    easing.type: Easing.InOutQuad
                }
                NumberAnimation {
                    from: 0.3
                    to: 1
                    duration: 500
                    easing.type: Easing.InOutQuad
                }
                PauseAnimation {
                    duration: 250
                }
            }
        }

        SequentialAnimation {
            running: true
            ParallelAnimation {
                PropertyAnimation {
                    target: letter
                    property: "state"
                    to: "check"
                }
                NumberAnimation {
                    target: grid;
                    property: "opacity";
                    to: 1;
                    duration: 500
                    easing.type: Easing.InOutQuad
                }
                NumberAnimation {
                    target: grid;
                    property: "zoom";
                    from: 10;
                    to: 2;
                    duration: 2000
                    easing.type: Easing.InOutQuad
                }
                NumberAnimation {
                    target: grid;
                    property: "gridThickness";
                    from: 0;
                    to: 1;
                    duration: 2000
                    easing.type: Easing.InOutQuad
                }
            }
            PropertyAnimation {
                target: letter
                property: "state"
                to: "check_parent"
            }
            ParallelAnimation {
                PropertyAnimation {
                    target: letter
                    property: "state"
                    to: ""
                }
                NumberAnimation {
                    target: grid
                    property: "anchors.verticalCenterOffset"
                    to: 200
                    duration: 500
                    easing.type: Easing.InOutQuad
                }
                NumberAnimation {
                    target: grid;
                    property: "gridThickness";
                    from: 1;
                    to: 0;
                    duration: 1000
                    easing.type: Easing.InOutQuad
                }
            }
            ParallelAnimation {
                PropertyAnimation {
                    target: letter
                    property: "anchors.horizontalCenterOffset"
                    to: -70 * dp
                    duration: 300
                }
                PropertyAnimation {
                    target: letter
                    property: "anchors.verticalCenterOffset"
                    to: 34 * dp
                    duration: 300
                }
                PropertyAnimation {
                    target: letter
                    property: "zoom"
                    to: 0.3
                    duration: 300
                }
            }
            ParallelAnimation {
                NumberAnimation {
                    target: blackout
                    property: "opacity"
                    duration: 1000
                    to: 0
                    easing.type: Easing.InOutQuad
                }
                ColorAnimation {
                    target: bg
                    property: "color"
                    duration: 1000
                    to: Theme.backgroundColor
                    easing.type: Easing.InOutQuad
                }
            }
            NumberAnimation {
                target: bsuir
                property: "opacity"
                to: 1
                easing.type: Easing.InOutQuad
            }
        }
    }

    Connections {
        target: SubjectHandler
        onDataInitialized: {
            popup.visible = true
        }
    }

    Loader {
        id: pageLoader
        asynchronous: true
        source: "main.qml"
        onLoaded: {
            pageLoader.item.parent = parent
            SubjectHandler.initData()

        }
        visible: false
        onVisibleChanged: pageLoader.item.visible = visible
    }
}
