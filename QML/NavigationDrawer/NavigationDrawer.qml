import QtQuick 2.2
import "../."

PopupBase {
    id: panel
    overlayLayer: "dialogOverlayLayer"

    property bool open: false                     // The open or close state of the drawer
    property int position: Qt.LeftEdge            // Which side of the screen the drawer is on, can be Qt.LeftEdge or Qt.RightEdge
    property Item visualParent: parent            // The item the drawer should cover, by default the parent of the Drawer

    // The fraction showing how open the drawer is
    readonly property real panelProgress:  (panel.x - _minimumX) / (_maximumX - _minimumX)

    function show() { open = true; }
    function hide() { open = false; }

    function toggle() {
        if (open) open = false;
        else open = true;
    }

    // Internal

    default property alias data: contentItem.data
    readonly property real expandedFraction: 0.78  // How big fraction of the screen realesatate that is covered by an open menu
    readonly property real _scaleFactor: visualParent.width / 320 // Note, this should really be application global
    readonly property int _pullThreshold: panel.width/2
    readonly property int _slideDuration: 260
    readonly property int _collapsedX: _rightEdge ? visualParent.width :  - panel.width
    readonly property int _expandedWidth: expandedFraction * visualParent.width
    readonly property int _expandedX: _rightEdge ? visualParent.width - width : 0
    readonly property bool _rightEdge: position === Qt.RightEdge
    readonly property int _minimumX:  _rightEdge ?  visualParent.width - panel.width : -panel.width
    readonly property int _maximumX: _rightEdge ? visualParent.width : 0
    readonly property int _openMarginSize: 20 * _scaleFactor

    property real _velocity: 0
    property real _oldMouseX: -1

    height: parent.height
    on_RightEdgeChanged: _setupAnchors()
    onOpenChanged: completeSlideDirection()
    width: _expandedWidth
    x: _collapsedX

    function _setupAnchors() {     // Note that we can't reliably apply anchors using bindings
        shadow.anchors.right = undefined;
        shadow.anchors.left = undefined;

        mouse.anchors.left = undefined;
        mouse.anchors.right = undefined;

        if (_rightEdge) {
            mouse.anchors.right = mouse.parent.right;
            shadow.anchors.right = panel.left;
        } else {
            mouse.anchors.left = mouse.parent.left;
            shadow.anchors.left = panel.right;
        }

        slideAnimation.enabled = false;
        panel.x =_rightEdge ? visualParent.width :  - panel.width;
        slideAnimation.enabled = true;
    }

    function completeSlideDirection() {
        if (open) {
            panel.x = _expandedX;
        } else {
            panel.x = _collapsedX;
            Qt.inputMethod.hide();
        }
    }

    function handleRelease() {
        var velocityThreshold = 5 * _scaleFactor;
        if ((_rightEdge && _velocity > velocityThreshold) ||
                (!_rightEdge && _velocity < -velocityThreshold)) {
            panel.open = false;
            completeSlideDirection()
        } else if ((_rightEdge && _velocity < -velocityThreshold) ||
                   (!_rightEdge && _velocity > velocityThreshold)) {
            panel.open = true;
            completeSlideDirection()
        } else if ((_rightEdge && panel.x < _expandedX + _pullThreshold) ||
                   (!_rightEdge && panel.x > _expandedX - _pullThreshold) ) {
            panel.open = true;
            panel.x = _expandedX;
        } else {
            panel.open = false;
            panel.x = _collapsedX;
        }
    }

    function handleClick(mouse) {
        if ((_rightEdge && mouse.x < panel.x ) || mouse.x > panel.width) {
            open = false;
        }
    }

    onPositionChanged: {
        if (! (position === Qt.RightEdge || position === Qt.LeftEdge ) ) {
            console.warn("SlidePanel: Unsupported position.")
        }
    }

    Behavior on x {
        id: slideAnimation
        enabled: !mouse.drag.active
        NumberAnimation {
            duration: _slideDuration
            easing.type: Easing.OutCubic
        }
    }

    Connections {
        target: visualParent
        onWidthChanged: {
            slideAnimation.enabled = false
            panel.completeSlideDirection()
            slideAnimation.enabled = true
        }
    }

    NumberAnimation on x {
        id: holdAnimation
        to: _collapsedX + (_openMarginSize * (_rightEdge ? -1 : 1))
        running : false
        easing.type: Easing.OutCubic
        duration: 200
    }

    MouseArea {
        id: mouse
        parent: visualParent

        y: visualParent.y
        width: open ? visualParent.width : _openMarginSize
        height: visualParent.height
        onPressed:  if (!open) holdAnimation.restart();
        onClicked: handleClick(mouse)
        drag.target: panel
        drag.minimumX: _minimumX
        drag.maximumX: _maximumX
        drag.axis: Qt.Horizontal
        drag.onActiveChanged: if (active) holdAnimation.stop()
        onReleased: handleRelease()
        onMouseXChanged: {
            _velocity = (mouse.x - _oldMouseX);
            _oldMouseX = mouse.x;
        }
    }

    Rectangle {
        id: backgroundDimmer
        parent: visualParent
        anchors.fill: parent
        opacity: 0.5 * Math.min(1, Math.abs(panel.x - _collapsedX) / visualParent.width/2)
        color: "black"
    }

    View {
        id: contentItem
        parent: visualParent
        width: panel.width
        height: panel.height
        x: panel.x
        y: panel.y
        z: 1
        elevation: panelProgress == 0 ? 0 : 3
        backgroundColor: "white"
    }
}
