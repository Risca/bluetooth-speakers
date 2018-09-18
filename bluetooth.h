#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <QBluetoothAddress>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothLocalDevice>
#include <QMap>
#include <QObject>
#include <QString>

class QBluetoothDeviceInfo;

class Bluetooth : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QBluetoothLocalDevice::HostMode hostMode READ hostMode WRITE setHostMode NOTIFY hostModeChanged)
    Q_PROPERTY(QString connectedDevice READ connectedDevice NOTIFY connectedDeviceChanged)

public:
    explicit Bluetooth(QObject *parent = nullptr);

    QBluetoothLocalDevice::HostMode hostMode() const;
    QString connectedDevice() const;

signals:
    void hostModeChanged(QBluetoothLocalDevice::HostMode);
    void connectedDeviceChanged(const QString& address);

public slots:
    void setHostMode(QBluetoothLocalDevice::HostMode mode);

private slots:
    void onDeviceConnected(const QBluetoothAddress& address);
    void onDeviceDisconnected(const QBluetoothAddress& address);
    void onError(QBluetoothLocalDevice::Error error);
    void onPairingConfirm(const QBluetoothAddress& address, const QString& pin);
    void onPairingPin(const QBluetoothAddress& address, const QString& pin);
    void onPairingFinished(const QBluetoothAddress& address, QBluetoothLocalDevice::Pairing pairing);
    void deviceDiscovered(const QBluetoothDeviceInfo& info);
    void onDeviceDiscoveryFinished();
    void onHostModeChanged(QBluetoothLocalDevice::HostMode mode);

private:
    QBluetoothLocalDevice host;
    QBluetoothDeviceDiscoveryAgent m_DeviceDiscoveryAgent;
    QMap<QBluetoothAddress, QString> m_KnownDevices;
    QBluetoothAddress m_ConnectedDevice;
};

#endif // BLUETOOTH_H
