import QtQuick
import QtMultimedia
import QtQuick.Layouts
import QtQuick.Dialogs
import App
import "global"
import "custom"

Rectangle {
    id: root
    color: Globals.color.background
    Background { }

    function setEditState() {
        output.visible = false
        captureSettings.visible = false
        editSetting.visible = true
        photoPreview.visible = true
    }

    function unsetEditState() {
        output.visible = true
        captureSettings.visible = true
        editSetting.visible = false
        photoPreview.visible = false
        photoPreview.source = imageCapture.preview
    }

    Rectangle {
        id: cameraView
        anchors.top: parent.top
        anchors.bottom: captureSettings.top
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

            //Component.onCompleted: mediaHelper.session = captureSession
        }

        Rectangle {
            id: previewContainer
            function getMargin() {
                if (Window.width < 700)
                    return 0;
                return 50
            }
            color: "transparent"
            /*
            anchors.fill: parent
            anchors.leftMargin: getMargin()
            anchors.rightMargin: getMargin()
            anchors.topMargin: getMargin()
            VideoOutput {
                id: output
                anchors.fill: parent
                fillMode: VideoOutput.PreserveAspectCrop
                focus: visible
                visible: true
            } */
            /*
            Image {
                id: photoPreview
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                clip: true
                source: imageCapture.preview
                visible: false
            } */
        }

    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        nameFilters: ["Image files (*.png *.jpeg *.jpg)"]
        fileMode: FileDialog.OpenFile
        options: FileDialog.ReadOnly
        onAccepted: {
            photoPreview.source = fileDialog.selectedFile
            setEditState()
        }
    }

    function getSpacing() {
        return Window.width / 7
    }

    Rectangle {
        id: captureSettings
        height: 100
        anchors.bottom: nav.top
        anchors.left: parent.left
        anchors.right: parent.right
        property bool spaceEvenly: false
        color: "#00000000"
        Flow {
            spacing: getSpacing()
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
                onClicked: {
                    fileDialog.open()
                }
            }

            IconButton {
                id: btnTakePicture
                width: parent.height
                hoverRadius: 25
                hoverColor: Globals.color.hoverIcon
                onClicked: {
                    //imageCapture.capture()
                    setEditState()
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
        id: editSetting
        height: 100
        anchors.bottom: nav.top
        anchors.left: parent.left
        anchors.right: parent.right
        color: "#00000000"
        visible: false
        Flow {
            spacing: getSpacing()
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
                id: btnAcceptImage
                width: parent.height
                hoverRadius: 25
                hoverColor: Globals.color.hoverIcon
                source: "qrc:/accept.svg"
                onClicked: {
                    //if(App.solveGame(photoPreview))
                    if(App.solveGame(photoPreview.source))
                        stackView.replace("qrc:/SudokuGame.qml")
                    else
                        console.log("not valid");
                }
            }

            IconButton {
                id: btnCancelImage
                width: parent.height
                hoverRadius: 25
                hoverColor: Globals.color.hoverIcon
                onClicked: {
                    unsetEditState()
                }
                source: "qrc:/cancel.svg"
            }

            IconButton {
                id: btnRotateImage
                width: parent.height
                hoverRadius: 25
                hoverColor: Globals.color.hoverIcon
                source: "qrc:/switch-camera.svg"
                onClicked: {
                    photoPreview.rotation += 90
                }
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
                onClicked: stackView.replace("qrc:/StartScreen.qml")
            }

            IconButton {
                id: btnSave
                width: parent.height
                hoverColor: Globals.color.hoverIcon
                source: "qrc:/save_game.svg"
                onClicked: stackView.replace("qrc:/StartScreen.qml")
            }
        }

    }


}
