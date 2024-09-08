#include "vehicle_battery.h"
#include <QDebug>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

VehicleBattery::VehicleBattery(QObject *parent) : QObject(parent), file(-1) {
    if (!initI2C()) {
        qDebug() << "Failed to initialize I2C interface.";
    }
}

bool VehicleBattery::initI2C() {
    // Open the I2C bus
    if ((file = open(device, O_RDWR)) < 0) {
        qDebug() << "Failed to open the I2C bus";
        return false;
    }

    // Set the I2C address for the slave device
    if (ioctl(file, I2C_SLAVE, addr) < 0) {
        qDebug() << "Failed to acquire bus access and/or talk to slave.";
        close(file);
        return false;
    }

    return true;
}

int VehicleBattery::readRegister() {
    char buf[2];
    buf[0] = reg;

    if (file < 0) {
        qDebug() << "I2C file is not initialized.";
        return -1;
    }

    // Write the register address to the I2C bus
    if (write(file, buf, 1) != 1) {
        qDebug() << "Failed to write to the I2C bus.";
        qDebug() << "Error still exists";
        return -1;
    }
    usleep(1000);

    // Read the data from the I2C bus
    char data[2];
    if (read(file, data, 1) != 1) {
        qDebug() << "Failed to read from the I2C bus.";
        return -1;
    }
    char result[1] = {0};
    result[0] = (data[1] >> 3);
    return static_cast<int>(result[0] * 4);
}
/**
 * @Battery Percentage calculation details
 *
 * Full charge voltage is 12.45V
 * RPi turns off when the bus voltage is 8.4V
 * Hence the low voltage is set to 9V
 * 
 *
 *
 * The difference in voltage is 12.4 - 9 = 3.45 
 * battery_percentage = ((actual voltage - low voltage) / diffVoltage) * 100
 */
float VehicleBattery::getBatteryVoltage() {
    // The battery voltage is stored in register 0x02
    float voltage = readRegister();
    float lowVoltage = 9;
    float diffVoltage = 3.45;
    if (voltage == -1) {
        qDebug() << "Failed to read battery voltage.";
        return -1;
    }
    float battery_percentage = (((voltage - lowVoltage) / diffVoltage )*100);
    qDebug() << battery_percentage << "%";
    return battery_percentage;
}
