import QtQuick 2.9
import QtQuick.Controls 2.2
import QtBluetooth 5.2

Page {
    id: page
    property alias busy: busy
    property alias pairingButton: pairingButton
    property alias deviceLabel: deviceLabel
    width: 320
    height: 240

    header: Rectangle {
        id: busy

        width: top.width * 0.7
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: top.top
        height: scanText.height * 1.2
        radius: 5
        color: "#1c56f3"

        Text {
            id: scanText
            text: qsTr("Pairing")
            font.bold: true
            font.pixelSize: Qt.application.font.pixelSize * 2
            padding: 10
            anchors.centerIn: parent
        }
    }

    Button {
        id: pairingButton
        text: "Pair!"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

    Text {
        id: text1
        text: qsTr("Currently connected to:")
        font.pixelSize: 24
    }

    Text {
        id: deviceLabel
        anchors.top: text1.bottom
        anchors.topMargin: 6
        anchors.left: parent.left
        anchors.leftMargin: 10
        font.pixelSize: 24
    }
}

/*##^## Designer {
    D{i:4;anchors_x:12;anchors_y:6}D{i:5;anchors_x:38;anchors_y:41}
}
 ##^##*/
