import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.2
import "../." // Singletons import

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

    function addItem(icon, title, extraData)
    {
        var component = Qt.createComponent("NavigationDrawerItem.qml");
        var listItem = component.createObject(listItems);
        listItem.icon = icon;
        listItem.caption = title;
        listItem.extraData = extraData;

        return listItem
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

    NavigationDrawerDivider {
        id: topDivider
    }

    NavigationDrawerItem
    {
        id: listHeader
        caption: "List"
        rightIcon: ""
        color: Theme.primaryColor
        iconColor: Theme.primaryColor

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

        Column {
            id: listItems

            anchors.left: parent.left
            anchors.right: parent.right
        }

        NavigationDrawerDivider {
            anchors.bottom: parent.bottom
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
                target: topDivider
                height: 0
                opacity: 0
            }
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
                target: listHeader
                color: Theme.textColor
                iconColor: Theme.iconColor
            }
        }
    ]
}

