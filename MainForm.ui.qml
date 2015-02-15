import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item {

    property alias button3: button3
    property alias button2: button2
    property alias button1: button1
    width: 200
    height: 500

    ColumnLayout {
        id: columnLayout1
        anchors.rightMargin: 0
        transformOrigin: Item.Center
        z: 0
        spacing: 5
        anchors.fill: parent

        ListModel {
           id: libraryModel
           ListElement{ title: "A Masterpiece" ; author: "Gabriel" }
           ListElement{ title: "Brilliance"    ; author: "Jens" }
           ListElement{ title: "Outstanding"   ; author: "Frederik" }
        }


        TableView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            transformOrigin: Item.Center
            width: 500

           TableViewColumn{ role: "title"  ; title: "Title" ; width: 100 }
           TableViewColumn{ role: "author" ; title: "Author" ; width: 200; delegate: ComboBox {
                   id: checkBox
                   model: [ "Banana", "Apple", "Coconut" ]
               } }
           model: libraryModel
        }

        RowLayout {
            transformOrigin: Item.Center
            z: 0

            Button {
                id: button1
                text: qsTr("Press Me 1")
            }

            Button {
                id: button2
                text: qsTr("Press Me 2")
            }

            Button {
                id: button3
                text: qsTr("Press Me 3")
            }
        }
    }
}
