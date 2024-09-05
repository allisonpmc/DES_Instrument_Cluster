/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -a cardash_adaptor carDash.xml
 *
 * qdbusxml2cpp is Copyright (C) The Qt Company Ltd. and other contributors.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#include "cardash_adaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class CarDashAdaptor
 */

CarDashAdaptor::CarDashAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

CarDashAdaptor::~CarDashAdaptor()
{
    // destructor
}

void CarDashAdaptor::setData(double speed, double rpm, double battery, const QString &gear)
{
    // handle method call local.carDash.setData
    QMetaObject::invokeMethod(parent(), "setData", Q_ARG(double, speed), Q_ARG(double, rpm), Q_ARG(double, battery), Q_ARG(QString, gear));
}
