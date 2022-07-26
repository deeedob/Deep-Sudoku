import QtQuick
import "global"

Rectangle {
    id: root
    anchors.fill: parent
    color: Globals.color.background
    property real btn_width: width/1.5
    property real btn_height: 60
    Background { }
    Column {
        anchors.centerIn: parent
        spacing: 10
        DSButton {
            text: "Capture"
            width: btn_width
            height: btn_height
            onClicked: {
                stackView.replace("CameraView.qml")
            }
        }
        DSButton {
            text: "Game"
            width: btn_width
            height: btn_height
            onClicked: stackView.replace("SudokuGame.qml")
        }
        DSButton {
            text: "History"
            width: btn_width
            height: btn_height
        }
    }
}
