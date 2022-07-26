import QtQuick

Rectangle {
    id: root
    property alias source: img.source
    property color hoverColor: "#ff0000ff"
    property int hoverRadius: 10
    signal clicked(var mouse)

    width: 50
    height: 50
    radius: 7
    color: mouseArea.containsMouse ? hoverCol : "#00000000"
    Image {
        id: img
        anchors.centerIn: parent
        sourceSize {
            width: parent.width - hoverRadius
            height: parent.height - hoverRadius
        }
        source: "qrc:/images/prevfile.svg"
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: root.clicked(mouse)
    }
}
