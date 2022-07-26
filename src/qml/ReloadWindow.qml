import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import "debug.js" as Debug
import "global"

Item {
    id: root
    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: StartScreen { }
    }
}
