#include "bluetooth.h"

#include <QDebug>
#include <QString>

Bluetooth::Bluetooth(QObject *parent) : QObject(parent)
{
    connect(&host, SIGNAL(deviceConnected(QBluetoothAddress)), this, SLOT(onDeviceConnected(QBluetoothAddress)));
    connect(&host, SIGNAL(deviceDisconnected(QBluetoothAddress)), this, SLOT(onDeviceDisconnected(QBluetoothAddress)));
    connect(&host, SIGNAL(error(QBluetoothLocalDevice::Error)), this, SLOT(onError(QBluetoothLocalDevice::Error)));
    connect(&host, SIGNAL(pairingDisplayConfirmation(QBluetoothAddress,QString)),
            this, SLOT(onPairingConfirm(QBluetoothAddress,QString)));
    connect(&host, SIGNAL(pairingDisplayPinCode(QBluetoothAddress,QString)),
            this, SLOT(onPairingPin(QBluetoothAddress,QString)));
    connect(&host, SIGNAL(pairingFinished(QBluetoothAddress,QBluetoothLocalDevice::Pairing)),
            this, SLOT(onPairingFinished(QBluetoothAddress,QBluetoothLocalDevice::Pairing)));
    connect(&host, SIGNAL(hostModeStateChanged(QBluetoothLocalDevice::HostMode)),
            this, SIGNAL(hostModeChanged(QBluetoothLocalDevice::HostMode)));
    QList<QBluetoothAddress> connectedDevices = host.connectedDevices();
    if  (!connectedDevices.isEmpty()) {
        m_ConnectedDevice = connectedDevices.first().toString();
    }
}

void Bluetooth::setHostMode(QBluetoothLocalDevice::HostMode mode)
{
    host.setHostMode(mode);
}

QBluetoothLocalDevice::HostMode Bluetooth::hostMode() const
{
    return host.hostMode();
}

QString Bluetooth::connectedDevice() const
{
    return m_ConnectedDevice;
}

void Bluetooth::onDeviceConnected(const QBluetoothAddress &address)
{
    qDebug() << "device connected" << address;
    host.setHostMode(QBluetoothLocalDevice::HostConnectable);
    host.requestPairing(address, QBluetoothLocalDevice::AuthorizedPaired);
    m_ConnectedDevice = address.toString();
    emit connectedDeviceChanged(m_ConnectedDevice);
}

void Bluetooth::onDeviceDisconnected(const QBluetoothAddress &address)
{
    qDebug() << "device disconnected" << address;
    m_ConnectedDevice.clear();
    emit connectedDeviceChanged(m_ConnectedDevice);
}

void Bluetooth::onError(QBluetoothLocalDevice::Error error)
{
    switch (error) {
    case QBluetoothLocalDevice::NoError:
        qDebug() << "device: no error";
        break;
    case QBluetoothLocalDevice::PairingError:
        qDebug() << "device: pairing error";
        break;
    case QBluetoothLocalDevice::UnknownError:
        qDebug() << "device: unknown error";
        break;
    }
}

void Bluetooth::onPairingConfirm(const QBluetoothAddress &address, const QString &pin)
{
    qDebug() << "pairing confirmation requested for " << address << "with pin code:" << pin;
    host.pairingConfirmation(true);
}

void Bluetooth::onPairingPin(const QBluetoothAddress &address, const QString &pin)
{
    qDebug() << "pin code for pairing with " << address << ":" << pin;
}

void Bluetooth::onPairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing pairing)
{
    switch (pairing) {
    case QBluetoothLocalDevice::Unpaired:
        qDebug() << "local device" << address << "unpaired";
        break;
    case QBluetoothLocalDevice::Paired:
    case QBluetoothLocalDevice::AuthorizedPaired:
        qDebug() << "local device" << address
                 << (pairing == QBluetoothLocalDevice::Paired ? "paired" : "paired (authorized)");
        break;
    }
}
