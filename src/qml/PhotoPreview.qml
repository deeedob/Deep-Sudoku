import QtQuick
import QtMultimedia

Item {
    property alias source : preview.source
    signal closed

    Image {
        id: preview
        anchors.fill : parent
        fillMode: Image.PreserveAspectFit
        smooth: true
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            parent.closed();
        }
    }
}
