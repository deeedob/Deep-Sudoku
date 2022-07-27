import QtQuick

Rectangle {
    id: root
    property alias source: img.source
    property color hoverColor: "red"
    property int hoverRadius: 10
    signal clicked()

    width: 50
    height: width
    radius: 7
    color: mouseArea.containsMouse ? hoverColor : "#00000000"
    Image {
        id: img
        anchors.centerIn: parent
        sourceSize {
            width: parent.width - hoverRadius
            height: parent.height - hoverRadius
        }
        source: "qrc:/prevfile.svg"
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: root.clicked()
    }
}