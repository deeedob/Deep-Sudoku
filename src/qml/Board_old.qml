import QtQuick
import QtQuick.Layouts
import "global"

Rectangle {
    id: board
    signal cellClicked(bool editable)
    property int currentIndex: -1

    color: Globals.color.background

    function setCurrentCellValue(value) {
        Logic.model.setCellValue(currrentIndex, value);
    }

    function clearCurrentCellValue() {
        Logic.model.clearCellValue(currentIndex);
    }

    function clearSelection() {
        Logic.model.clearSelection();
    }

    function getCellColor(status, conflict, index) {
        if(conflict)
            return "red";
        if(status === "selected")
            return Globals.color.selectedCell;
        else if(status === "same_row")
            return Globals.color.sameRowCell;
        else if(status === "same_column")
            return Globals.color.sameColumnCell;

        var row = Math.floor(index / 9);
        var col = index % 9;
        var quadIndex = Math.floor(row/3)*3 + Math.floor(col/3);
        return quadIndex % 2 === 0 ? Globals.color.basicCell : Globals.color.alternateCell
    }

    Image {
        anchors.fill: parent
        source: "qrc:/images/game_background.png"
        opacity: 0.18
    }
    Rectangle {
        anchors.fill: baseGrid
        color: Globals.color.gridSpace
    }

    GridLayout {
        id: baseGrid

        signal cellClicked(bool editable)

        rows: 9
        columns: 9
        width: parent.width
        height: parent.height
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 25
        Repeater {
            model: 81
            Rectangle {
                Layout.fillHeight: true
                Layout.fillWidth: true
                color: getCellColor(model.status, model.conflict, index)
                border.width: 1
                border.color: "black"
                Text {
                    anchors.centerIn: parent
                    text: model.value !== undefined && model.value !== -1 ? model.value : ""
                    font.pointSize: 20
                    font.weight: model.editable ? Font.DemiBold : Font.Black
                    color: model.editable ? Globals.color.editFont : Globals.color.nonEditFont
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        board.cellClicked(model.editable);
                        board.currentIndex = model.index;
                        model.status = "selected";
                    }
                }
            }
        }
    }

    NumberInput {
        id: numbersIn
    }

}
