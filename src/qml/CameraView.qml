import QtQuick
import QtMultimedia
import QtQuick.Layouts
import "global"
import "custom"

Rectangle {
    id: root
    color: Globals.color.background
    Background { }

    Rectangle {
        id: cameraView
        anchors.top: parent.top
        anchors.bottom: settings.top
        anchors.left: parent.left
        anchors.right: parent.right
        color: "transparent"

        CaptureSession {
        id: captureSession

            camera: Camera {
                id: camera
            }

            imageCapture: ImageCapture {
                id: imageCapture
            }

            videoOutput: output

            Component.onCompleted: mediaHelper.session = captureSession

        }


        VideoOutput {
            id: output
            //orientation: Qt.platform.os === "android" ? 0 : 90
            anchors.fill: parent
            anchors.topMargin: 50
            anchors.leftMargin: 50
            anchors.rightMargin: 50
            focus: visible
        }

        Image {
            id: photoPreview
            anchors.fill: parent
            anchors.centerIn: parent
            source: imageCapture.preview
            fillMode: Image.PreserveAspectFit
        }

    }



    Rectangle {
        id: settings
        height: 100
        anchors.bottom: nav.top
        anchors.left: parent.left
        anchors.right: parent.right
        property bool spaceEvenly: false
        color: "#00000000"
        Flow {
            spacing: 200
            function mar() {
                if(children.length <= 1)
                    return (parent.width / 2) - (children[0].width) / 2
                var elementsWidth = ((children.length * children[0].width) + ((children.length -1 ) * spacing)) / 2
                return (parent.width / 2) - elementsWidth
            }
            anchors {
                fill: parent
                leftMargin: mar()
                rightMargin: mar()
            }

            IconButton {
                id: btnLoadImg
                width: parent.height
                hoverRadius: 25
                hoverColor: Globals.color.hoverIcon
                source: "qrc:/images2.svg"
            }

            IconButton {
                id: btnTakePicture
                width: parent.height
                hoverRadius: 25
                hoverColor: Globals.color.hoverIcon
                onClicked: {
                    imageCapture.capture()
                }
                source: "qrc:/camera.svg"
            }

            IconButton {
                id: btnFlipCamera
                width: parent.height
                hoverRadius: 25
                hoverColor: Globals.color.hoverIcon
                source: "qrc:/switch-camera.svg"
            }
        }
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
                onClicked: stackView.replace("StartScreen.qml")
            }
        }

    }

}
