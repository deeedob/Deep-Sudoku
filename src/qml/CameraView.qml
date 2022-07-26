import QtQuick
import QtMultimedia
import QtQuick.Layouts
import "global"

Rectangle {
    id: root
    color: Globals.color.background
    Background { }

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
