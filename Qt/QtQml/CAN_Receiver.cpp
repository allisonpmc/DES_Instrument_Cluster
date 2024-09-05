#include <QCoreApplication>
#include <QCanBus>
#include <QCanBusDevice>
#include <QCanBusFrame>
#include <QObject>
#include <QTimer>

class CanReceiver : public QObject {
    Q_OBJECT
    Q_PROPERTY(int speed READ speed WRITE setSpeed NOTIFY speedChanged)

public:
    CanReceiver(QObject *parent = nullptr) : QObject(parent), m_speed(0) {
        // Set up CAN bus device
        m_device = QCanBus::instance()->createDevice("socketcan", "can0"); // Adjust the parameters as needed
        if (!m_device) {
            qWarning() << "Failed to create CAN bus device.";
            return;
        }

        connect(m_device, &QCanBusDevice::framesReceived, this, &CanReceiver::processFrames);
        if (!m_device->connectDevice()) {
            qWarning() << "Failed to connect CAN bus device.";
        }

        // Example of a timer to periodically update speed
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &CanReceiver::updateSpeed);
        timer->start(1000); // Update speed every second
    }

    int speed() const { return m_speed; }
    void setSpeed(int speed) {
        if (m_speed != speed) {
            m_speed = speed;
            emit speedChanged();
        }
    }

signals:
    void speedChanged();

private slots:
    void processFrames() {
        while (m_device->framesAvailable()) {
            QCanBusFrame frame = m_device->readFrame();
            // Process the frame to extract speed
            // This is just a placeholder for your actual logic
            if (frame.frameId() == 0x123) { // Adjust frame ID as needed
                // Example: frame.payload() contains speed data
                // Update m_speed based on the received CAN frame data
                setSpeed(frame.payload().at(0)); // Example: taking the first byte as speed
            }
        }
    }

    void updateSpeed() {
        // Update speed periodically or based on some logic
        // This is just an example
        setSpeed(m_speed + 1); // Just an example increment
    }

private:
    QCanBusDevice *m_device;
    int m_speed;
};
