import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import "debug.js" as Debug
import "global"

Item {
    anchors.fill: parent

    SwipeView {
        id:view
        anchors.fill: parent
        currentIndex: 1

        Item {

            id: cameraUI

            Rectangle {
                anchors.fill: parent
                color: Globals.color.background
                Image {
                    anchors.fill: parent
                    source: "qrc:/images/game_background.png"
                    opacity: 0.18
                }

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
                    orientation: Qt.platform.os === "android" ? 0 : 90
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.bottom: settings.top
                    anchors.topMargin: 50
                    anchors.leftMargin: 50
                    anchors.rightMargin: 50
                }

                Rectangle {
                    id: settings
                    height: 100
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    color: "#00000000"
                    RowLayout {
                        anchors.fill: parent
                        layoutDirection: Qt.LeftToRight
                        spacing: 10
                        Rectangle {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                color: "#00000000"
                            Image {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                                sourceSize.width: 50
                                sourceSize.height: 50
                                source: {
                                    "qrc:/images/images2.svg"
                                }
                            }
                        }

                        Rectangle {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                color: "#00000000"
                            Image {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                                sourceSize.width: 50
                                sourceSize.height: 50
                                source: {
                                    "qrc:/images/camera.svg"
                                }
                            }
                        }

                        Rectangle {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                color: "#00000000"
                            Image {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                                sourceSize.width: 50
                                sourceSize.height: 50
                                source: {
                                    "qrc:/images/switch-camera.svg"
                                }
                            }
                        }
                    }

                }
            }

        }

        /* Sudoku Page */
        Item {
            id: secondPage
            objectName: qsTr("Sudoku")
            Board {}
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
