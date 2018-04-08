import QtQuick 2.9
import QtQuick.Controls 2.2
import QtBluetooth 5.2

import se.dalakolonin.bluetooth 1.0

ApplicationWindow {
    visible: true
    width: 320
    height: 240
    title: qsTr("Tabs")

    Bluetooth {
        id: bluetooth
        hostMode: QBluetoothLocalDevice.HostConnectable
    }

    BluetoothDiscoveryModel {
        id: btModel
        running: false
        discoveryMode: BluetoothDiscoveryModel.DeviceDiscovery
        uuidFilter: "0000110a-0000-1000-8000-00805f9b34fb"
        onServiceDiscovered: {
            var str = "Found new service"
            str += ", name: " + service.deviceName
            str += ", address: " + service.deviceAddress
            str += ", registered: " + service.registered
            str += ", service: " + service.serviceName
            str += ", description: " + service.serviceDescription
            str += ", protocol: "
            switch (service.serviceProtocol) {
            case BluetoothService.RfcommProtocol:
                str += "RFCOMM"; break
            case BluetoothService.L2CapProtocol:
                str += "L2CAP"; break
            case BluetoothService.UnknownProtocol:
            default:
                str += "unknown"; break
            }
            str += ", UUID: " + service.serviceUuid
            console.log(str)
        }
        onDeviceDiscovered: console.log("New device: " + device)
        onErrorChanged: {
                switch (error) {
                case BluetoothDiscoveryModel.PoweredOffError:
                    console.log("Error: Bluetooth device not turned on"); break;
                case BluetoothDiscoveryModel.InputOutputError:
                    console.log("Error: Bluetooth I/O Error"); break;
                case BluetoothDiscoveryModel.InvalidBluetoothAdapterError:
                    console.log("Error: Invalid Bluetooth Adapter Error"); break;
                case BluetoothDiscoveryModel.NoError:
                    break;
                default:
                    console.log("Error: Unknown Error"); break;
                }
        }
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Page1Form {
            id: page1Form
            busy {
                SequentialAnimation on color {
                    id: busyThrobber
                    ColorAnimation {
                        easing.type: Easing.InOutSine
                        from: "#1c56f3"
                        to: "white"
                        duration: 1000
                    }
                    ColorAnimation {
                        easing.type: Easing.InOutSine
                        to: "#1c56f3"
                        from: "white"
                        duration: 1000
                    }
                    loops: Animation.Infinite
                }
            }
        }

        Page2Form {
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: qsTr("Pairing")
        }
        TabButton {
            text: qsTr("Page 2")
        }
    }
}
