#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <QBluetoothLocalDevice>
#include <QObject>
#include <QString>

class QBluetoothAddress;

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
    void onDeviceConnected(const QBluetoothAddress& address);
    void onDeviceDisconnected(const QBluetoothAddress& address);
    void onError(QBluetoothLocalDevice::Error error);
    void onPairingConfirm(const QBluetoothAddress& address, const QString& pin);
    void onPairingPin(const QBluetoothAddress& address, const QString& pin);
    void onPairingFinished(const QBluetoothAddress& address, QBluetoothLocalDevice::Pairing pairing);
    void setHostMode(QBluetoothLocalDevice::HostMode mode);

private:
    QBluetoothLocalDevice host;
    QString m_ConnectedDevice;
};

#endif // BLUETOOTH_H
