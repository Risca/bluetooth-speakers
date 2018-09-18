#include "bluetooth.h"

#include <QBluetoothDeviceInfo>
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
    connect(&host, SIGNAL(hostModeStateChanged(QBluetoothLocalDevice::HostMode)),
            this, SLOT(onHostModeChanged(QBluetoothLocalDevice::HostMode)));

    connect(&m_DeviceDiscoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(deviceDiscovered(QBluetoothDeviceInfo)));
    connect(&m_DeviceDiscoveryAgent, SIGNAL(finished()),
            this, SLOT(onDeviceDiscoveryFinished()));

    QList<QBluetoothAddress> connectedDevices = host.connectedDevices();
    if  (connectedDevices.isEmpty()) {
        host.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
    }
    else {
        m_ConnectedDevice = connectedDevices.first();
        m_DeviceDiscoveryAgent.start();
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
    QString device = m_KnownDevices[m_ConnectedDevice];
    if (device.isEmpty() && !m_ConnectedDevice.isNull()) {
        device = m_ConnectedDevice.toString();
    }
    return device;
}

void Bluetooth::onDeviceConnected(const QBluetoothAddress &address)
{
    qDebug() << "device connected" << address;
    host.setHostMode(QBluetoothLocalDevice::HostConnectable);
    host.requestPairing(address, QBluetoothLocalDevice::AuthorizedPaired);
    m_ConnectedDevice = address;
    QString deviceName = m_KnownDevices[address];
    if (deviceName.isEmpty()) {
        deviceName = address.toString();
    }
    emit connectedDeviceChanged(deviceName);
}

void Bluetooth::onDeviceDisconnected(const QBluetoothAddress &address)
{
    qDebug() << "device disconnected" << address;
    m_ConnectedDevice.clear();
    emit connectedDeviceChanged(QString());
    setHostMode(QBluetoothLocalDevice::HostDiscoverable);
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
                 << (pairing == QBluetoothLocalDevice::Paired
                    ? "paired" : "paired (authorized)");
        break;
    }
}

void Bluetooth::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    qDebug() << "discovered device:" << device.name() << "(" << device.address() << ")";
    const QBluetoothAddress address = device.address();
    QString &name = m_KnownDevices[address];
    bool wasEmpty = name.isEmpty();
    name = device.name();
    if (address == m_ConnectedDevice && wasEmpty) {
        qDebug() << "Updating connected device name to" << name
                 << "- was" << address.toString();
        emit connectedDeviceChanged(name);
    }
}

void Bluetooth::onDeviceDiscoveryFinished()
{
    switch (hostMode()) {
    case QBluetoothLocalDevice::HostConnectable:
        m_DeviceDiscoveryAgent.start();
        break;
    case QBluetoothLocalDevice::HostPoweredOff:
    case QBluetoothLocalDevice::HostDiscoverable:
    case QBluetoothLocalDevice::HostDiscoverableLimitedInquiry:
        m_DeviceDiscoveryAgent.stop();
        break;
    }
}

void Bluetooth::onHostModeChanged(QBluetoothLocalDevice::HostMode mode)
{
    switch (mode) {
    case QBluetoothLocalDevice::HostConnectable:
        m_DeviceDiscoveryAgent.start();
        break;
    case QBluetoothLocalDevice::HostPoweredOff:
    case QBluetoothLocalDevice::HostDiscoverable:
    case QBluetoothLocalDevice::HostDiscoverableLimitedInquiry:
        m_DeviceDiscoveryAgent.stop();
        break;
    }
}
