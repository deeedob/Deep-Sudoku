pragma Singleton
import QtQuick 2.9

QtObject {
    property QtObject color: QtObject {
        property color background: "#ffd782"
        property color backgroundDark: "#ffa500"
        property color navBar: "#b85b56"
        property color hoverIcon: "#E7C7C5"

        property color gridSpace: "#4F2D02"
        property color basicCell: "#C7B3D1"
        property color alternateCell: "#AB8CBA"
        property color selectedCell: "#67cbb6"
        property color sameRowCell: "#8dd6c9"
        property color sameColumnCell: "#b7e3dc"
        property color editFont: "#212322"
        property color nonEditFont: "#363D3C"

    }
}
