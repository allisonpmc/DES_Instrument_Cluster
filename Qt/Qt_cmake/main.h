#ifndef MAIN_H
#define MAIN_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "speedometer.h"
#include <QTimer>
#include <QtDBus/QDBusConnection>
#include <QtSerialBus/QCanBus>
#include <QtSerialBus/QCanBusFrame>
#include <QtSerialBus/QCanBusDevice>
#include <QWidgetList>


class vehicle:  public QObject
{
    Q_OBJECT
    Q_PROPERTY(double battery READ battery WRITE setBattery NOTIFY onBatterychanged)

public:
    explicit vehicle(QObject *parent = nullptr) : QObject(parent), m_battery(0.0) {}

    double battery() const {
        return m_battery;
    }

    // Set the new battery percentage when the battery percentage is changed
    void setBattery(double newBattery) {
        if (m_battery != newBattery) {
            m_battery = newBattery;
            emit onBatterychanged(m_battery);
        }
    }

signals:
    void speedChanged(double speed);
    void onBatterychanged(double battery);

public:
    void changeSpeed(double newSpeed) {
        emit speedChanged(newSpeed);
    }

    void changeBattery(double newBattery) {
        setBattery(newBattery);
    }

private:
    double m_battery;
};

#endif // MAIN_H

