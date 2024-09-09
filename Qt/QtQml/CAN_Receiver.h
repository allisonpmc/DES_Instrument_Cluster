#ifndef CAN_RECEIVER_H
#define CAN_RECEIVER_H

#include <QCoreApplication>
#include <QCanBus>
#include <QCanBusDevice>
#include <QCanBusFrame>
#include <QObject>
#include <QTimer>
#include <QDebug>

class CanReceiver : public QObject {
    Q_OBJECT
    Q_PROPERTY(double speed READ speed WRITE setSpeed NOTIFY speedChanged)

public:
    CanReceiver(QObject *parent = nullptr);
    double speed() const;
    void setSpeed(double speed);
    ~CanReceiver();

signals:
    void speedChanged();

private slots:
    void processFrames();

private:
    QCanBusDevice *m_device;
    double m_speed;
};

#endif // CAN_RECEIVER_H
