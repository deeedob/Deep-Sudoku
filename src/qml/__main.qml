import QtQuick 2.9
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtMultimedia 5.8
import QtQuick.Controls 2.1
import Qt.labs.settings 1.0
import "com.dev.prod"

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 800
    height: 600
    title: qsTr("Quick-Camera-CV")

    Settings
    {
        property alias cameraCombo: cameraCombo.currentIndex
        property alias algorithmCombo: algorithmCombo.currentIndex
    }

    /*
        This dialog is used to display required messages. It has only one "OK" button to close the dialog.
    */
    Dialog
    {
        property alias text: msgDlgLabel.text
        id: messageDialog
        modal: true
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        standardButtons: Dialog.Ok

        Label
        {
            id: msgDlgLabel
            font.bold: true
            wrapMode: Text.Wrap
            width: parent.width
            onLinkActivated:
            {
                if(link == "preview")
                {
                    tabBar.currentIndex = 2
                }

                messageDialog.close()
            }
        }
    }

    CvImageProcessor
    {
        id: imageProcessor

        onErrorOccured:
        {
            messageDialog.title = qsTr("Error")
            messageDialog.text = msg
            messageDialog.open()
        }

        onImageProcessed:
        {
            imageViewer.source = "file:///" + fname
            messageDialog.title = qsTr("Saved to Pictures")
            messageDialog.text = "File : " + fname + "<br><br>Switch to <a href=\"preview\">Preview View</a> mode to see it."
            messageDialog.open()
        }
    }

    SwipeView
    {
        id: swipeView
        anchors.fill: parent

        onCurrentIndexChanged:
        {
            tabBar.setCurrentIndex(swipeView.currentIndex)
        }

        /*
            First page is also help page
        */
        Page
        {
            ColumnLayout
            {
                anchors.fill: parent

                GroupBox
                {
                    title: qsTr("Help & Introduction")
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.margins: 10
                    ColumnLayout
                    {
                        anchors.fill: parent
                        Flickable
                        {
                            id: flickable1
                            clip: true
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            contentWidth: parent.width
                            contentHeight: helpText.height
                            ScrollBar.vertical: ScrollBar
                            {
                                active: true
                            }
                            Text
                            {
                                id: helpText
                                wrapMode: Text.Wrap
                                anchors.left: parent.left
                                anchors.right: parent.right
                                text: "<b>Quick-Camera-CV</b><br>" +
                                      "<br>" +
                                      "<br>" +
                                      "This application helps you take a picture, apply Computer Vision algorithms and save it." +
                                      "<br>" +
                                      "It is aimed to help C++/Qt/QML/OpenCV developers by showing an example project that uses all of those technologies." +
                                      "<br>" +
                                      "All source codes and required files for other platforms can be found at <a href=\"http://amin-ahmadi.com/quick-camera-cv/\">http://amin-ahmadi.com/quick-camera-cv</a>" +
                                      "<br>" +
                                      "<br>" +
                                      "<b>How to Use</b><br>" +
                                      "Use <a href=\"settings\">Settings View</a> to set the Camera and CV Algorithm." +
                                      "<br>" +
                                      "Go to <a href=\"camera\">Camera View</a> and press anywhere on the video to take a picture." +
                                      "<br>" +
                                      "Output processed image will be saved to default pictures folder and also displayed in <a href=\"preview\">Preview View</a>." +
                                      "<br>" +
                                      "<br>" +
                                      "For all your questions please contact me using the following link:" +
                                      "<br>" +
                                      "<a href=\"http://amin-ahmadi.com/contact-me/\">http://amin-ahmadi.com/contact-me</a>" +
                                      "<br>" +
                                      "<br>" +
                                      "<b>Credits & Acknowledgements</b>" +
                                      "This application uses <a href=\"http://qt.io/\">Qt Framework</a> for its front-end and back-end programming." +
                                      "<br>" +
                                      "This application uses <a href=\"http://opencv.org/\">OpenCV</a> for Computer Vision algorithms." +
                                      "<br>" +
                                      "All icons used in this application are downloaded from <a href=\"http://flaticon.com/\">Flat Icon</a>."

                                onLinkActivated:
                                {
                                    if(link === "camera")
                                    {
                                        tabBar.currentIndex = 1
                                    }
                                    else if(link === "preview")
                                    {
                                        tabBar.currentIndex = 2
                                    }
                                    else if(link === "settings")
                                    {
                                        tabBar.currentIndex = 3
                                    }
                                    else
                                    {
                                        Qt.openUrlExternally(link)
                                    }
                                }
                            }
                        }
                    }
                }
            }

        }

        /*
            Camera output is shown in this page
        */
        Page
        {
            ColumnLayout
            {
                anchors.fill: parent
                GroupBox
                {
                    title: qsTr("Preview")
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.margins: 10

                    VideoOutput
                    {
                        id: videoOutput
                        anchors.fill: parent
                        Camera
                        {
                            id: camera

                            Component.onCompleted:
                            {
                                imageProcessor.setCamera(camera)
                            }
                        }

                        source: camera
                        fillMode: Qt.KeepAspectRatio
                        autoOrientation: true

                        MouseArea
                        {
                            anchors.fill: parent
                            onReleased:
                            {
                                messageDialog.title = qsTr("Please wait ...")
                                messageDialog.text = qsTr("Depending on the chosen algorithm this might take some time ...")
                                messageDialog.open()
                                imageProcessor.capture()
                            }
                        }
                    }
                }
            }
        }

        /*
            Processed image is displayed here
        */
        Page
        {
            ColumnLayout
            {
                anchors.fill: parent

                GroupBox
                {
                    title: qsTr("Processed Image Preview")
                    Layout.margins: 10
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Image
                    {
                        id: imageViewer
                        fillMode: Qt.KeepAspectRatio
                        anchors.fill: parent
                    }
                }
            }
        }

        Page
        {
            ColumnLayout
            {
                anchors.fill: parent

                GroupBox
                {
                    title: qsTr("Settings")
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.margins: 10

                    ColumnLayout
                    {
                        anchors.fill: parent
                        RowLayout
                        {
                            Layout.margins: 10
                            Label
                            {
                                text: qsTr("Camera")
                                font.bold: true
                            }

                            ComboBox
                            {
                                id: cameraCombo
                                Layout.fillWidth: true
                                model: QtMultimedia.availableCameras
                                textRole: "displayName"
                                delegate: ItemDelegate
                                {
                                    text: modelData.displayName
                                }
                                onCurrentIndexChanged:
                                {
                                    camera.stop()
                                    camera.deviceId = model[currentIndex].deviceId
                                    camera.start()
                                }
                            }
                        }

                        RowLayout
                        {
                            Layout.margins: 10
                            Label
                            {
                                text: qsTr("Algorithm")
                                font.bold: true
                            }

                            ComboBox
                            {
                                id: algorithmCombo
                                Layout.fillWidth: true
                                model: ListModel
                                {
                                    ListElement { key: "None"; value:  "none"}
                                    ListElement { key: "Black and White"; value:  "gray"}
                                    ListElement { key: "DFT"; value: "dft" }
                                    ListElement { key: "Canny"; value: "canny" }
                                }
                                textRole: "key"
                                delegate: ItemDelegate
                                {
                                    text: model.key
                                }

                                onCurrentIndexChanged:
                                {
                                    imageProcessor.setAlgorithm(model.get(currentIndex).value)
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    footer: TabBar
    {
        id: tabBar

        padding: 10

        onCurrentIndexChanged:
        {
            swipeView.setCurrentIndex(tabBar.currentIndex)
        }

        TabButton
        {
            text: qsTr("Info")
            height: parent.height
            contentItem: Image
            {
                anchors.fill: parent
                fillMode: Qt.KeepAspectRatio
                source: "qrc:/images/png/help.png"
            }
        }

        TabButton
        {
            text: qsTr("Camera")
            height: parent.height
            contentItem: Image
            {
                anchors.fill: parent
                fillMode: Qt.KeepAspectRatio
                source: "qrc:/images/png/camera.png"
            }
        }

        TabButton
        {
            text: qsTr("Processed")
            height: parent.height
            contentItem: Image
            {
                anchors.fill: parent
                fillMode: Qt.KeepAspectRatio
                source: "qrc:/images/png/landscape.png"
            }
        }

        TabButton
        {
            text: qsTr("Settings")
            height: parent.height
            contentItem: Image
            {
                anchors.fill: parent
                fillMode: Qt.KeepAspectRatio
                source: "qrc:/images/png/cogwheel.png"
            }
        }
    }
}