#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "CAN_Receiver.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    CanReceiver canReceiver;
    engine.rootContext()->setContextProperty("canReceiver", &canReceiver);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

