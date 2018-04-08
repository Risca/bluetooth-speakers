#include "bluetooth.h"

#include <QBluetoothSocket>
#include <QBluetoothUuid>
#include <QByteArray>
#include <QDebug>
#include <QString>

Bluetooth::Bluetooth(QObject *parent) : QObject(parent), socket(0)
{
    connect(&host, SIGNAL(deviceConnected(QBluetoothAddress)), this, SLOT(onDeviceConnected()));
    connect(&host, SIGNAL(deviceDisconnected(QBluetoothAddress)), this, SLOT(onDeviceDisconnected()));
    connect(&host, SIGNAL(error(QBluetoothLocalDevice::Error)), this, SLOT(onError(QBluetoothLocalDevice::Error)));
    connect(&host, SIGNAL(pairingDisplayConfirmation(QBluetoothAddress,QString)),
            this, SLOT(onPairingConfirm(QBluetoothAddress,QString)));
    connect(&host, SIGNAL(pairingDisplayPinCode(QBluetoothAddress,QString)),
            this, SLOT(onPairingPin(QBluetoothAddress,QString)));
    connect(&host, SIGNAL(pairingFinished(QBluetoothAddress,QBluetoothLocalDevice::Pairing)),
            this, SLOT(onPairingFinished(QBluetoothAddress,QBluetoothLocalDevice::Pairing)));
}

void Bluetooth::setHostMode(QBluetoothLocalDevice::HostMode mode)
{
    host.setHostMode(mode);
}

QBluetoothLocalDevice::HostMode Bluetooth::hostMode() const
{
    return host.hostMode();
}

void Bluetooth::onDeviceConnected()
{
    qDebug() << "device connected";
}

void Bluetooth::onDeviceDisconnected()
{
    qDebug() << "device disconnected";
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
    default:
        qDebug() << "device: unhandled error: " << error;
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
    case QBluetoothLocalDevice::AuthorizedPaired: {
        qDebug() << "local device" << address
                 << (pairing == QBluetoothLocalDevice::Paired ? "paired" : "paired (authorized)");
        if (socket) {
            socket->close();
            socket->deleteLater();
        }
        socket = new QBluetoothSocket(QBluetoothServiceInfo::L2capProtocol, this);
        connect(socket, SIGNAL(connected()), this, SLOT(onAudioSocketConnected()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(onAudioSocketDisconnected()));
        connect(socket, SIGNAL(error(QBluetoothSocket::SocketError)), this,
                SLOT(onError(QBluetoothSocket::SocketError)));
        connect(socket, SIGNAL(stateChanged(QBluetoothSocket::SocketState)),
                this, SLOT(onStateChanged(QBluetoothSocket::SocketState)));
        connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        socket->connectToService(QBluetoothAddress(address), QBluetoothUuid(QBluetoothUuid::AudioSource));
        break;
    }
    default:
        qDebug() << "unhandled pairing for" << address << ":" << pairing;
        break;
    }
}

void Bluetooth::connectToAudioDevice(const QString &device)
{
    host.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
    host.requestPairing(QBluetoothAddress(device), QBluetoothLocalDevice::AuthorizedPaired);
}

void Bluetooth::onAudioSocketConnected()
{
    qDebug() << "audio socket connected";
}

void Bluetooth::onAudioSocketDisconnected()
{
    qDebug() << "audio socket disconnected";
}

void Bluetooth::onError(QBluetoothSocket::SocketError error)
{
    switch (error) {
    case QBluetoothSocket::UnknownSocketError:
        qDebug() << "socket: unknown error";
        break;
    case QBluetoothSocket::NoSocketError:
        qDebug() << "socket: no error";
        break;
    case QBluetoothSocket::HostNotFoundError:
        qDebug() << "socket: host not found";
        break;
    case QBluetoothSocket::ServiceNotFoundError:
        qDebug() << "socket: service not found";
        break;
    case QBluetoothSocket::NetworkError:
        qDebug() << "socket: network error";
        break;
    case QBluetoothSocket::UnsupportedProtocolError:
        qDebug() << "socket: unsupported protocol";
        break;
    case QBluetoothSocket::OperationError:
        qDebug() << "socket: operation error";
        break;
    case QBluetoothSocket::RemoteHostClosedError:
        qDebug() << "socket: remote host closed";
        break;
    default:
        qDebug() << "socket: unhandled error: " << error;
        break;
    }
}

void Bluetooth::onStateChanged(QBluetoothSocket::SocketState state)
{
    switch (state) {
    case QBluetoothSocket::UnconnectedState:
        qDebug() << "socket state: unconnected";
        break;
    case QBluetoothSocket::ServiceLookupState:
        qDebug() << "socket state: service lookup";
        break;
    case QBluetoothSocket::ConnectingState:
        qDebug() << "socket state: connecting";
        break;
    case QBluetoothSocket::ConnectedState:
        qDebug() << "socket state: connected";
        break;
    case QBluetoothSocket::BoundState:
        qDebug() << "socket state: bound";
        break;
    case QBluetoothSocket::ClosingState:
        qDebug() << "socket state: closing";
        break;
    case QBluetoothSocket::ListeningState:
        qDebug() << "socket state: listening";
        break;
    default:
        qDebug() << "unknown socket state: " << state;
        break;
    }
}

void Bluetooth::onReadyRead()
{
    QByteArray data = socket->read(335);
    qDebug() << "got data:" << data;
}
