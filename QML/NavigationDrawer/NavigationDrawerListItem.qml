import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.2
import "../." // Singletons import

Item
{
    property alias icon: listHeader.icon
    property alias rightIcon: listHeader.rightIcon
    property alias caption: listHeader.caption
    property var listModel

    anchors.left: parent.left
    anchors.right: parent.right
    height: childrenRect.height

    state: "collapsed"

    function addItem(icon, title, extraData, clickCallback)
    {
        var listItem = {}
        var model = listModel
        listItem["icon"] = icon;
        listItem["caption"] = title;
        listItem["extraData"] = extraData;
        listItem["clickCallback"] = clickCallback;

        if (!model)
            model = []

        model.push(listItem)
        listModel = model
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
        return !listModel || listModel.length === 0
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

        secondaryAction.enabled: true
        secondaryAction.onClicked: {
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
            enabled: false
        }

        Column {
            id: listItems

            anchors.left: parent.left
            anchors.right: parent.right

            Repeater {
                model: listModel
                NavigationDrawerItem {
                    icon: listModel[index].icon
                    caption: listModel[index].caption
                    extraData: listModel[index].extraData

                    onTriggered: {
                        listModel[index].clickCallback(extraData);
                    }
                }
            }
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

