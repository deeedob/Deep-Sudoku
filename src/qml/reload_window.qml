import QtQuick
import QtQuick.Controls
import QtMultimedia
import "debug.js" as Debug

Item {
    anchors.fill: parent

    SwipeView {
        id:view
        anchors.fill: parent
        currentIndex: 0

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
                anchors.fill: parent
            }
            MediaDevices {
                id: mediaDevices
            }
        }


        Item {
            id: secodPage
            objectName: qsTr("Sudoku")
            Text {
                text: "secondPage"
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
