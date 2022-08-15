import QtQuick
import QtQuick.Controls

Button {
    id: button

    property color col_basic: "#F9C74F"
    property color col_hover: "#ffa500"
    property color col_click: "#9D7006"
    property int   base_font: 14

    text: "DS Button"
    font.bold: true
    font.pixelSize: (mouseArea.containsMouse) ? base_font*1.3 : base_font
    function getColor(){
        if(button.down)
            return col_click
        else if(mouseArea.containsMouse)
            return col_hover
        else
            return col_basic
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 40
        color: getColor()
        border.color: "#624604"
        border.width: 2
        radius: 7
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
        }
    }
}
