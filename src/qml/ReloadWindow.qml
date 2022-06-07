import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import "debug.js" as Debug

Item {
    anchors.fill: parent

    SwipeView {
        id:view
        anchors.fill: parent
        currentIndex: 1

        Item {
            id: cameraUI

            Camera {
                id: camera
            }

            CaptureSession {
                id: captureSession
                camera: camera
                videoOutput: output
            }

            VideoOutput {
                id: output
                orientation: 90
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: settings.bottom
            }
            Rectangle {
                id: settings
                height: 100
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                RowLayout {
                    anchors.fill: parent
                    layoutDirection: Qt.LeftToRight
                    spacing: 10
                    /* TODO: put into seperate module */
                    Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        Image {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            sourceSize.width: 50
                            sourceSize.height: 50
                            source: {
                                "qrc:/icons/images2.svg"
                            }
                        }
                    }

                    Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        Image {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            sourceSize.width: 50
                            sourceSize.height: 50
                            source: {
                                "qrc:/icons/camera.svg"
                            }
                        }
                    }

                    Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        Image {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            sourceSize.width: 50
                            sourceSize.height: 50
                            source: {
                                "qrc:/icons/switch-camera.svg"
                            }
                        }
                    }
                }

            }
        }

        Item {
            id: secodPage
            objectName: qsTr("Sudoku")

            GridLayout {
                id: baseGrid
                rows: 3
                columns: 3
                columnSpacing: 10
                rowSpacing: 10

                width: parent.width
                height: width
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.topMargin: 25
                anchors.leftMargin: 25
                anchors.rightMargin: 25
                Repeater {
                    id: repeater
                    model: 9
                    Field {
                        width: 213
                        height: 213
                    }
                }
            }
        }

        Item {
            id: thirdPage
            objectName: qsTr("About")
            Text {
                text: "thirdPage"
            }
        }

    }

    PageIndicator {
        id: indicator
        count: view.count
        currentIndex: view.currentIndex

        anchors.bottom: view.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        delegate: Rectangle {
            implicitWidth: 40
            implicitHeight: 15
            radius: width/2
            color: "#33ff00"

            opacity: index === indicator.currentIndex ? 0.95 : pressed ? 0.7 : 0.45
            Behavior on opacity {
                OpacityAnimator {
                    duration: 200
                }
            }

            Text {
                anchors.centerIn: parent
                text: index
                Component.onCompleted: {
                    //Debug.listAll(model)
                }
            }
        }
        Component.onCompleted: captureSession.camera.start()

    }
}
