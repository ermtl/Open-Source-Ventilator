/*

  Open source Ventilator

  This file contains OEM parameters for the Open Source Ventilator

  GPL V3 Licence
    --------------
             
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

  Author: Eric Vinter.
  Created March 19, 2020
  EMAIL : erv at mailpeers dot net
  
*/

//************************   DEVICE OPERATIONAL PARAMETERS   ************************  

/*
       WARNING : When changing min and max value, manually change the text in the SerialCommand procedure accordingly

       The values are taken from various documents including :
       https://www.gov.uk/government/publications/coronavirus-covid-19-ventilator-supply-specification/rapidly-manufactured-ventilator-system-specification
       https://docs.google.com/document/d/1h77FkKXqPOwVqfOj-PIJSjYX9QiEx69Av2gYuzqZolw/edit#

       some changes have been made to havee a lower default volume in case the machine is used 
       with an infant to prevent damaging their lungs with an adult setting.
*/

#define minBPM                     10     // minimum respiratory speed
#define defaultBPM                 20     // default respiratory speed
#define stepBPM                     1     // adjustment step for respiratory speed
#define maxBPM                     35     // maximum respiratory speed
#define minVolume                 100     // minimum respiratory volume in milliliters 
#define defaultVolume             400     // default respiratory volume in milliliters 
#define stepVolume                 20     // adjustment step for respiratory volume in milliliters 
#define maxVolume                1000     // maximum respiratory volume in milliliters 
#define minCompression             10     // minimum compression for the ambu-bag in cm H2O
#define stepCompression             2     // adjustment step for compression for the ambu-bag in cm H2O
#define defaultCompression         20     // default compression for the ambu-bag in cm H2O
#define maxCompression             80     // maximum compression for the ambu-bag in cm H2O
#define minPEEPCompression          0     // minimum post expiration compression for the ambu-bag in cm H2O
#define stepPEEPCompression         1     // adjustment step for post expiration compression for the ambu-bag in cm H2O
#define defaultPEEPCompression      4     // default post expiration compression for the ambu-bag in cm H2O
#define maxPEEPCompression         25     // maximum post expiration compression for the ambu-bag in cm H2O
#define minApnea                    0     // minimum value (no detection, no alarm)
#define stepApnea                   1     // adjustment step for apnea alarm (in breathing cycles)
#define defaultApnea                2     // default apnea alarm
#define maxApnea                    6     // maximum apnea alarm

#define minsyncRatio                0     // minimum portion of the cycle for breath synchronisation
#define stepsyncRatio               5     // adjustment step for portion of the cycle for breath synchronisation
#define defaultsyncRatio           15     // default portion of the cycle for breath synchronisation
#define maxsyncRatio               50     // maximum portion of the cycle for breath synchronisation
#define minExpirationRatio          1.00  // minimum portion of the cycle for breath synchronisation
#define stepExpirationRatio         0.2   // adjustment step for portion of the cycle for breath synchronisation
#define defaultExpirationRatio      1.2   // default portion of the cycle for breath synchronisation
#define maxExpirationRatio          3.00  // maximum portion of the cycle for breath synchronisation
#define failVolumeRatio             0.80  // In CP mode, Alarm if pressure reached before that portion of programmed volume.
#define defaultCVMode               true  // CV mode by default 

#define ambientPressureFilter       0.02  // IIR filtering ratio (lower value produce longer time constant)
#define avgPressureFilter           0.25  // IIR filtering ratio (lower value produce longer time constant)
#define motorTargetFilter           0.28  // IIR filtering ratio (lower value produce longer time constant)
#define motorReboundFilter          0.05  // IIR filtering ratio (lower value produce longer time constant)
#define volumeAverageFilter         0.3   // value used to filter the the averaged volume
#define defaultPressure          1033.22  // Pressure in cm H2O returned when no sensor is found (1 atm)
#define defaultAmbientPressure   1033.22  // assumed ambient pressure in cm H2O returned when no sensor is found (1 atm)
#define defaultHumidity            50     // humidity in % RH returned when no sensor is found
#define defaultTemperature         20.00  // temperature in °C returned when no sensor is found
#define alarmCompressionValue       1.1   // if the pressure exceeds the presed value * alarmCompressionValue, 
                                          // then trigger an alarm and stop the motor if needed  
#define minAtmosphericPressure    583     // minimum atmospheric pressure that would be considered valid (15000 ft / 4577m)                                          
                                          // Source : https://www.avs.org/AVS/files/c7/c7edaedb-95b2-438f-adfb-36de54f87b9e.pdf
#define maxAtmosphericPressure   1200     // maximum atmospheric pressure that would be considered valid                                           


