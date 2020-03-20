# Open-Source-Ventilator
Complete control software for an emergency medical ventilator.

This software is designed to control a Bag Valve Mask (BVM) AmbuBag medical Ventilator as part of the global effort to develop a fast, effective, easily replicable device using distributed digital manufacturing techniques including 3D printing, CNC or laser cutting. The project uses an Arduino Nano with Atmega328P. The software can be used with a wide range of mechanical designs.

Short video presentation / demo : 
<a href="http://www.youtube.com/watch?feature=player_embedded&v=izh4YnG1fPI
" target="_blank"><img src="http://img.youtube.com/vi/izh4YnG1fPI/0.jpg" 
alt="IMAGE ALT TEXT HERE" width="240" height="180" border="10" /></a>
(I'm not a youtuber and English's not my main language, you've been warned !)

# Main components

- The arduino Nano was chosen as a controller for it's ability to perform the realtime tasks with relatively low latency (step generation & data acquisition) and it's universal availability. More sophisticated controllers such as the Raspberry Pi usually have real time issues and unpredictable latency. Also, the Arduino ecosystem is very rich in well tested libraries.

- The Bosch Sensortech BME280 / BMP280 sensor allows precise measurement of the air pressure delivered to the patient. It also allows measurement of the temperature and pressure (BME280 only). The software allows to use either of these. It'a a well known, mass produced, low cost sensor. 
https://www.bosch-sensortec.com/products/environmental-sensors/humidity-sensors-bme280/

- The TM1638 display / LED / keyboard
This little board merges leds, a 8 digits 7 segments display and 8 buttons. It's a very common Chinese board that possesses numerous advantages. It's main drawback is the Old School look of it's display, but does it really matter for this usage ?
The main advantages are that the board is mechanically integrated making mechanical assembly very easy. Connection to the processor uses only 3 pins (Data, Clock, Strobe) The data is clocked and not timing sensitive. The whole board typically costs 1.50$
2x16 character LCDs require a lot of pin connections or, when used with a "backpack", they require a serial interface. Also, the LCD needs a temperature dependant analog adjustment to be well readable. The display is not very bright and can't be read from a distance. That could be a problem when a person has to monitor several heavily infected patients. They also require a separate keyboard. OLED displays are not suited for this device as their active lifetime is limited. color LCDs would be another possibility, possibly with a touchscreen, but it would take longer to develop and require a more powerful processor without adding much to the main function of the device.

- StepStick stepper driver
The stepstick stepper motor drivers are widely available as they are used to drive 3D printer motors.
When used with 24V and a low current/high torque NEMA23 motor, they can offer a very good torque. There are several types depending on the chip they use (A4988 / DRV8825 / Trinamic TMC2209 & TMC5160). The TMC5160 drives up to 4.4Amps and is even suitable for a NEMA24 or NEMA34 motor. All Trinamic drivers include advanced functions such as load  measurement, stall prevention and detection, noiseless operation, etc ...
https://www.trinamic.com/products/integrated-circuits/
Since the software uses industry standard STEP/DIR interface, any external driver may be used to control a stepper, a servo-stepper or a 3 phases servo. A servo would probably be required for a leadscrew design, while a stepper would be more suited to a direct drive design.


To compile the software, you will need the following libraries (all from the library manager) :
```
#include <BME280I2C.h>      // Temperature / humidity / pressure
                            //  by Tyler Glenn - https://github.com/finitespace/BME280
#include <Wire.h>           // I2C protocol
#include <EEPROM.h>         // read / write to the processor's internal EEPROM
#include <TM1638plus.h>     // Keyboard / display / LED combo board
                            //  By Gavin Lyons - https://github.com/gavinlyonsrepo/TM1638plus
#include <AccelStepper.h>   // Stepper / servo library with step pulse / dir interface
                            //  By Mike McCauley - http://www.airspayce.com/mikem/arduino/AccelStepper
#include "TimerOne.h"       // Timer component
                            //  By Jesse Tane, Jérôme Despatis, Michael Polli, Dan Clemens, Paul Stroffregen
                            //  https://playground.arduino.cc/Code/Timer1/
```
# Software Functions

The software is contained in an Arduino "sketch" with many comments, nearly all parameters are in defined constants and can be easily changed. It is herein distributed under the GPL V3.0 licence. The software has the following functions:

- Initialisation with display keyboard and sensor detection.
The software have been designed to allow operation in degraded mode without BME/BMP280 sensor and even without keyboard / display (it will autostart with stored parameters)
During the startup process, detailed debug information is provided through the serial/USB port. 
TBD : remote operation controlled by USB

- User interface
The software manages the display, keyboard navigation in the settings menu and an alarm beeper.
The 8 leds above the keyboard are used as a bar-graph of the measured pressure and a breath cycle indicator.
The 3rd LED represent neutral atmospheric pressure and blinks at the start of each breathing cycle. 1st and 2nd LEDs on the left indicate a low pressure, those on the right indicate a higher pressure. Display is scaled according to the preset pressure.
The menu allows to change the following parameters:

 - Pulse rate
 The pulse rate in breathing cycles per second can be adjusted between 5 and 60 BPM.

 - Breathing volume
 The breathing volume in liter per cycle can be adjusted between 0.1l and 2l. The motor travel and acceleration are changed according to this parameter. There is a yet to be determined limit for high volume at high pulse rate depending on the mechanical design.
 
 - Breathing Pressure 
 The breathing pressure in KPa can be adjusted between 3Kpa and 30Kpa.
The pressure is used to limit the motor speed / travel to prevent exceeding the preset value.
TBD : This feature is not fully implemented yet (but measurements are available and the stepper library allows to alter the motor parameters while it's rotating).
 
 - Save settings
This saves the 3 settings above  and the running state, thus allowing for automatic start upon powerup.

The user interface management is done in a timed interrupt to be responsive and remain functionnal at all times, so that all parameters can be visualised and changed while the device is operating.

When changing the settings, the change will be gradual with each subsequent breath cycle to prevent sudden changes. The variation speed can be defined for all parameters.

- data acquisition.
Data is acquired from the BME/BMP280 sensor every 100ms. the acquisition includes filtering of the pressure 

# Required hardware

- Arduino Nano
  Any arduino Nano or clone can be used, as long as it contains an Atmega328 processor
  https://www.aliexpress.com/item/32845440152.html
- Bosch Sensortech BME280 / BMP280 sensor breakout
  Warning, there are 3.3V breakout and 5V breakout boards. Some have 4 pins, others 6 pins.
  They can all be used. Connect the supply accordingly, the 2 extra pins are unused (SPI)
  https://www.aliexpress.com/item/32672562548.html
- TM1638 Display / Leds / Keyboard combo module
  They all have the pin header on top, you can remove the plastic part and bend the pin for side connection
  There are other types of boards with TM1638 chip with 16 buttons, but they lack the LEDs that are important to monitor air pressure measurement.
  https://www.aliexpress.com/item/32807348409.html
- Beeper
  This active beeper is easy to drive and the sound level is mild. 
  https://www.aliexpress.com/item/1893768547.html
  Here is a louder one.
  https://www.aliexpress.com/item/32671465979.html
- Regulator
  Even at 12V, the motor can generate spikes that will damage the controller's regulator. 
  These regulators are cheap, and rated for high voltage (max 50V). 
  The potentiometer needs to be adjusted for 7.5V to 8V
  https://www.aliexpress.com/item/1859113573.html
- Driver and motor.
  The driver and motor required will depend on the mechanical design used.
  here are a few :
  Cheapest, but rugged A4988 Stepstick:
  https://www.aliexpress.com/item/1851964212.html
  DRV8825 StepStick
  https://www.aliexpress.com/item/1937880897.html
  TMC2209 StepStick (silent, with torque detection). 
  Could approximate a pressure measurement for a setup without pressure sensor.
  https://www.aliexpress.com/item/33029587820.html
  TMC5160 StepStick (by far the best option, 4.4A/phase, can drive NEMA34 steppers)
  https://www.aliexpress.com/item/33051621409.html

  For direct drive, a large Nema23 motor with low current and high torque would be best. 
  these motors quickly lose torque as their speed increases, but in this application, 
  their performance should be a good match.
  This one is rated for 1.9NM with only 2.12A (bipolar mode)
  https://www.aliexpress.com/item/4000032106569.html

  For designs using a ballscrew, a servomotor is required to get the necessary speed.
  Here is a good, not too expensive option with integrated controller:
  180W 3000 RPM version https://www.aliexpress.com/item/4000333220649.html
  100W 3000 RPM version https://www.aliexpress.com/item/32945533935.html

# versions

  0.10 - Initial public version
  
  0.11 - Add interactive terminal command line complete control (through USB link)
          A minimal device without screen or keyboard is possible with the same software.
