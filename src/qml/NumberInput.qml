import QtQuick
import QtQuick.Layouts
import "custom"

Rectangle {
    id: root
    property int marginVal: 25
    property int bottomMarginVal: 30
    anchors.margins: marginVal
    anchors.bottomMargin: bottomMarginVal
    color: "#00000000"

    signal buttonClicked(int value)
    signal clearClicked()
    signal solveClicked()

    GridLayout {
        id: grid
        anchors.fill: parent
        columns: 6
        rows: 4

        columnSpacing: 10
        rowSpacing: 10

        property double colMulti : (grid.width - marginVal) / grid.columns
        property double rowMulti : (grid.height - bottomMarginVal) / grid.rows
        function prefWidth(item){
            var val = colMulti * item.Layout.columnSpan
            return val
        }
        function prefHeight(item){
            var val = rowMulti * item.Layout.rowSpan
            if( val <= 25)
                return 25;
            return val
        }


        /* top row */
        DSButton {
            Layout.columnSpan: 2
            Layout.preferredWidth: grid.prefWidth(this)
            Layout.preferredHeight: grid.prefHeight(this)
            text: qsTr("7")
            onClicked: {
                root.buttonClicked(7)
            }
        }

        DSButton {
            Layout.columnSpan: 2
            Layout.preferredWidth: grid.prefWidth(this)
            Layout.preferredHeight: grid.prefHeight(this)
            text: qsTr("8")
            onClicked: {
                root.buttonClicked(8)
            }
        }

        DSButton {
            Layout.columnSpan: 2
            Layout.preferredWidth: grid.prefWidth(this)
            Layout.preferredHeight: grid.prefHeight(this)
            text: qsTr("9")
            onClicked: {
                root.buttonClicked(9)
            }
        }

        /* second row */
        DSButton {
            Layout.columnSpan: 2
            Layout.preferredWidth: grid.prefWidth(this)
            Layout.preferredHeight: grid.prefHeight(this)
            text: qsTr("4")
            onClicked: {
                root.buttonClicked(4)
            }
        }

        DSButton {
            Layout.columnSpan: 2
            Layout.preferredWidth: grid.prefWidth(this)
            Layout.preferredHeight: grid.prefHeight(this)
            text: qsTr("5")
            onClicked: {
                root.buttonClicked(5)
            }
        }

        DSButton {
            Layout.columnSpan: 2
            Layout.preferredWidth: grid.prefWidth(this)
            Layout.preferredHeight: grid.prefHeight(this)
            text: qsTr("6")
            onClicked: {
                root.buttonClicked(6)
            }
        }

        /* third row */
        DSButton {
            Layout.columnSpan: 2
            Layout.preferredWidth: grid.prefWidth(this)
            Layout.preferredHeight: grid.prefHeight(this)
            text: qsTr("1")
            onClicked: {
                root.buttonClicked(1)
            }
        }

        DSButton {
            Layout.columnSpan: 2
            Layout.preferredWidth: grid.prefWidth(this)
            Layout.preferredHeight: grid.prefHeight(this)
            text: qsTr("2")
            onClicked: {
                root.buttonClicked(2)
            }
        }

        DSButton {
            Layout.columnSpan: 2
            Layout.preferredWidth: grid.prefWidth(this)
            Layout.preferredHeight: grid.prefHeight(this)
            text: qsTr("3")
            onClicked: {
                root.buttonClicked(3)
            }
        }

        /* fourth row */
        DSButton {
            Layout.columnSpan: 3
            Layout.preferredWidth: grid.prefWidth(this) + marginVal/4
            Layout.preferredHeight: grid.prefHeight(this)
            text: qsTr("SOLVE")
            onClicked: {
                root.solveClicked()
            }
        }

        DSButton {
            Layout.columnSpan: 3
            Layout.preferredWidth: grid.prefWidth(this) + marginVal/4
            Layout.preferredHeight: grid.prefHeight(this)
            text: qsTr("CLEAR")
            onClicked: {
                root.clearClicked()
            }
        }

    }
}
