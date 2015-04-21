import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.2

Item
{
    default property alias data: listItems.data
    property alias icon: listHeader.icon
    property alias rightIcon: listHeader.rightIcon
    property alias caption: listHeader.caption

    anchors.left: parent.left
    anchors.right: parent.right
    height: childrenRect.height

    state: "collapsed"

    function addItem(icon, title)
    {
        var component = Qt.createComponent("NavigationDrawerItem.qml");
        var listItem = component.createObject(listItems);
        listItem.icon = icon;
        listItem.caption = title;
    }

    function toogle()
    {
        if (state == "collapsed")
            state = "";
        else
            state = "collapsed";
    }

    function isEmpty()
    {
        return listItems.children.length === 0
    }

    NavigationDrawerItem
    {
        id: listHeader
        caption: "List"
        rightIcon: ""
        captionItem.font.bold: true

        onClicked: {
            toogle()
        }
    }

    Rectangle {
        id: listContainer
        anchors.top: listHeader.bottom

        width: parent.width
        height: isEmpty() ? dummyItem.height : listItems.height

        color: "#D9D9D9"

        NavigationDrawerItem {
            id: dummyItem
            icon: ""
            caption: "No items"
            visible: isEmpty()
        }

        Rectangle {
            height: 4 * dp
            width: parent.width
            z: 1
            transformOrigin: Item.TopLeft
            gradient: Gradient{
                GradientStop { position: 1; color: "#00000000"}
                GradientStop { position: 0; color: "#2c000000"}
            }
        }

        Column {
            id: listItems

            anchors.left: parent.left
            anchors.right: parent.right
        }

        Rectangle {
            height: 4 * dp
            width: parent.width
            z: 1
            anchors.bottom: parent.bottom
            gradient: Gradient {
                GradientStop { position: 0; color: "#00000000"}
                GradientStop { position: 1; color: "#2c000000"}
            }
        }

        Behavior on height {
            NumberAnimation {
                duration: 300
                easing.type: Easing.OutCubic
            }
        }

        Behavior on opacity {
            NumberAnimation {
                duration: 300
                easing.type: Easing.OutCubic
            }
        }
    }

    // todo: animate list icon

    states: [
        State {
            name: "collapsed"
            PropertyChanges {
                target: listContainer
                height: 0
                opacity: 0
            }
            PropertyChanges {
                target: listHeader.rightIconItem
                rotation: -180
            }
            PropertyChanges {
                target: listHeader.captionItem
                font.bold: false
            }
        }
    ]
}

