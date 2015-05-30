import QtQuick 2.0
import "../." // Singletons import

Rectangle {
    id: divider
    anchors.left: parent.left
    width: parent.width
    height: 1 * dp
    color: Theme.dividerColor
}

