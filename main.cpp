#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "bluetooth.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<Bluetooth>("se.dalakolonin.bluetooth", 1, 0, "Bluetooth");
    qmlRegisterType<QBluetoothLocalDevice>("se.dalakolonin.bluetooth", 1, 0, "QBluetoothLocalDevice");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/Page1Form.ui.qml")));
    engine.load(QUrl(QStringLiteral("qrc:/Page2Form.ui.qml")));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
