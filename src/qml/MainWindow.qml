import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtMultimedia

Window {
    id: root
    width: 450
    height: 800
    visible: true

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: StartScreen { }
    }

}