# AerosolTools
Arduino/Teensy components to run devices necessary for aerosol measurements at Oklahoma State University.

## Code
SDP6_V1.0.ino is the Arduino file that executes datalogging over three SDP610 sensors. Formatted to run on Teensy 3.6/4.1.

## Libraries
### Standard TeensyDuino Libraries (as of TeensyDuino 1.8.5, if at or above these are not needed)
SD.zip and SPI.zip are required to access SD card info, including to write data.

Wire.zip is required to handle I2C data reads, in particular for 3 I2C buses (Teensy 3.6/4.1)

### Sensor Libraries
Sensirion_SDP6x.zip is required to handle data communication with the SDP610 sensors. Inside are three variants of the base library, each used to operate a sensor on a different I2C bus.
