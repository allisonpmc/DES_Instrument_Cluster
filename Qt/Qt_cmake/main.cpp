#include "main.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

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
    vehicle vehicle;
    engine.rootContext()->setContextProperty("vehicle", &vehicle); // Expose the vehicle instance to QML under the name "vehicle"
    // QObject::connect(&vehicle, &vehicle::speedChanged, ptr_speedometer, &Speedometer::setSpeed);

    // Example speed change
    // vehicle.changeSpeed(80.5);
    vehicle.changeBattery(50);

    // Example timer for showing speedometer speed change
    qreal val = 0;
    QTimer timer1;
    bool direction;
    QObject::connect(&timer1, &QTimer::timeout, [&]()
                     {
                         if(val >= 300)
                             direction = false;
                         else if(val <= 1)
                             direction = true;

                         if(direction)
                             val= val + 10;
                         else
                             val = val - 10;

                         ptr_speedometer->setSpeed(val);
                     });

    timer1.start(1000);

    // Example CAN setup

    // QString interfaceName = "can0";  // Your CAN interface name

    // // Create and connect to the CAN bus device
    // QCanBusDevice *device = QCanBus::instance()->createDevice("socketcan", interfaceName);

    // if (!device) {
    //     qWarning() << "Failed to create CAN bus device";
    //     return -1;
    // }

    // if (!device->connectDevice()) {
    //     qWarning() << "Failed to connect to CAN bus device";
    //     delete device;
    //     return -1;
    // }

    // QObject::connect(device, &QCanBusDevice::framesReceived, [&]() {
    //     while (device->framesAvailable()) {
    //         QCanBusFrame frame = device->readFrame();
    //         QString canMessage = frame.toString();
    //         emit ptr_speedometer->setSpeed(canMessage);
    //     }

    // });

    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
