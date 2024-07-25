Overview

This is a CAN BUS shield for Raspberry Pi(hereinafter referred to as 2 channel pi hat), 2 channel CAN BUS I/O, support CAN FD. 
CAN FD support much faster transmission speed(up to 8Mbps).
Also it have two On-board 120Ω terminating resistors which are controlled by the switches.

Dependencies

1. Open the 1. Open config.txt file

sudo nano /boot/firmware/config.txt

2. Add the following line at the end of the file

dtoverlay=seeed-can-fd-hat-v2

3. Reboot Raspberry Pi

sudo shutdown -r now

4. Check the kernel log to see if CAN-BUS HAT was initialized successfully. You will also see can0 and can1 appear 
   in the list of ifconfig results

dmesg | grep spi

ifconfig -a


CAN Configuration

Set the CAN FD protocol, and the dbitrate can be set to 8M speed

sudo ip link set can0 up type can bitrate 1000000   dbitrate 8000000 restart-ms 1000 berr-reporting on fd on
sudo ip link set can1 up type can bitrate 1000000   dbitrate 8000000 restart-ms 1000 berr-reporting on fd on

sudo ifconfig can0 txqueuelen 65536
sudo ifconfig can1 txqueuelen 65536

Supported Bitrate 125kbps, 500kbps and 1Mbps. 

CAN Message Generation and Recieving

1. Install can-utils

sudo apt-get install can-utils

2. Open two terminal windows and enter the following commands in the Windows to test can fd protocol.

Terminal 1 - Gernerates random CAN message
cangen can0 -mv

Terminal 2 - Shows the recieved CAN message
candump can0

Reference

https://wiki.seeedstudio.com/2-Channel-CAN-BUS-FD-Shield-for-Raspberry-Pi/#install-can-hat
