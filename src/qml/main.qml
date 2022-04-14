import QtQuick
import QtMultimedia
//import "com.dev.prod"
import QtQuick.Window 2.3

Window {
    id: root
    width: 800
    height: 600
    visible: true

Rectangle {
    id: cameraUI
    anchors.fill: parent
    visible: true
    state: "PhotoCapture"
    property string platformScreen: ""
    property int buttonsPanelLandscapeWidth: 328
    property int buttonsPanelPortraitHeight: 180
    color: "#333333"

    onWidthChanged: {
        setState()
    }

    function setState() {
        if(Qt.platform.os === "android" || Qt.platform.os === "ios") {
            stillControls.state = "Other";
        }
    }

    states: [
        State {
            name: "PhotoCapture"
            StateChangeScript {
                script: {
                    camera.start()
                }
            }
        },
        State {
            name: "PhotoPreview"
        }
    ]

    CaptureSession {
        id: captureSession
        camera: Camera {
            id: camera
        }
        imageCapture: ImageCapture {
            id: imageCapture
        }

        recorder: MediaRecorder {
            id: recorder
//             resolution: "640x480"
//             frameRate: 30
        }
        videoOutput: viewfinder
    }

    //Producer {
    //    id: producer
    //    videoSink: viewfinder.videoSink
    //}

    PhotoPreview {
        id : photoPreview
        anchors.fill : parent
        onClosed: cameraUI.state = "PhotoCapture"
        visible: (cameraUI.state === "PhotoPreview")
        focus: visible
        source: imageCapture.preview
    }


    VideoOutput {
        id: viewfinder
        visible: ((cameraUI.state === "PhotoCapture") || (cameraUI.state === "VideoCapture"))
        anchors.fill: parent

        //        autoOrientation: true
    }

    Rectangle {
            color: "#13FFFFFF";
            width: parent.width-50;
            height: parent.width-50;
            anchors.centerIn: parent;
    }

}
}
