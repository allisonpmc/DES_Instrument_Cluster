# DES02_Instrument_Cluster

## Table of Contents
- [Introduction](## Introduction)
- [Project Overview]
- [Project Goals and Objectives]
- [Installation and Setup]
- [Hardware Components]
- [System Architecture]
- [Docker]
- [Results]

## Introduction
The document includes the system architecture, hardware components, Qt Framework and GUI details of the Instrument cluster.

## Project Overview
The PiRacer instrument cluster Qt application project is aimed at creating a real-time speedometer for a PiRacer car. The application will run on a Raspberry Pi and receive speed data from a speed sensor via the In-Vehicle Network (IVN) communication using Controller Area Network (CAN) protocol.

## Project Goals and Objectives
The main goal of the PiRacer Instrument Cluster Qt Application project is to create a functioning instrument cluster for a PiRacer car that displays real-time speed data from a speed sensor via CAN bus.

The following are the specific objectives of the project:

- To design and implement a Qt-based graphical user interface (GUI) for the instrument cluster.
- To integrate an arduino based speed sensor system with the Raspberry Pi using the CAN bus protocol.
- To display real-time speed data on the instrument cluster GUI.
- To test the functionality of the instrument cluster using realistic driving scenarios.
BONUS Part:
- To display battery level of PiRacer on Instrument Cluster.

# Installtion and Setup

## Raspberry Pi Setup

### 2-CH CAN Shield

**Dependencies**

1. Open the config.txt file
    ```
    sudo nano /boot/firmware/config.txt
    ```
2. Add the following line at the end of the file
    ```
    dtoverlay=seeed-can-fd-hat-v2
    ```
3. Reboot Raspberry Pi
    ```
    sudo shutdown -r now
    ```

4. Check the kernel log to see if CAN-BUS HAT was initialized successfully. You will also see can0 and can1 appear in the list of ifconfig results
    ```
    dmesg | grep spi
    ifconfig -a
    ```


**CAN Configuration**

1. Set the CAN FD protocol, and the dbitrate can be set to 8M speed 
```
sudo ip link set can0 up type can bitrate 1000000 dbitrate 8000000 restart-ms 1000 berr-reporting on fd on
sudo ip link set can1 up type can bitrate 1000000 dbitrate 8000000 restart-ms 1000 berr-reporting on fd on
sudo ifconfig can0 txqueuelen 65536
sudo ifconfig can1 txqueuelen 65536
```
Supported Bitrate 125kbps, 500kbps and 1Mbps. 

2. If the bus is already busy, use the following command to reset the bitrate and dbitrate of can0 
```
sudo ip link set can0 down
sudo ip link set can0 up type can bitrate 1000000 dbitrate 8000000 restart-ms 1000 berr-reporting on fd on
sudo ifconfig can0 txqueuelen 65536
```

**CAN Message Generation and Recieving**

1. Install can-utils
     ```
     sudo apt-get install can-utils
     ```

2. Open two terminal windows and enter the following commands in the Windows to test can fd protocol.
    ```
    #Terminal 1 - Gernerates random CAN message
    cangen can0 -mv

    #Terminal 2 - Shows the recieved CAN message
    candump can0
    ```

### Touch Screen Display

**Physical Interface Instructions**

1. Use a 15PIN FPC cable to connect the DSI interface of the display screen to the DSI interface of the Raspberry Pi board.
2. Install the Raspberry Pi on the display board with the back facing down, and connect the 5V power supply and I2C communication
   through the 4-pin header.

Settings at Config.txt - Following lines to be added:
```
dtoverlay=vc4-kms-v3d
#DSI1 Use
dtoverlay=vc4-kms-dsi-waveshare-panel,7_9_inch
#DSI0 Use
#dtoverlay=vc4-kms-dsi-waveshare-panel,7_9_inch,dsi0
```

# Hardware Components

## PiRacer Car

Pi Racer
    - [Pi-Racer Wiki](https://www.waveshare.com/piracer-ai-kit.htm)

## Arduino UNO with Speed Sensor

Arduino UNO is a microcontroller board based on the ATmega328P. It has 14 digital input/output pins (of which 6 can be used as PWM outputs), 6 analog inputs, a 16 MHz ceramic resonator, a USB connection, a power jack, an ICSP header and a reset button. IR Speed Sensor is used to send the rpm data to Arduino.

## CAN-BUS Shield V2.0 (seeed studio)

Since Arduino has no in-built CAN interfaces, external CAN-BUS Shield is used. The CAN-BUS shield communicate via Arduino's SPI Interface to collect the speed data.
    -[CAN-BUS SHIELD Schematic](https://docs.rs-online.com/d7d8/A700000007766014.pdf)

## Raspberry Pi 4 Model B 

Raspberry Pi 4 Model B features a high-performance 64-bit quad-core processor,
dual-display support at resolutions up to 4K via a pair of micro HDMI ports, hardware
video decode at up to 4Kp60, up to 8GB of RAM, dual-band 2.4/5.0 GHz wireless LAN,
Bluetooth 5.0, Gigabit Ethernet, USB 3.0, and PoE capability (via a separate PoE HAT
add-on). 
The Qt Application runs in the RaspberryPi.
    -[RaspberryPi4B](https://www.raspberrypi.com/products/raspberry-pi-4-model-b/specifications/)

## CAN-BUS (FD) HAT for Raspberry Pi (seeed studio)

Similar to Arduino, RPi 4 also has no in-built CAN Interface. The 2-Channel CAN-BUS(FD) Shield for the Raspberry Pi is based on an MCP2518 CAN FD controller and the MCP2557 CAN FD transceiver, which communicates with the Raspberry Pi via high-speed SPI. It allows to receive data from the Arduino.
    -[CAN-BUS HAT](https://wiki.seeedstudio.com/2-Channel-CAN-BUS-FD-Shield-for-Raspberry-Pi/#install-can-hat)

## Touch Screen Display

It is the display connected with RPi inorder to represent the GUI.
    -[7.9inch DSI LCD, 400*1280](https://www.waveshare.com/wiki/7.9inch_DSI_LCD)


# System Architecture

## Hardware Architecture

<img src=https://github.com/tweetyboo/DES---Instrument_Cluster/blob/main/images/Hardware_Architecture.png style="width:600px;">

## System Architecture

<img src=https://github.com/tweetyboo/DES---Instrument_Cluster/blob/main/images/System_Architecture.png style="width:600px;">

# Docker

Docker is a software platform that allows you to build, test, and deploy applications quickly. Docker packages software into standardized units called containers that have everything the software needs to run including libraries, system tools, code, and runtime. Using Docker, one can quickly deploy and scale applications into any environment and know that the code will run.

The project specific docker container creation is explained in the below link.
 -[Docker](https://github.com/tweetyboo/DES---Instrument_Cluster/tree/main/Docker)

# Results






