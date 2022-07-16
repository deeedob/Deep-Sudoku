pragma Singleton
import QtQuick 2.9

//https://coolors.co/f94144-f3722c-f8961e-f9844a-f9c74f-90be6d-43aa8b-4d908e-577590-277da1
QtObject {
    property QtObject color: QtObject {
        property color background: "#F8961E"
        property color gridSpace: "#4F2D02"

        property color basicCell: "#f3d5b0"
        property color alternateCell: "#f3e1c9"
        property color selectedCell: "#67cbb6"
        property color sameRowCell: "#8dd6c9"
        property color sameColumnCell: "#b7e3dc"
        property color editFont: "#125147"
        property color nonEditFont: "#0d3832"

    }
}
