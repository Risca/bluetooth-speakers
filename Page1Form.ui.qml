import QtQuick 2.9
import QtQuick.Controls 2.2
import QtBluetooth 5.2

Page {
    property alias busy: busy

    header: Rectangle {
        id: busy

        width: top.width * 0.7
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: top.top
        height: scanText.height * 1.2
        radius: 5
        color: "#1c56f3"
        visible: btModel.running

        Text {
            id: scanText
            text: qsTr("Scanning")
            font.bold: true
            font.pixelSize: Qt.application.font.pixelSize * 2
            padding: 10
            anchors.centerIn: parent
        }
    }

    ListView {
        id: listView
        anchors.fill: parent
        model: btModel
        delegate: BtListDelegate {
            id: btDelegate
            model: btModel
        }

        Button {
            id: scan
            text: "Start scan"
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            onClicked: btModel.running = true
            enabled: !btModel.running
        }
    }
}
