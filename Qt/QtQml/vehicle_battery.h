#ifndef VEHICLE_BATTERY_H
#define VEHICLE_BATTERY_H

#include <QObject>

class VehicleBattery : public QObject {
    Q_OBJECT

public:
    explicit VehicleBattery(QObject *parent = nullptr);

    Q_INVOKABLE int getBatteryVoltage(); // This method will be accessible from QML

private:
    int file;
    const char *device = "/dev/i2c-1"; // I2C bus device
    int addr = 0x41; // The I2C address of the battery monitoring device
    char reg = 0x02;
    bool initI2C();  // Method to initialize the I2C interface
    int readRegister(); // Method to read a value from a register
};

#endif // VEHICLE_BATTERY_H