/*******************************   MOTOR PARAMETERS   *******************************
 *    
 *     These values will be highly dependant on mechanical design.
 *     When modifying the values, always check with an oscilloscope that the movements completes    
 *     without overlaps and without too much idle time. 
 *     Also check that the motor can properly follow the speed acceleration required under load.
 *     Wrong values can cause unpredictables moves and motor stalls.
 *     The worst case scenario is at max BPM, with max volume, max sync period and max ratio
 *     With default parameters, the whole compression can become as short as 250 ms
 */

#define motorSpeed               5000     // Speed for 1 liter/second
#define motorAcceleration        8500     // Acceleration for 1 liter / second (inverse square of flow)
#define motorMaxAcceleration    30000     // deceleration when pressure limit reached
#define motorMinAcceleration      100     // minimum acceleration to prevent stalling / very slow moves
#define motorMaxSpeed            4000     // Max steps per second. 
#define minMotorMove                5     // the motor does not move for less than this distance
#define motorVolumeRatio            0.8   // Ratio of distance in steps to air volume in step per milliliter (after linearity correction).
#define StallTimeout              500     // force move if motor not positionned (safety)

#define alarmDuration             300     // Length of the alarm for each error (around 4 seconds) 


/*******************************   HARDWARE OPTIONS   *******************************
 *    
 *    It's normal for the program to not compile if some of these are undefined as they need an alternative
 *    
 */

#define arduinoNano        // defines the board as an Arduino nano with with Atmega328P processor
                           // You can define your own board and pinout.

#define USBcontrol         // USB / serial Command line interface.
                           // This allows complete control of all parameters.

#define E2PROM             // Uses the internal EEPROM for parameter storage

#define analogKeyboard


#define PCF8574LCDDisplay  // Use a 2x16 characters LCD with a PCF8574 'backpack' as a display

#define PCF8574LCDbargraph // Use a 2x16 characters LCD with a PCF8574 'backpack' for the Led pressure bargraph

//#define TM1638Keyboard     // Use a TM1638 for the keyboard

//#define TM1638Display      // Use a TM1638 for the display

//#define TM1638bargraph     // Use a TM1638 for the Led pressure bargraph

#define activeBeeper       // active beeper can be used on any pin. Passive beeper will require a PWM capable pin

#define Led                // Led debugging / Signal (should be disabled if SPI peripherals are present)

#define BoschBMxSensor     // Bosch Sensortech BMP280 or BME280

//#define BoschBMP180Sensor  // Bosch Sensortech BMP180 

//#define TwoPressureSensors // Double pressure sensor (one for barometric pressure)

#define stepDirMotor       // Control the motor with step and Direction signals

#define disableMotorctrl   // control moter activation / desactivation      

//#define CurrentSense     // uncomment to add motor current sensing with the Allegromicro ACS712 sensor
                           // This is not enabled by default as first test show the sensor is not sensitive
                           // enough. Another sensor should probably be used.

// # define watchdog       // WARNING: Do not enable this setting  while debugging, you might brick 
                           // your dev. board. If this happens, you'll need to flash not just the 
                           // bootloader, but the whole program space.
                           // The problem is worse in Nanos with a bootloader that does not clear
                           // the watchdog settings. 

#define serialTelemetry    // Sends real time data about time, current breathing phase, motor position, pressure 
                           // while the machine works in CSV format

//#define rampUp           // The cycle volume will ramp up when the machine start and regulate to prevent overpressure                           
                           // As of now, this is highly unstable, to be revised.  

#define eepromVersion  100 // version 1.00 of the eeprom data structure

//************************************   DEBUGGING   *************************************

// #define debug_0      // print data for each cycle
// #define debug_1      // print data for user triggered cycle    
// #define debug_2      // print data for 1 in 10 measurement cycles
// #define debug_3      // print sensor datadata 
// #define debug_4      // temperature / humidity / pressure 
// #define debug_5      // See all values read from EEPROM 
// #define debug_6      // Display raw and corrected volumes 
#define debug_7       // makes the alarm sound much shorter
//#define debug_8       // makes the alarm sound much shorter

//#define debugAnalogKeyboard   // Display the actual ADC value for the various keys
//#define I2CCheck              // does an I2C bus scan and reports valid addresses. 
#define debugLed              // Interrupt routine debugging
#define ArduinoPlotter        // This will turn on telemetry automatically and disable other debugs 
                              // With this feature on, absolute pressure and motor speeds are scaled.  
#define debugShortEnter       // When stopping the ventilator, Enter needs to be pressed 2 seconds
                              // and an alarm is sounded. Defining debugShortEnter removes it during debugging
