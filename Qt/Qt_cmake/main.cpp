#include "speedometer.h"
#include "vehicle_battery.h"
#include <QtGui>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>
#include <QWidgetList>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    Speedometer* ptr_speedometer = new Speedometer();
    qmlRegisterType<Speedometer>("CustomComponents", 1, 0, "Speedometer");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    if (engine.rootObjects().isEmpty()) {
        qWarning() << "No root objects found. Exiting.";
        return -1;
    }

    QObject *rootObject = engine.rootObjects().constFirst();  // Safely get the first root object
    if (!rootObject) {
        qWarning() << "Root object is null. Exiting.";
        return -1;
    }

    // Find the Speedometer object
    QObject *speedometerObj = rootObject->findChild<QObject*>("speedoMeter");
    if (!speedometerObj) {
        qWarning() << "Speedometer object not found. Exiting.";
        return -1;
    }

    // Cast QObject to Speedometer
    Speedometer *ptr_speedometer = qobject_cast<Speedometer*>(speedometerObj);
    if (!ptr_speedometer) {
        qWarning() << "Failed to cast QObject to Speedometer. Exiting.";
        return -1;
    }

    // Create and connect the vehicle object
    VehicleBattery battery;
    engine.rootContext()->setContextProperty("vehicleBattery", &battery);

    // Example speed change
    // vehicle.changeSpeed(80.5);

    // Example timer for showing speedometer speed change
    // qreal val = 0;
    // QTimer timer1;
    // bool direction;
    // QObject::connect(&timer1, &QTimer::timeout, [&]()
    //                  {
    //                      if(val >= 300)
    //                          direction = false;
    //                      else if(val <= 1)
    //                          direction = true;

    //                      if(direction)
    //                          val= val + 10;
    //                      else
    //                          val = val - 10;

    //                      ptr_speedometer->setSpeed(val);
    //                  });

    // timer1.start(1000);

    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
