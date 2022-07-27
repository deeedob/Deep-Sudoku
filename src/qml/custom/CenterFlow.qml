import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Flow {
    property int rowCount: parent.width / (elements.itemAt(0).width + spacing)
    property int rowWidth: rowCount * elements.itemAt(0).width + (rowCount - 1) * spacing
    property int mar: (parent.width - rowWidth) / 2

    anchors {
        fill: parent
        leftMargin: mar
        rightMargin: mar
    }

    spacing: 6
    Repeater {
        id: elements
        model: 5
        Rectangle {
            color: "#aa6666"
            width: 100; height: 100
        }
    }
}
