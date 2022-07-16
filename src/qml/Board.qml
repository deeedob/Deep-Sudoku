import QtQuick
import QtQuick.Layouts
import "global"

Rectangle {
    id: board
    signal cellClicked(bool editable)
    property int currentIndex: -1

    anchors.fill: parent
    color: Globals.color.background

    Image {
        anchors.fill: parent
        source: "qrc:/images/game_background.png"
        opacity: 0.18
    }

    function setCurrentCellValue(value) {
        Logic.model.setCellValue(currentIndex, value);
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

    GridLayout {
        id: baseGrid

        signal cellClicked(bool editable)

        rows: 9
        columns: 9
        width: parent.width
        height: width
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 25
        Repeater {
            model: Logic.model
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
        anchors.top: baseGrid.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        onButtonClicked: {
            //board.setCurrentCellValue(value)
            Logic.model.value = value;
            console.log(value);
        }

        onClearClicked: {
            boar.clearCurrentCellValue()
        }

    }

}
