//*******************************   REQUIRED LIBRARIES   *******************************

#ifdef TM1638                      // Keyboard / display / LED combo board
#include <TM1638plus.h>            //  By Gavin Lyons - https://github.com/gavinlyonsrepo/TM1638plus
#endif                      


#ifdef PCF8574LCDDisplay           // LCD with PCF8574 "backpack"
#include <LiquidCrystal_PCF8574.h> // Matthias Hertel - http://www.mathertel.de/Arduino/LiquidCrystal_PCF8574.aspx 
#endif

       
#ifdef I2C
//#define jm_Wire                 // The jm_wire library corrects the longstanding I2C hanging problem with the arduino Wire library.
#ifdef jm_Wire
#include <jm_Wire.h>              // I2C protocol, contains a workaround for a longstanding issue with the Wire library
                                  // by Jean-Marc Paratte - https://github.com/jmparatte/jm_Wire
                                  // The library is optional during developpement (unless you encounter lockups)
                                  // but must be used for any production software. To use it, you need to change
                                  // the #include "Wire.h" line in all libraries that use the I2C bus.
                                  // INFO:
#else                             //  https://github.com/ermtl/Open-Source-Ventilator/blob/master/OpenSourceVentilator/README.md 
#include <Wire.h>                 // I2C protocol (flawed library)
#endif
#endif

// Add your sensor library routines here 

#ifdef BoschBMxSensor           // Temperature / humidity / pressure
#include <BME280I2C.h>          //  by Tyler Glenn - https://github.com/finitespace/BME280
#define tempSensor              // contains a temperature sensor
#define humiditySensor          // contains a humidity sensor
#define pressureSensor          // contains a pressure sensor
#endif               

#ifdef BoschBMP180Sensor        // Temperature / pressure
#include <BMP180I2C.h>          //  by Gregor Christandl - https://bitbucket.org/christandlg/bmp180mi/src/master/
#define tempSensor              // contains a temperature sensor
#define ambientPressureSensor   // contains a pressure sensor
#endif               



#ifdef currentSense
#include <avr/sleep.h>          // Sleep management library
#endif

#ifdef E2PROM
#include <EEPROM.h>             // read / write to the processor's internal EEPROM
#endif

#ifdef StepGen
#include <AccelStepper.h>       // Stepper / servo library with step pulse / dir interface
#endif                          //  By Mike McCauley - http://www.airspayce.com/mikem/arduino/AccelStepper

#ifdef  watchdog
#include <Adafruit_SleepyDog.h> // watchdog library by Adafruit   
                                // https://github.com/adafruit/Adafruit_SleepyDog
#endif

#include "TimerOne.h"           // Timer component
                                //  By Jesse Tane, Jérôme Despatis, Michael Polli, Dan Clemens, Paul Stroffregen
                                //  https://playground.arduino.cc/Code/Timer1/
