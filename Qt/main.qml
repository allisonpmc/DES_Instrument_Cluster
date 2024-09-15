import QtQuick 2.15
import QtQuick.Window 2.15
import CustomComponents 1.0
import QtQuick.Controls

Window {

    visible: true
    width: 1280
    height: 400
    color: "black"

    Speedometer {
        objectName: "speedoMeter"
               anchors.horizontalCenter:  parent.horizontalCenter
               anchors.verticalCenter: parent.verticalCenter
               width: speedometerSize
               height: speedometerSize
               startAngle: startAngle
               alignAngle: alignAngle
               lowestRange: lowestRange
               highestRange: highestRange
               speed: speed
               arcWidth: arcWidth
               outerColor: outerColor
               innerColor: innerColor
               textColor: textColor
               backgroundColor: backgroundColor

               }

    Text {
           id: batteryStatus
           text: "Battery: " + vehicle.battery + "%"
           anchors.top: parent.top
           anchors.right: parent.right
           anchors.margins: 10  // Optional: adds some padding from the edges
           font.pixelSize: 24   // Set the text size as per your preference
           color: "white"       // Set the text color
       }

    Connections {
            target: vehicle
            function onBatterychanged() {
                batteryStatus.text = "Battery: " + m_battery + "%";
            }
        }

}






