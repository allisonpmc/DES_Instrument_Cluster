#include "CAN_Receiver.h"
#include "Kalman_Filter.h"

// Initialization for Kalman filter and timers
KalmanFilter kalmanFilter(0.07, 3.0, 0.1, 0.0);
QElapsedTimer timer1;

CanReceiver::CanReceiver(QObject *parent)
    : QObject(parent), m_speed(0.0) // Initialize speed to 0.0
{
    // Set up CAN bus device with error handling
    m_device = QCanBus::instance()->createDevice("socketcan", "can0");
    if (!m_device) {
        qWarning() << "Failed to create CAN bus device. Check if the CAN device driver is properly installed.";
        return;
    }

    // Use QVariant to wrap the integer value
    m_device->setConfigurationParameter(QCanBusDevice::BitRateKey, QVariant(500000));

    // Error signal connection for CAN bus errors
    connect(m_device, &QCanBusDevice::errorOccurred, this, &CanReceiver::handleError);

    connect(m_device, &QCanBusDevice::framesReceived, this, &CanReceiver::processFrames);
    if (!m_device->connectDevice()) {
        qWarning() << "Failed to connect to CAN bus device. Check device permissions or if the device is active.";
        return;
    }

    // Optionally check device error status after connection
    if (m_device->state() != QCanBusDevice::ConnectedState) {
        qWarning() << "CAN bus device is not in the connected state after connecting.";
        return;
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

void CanReceiver::processFrames() {
    // Ensure that CAN device is connected before processing frames
    if (m_device->state() != QCanBusDevice::ConnectedState) {
        qWarning() << "Cannot process frames. CAN bus device is not connected.";
        return;
    }
    while (m_device->framesAvailable()) {
        QCanBusFrame frame = m_device->readFrame();
        if (frame.isValid()) {
            // Process the frame to extract speed
            if (frame.frameId() == 0x100) { // Check for the correct frame ID
                if (frame.payload().size() >= sizeof(float)) {
                    try {
                        qint64 elapsed = timer1.restart();
                        double dt = elapsed / 1000.0; // Convert to seconds
                        // Extract speed from payload (assuming little-endian float)
                        QByteArray payload = frame.payload();
                        float speed = qFromLittleEndian<float>(reinterpret_cast<const uchar*>(payload.constData()));
                        qDebug() << "Received Speed: " << speed;
                            // Apply Kalman filter after stabilization period
                            double filtered_Speed = kalmanFilter.update(speed, dt);
                            setSpeed(filtered_Speed); // Update the speed
                            qDebug() << "Filtered Speed: " << filtered_Speed;
                    } catch (const std::exception &e) {
                        qWarning() << "Exception occurred while processing CAN frame: " << e.what();
                    }
                } else {
                    qWarning() << "Received CAN frame payload is too small.";
                }
            } else {
                qWarning() << "Received unexpected CAN frame ID: " << frame.frameId();
            }
        } else {
            qWarning() << "Invalid CAN frame received.";
        }
    }
}

// Error handling method
void CanReceiver::handleError(QCanBusDevice::CanBusError error) {
    switch (error) {
    case QCanBusDevice::ReadError:
        qWarning() << "CAN bus read error occurred.";
        break;
    case QCanBusDevice::WriteError:
        qWarning() << "CAN bus write error occurred.";
        break;
    case QCanBusDevice::ConnectionError:
        qWarning() << "CAN bus connection error occurred. Please check the connection.";
        break;
    case QCanBusDevice::ConfigurationError:
        qWarning() << "CAN bus configuration error occurred.";
        break;
    case QCanBusDevice::UnknownError:
    default:
        qWarning() << "Unknown CAN bus error occurred.";
        break;
    }
}
