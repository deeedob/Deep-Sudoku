pragma Singleton
import QtQuick 2.9

QtObject {
    property QtObject color: QtObject {
        property color background: "#ffa837"
        property color backgroundDark: "#de741c"
        property color navBar: "#b85b56"
        property color hoverIcon: "#E7C7C5"

        property color gridSpace: "#4F2D02"
        property color basicCell: "#C7B3D1"
        property color alternateCell: "#AB8CBA"
        property color selectedCell: "#67cbb6"
        property color sameRowCell: "#8dd6c9"
        property color sameColumnCell: "#b7e3dc"
        property color editFont: "#125147"
        property color nonEditFont: "#0d3832"

    }
}
