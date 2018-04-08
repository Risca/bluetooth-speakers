import QtQuick 2.0
import QtBluetooth 5.2

Rectangle {
    width: parent.width
    height: column.height + 10

    property BluetoothDiscoveryModel model
    property bool expended: false;
    clip: true

    Column {
        id: column
        anchors.left: parent.left
        anchors.leftMargin: 5
        Text {
            id: bttext
            text: deviceName ? deviceName : name
            font.family: "FreeSerif"
            font.pointSize: 16
        }

        Text {
            id: details
            function get_details(s) {
                var str = "Address: "
                if (model.discoveryMode === BluetoothDiscoveryModel.DeviceDiscovery) {
                    //We are doing a device discovery
                    str += remoteAddress;
                    return str;
                } else {
                    str += s.deviceAddress;
                    if (s.serviceName) { str += "<br>Service: " + s.serviceName; }
                    str += "<br>Registered: " + s.registered
                    return str;
                }
            }
            visible: opacity !== 0
            opacity: expended ? 1 : 0.0
            text: get_details(service)
            font.family: "FreeSerif"
            font.pointSize: 14
            Behavior on opacity {
                NumberAnimation { duration: 200}
            }
        }
    }
    Behavior on height { NumberAnimation { duration: 200} }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (model.discoveryMode === BluetoothDiscoveryModel.DeviceDiscovery) {
                bluetooth.connectToAudioDevice(remoteAddress)
            }
            else {
                bluetooth.connectToAudioDevice(service.deviceAddress)
            }
            expended = !expended
        }
    }
}
