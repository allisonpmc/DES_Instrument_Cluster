#include <QPainter>
#include <QConicalGradient>
#include "speedometer.h"

Speedometer::Speedometer(QQuickItem *parent)
    : QQuickPaintedItem(parent),
    m_SpeedometerSize(400),
    m_StartAngle(30),
    m_AlignAngle(270),
    m_LowestRange(0),
    m_HighestRange(300),
    m_Speed(0),
    m_ArcWidth(6),
    m_OuterColor(QColor(12, 16, 247)),
    m_InnerColor(QColor(51, 88, 255, 80)),
    m_TextColor(QColor(255, 255, 255)),
    m_BackgroundColor(Qt::transparent)
{
}

void Speedometer::paint(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing);
    QRectF rect = boundingRect();

    drawBackground(painter, rect);
    drawFullArc(painter, rect);
    drawInnerPie(painter, rect);
    drawMajorTicks(painter, rect);
    drawMinorTicks(painter, rect);
    drawNumbers(painter, rect);
    drawCurrentSpeedArc(painter, rect);
    drawSpeedText(painter, rect);
}

void Speedometer::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->save();
    painter->fillRect(rect, m_BackgroundColor);
    painter->restore();
}

void Speedometer::drawFullArc(QPainter *painter, const QRectF &rect)
{
    painter->save();
    QPen pen(m_InnerColor);
    pen.setWidth(m_ArcWidth);
    pen.setCapStyle(Qt::FlatCap);
    painter->setPen(pen);

    double startAngle = m_StartAngle - 45;
    double spanAngle = 0 - m_AlignAngle;

    painter->drawArc(rect.adjusted(m_ArcWidth, m_ArcWidth, -m_ArcWidth, -m_ArcWidth), startAngle * 16, spanAngle * 16);
    painter->restore();
}

void Speedometer::drawMajorTicks(QPainter *painter, const QRectF &rect)
{
    painter->save();
    QPen pen = painter->pen();
    pen.setColor(m_OuterColor);
    pen.setWidth(4);
    painter->setPen(pen);

    double angleStep = m_AlignAngle / 10.0;
    double startAngle = m_StartAngle - 45;
    painter->translate(width() / 2, height() / 2);
    painter->rotate(-startAngle);
    // double radius = (rect.width() - m_ArcWidth) / 2;

    for (int i = 0; i <= 10; ++i)
    {
        painter->drawLine(rect.width()/2 - 25, 0, rect.width()/2 - 6, 0);
        painter->rotate(angleStep);
    }
    painter->translate(width() / 2, height() / 2);
    painter->restore();
}

void Speedometer::drawMinorTicks(QPainter *painter, const QRectF &rect)
{
    painter->save();
    QPen pen = painter->pen();
    pen.setColor(m_InnerColor);
    pen.setWidth(2);
    painter->translate(width() / 2, height() / 2);
    painter->rotate(135);
    painter->setPen(pen);
    int minorTicks = 8* 11;
    qreal angleStepMin = 270 / minorTicks;
    for(int i = 0; i <= minorTicks+1; ++i)
    {
        painter->drawLine(rect.width()/2 - 20, 0, rect.width()/2 - 6, 0);
        painter->rotate(angleStepMin);
    }
    painter->translate(-width() / 2, -height() / 2);
    painter->restore();
}

void Speedometer::drawNumbers(QPainter *painter, const QRectF &rect)
{
    painter->save();
    QPen pen = painter->pen();
    pen.setColor(m_OuterColor);
    painter->setPen(pen);

    const int radius = rect.width() / 2 - 45;  // Determines the position of the numbers
    QFontMetrics fontMat(painter->font());
    QString numbr;

    double startAngle = 135;   // Start at 135 degrees
    double spanAngle = 270;     // Span 270 degrees
    int divisions = 10;         // Number of major tick divisions (e.g., 10)
    double speedIncrement = 30;
    // Increment based on the span angle and number of divisions
    double angleIncrement = spanAngle / divisions;

    for (int i = 0; i <= divisions; ++i)
    {
        // Calculate the angle for this number
        double angle = startAngle + i * angleIncrement;

        // Convert the angle to radians for the trigonometric functions
        double radian = angle * M_PI / 180.0;

        // Calculate the position for each number
        QPoint p(
            rect.center().x() + radius * cos(radian),
            rect.center().y() + radius * sin(radian)
            );

        // Create the number string to display (adjust based on your numbering scheme)
        numbr = QString::number(i * speedIncrement);

        // Get the bounding rectangle for the text
        QRect bound = fontMat.boundingRect(numbr);

        // Draw the number, centered on the calculated point
        painter->drawText(QPointF(p.x() - bound.width() / 2, p.y() + bound.height() / 2), numbr);
    }

    painter->restore();
}



