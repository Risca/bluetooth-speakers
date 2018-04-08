#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <QBluetoothLocalDevice>
#include <QBluetoothSocket>
#include <QObject>

class QBluetoothServiceInfo;
class QByteArray;
class QString;

class Bluetooth : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QBluetoothLocalDevice::HostMode hostMode READ hostMode WRITE setHostMode NOTIFY hostModeChanged)

public:
    explicit Bluetooth(QObject *parent = nullptr);

    void setHostMode(QBluetoothLocalDevice::HostMode mode);
    QBluetoothLocalDevice::HostMode hostMode() const;

signals:
    void hostModeChanged(QBluetoothLocalDevice::HostMode);

public slots:
    void onDeviceConnected();
    void onDeviceDisconnected();
    void onError(QBluetoothLocalDevice::Error error);
    void onPairingConfirm(const QBluetoothAddress& address, const QString& pin);
    void onPairingPin(const QBluetoothAddress& address, const QString& pin);
    void onPairingFinished(const QBluetoothAddress& address, QBluetoothLocalDevice::Pairing pairing);

    void connectToAudioDevice(const QString& device);
    void onAudioSocketConnected();
    void onAudioSocketDisconnected();
    void onError(QBluetoothSocket::SocketError error);
    void onStateChanged(QBluetoothSocket::SocketState state);
    void onReadyRead();

private:
    QBluetoothLocalDevice host;
    QStringList devices;
    QBluetoothSocket* socket;
};

#endif // BLUETOOTH_H
