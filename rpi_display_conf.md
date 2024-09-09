Physical Interface Instructions:

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
Link to refer the display configuration:

https://www.waveshare.com/wiki/7.9inch_DSI_LCD
