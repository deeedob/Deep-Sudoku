import QtQuick
import "global"
import "custom"

Rectangle {
    id: root
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
            onPressed: {
                stackView.replace("qrc:/CameraView.qml")
            }
        }
        DSButton {
            text: "Game"
            width: btn_width
            height: btn_height
            onPressed: stackView.replace("qrc:/SudokuGame.qml")
        }
        DSButton {
            text: "History"
            width: btn_width
            height: btn_height
        }
    }
}
