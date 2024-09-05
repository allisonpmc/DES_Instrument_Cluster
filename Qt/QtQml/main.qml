import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

ApplicationWindow {
    visible: true
    width: 1280
    height: 400
    title: "Instrument Cluster"

    property int maxValue: 240  // Maximum speed value
    property int majorTickInterval: 20  // Interval between major ticks
    property int minorTickCount: 9  // Number of minor ticks between two major ticks
    property int currentValue: 0  // Current speed value
    property color needleColor: "blue"
    property color dialColor: "blue"
    property color backgroundColor: "black"
    property double speed: 0

    // property var vehicleBattery: {
    //        return {
    //            getBatteryVoltage: function() {
    //                vehicleBattery ? "Battery: " + vehicleBattery.getBatteryVoltage() + "%" : "Battery: N/A"
    //            }
    //        };
    //    }

    Canvas {
        id: speedometer
        width: 300
        height: 300
        anchors.centerIn: parent

        onPaint: {
            var ctx = getContext("2d");
            ctx.reset();
            var centerX = width / 2;
            var centerY = height / 2;
            var radius = Math.min(width, height) / 2.5;

            // Draw the outer circle (background)
            ctx.beginPath();
            ctx.arc(centerX, centerY, radius, 0.75 * Math.PI, 2.25 * Math.PI);
            ctx.fillStyle = backgroundColor;
            ctx.fill();

            // Draw the dial
            ctx.strokeStyle = dialColor;
            ctx.lineWidth = 3;
            ctx.beginPath();
            ctx.arc(centerX, centerY, radius, 0.75 * Math.PI, 2.25 * Math.PI);
            ctx.stroke();

            // Draw major and minor ticks
            ctx.lineWidth = 2;
            var totalTicks = (maxValue / majorTickInterval) * minorTickCount + (maxValue / majorTickInterval) + 1;

            for (var i = 0; i < totalTicks; i++) {
                var angle = (0.75 * Math.PI) + (i / (totalTicks - 1)) * 1.5 * Math.PI;
                var isMajorTick = (i % (minorTickCount + 1) === 0);
                var x1, y1, x2, y2;

                if (isMajorTick) {
                    // Major ticks
                    x1 = centerX + radius * Math.cos(angle);
                    y1 = centerY + radius * Math.sin(angle);
                    x2 = centerX + (radius - 15) * Math.cos(angle);
                    y2 = centerY + (radius - 15) * Math.sin(angle);

                    // Draw the speed number for major ticks
                    var speedValue = (i / (minorTickCount + 1)) * majorTickInterval;
                    ctx.fillStyle = "white";
                    ctx.font = "bold 12px Arial";
                    var textX = centerX + (radius - 30) * Math.cos(angle);
                    var textY = centerY + (radius - 30) * Math.sin(angle);
                    ctx.fillText(speedValue.toString(), textX - 10, textY + 5);
                } else {
                    // Minor ticks
                    x1 = centerX + radius * Math.cos(angle);
                    y1 = centerY + radius * Math.sin(angle);
                    x2 = centerX + (radius - 8) * Math.cos(angle);
                    y2 = centerY + (radius - 8) * Math.sin(angle);
                }

                ctx.beginPath();
                ctx.moveTo(x1, y1);
                ctx.lineTo(x2, y2);
                ctx.strokeStyle = "blue";
                ctx.stroke();
            }

            // Draw the needle
            var needleAngle = (0.75 * Math.PI) + (canReceiver.speed / maxValue) * 1.5 * Math.PI;
            var needleLength = radius - 20;
            var needleX = centerX + needleLength * Math.cos(needleAngle);
            var needleY = centerY + needleLength * Math.sin(needleAngle);

            ctx.beginPath();
            ctx.moveTo(centerX, centerY);
            ctx.lineTo(needleX, needleY);
            ctx.strokeStyle = needleColor;
            ctx.lineWidth = 4;
            ctx.stroke();

            // Draw the center circle
            ctx.beginPath();
            ctx.arc(centerX, centerY, 5, 0, 2 * Math.PI);
            ctx.fillStyle = "white";
            ctx.fill();
        }

        Component.onCompleted: requestPaint()
    }

    Slider {
        id: speedSlider
        width: 300
        from: 0
        to: maxValue
        value: canReceiver.speed
        anchors.top: speedometer.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        onValueChanged: {
            canReceiver.speed = value;
            speedometer.requestPaint();
        }
    }

    Text {
        id: speedLabel
        text:  canReceiver ? "Speed: " + canReceiver.speed + " km/h" : "Speed: N/A"
        color: "blue"
        font.pixelSize: 20
        anchors.top: speedometer.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Text {
              id: batteryStatus
              text:  vehicleBattery ? "Battery: " + vehicleBattery.getBatteryVoltage() + "%" : "Battery: N/A"
              anchors.top: parent.top
              anchors.right: parent.right
              anchors.margins: 10  // Optional: adds some padding from the edges
              font.pixelSize: 24   // Set the text size as per your preference
              color: "Blue"       // Set the text color
          }
}
