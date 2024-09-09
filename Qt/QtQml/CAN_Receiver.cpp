#include "CAN_Receiver.h"
#include "Kalman_Filter.h"

CanReceiver::CanReceiver(QObject *parent)
    : QObject(parent), m_speed(0.0) // Initialize speed to 0.0
{
    // Set up CAN bus device
    m_device = QCanBus::instance()->createDevice("socketcan", "vcan0");
    if (!m_device) {
        qWarning() << "Failed to create CAN bus device.";
        return;
    }

    connect(m_device, &QCanBusDevice::framesReceived, this, &CanReceiver::processFrames);
    if (!m_device->connectDevice()) {
        qWarning() << "Failed to connect CAN bus device.";
    }
}

CanReceiver::~CanReceiver() 
{
    // Clean up and disconnect the CAN bus device
    if (m_device) {
        if (m_device->state() == QCanBusDevice::ConnectedState) {
            m_device->disconnectDevice();
        }
        delete m_device;
        m_device = nullptr;
    }
}

double CanReceiver::speed() const {
    return m_speed;
}

void CanReceiver::setSpeed(double speed) {
    if (m_speed != speed) {
        m_speed = speed;
        emit speedChanged();
    }
}

KalmanFilter kalmanFilter(0.1, 0.1, 0.1, 0.0);

// Initialize a timer to measure the time delta
QElapsedTimer timer1;

void CanReceiver::processFrames() {
    while (m_device->framesAvailable()) {
        QCanBusFrame frame = m_device->readFrame();
        // Process the frame to extract speed
        if (frame.frameId() == 0x100) { // Adjust frame ID as needed
            if (frame.payload().size() >= sizeof(double)) {
                qint64 elapsed = timer1.restart();
                double dt = elapsed / 1000.0;
                // Convert the payload to double (assuming little-endian encoding)
                double speed = 0;
                memcpy(&speed, frame.payload().constData(), sizeof(double));
                double filtered_Speed = kalmanFilter.update(speed, dt);
                setSpeed(filtered_Speed); // Set the extracted speed
                qDebug() << "Received_Speed: " << speed << "Filtered_Speed: " << filtered_Speed;
            }
        }
    }
}
