import QtQuick
import QtQuick.Layouts

Rectangle {

    anchors.top: baseGrid.bottom
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.margins: 25
    anchors.bottomMargin: 30
    color: "blue"
    GridLayout {
        id: root
        rows: 4
        columns: 3
        columnSpacing: 10
        rowSpacing: 10
        //width: parent.width
        //height: parent.height / 5
        anchors.fill: parent
        Layout.alignment: Qt.AlignBottom


        /* top row */
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            color: "red"
            Text {
                text: qsTr("7")
                anchors.centerIn: parent
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            color: "green"
            Text {
                text: qsTr("8")
                anchors.centerIn: parent
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            color: "red"
            Text {
                text: qsTr("9")
                anchors.centerIn: parent
            }
        }

        /* second row */
        Rectangle {
            Layout.fillWidth: true
            color: "red"
            Layout.preferredHeight: 40
            Text {
                text: qsTr("4")
                anchors.centerIn: parent
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            color: "green"
            Text {
                text: qsTr("5")
                anchors.centerIn: parent
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            color: "red"
            Text {
                text: qsTr("6")
                anchors.centerIn: parent
            }
        }

        /* third row */
        Rectangle {
            Layout.fillWidth: true
            color: "red"
            Layout.preferredHeight: 40
            Text {
                text: qsTr("1")
                anchors.centerIn: parent
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            color: "green"
            Text {
                text: qsTr("2")
                anchors.centerIn: parent
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            color: "red"
            Text {
                text: qsTr("3")
                anchors.centerIn: parent
            }
        }

        /* fourth row */
        Rectangle {
            Layout.fillWidth: true
            color: "red"
            Layout.preferredHeight: 40
            Text {
                text: qsTr("SOLVE")
                anchors.centerIn: parent
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            color: "green"
            Text {
                text: qsTr("0")
                anchors.centerIn: parent
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            color: "red"
            Text {
                text: qsTr("DELETE")
                anchors.centerIn: parent
            }
        }

    }
}
