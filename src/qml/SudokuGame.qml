import QtQuick
import QtQuick.Layouts
import App
import "global"
import "custom"

Rectangle {
    id: root
    Background { }
    color: Globals.color.background

    property int currentIndex: -1
    property int elapsedTicks: 0
    property int _inputFieldMargin: 150

    function setCurrentCellValue(value) {
        App.model.setCellValue(currentIndex, value);
    }

    function clearCurrentCellValue() {
        App.model.clearCellValue(currentIndex);
    }

    function clearSelection() {
        App.model.clearSelection();
    }

    function solveField() {
        var solvedVal = App.model.getCellSolved(currentIndex)
        setCurrentCellValue(solvedVal)
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

    function displayTime() {
        var seconds = elapsedTicks % 60;
        var minutes = parseInt(elapsedTicks/60);
        var hours = parseInt(minutes/60);

        return _toString(hours) + " : " + _toString(minutes) + " : " + _toString(seconds);
    }

    function _toString(val) {
        var valString = val + "";
        if(valString.length < 2)
            return "0" + valString;
        else
            return valString;
    }

    function getMaxGridSize() {
        if(Window.width > Window.height / 1.5)
            return (Window.height / 1.5) - 50
        return Window.width - 50
    }

    Rectangle {
        id: timeCont
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 75
        color: "#00000001"

        Text {
            id: time
            anchors.centerIn: parent
            font.pointSize: 25
            font.weight: Font.Black

            Timer {
                interval: 1000
                running: true
                repeat: true
                onTriggered: {
                    elapsedTicks++;
                    time.text = displayTime();
                }
            }
        }
    }

    GridLayout {
        id: grid
        rows: 9
        columns: 9
        width: getMaxGridSize()
        height: width
        anchors.top: timeCont.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        //anchors.left: parent.left
        //anchors.right: parent.right
        //anchors.margins: 25
        Repeater {
            model: App.model
            Rectangle {
                id: cell
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
                    hoverEnabled: true
                    anchors.fill: parent
                    onClicked: {
                        root.currentIndex = model.index;
                        model.status = "selected";
                    }
                }
            }
        }
    }

    NumberInput {
        id: numbersIn
        anchors.top: grid.bottom
        anchors.bottom: nav.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: _inputFieldMargin
        anchors.rightMargin: _inputFieldMargin
        onButtonClicked: (num) => root.setCurrentCellValue(num)
        onClearClicked: root.clearCurrentCellValue()
        onSolveClicked: root.solveField()
    }

    Rectangle {
        id: nav
        color: Globals.color.backgroundDark
        height: 75
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        Flow {
            spacing: 20
            function mar() {
                var elementsWidth = (children.length * children[0].width + spacing) / 2
                return (parent.width / 2) - elementsWidth
            }
            anchors {
                fill: parent
                leftMargin: mar()
                rightMargin: mar()
            }


            IconButton {
                id: btnReturn
                width: parent.height
                hoverColor: Globals.color.hoverIcon
                onClicked: stackView.replace("StartScreen.qml")
            }

            IconButton {
                id: btnSave
                width: parent.height
                hoverColor: Globals.color.hoverIcon
                source: "qrc:/save_game.svg"
            }
        }

    }

}
