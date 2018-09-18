import QtQuick 2.9
import QtQuick.Controls 2.2
import QtBluetooth 5.2

Page {
    id: page
    property alias busy: busy
    property alias deviceLabel: deviceLabel

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
            text: qsTr("Connecting...")
            font.bold: true
            font.pixelSize: Qt.application.font.pixelSize * 2
            padding: 10
            anchors.centerIn: parent
        }
    }

    Text {
        id: deviceLabel
        anchors.centerIn: parent
        font.pixelSize: 24
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:4;anchors_x:12;anchors_y:6}
}
 ##^##*/
