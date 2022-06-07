import QtQuick.Window 2.3
import QtQuick 2.9

Window {
    id: root
    width: 1260
    height: 980
    visible: true
    property string loaded: "ReloadWindow.qml"
    Shortcut{
        sequence: "Ctrl+R";
        onActivated: _loader.reload()
    }
    Loader {
            id: _loader
            function reload() {
                console.log("--- refresh ---");
                source = "";
                $QmlEngine.clearCache();
                source = "ReloadWindow.qml";
            }

            anchors.fill: parent
            source: "ReloadWindow.qml"
        }

}