void Speedometer::drawInnerPie(QPainter *painter, const QRectF &rect)
{
    painter->save();
    int pieSize = m_SpeedometerSize / 5;
    QPen pen(m_OuterColor);
    pen.setWidth(m_ArcWidth / 2);
    pen.setCapStyle(Qt::FlatCap);
    painter->setPen(pen);

    QConicalGradient gradient(rect.center(), -135 + 270 / 2);
    gradient.setColorAt(0.0, m_OuterColor);
    gradient.setColorAt(1.0, m_InnerColor);

    painter->setBrush(gradient);
    double startAngle = -135;
    double spanAngle = -270;

    painter->drawPie(rect.adjusted(pieSize, pieSize, -pieSize, -pieSize), startAngle * 16, spanAngle * 16);
    painter->restore();
}


void Speedometer::drawSpeedText(QPainter *painter, const QRectF &rect)
{
    painter->save();
    QFont font("Helvetica", 40, QFont::Bold);
    painter->setFont(font);
    QPen pen(m_TextColor);
    painter->setPen(pen);

    QString speedText = QString::number(m_Speed, 'f', 1);
    painter->drawText(rect.adjusted(m_SpeedometerSize / 30, m_SpeedometerSize / 30, -m_SpeedometerSize / 30, -m_SpeedometerSize / 4), Qt::AlignCenter, speedText);

    painter->restore();
}

void Speedometer::drawCurrentSpeedArc(QPainter *painter, const QRectF &rect)
{
    painter->save();
    QPen pen(m_OuterColor);
    pen.setWidth(m_ArcWidth);
    pen.setCapStyle(Qt::FlatCap);
    painter->setPen(pen);

    int startAngle = -135;
    int spanAngle = -270;
    qreal valueToAngle = ((m_Speed - m_LowestRange) / (m_HighestRange - m_LowestRange)) * spanAngle;

    painter->drawArc(rect.adjusted(m_ArcWidth, m_ArcWidth, -m_ArcWidth, -m_ArcWidth), startAngle * 16, valueToAngle * 16);

    painter->restore();
}

qreal Speedometer::getSpeedometerSize()
{
    return m_SpeedometerSize;
}

qreal Speedometer::getStartAngle()
{
    return m_StartAngle;
}


qreal Speedometer::getAlignAngle()
{
    return m_AlignAngle;
}


qreal Speedometer::getLowestRange()
{
    return m_LowestRange;
}

qreal Speedometer::getHighestRange()
{
    return m_HighestRange;
}


qreal Speedometer::getSpeed()
{
    return m_Speed;
}

int Speedometer::getArcWidth()
{
    return m_ArcWidth;
}

QColor Speedometer::getOuterColor()
{
    return m_OuterColor;
}

QColor Speedometer::getInnerColor()
{
    return m_InnerColor;
}

QColor Speedometer::getTextColor()
{
    return m_TextColor;
}

QColor Speedometer::getBackgroundColor()
{
    return m_BackgroundColor;
}

void Speedometer::setSpeedometerSize(qreal size)
{
    if(m_SpeedometerSize == size)
        return;
    m_SpeedometerSize = size;

    emit speedometerSizeChanged();
}

void Speedometer::setStartAngle(qreal startAngle)
{
    if(m_StartAngle == startAngle)
        return;

    m_StartAngle = startAngle;

    emit startAngleChanged();
}

void Speedometer::setAlignAngle(qreal angle)
{
    if(m_StartAngle == angle)
        return;

    m_StartAngle = angle;

    emit alignAngleChanged();
}

void Speedometer::setLowestRange(qreal lowestRange)
{
    if(m_LowestRange == lowestRange)
        return;

    m_LowestRange = lowestRange;

    emit lowestRangeChanged();
}

void Speedometer::setHighestRange(qreal highestRange)
{
    if(m_HighestRange == highestRange)
        return;

    m_HighestRange = highestRange;

    emit highestRangeChanged();
}

void Speedometer::setSpeed(qreal speed)
{
    if(m_Speed == speed)
        return;

    m_Speed = speed;
    qDebug() << "The speed is now:" << m_Speed;
    update();
    emit speedChanged();
}

void Speedometer::setArcWidth(int arcWidth)
{
    if(m_ArcWidth == arcWidth)
        return;

    m_ArcWidth = arcWidth;

    emit arcWidthChanged();
}

void Speedometer::setOuterColor(QColor outerColor)
{
    if(m_OuterColor == outerColor)
        return;

    m_OuterColor = outerColor;

    emit outerColorChanged();
}

void Speedometer::setInnerColor(QColor innerColor)
{
    if(m_InnerColor == innerColor)
        return;

    m_InnerColor = innerColor;

    emit innerColorChanged();
}

void Speedometer::setTextColor(QColor textColor)
{
    if(m_TextColor == textColor)
        return;

    m_TextColor = textColor;

    emit textColorChanged();
}

void Speedometer::setBackgroundColor(QColor backgroundColor)
{
    if(m_BackgroundColor == backgroundColor)
        return;

    m_BackgroundColor = backgroundColor;

    emit backgroundColorChanged();
}

