#include <QDataStream>
#include <QtSerialBus/QCanBus>
#include <QElapsedTimer>
#include <QDebug>
#include "Kalman_Filter.h"
#include <QQmlApplicationEngine>
#include <QQmlContext>

// Function to convert byte array to float
double byteArrayToDouble(const QByteArray &byteArray) {
    double value;
    QDataStream stream(byteArray);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream >> value;
    return value;
}

// Function to initialize CAN bus and handle incoming messages
bool initializeCanBus(QObject *ptr_speedometer) {
    // Initialize CAN bus device
    QCanBusDevice *canDevice = QCanBus::instance()->createDevice("socketcan", "vcan0");
    if (!canDevice) {
        qWarning() << "Failed to create CAN device. Exiting.";
        return false;
    }

    // Connect to CAN bus
    if (!canDevice->connectDevice()) {
        qWarning() << "Failed to connect to CAN bus. Exiting.";
        return false;
    }

    // Initialize Kalman filter
    KalmanFilter kalmanFilter(0.1, 0.1, 0.1, 0.0);

    // Initialize a timer to measure the time delta
    QElapsedTimer timer;
    timer.start();

    // Slot to handle incoming CAN messages
    QObject::connect(canDevice, &QCanBusDevice::framesReceived, [canDevice, ptr_speedometer, &kalmanFilter, &timer]() {
        while (canDevice->framesAvailable()) {
            QCanBusFrame frame = canDevice->readFrame();
            QByteArray payload = frame.payload();
            double speed = byteArrayToDouble(payload); // Ensure byteArrayToDouble is defined

            // Calculate time delta (in seconds)
            qint64 elapsed = timer.restart();
            double dt = elapsed / 1000.0; // Convert milliseconds to seconds

            // Apply Kalman filter with time delta
            double filteredSpeed = kalmanFilter.update(speed, dt);
            qDebug() << "Received speed:" << speed << "Filtered speed:" << filteredSpeed;

            // Update the Speedometer object
            QMetaObject::invokeMethod(ptr_speedometer, "changeSpeed", Q_ARG(double, filteredSpeed));
        }
    });

    return true;
}
