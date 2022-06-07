import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls

Item {
    Layout.fillHeight: true
    Layout.fillWidth: true

    GridLayout{
        id : subGrid
        rows : 3
        columns : 3
        rowSpacing: 2
        columnSpacing: 2
        anchors.fill: parent
        Repeater{
            id : repeater1
            model : 9

            Rectangle{
                id: field
                Layout.fillHeight: true
                Layout.fillWidth: true
                color : mouseArea.containsMouse ? "red" : "white"
                property alias input : input
                border.width : 2
                border.color : "#000000"

                TextInput{
                    id : input
                    text: index +1
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onPressed: {
                        field.color = "yellow"
                    }
                    onReleased: {
                        field.color = "white"
                    }
                }
            }
        }
    }
}
