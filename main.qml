import QtQuick 2.9
import QtQuick.Controls 2.2
import QtBluetooth 5.9

import se.dalakolonin.bluetooth 1.0

ApplicationWindow {
    visible: true
    width: 320
    height: 240
    title: qsTr("Tabs")
    property string connectedDeviceName: bluetooth.connectedDevice

    Bluetooth {
        id: bluetooth
        onConnectedDeviceChanged: {
            if (address) {
                swipeView.currentIndex = 1
            }
            else {
                swipeView.currentIndex = 0
            }
        }
    }

    function is_pairing() {
        if (bluetooth.hostMode === QBluetoothLocalDevice.HostDiscoverable
            || bluetooth.hostMode === QBluetoothLocalDevice.HostDiscoverableLimitedInquiry)
            return true
        else
            return false
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
                visible: is_pairing()
            }

            deviceLabel {
                text: connectedDeviceName ? connectedDeviceName : "<no device>"
            }
        }
        Page2Form {
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: qsTr("Connect")
        }
        TabButton {
            text: qsTr("Controls")
        }
    }
}
