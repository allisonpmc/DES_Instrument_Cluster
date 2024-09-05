#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "CAN_Receiver.h"
#include "vehicle_battery.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    CanReceiver canReceiver;
    engine.rootContext()->setContextProperty("canReceiver", &canReceiver);

    VehicleBattery battery;
    engine.rootContext()->setContextProperty("vehicleBattery", &battery);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

