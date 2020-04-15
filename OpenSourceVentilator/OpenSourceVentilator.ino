/*

  Open source Ventilator


    0.10 Initial public version

    0.11 Add interactive terminal command line complete control (through USB link) 
         A minimal device without screen or keyboard is possible with the same software.

    0.12 Add "Assist Control" mode with adaptive negative pressure trigger cycle. 
         This mode was requested by a MD : "ER doc here chiming in, hopefully 
         I can answers some questions or clarify minimal requirements.The basic 
         mode you need is Assist Control. This mode provides a breath whenever 
         the patient triggers a breath (by inhaling and creating negative pressure 
         past a certain threshold) or whenever the patient is due for a breath 
         (based on the set rate). The breath it provides are either a fixed volume 
         (and the doc monitors the pressures delivered) or a fixed pressure (and 
         the doc monitors the volume delivered). AC/volume is often called Volume 
         Control, and AC/pressure is often called Pressure Control. While I do not 
         have direct experience ventilating COVID patients yet, these 2 modes will 
         cover 99% of ICU cases." 
         This version also adds editing, permanent storage and USB setting of the new 
         Patient Synchronization parameter and an audible alarm when pressure remains 
         above 120% of preset pressure for 1 second or mode.

    0.13 This version is reorganized for better code modularity, asymmetric breathing
         and bug fix. It is a major reorganization of the code done to get a more 
         modular software, ready for bringing in other sensors, a different user 
         interface and ports to other controller architectures. Very long functions 
         are broken down into smaller functionnal blocks that should be much easier 
         to understand and modify, each global variable is commented. 
         This version also implements most of the guidelines in the UK minimal emergency 
         ventilator device specifications. Namely, all variable limits have been tweaked 
         and the variable Inspiratory:Expiratory ratio is fully functionnal. 
         A bug that allowed floating point variables retrieved from a garbage filled EEPROM 
         to bypass the checks leading to malfunction have been corrected (NaN detection).

    0.14 This version include proper timing, speed and acceleration for the motor control.
         The pressure controlled and volume controlled modes are implemented.

    0.15 Minor version change.
         This version allows the replacement of the buggy Wire.h arduino library  (can hang
         the controller) with acorrect version known as jm_Wire
         https://github.com/ermtl/Open-Source-Ventilator/blob/master/OpenSourceVentilator/README.md
         'Wire.h' is still the default library to prevent compiler errors.
         The processor's hardware watchdog can now be enabled (off by default, use with care, you
         risk bricking your processor. 
         Modularisation is getting better (work in progress)

    0.16 Double pressure sensor, breath phases modularization
         This version can use 2 absolute sensors (or still use just 1, configurable) to
         constantly monitor the ambient air pressure. This is important for patients that 
         are heliported and for patients placed in a negative pressure room, 2 situations
         where the ambient pressure can change rapidly.
         The 2 sensors together will behave as a differential pressure sensor.
         Since pressure is a critical data, several safeguards and coping strategies have 
         been added to make sure data from the sensor is accurate and there is a failsafe.
         Things such as randomly connecting/disconnecting sensors are non blocking and behave 
         as expected.
         this feature requires the #define jm_Wire to be uncommented.Please see the link in
         the description for version 0.15 above

         Also, as requested, the various phases and functions managing the breathing cycle 
         itself have been modularized to allow third parties to implement more sophisticated
         control strategies.

         Finally, the processor's program memory is filing up quickly as the control becomes 
         more complex. There is still opportunities for important optimizations, but the base 
         version will have to give up some non essential functions to fit in a regular Arduino
         Nano (such as the verbose serial interface or the ability to manage both the native
         keyboard / display and the USB serial command line interface.

         This version barely fits with the USB commands on, so they've been deactivated.
         You can get them back by uncommenting the #define USBcontrol line.

         The following versions will both target the Nano and the Mega with Atmega2560 processor
         for 8 times the available program memory.

    0.17 2x16 LCD Display - Telemetry
         
         The ability to use an alphanumeric 2 lines * 16 characters LCD display had been
         requested, it will now be fully supported. Since this LCD uses lots of IO pins, 
         the retained version is connected to a PCF8574 "Backpack" board to make it I2C 
         compatible. The LCD+the backpack are sold online for around 2.50$.
         The TM1638 display is still supported as a minimal option.
         A good side effect is that the library is lean and so USB/serial terminal control
         is back by default.
         At this point, the keyboard and bargraph are still done with the TM1638, but a
         simple button keyboard will be supported very soon.
         Telemetry will send information in real time about the breathing cycle for further 
         data analysis and / or graphical display !
         This will be very very useful for people developing mechanical solutions, and 
         doing data analysis to develop motor position <=> volume mapping
         Data is sent in common CSV format, around 1 data point every 20 millisecond
         The "T" command, sent through the serial port toggles telemetry on or off.

         every data point includes :
         - Time since the breathing was activated.
         - Breathing cycle phase (more phases might be added in the future)
           0 : Start of new cycle
           1 : Inspiration
           2 : Start of expiration
           3 : Expiration
           4 : Patient breathing initiation search
           5 : Patient initiated a breathing cycle
          - Motor position (in step) Travelled distance depends on driver microstepping
                                     and driving mechanism.
          - relative pressure in cm H2O
          In addition to these data points, during phase 0 (start of cycle), the following 
          cycle data is sent:
          - Ambient pressure (mBar)
          - Breathing volume per cycle in ml
          - Motor destination / target (steps)
          - Breathing length (in seconds)
          - Breathing in duration (in seconds)
          - breathing in top speed (in steps/second)
          - Motor acceleration during inspiration (in steps per second squared)
          - Breathing out duration (in seconds)
          - breathing out top speed (in steps/second)
          - Motor acceleration during expiration (in steps per second squared)

          
    0.20 Realtime graphics, Keyboard, trajectory
         
         This version adds realtime graphics, compatible with the Arduino serial plotter.
         This is an extension of the telemetry in Version 0.17, completed and scaled properly
         for display.
         It shows 2 series of values. The first is updated in realtime (every 20ms) and shows 
         the breathing cycle itself, the commanded motor position, it's actual movement, the 
         pressure sensor output and alarm number.
         The second set is updated once per cycle and shows various values linked to timing, 
         speed, acceleration for various parts of the cycle.
         This data should be extremely useful for developping mechanical prototypes and shows
         the ability to use a remote HMI if desired.
         define ArduinoPlotter for proper visualisation scaling, comment it for real telemetry data 
         This version also add a keyboard as an alternative to the TM1638 keyboard.
         The keyboard used is made with 5 buttons all connected to a single analog input for
         simpler cabling and preserving controller pins.
         https://electronics.stackexchange.com/questions/101409/how-to-debouce-six-buttons-on-one-analog-pin-with-arduino
         The keyboard I use have the following values:
         - 10K to ground
         - 'Enter' button connected to +5V
         - 'Prev'  button connected with a 22Ko resistor
         - 'Next'  button connected with a 10Ko resistor
         - 'Up'    button connected with a 3.6Ko resistor
         - 'Down'  button connected with a 1Ko resistor
         The resulting analog values are stored in the KbdVals[] array, and value tolerance in KbdTols[] array
         If you use different resistor values, uncomment debugAnalogKeyboard to get the analog values to fill the 
         array values.
         Please note that when plotting the graph, while the controller would accept serial data input,the arduino plotter 
         does not allow it, so data needs to be changed using the physical keyboard.
         A 3D printed faceplate containing the display, the buttons and the buzzer is available (including FreeCAD drawing)
         in the 3D-Print directory .
         While graphing the data, a few corrections were made to the trajectory engine. 

    0.21 Pressure unit converted to cm H2O / minor changes
         
         - This version implements the requested change in pressure units as the medical community uses cm H2O and the sensors use Pa.
         - The 'Up' and 'Down' keys now have an auto repeat.
         - When the ventilator is active and 'Enter' is used to stop it, the key press must be pressed 2 seconds and an alarm is sounded.
         - The data dispay includes ambiant pressure (can be used to check negative pressure rooms)
         - It now uses the updated TM1638 library (Version 1.4+), you need to update or you'll have a compile error.
         - #define I2CCheck : displays a list of the I2C devices found upon startup (only for debug or for external HMI)

    0.22 Misc. changes

         - PEEP parameter added
         - Apnea alarm parameter added
         - Display PIP and PEEP in real time (in progress)
         - The bargraph (pressure in VC mode, volume in PC mode) is now displayed on the LCD
         - New automatic motor speed limit to prevent interrupt overflow (interrupt limited reentrancy)
         - reorganisation of items stored in EEPROM. They are now included in a struct and their name starts with ee.
           This will allow easy expansion of the variables stored in EEPROM and the way to treat them as a whole.
         
    0.23 Bug fix with TM1638 display
         When using TM1638 for the display, the menu was broken and displayed an error message.


    0.3 Complete ventilator release with plans, 3D prints and CNC 
     - Complete ventilator release with plans, CAD for 3D prints and CNC
     - non linear relation between air volume and motor target position (allows for better adaptation to various non linear mechanisms).
       defining debug_6 allows to check the raw / translated values.
     - fix for HD44780 occasionnal display garbling (I2C bus error recovery), requires patched LiquidCrystal_PCF8574 library
     - fix for AccelStepper library being too slow to drive microstepped motors, requires patched AccelStepper library
     - there is now a separate "OpenSourceVentilator.h" file that contains OEM parameters. This prevents custom changes from being 
       reverted with each new version.
     - First version of proportional pressure control  
     - Now displays actual volume + actual BPM while breathing
     - bug fixes in EEPROM save and restore functions
     - bug fix for a mistake that delayed breathing restart
     - use of function and unsigned long to prevent millis() rollover problems

         

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

#include "OpenSourceVentilator.h"

//******************************   IMPLIED DEFINITIONS  ********************************

#ifdef BoschBMxSensor
#ifndef I2C
#define I2C
#endif
#endif

#ifdef BoschBMP180Sensor
#ifndef I2C
#define I2C
#endif
#endif

#ifdef PCF8574LCDDisplay
#ifndef I2C
#define I2C
#endif
#define dispMenus         // Alternate displays must also enable this
#else
#undef PCF8574LCDbargraph // can't display bargraph on the LCD if no display on the LCD
#endif

#ifdef  TM1638Display
#ifndef TM1638
#define TM1638
#endif
#define dispMenus         // Alternate displays must also enable this
#endif

#ifdef  TM1638Keyboard
#define TM1638
#define isKeyboard        // Alternate keyboards must also enable this
#endif

#ifdef analogKeyboard
#define isKeyboard        // Alternate keyboards must also enable this
#endif

#ifdef  TM1638bargraph
#ifndef TM1638
#define TM1638
#endif
#define dispBargraph     // Alternate physical (LED) bargraphs must also enable this
#endif

#ifdef  PCF8574LCDbargraph
#define dispBargraph     // Alternate physical (LED) bargraphs must also enable this
#endif

#ifdef  activeBeeper
#define Beeper
#endif

#ifdef  watchdog
#define watchdogProtect          // This prevents the watchdog from being fired before watchdogDelay  
                                 // To allow an new software to be uploaded     
#define watchdogDelay      1000  // Maximum time before the watchdog times out and resets 
#define watchdogStartDelay 8000  // Delay before the watchdog becomes active (used to have the time
                                 // to reload the program if it hangs the controller. Only for development.
#endif


#ifndef analogKeyboard
#undef debugAnalogKeyboard
#endif

#ifdef debugAnalogKeyboard
#undef serialTelemetry
#undef ArduinoPlotter
#endif

#ifdef debug_7
#undef alarmDuration
#define alarmDuration 6
#endif


#ifdef ArduinoPlotter            // The plotter needs telemetry data
#ifndef serialTelemetry
#define serialTelemetry
#endif
#define arduinoPlotterPressureOffset 800  // Y axis offset for the pressure curve
#define arduinoPlotterOffset         1200  // Y offset for the data displayed once per cycle
#define arduinoPlotterMotorOffset    25   // Y offset for the motor position (prevents shifts if undershoot)
#undef debug_0
#undef debug_1
#undef debug_2
#undef debug_3
#undef debug_4
#undef debug_5
#undef debug_6
#undef debug_8
#endif

//********************************   CONNECTION PINS   ********************************

#ifdef arduinoNano // This configuration applies to an Arduino Nano controller with Atmega328P processor 

#ifdef TM1638
// Datasheet : https://csserver.evansville.edu/~mr63/Courses/Projects/TM1638en.pdf

#define  pin_Strobe_TM         2
#define  pin_Clock_TM          3
#define  pin_DIO_TM            4
#endif

#ifdef analogKeyboard
#define pin_analogKeyboard    A6
#endif

#ifdef I2C
#define pin_SDA               A4
#define pin_SCL               A5
#endif

#ifdef Beeper
#define pin_Beep               8
#endif

#ifdef Led
#define pin_LED               13
#endif

#ifdef stepDirMotor
#define pin_Stepper_DIR        6
#define pin_Stepper_Step       7
#define StepGen
#endif

#ifdef disableMotorctrl
#define pin_Stepper_Disable    5
#endif

#ifdef CurrentSense
#define pin_current_Sense     A3  // do not use A6 or A7 presence detection needs a regular pin with pullup
#endif

#endif

//*******************************   REQUIRED LIBRARIES   *******************************

#ifdef TM1638                      // Keyboard / display / LED combo board
#include <TM1638plus.h>            //  By Gavin Lyons - https://github.com/gavinlyonsrepo/TM1638plus
#endif                      


#ifdef PCF8574LCDDisplay           // LCD with PCF8574 "backpack"
#include <LiquidCrystal_PCF8574.h> // Matthias Hertel - http://www.mathertel.de/Arduino/LiquidCrystal_PCF8574.aspx 
#define LCDReset
#endif

       
#ifdef I2C
#define jm_Wire                 // The jm_wire library corrects the longstanding I2C hanging problem with the arduino Wire library.
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


//***************************************   END   ***************************************

#ifdef TM1638Keyboard
#define maxBtn  8   // should be 8 or 16
#define btnPrev 7   // position for the 'Prev'  button
#define btnNext 6   // position for the 'Next'  button
#define btnDn   5   // position for the 'Down'  button
#define btnUp   4   // position for the 'Up'    button
#define btnOk   0   // position for the 'Enter' button
#define debounce 3  // Keyboard debouncing
#endif

#ifdef analogKeyboard
#define maxBtn  8   // should be 8 or 16
#define btnPrev 3   // position for the 'Prev'  button
#define btnNext 4   // position for the 'Next'  button
#define btnDn   6   // position for the 'Down'  button
#define btnUp   5   // position for the 'Up'    button
#define btnOk   7   // position for the 'Enter' button
#define debounce 3  // Keyboard debouncing
const int KbdVals[] = {335,510,765,934,1022};
const int KbdTols[] = {17,26,39,47,52};
#define btns    5
#define analogTol 0.05 // percentage of allowed error
#endif

#ifdef isKeyboard
#define keyRpt1  60  // Number of ticks before first key repeat
#define keyRpt   30  // Number of ticks between subsequent key repeats
#ifdef debugShortEnter
#define okLong   30  // short value while debugging (at least keyRpt)
#else
#define okLong  120  // around 2.5 seconds 
#endif
#endif



#ifdef TM1638Display
#ifndef PCF8574LCDDisplay
#define dispBufferLength 12
#endif
#endif
#ifdef PCF8574LCDDisplay
#define dispBufferLength 40
#endif

#ifdef TM1638
TM1638plus tm(pin_Strobe_TM, pin_Clock_TM , pin_DIO_TM);  //Constructor object
#endif

#ifdef PCF8574LCDDisplay
LiquidCrystal_PCF8574 lcd(0x27); // set the LCD address to 0x27 for a 16 chars and 2 line display
#endif


#ifdef BoschBMxSensor
// Connecting the BME280 Sensor:
// Sensor              ->  Board
// -----------------------------
// Vin (Voltage In)    ->  3.3V
// Gnd (Ground)        ->  Gnd
// SDA (Serial Data)   ->  A4 on Uno/Pro-Mini, 20 on Mega2560/Due, 2 Leonardo/Pro-Micro
// SCK (Serial Clock)  ->  A5 on Uno/Pro-Mini, 21 on Mega2560/Due, 3 Leonardo/Pro-Micro

BME280I2C bme;    // Default : forced mode, standby time = 1000 ms
                  // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,
#endif


#ifdef BoschBMP180Sensor
#define BMP180_I2C_ADDRESS 0x77
 BMP180I2C bmp180(BMP180_I2C_ADDRESS); //create an BMP180 object using the I2C interface
#endif


#ifdef StepGen
// Define the stepper and the pins it will use
AccelStepper stepper(AccelStepper::DRIVER, pin_Stepper_Step, pin_Stepper_DIR);
#endif

#ifdef jm_Wire
extern uint16_t twi_readFrom_timeout; // Allows to add a longer timeout for I2C reads
extern uint16_t twi_writeTo_timeout;  // Allows to add a longer timeout for I2C reads
extern bool twi_readFrom_wait;        // Must be set to true to activate the timeouts
extern bool twi_writeTo_wait;         // Must be set to true to activate the timeouts
#endif

#define SERIAL_BAUD 115200  // Serial port communication speed

#ifdef E2PROM
#define eeStart  48         // EEPROM Offset for data
#endif 

#ifdef CurrentSense
 #define current0          512        // Nominal zero point of the current sensor is VCC/2
 #define currentMaxIdle     50                  // Analog maximum deviation when the motor is idle (including sensor midpoint error)
 #define analogCurrentRatio  0.07398       // Value in Amp per analog step
 #define idleCurrentFilter   0.002  // IIR filtering ratio (lower value produce longer time constant)
 #define currentFilter       0.05   // IIR filtering ratio (lower value produce longer time constant)
#endif

#ifdef disableMotorctrl
 #define motorDriverRecovery 20  // Allow some time (in miliseconds) for the motor driver to recover after being idle
#endif

#define std_dly 70  // display standard delay



//#define serialVerbose   // Should the messages sent to the serial port be verbose or not

#define samplePeriod 20  // 20 ms sensor sampling period
#define highPressureAlarmDetect 10  // delay before an overpressure alarm is triggered (in samplePeriod increments)
#define delayAmbientPressure 2500  // In dual pressure sensor mode, delay when the main ambient pressure sensor fails
                                   // before we start hunting for ambient pressure value on the second sensor  

#define breathPhaseStart           0     // In this phase, all the data for the next breathing cycle are calculated
#define breathPhasePlanedBreathIn  1     // In this phase the motor movement starts  with the programmed values for as long as the planned pressure is not reached
#define breathPhaseStartBreathOut  2     // In this phase, the breath out is calculated and started
#define breathPhasePlanedBreathOut 3     // In this phase the motor movement starts  with the programmed values for as long as the motor position is above 0
#define breathPhasePatientTrigger  4     // In this phase, the patient has inspired (rapid pressure drop)) thus starting a new assisted cycle 

#define startup_message       0    // power up start
#define error_message        10    // 10 + error code
#define Menu_message        100    // screen for each menu item 
#define Measurement_Start   101    // Screen with "start" message
#define Stop_message        102    // stop indicator screen  
#define Measurement_Message 103    // first of measurement messages
#define  RATE_PEEP           98    // Display current values for Cycle rate and PEEP
#define  VOL_PIP             99    // Display current values for Volume and PEEP

#define MenuItems             9
#define keyIdleReturn     16000    // returns to main menu after 16 seconds without key press
#define motorOffDelay     10000    // Motor is cut off after that time being inactive.
#define interruptMaxNest      4    // delay steps to prevent a stack runaway

#define PaToCmH2O 0.010197162129779282 // Conversion coefficient


boolean term,                  // true if the terminal is in human mode, false in machine mmode (not fully implemented)
        stopMove;              // Immediately stop motor movements (E-Stop)
int     dispDelay,             // delay before the next display update or change
        dispPhase,             // Main variable that controls the display and menu start machine. Contains the current screen number
        dispParam,             // additional parameter used on certain display screens
        dispCnt,               // Counter for dispDelay
        breathGraphCnt,        // Counter for the visual blinking of the bargraph at each breathing cycle start
        beepCnt,               // Counter for the delay before the end of the current beep (or 0). Time is in 10ms increments
        timerLvl,              // Count timer interrupt nesting and skips interface update when more than one
        syncDelay,             // Time (in ms) when the patient can symchronize the breathing cycle by inhaling (negative pressure)
        highPressureAlarmCnt;  // delay when the pressure is too high (>1.2x preset value) before an alarm is triggered
byte    timCnt;                // Timer interrupt cycle counter used to spread the load over time
boolean doDisp,                // indicates if a display update is required or not.
        sensHumidity,          // True if a humidity sensor is present, false otherwise
        sensTemperature,       // True if a temperature sensor is present, false otherwise
        sensPressure,          // True if a pressure sensor is present, false otherwise
        sensAmbientPressure,   // True if a separate ambient pressure sensor is present, false otherwise
        sensFlow,              // True if an airflow sensor is present, false otherwise (not used yet)
        Mactive,               // active memory (transition detection  
        motorState,            // True if the motor is activated, false otherwise, false otherwise
        rebound,               // true if the motor target increrases again after going downn
#ifdef  watchdogProtect        
        watchdogStarted,       // True when the watchdog has been activated. the first few seconds after reset are without watchdog protection
#endif        
        BMP180Phase,           // Indicates if the expected value is a temperature or a pressure
        I2CBusy,               // Tells whenever the sensors are used in the main lop so that the display can wait.
        telemetry,             // true if the serial telemetry mode is active, false otherwise.
        debug;                 // True if a debugging mode is used, false otherwise
#ifdef dispMenus     // Alternate displays must also enable this
char    disp[dispBufferLength];// Display data buffer
#endif        
float   pressure,              // Current pressure, as read from the sensor
        peakPressure,          // high pass filtered value of the pressure. Used to detect patient initiated breathing cycles
        avgPressure,           // Averaged pressure (used to limit the motor speed with short spikes filtered out
        relPressure,           // relative pressure (measured pressure - ambient pressure)
        presM1,presM2,         // Memoies used to extrapolate pressure values when a new sensor measurement is not available
        temperature,           // temperature as read by the sensor, in Celsius
        temperature2,          // temperature as read by the 2nd sensor, in Celsius (if available)
        breathInSpeed,         // Speed of the breath inhalation in liter / second
        breathInAcceleration,  // Acceleration in step squared/second      
        breathOutSpeed,        // speed of the breath exhalation in liter / second
        breathOutAcceleration, // Acceleration in step squared/second      
        expectedPressurebyStep;// amount of pressure for each step of the motor to reach programmed pressure at the end of motion.
        
unsigned long
        tick,                  // counter used to trigger sensor measurements
        breathe,               // counter used to track the current breathing cycle
        lastKey,               // last time when a key was pressed (used to automatically return to the main menu) 
        lastValChange,         // Last time the PIP or PEEP changed (used for display)
        lastCycleStart,        // Time the previous cycle was started (used to compute real duration)
        commTime,              // Last time a serial communication was received
        lastactive,            // Last time the motor was active
        startactive,           // last time the motor was started
        lastAmbientPressure,   // last time a valid ambient pressure was collected
        lastLCDReset;          // Last time the LCD display was reset (HD44780 quirk)
long    breathLength,          // duration of the current breathing cycle in milliseconds. This is 60000/BPM.
        breathTime,            // Length of cycle minus the sync period
        breathIn,              // Length of the breath inhalation
        breathOut;             // Length of the breath exhalation
float   motorTargetFlt,        // Exprected motor position at the end of the cycle (filtered value)          
        volumeAverage;         // average of the past breathing cycles volume (after regulation)
int     motorTarget,           // Exprected motor position at the end of the cycle
        motorInitialTarget,    // Exprected motor position at the end of the cycle as calculated at the start of the cycle
        pipValue,              // Peak pressure
        peepValue,             // End of cycle pressure 
        BPMvalue,              // Actual measured BPM
        volumeValue,           // actual measured volume for the last cycle
        displayPip,            // Peak pressure
        displayPeep,           // End of cycle pressure 
        displayBPM,            // Rate of the last cycle 
        displayVol,            // Volume of the last cycle
        volume,                // Breathing volume per cycle in ml
        volumePos,             // Breathing volume per cycle with nonlinearity correction
        compression,           // Max / target compression while inhaling
        humidity,              // humidity as read by the sensor, in % RH
        BPM;                   // beats Per Minute. This is the minimum breathing rate, as defined by the user        
float   
        
        compressionScale,      // Scale of the compression relative to set value
        volumeScale;           // Scale of the volume relative to set value
byte    sc,                    // counter for displaying debugging info about the sensors 
        breathPhase,           // Current phase of the breathing cycle;
        barGraph,              // segments of the pressure bar graph
        bargraphDot,           // rate indicator blinking segment on the bargraph
        menuItem,              // Menu item currently being set by the user
        alarmNum,              // 0 normal, else alarm number
        dispTick;              // Auxilary value for the dislpay. Can be used as a blink counter or similar 
uint16_t mBargraph;            // previous display on the bargraph
 
#ifdef isKeyboard        
byte    keys[maxBtn],          // Debounced keyboard keys
        kcnt[maxBtn],          // Keyboard debounce counter
        mkeys[maxBtn];         // Key memory (allows to detect button transitions)
int     okDelay;               // Delayed action for stopping the ventilator 
#ifdef analogKeyboard
int    analogKeys;            // Buffered analog value for keyboard
#endif
#endif

// Parameters saved to / recovered from EEPROM


typedef struct
  {
   int     eeVersion;              // version of the data (allows resetting if incompatible software is uploaded 
   int     reqBPM;                // respiratory frequency
   int     reqVolume;             // respiratory volume in milliliters 
   int     reqCompression;        // compression for the ambu-bag in cm H2O
   byte    syncRatio;             // portion of the cycle for breath synchronisation
   float   expirationRatio;       // The proportion of each breathing cycle that is spent breathing in compared to breathing out
   boolean CVmode;                // CV or CP mode indicator; 
   boolean active;                // True if the machine is currently doing breathing cycles, false otherwise
   float   ambientPressure;       // Calculated ambient air pressure (averaged)
   int     peep;                  // Minimum post expiratory pressure (PEEP)
   byte    apnea;                 // Apnea detector (0 = off, 1 to 6)
  } EEPROM_Data;


EEPROM_Data ee;

#ifdef  CurrentSense
int     currentVal;            // ADC result of the current measurement
boolean sensCurrent;           // True if a current sensor is present, false otherwise
float   idleCurrent,           // Averaged value of the ADC when the motor is idle
        currentRaw,            // unfiltered, scaled current value 
        current;               // filtered, scaled current value 
#endif
 

void (* resetFunction)(void) = 0;  // Self reset (to be used with watchdog)

void  __attribute__ ((noinline)) tab ()
{
 if (term) Serial.print('\t');
}

void  __attribute__ ((noinline)) sep ()
{
 if (term) Serial.print(',');
}

boolean checkMillis (long last,long interval)
{
  return ((unsigned long)(millis() - last) >= interval);
}


boolean checkValues()
{
 boolean isOk=(ee.reqBPM>=minBPM);                      // BPM in allowed range ?
 if (ee.reqBPM>maxBPM) isOk=false;                 
 if (ee.reqVolume<minVolume) isOk=false;                // Volume in allowed range ?
 if (ee.reqVolume>maxVolume) isOk=false;           
 if (ee.reqCompression<minCompression) isOk=false;      // Compression in allowed range ?
 if (ee.reqCompression>maxCompression) isOk=false; 
 if (ee.peep<minPEEPCompression) isOk=false;            // Post expiratory compression in allowed range ?
 if (ee.peep>maxPEEPCompression) isOk=false; 
 if (ee.syncRatio<minsyncRatio) isOk=false;             // portion of the cycle for breath synchronisation in allowed range ?
 if (ee.syncRatio>maxsyncRatio) isOk=false;
 if (ee.expirationRatio<minExpirationRatio) isOk=false; // portion of the cycle for breath synchronisation in allowed range ?
 if (ee.expirationRatio>maxExpirationRatio) isOk=false;
 if (ee.ambientPressure<minAtmosphericPressure) isOk=false; // Ambiant pressure in allowed range ?
 if (ee.ambientPressure>maxAtmosphericPressure) isOk=false;
 if (ee.apnea<minApnea) isOk=false;                         // Apnea alarm in allowed range ?
 if (ee.apnea>maxApnea) isOk=false;
 if (ee.eeVersion!=eepromVersion) isOk=false;               // Error if not the right EEPROM data version
 if (isnan(ee.expirationRatio)) isOk=false;  
 if (isnan(ee.ambientPressure)) isOk=false;  
 return isOk;     
}

void meas()
{
#ifdef serialVerbose 
 Serial.print(F("Temp:")); 
 Serial.print(temperature);
 Serial.print(F("°C\tHumidity:")); 
 Serial.print(humidity);
 Serial.print(F("% RH\tPressure:")); 
 Serial.print(pressure);
 Serial.print(F(" Pa\tAmbient Pressure:")); 
 Serial.print(ee.ambientPressure);
 Serial.print(F(" Pa\tRelative Pressure:")); 
 Serial.print(relPressure);
 Serial.println(F(" Pa"));  
#else
 Serial.print(temperature);
 tab();
 Serial.print(humidity);
 tab();
 Serial.print(pressure);
 tab();
 Serial.print(ee.ambientPressure);
 tab();
 Serial.println(relPressure);
#endif 
}


void setDisplayMenu(int ph, int dly=std_dly, int param=0) 
{
 dispDelay=dly;   // duration for which the info will be displayed
 dispCnt=0;       // resets the display delay
 dispPhase=ph;    // which page should be displayed
 dispParam=param; // Page dependant additional parameter
 dispTick=0;      // Can be used as a blink counter or similar 
}


/*
 *   displays a set of bars representing the scaled pressure onscreen or with Leds
 */

void displayBargraph (uint16_t value)
{
 if (value!=mBargraph) // don't overwrite if same value
  { 
    mBargraph=value;
// add alternate ways of displaying the bargraph here

#ifdef PCF8574LCDbargraph // Dynamically redefine the bargraph

#define bar 30
int defchar[8];
    memset(defchar,0,sizeof(defchar));
    if (value & 128) defchar[1]=bar;
    if (value & 64) defchar[3]=bar;
    if (value & 32) defchar[5]=bar;
    if (value & 16) defchar[7]=bar;
    lcd.createChar(1, (int*)defchar);
    memset(defchar,0,sizeof(defchar));
    if (value & 8) defchar[0]=bar;
    if (value & 4) defchar[2]=bar;
    if (value & 2) defchar[4]=bar;
    if (value & 1) defchar[6]=bar;
    lcd.createChar(2, (int*)defchar);
#endif  

#ifdef TM1638bargraph
  
  for (uint8_t position = 0; position < 8; position++)
   {
    tm.setLED(position, value & 1);
    value = value >> 1;
   }
#endif  
  }
}


void updateBargraph()
{
 if (ee.CVmode)  // CV Mode - bargraph needs to show compression
  {
   bargraphDot=4;
   byte b=1*(relPressure<-0.2*compressionScale)+2*(relPressure<-0.1*compressionScale)+4+  // Calculate the bargraph content from the pressure
          8*(relPressure>0.1*compressionScale)+16*(relPressure>0.2*compressionScale)+
          32*(relPressure>0.4*compressionScale)+64*(relPressure>0.8*compressionScale)+
          128*(relPressure>compressionScale);
   barGraph=b; 
  } else
  {           // CP Mode - bargraph needs to show volume 
   bargraphDot=1;
   float vol=stepper.currentPosition()/motorVolumeRatio;
   int c=int ((8*vol)/volumeScale+1); // Always make sure at least the leftmost position is active
   barGraph=0;
   while (c--) barGraph+=barGraph+1;
  }
}

void doDisplay()
{
#ifdef PCF8574LCDDisplay  
 byte b=0;
 int n=-1;
 while (b<39) if (disp[b++]=='\n')
  {
   disp[b-1]=0;  
   n=b;
  }
 disp[38]=0;   // Safety

 if (n!=1)
  {
#ifdef LCDReset
 if (checkMillis(lastLCDReset,5000))
 {
   // sequence to reset. see "Initializing by Instruction" in datatsheet
  lcd._sendNibble(0x03);
  delayMicroseconds(4500);
  lcd._sendNibble(0x03);
  delayMicroseconds(200);
  lcd._sendNibble(0x03);
  delayMicroseconds(200);
  lcd._sendNibble(0x02);   // finally, set to 4-bit interface

  // Instruction: Function set = 0x20
  lcd._send(0x28);
  lcd.display();
  lcd.leftToRight();
  lastLCDReset=millis();
 }
#endif
 // lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(char(1));
    lcd.print(disp);
  }
 if (n>=0)
  {
   lcd.setCursor(0,1);
    lcd.print(char(2));
   lcd.print(&disp[n]);  
  }
#endif
#ifdef TM1638Display
 byte f=0;
 byte c=8;
 while (disp[f]) if (disp[f++]=='.') c++; // Calculates the stringt length without the dot characters
 disp[c]=0; // cut string length 
 tm.displayText(disp);   
 byte l=strlen(disp)-c+8;                 // Fills the right end of the display
 while (l<8) tm.displayASCII(l++,' ');
#endif  
}

#ifdef isKeyboard
/* returns a byte with a bit for each of up to 16 buttons. data is raw, not debounced
 *
 * ading another kind of keyboard should be done here 
 *
 */

uint16_t readKeyboard()  
{
#ifdef TM1638Keyboard
 return tm.readButtons(); // returns a byte with values of button s8s7s6s5s4s3s2s1  
#endif 
#ifdef analogKeyboard
byte b=1;
byte c=btns;
while (c--)
 {
  if ((analogKeys>KbdVals[c]-KbdTols[c]) && (analogKeys<KbdVals[c]+KbdTols[c])) return b;
  b+=b;
 }
return 0; 
#endif 
}

void debounceKeyboard()
{
 uint16_t b = readKeyboard(); // returns a byte with values of button s8s7s6s5s4s3s2s1
 uint16_t c=maxBtn;
 uint16_t m=1;
 while (c--)  //Keyboard debounce
  {
   if (m & b) // raw key pressed
     {if (kcnt[c]>debounce) keys[c]=true; if(kcnt[c]<keyRpt1) kcnt[c]++;}  
    else
     {if (kcnt[c]==0) keys[c]=false; else if (kcnt[c]>debounce) kcnt[c]=debounce; else kcnt[c]--;}
   m+=m; 
  }
}

void repeatCheck(byte k)  // if the repeat delay for the button is elapsed, clear memory (simulate new press)
{
 if(kcnt[k]==keyRpt1)
  {
   kcnt[k]-=keyRpt; 
   mkeys[k]=false; 
  }
}  

#ifdef analogKeyboard
void   analogReadKeys()
{
   analogKeys=analogRead(pin_analogKeyboard);            // Buffered analog value for keyboard
#ifdef debugAnalogKeyboard
   Serial.print(F("Analog Key Value: "));
   Serial.print(analogKeys); 
   Serial.print(F("\t Tolerance (5%): "));
   Serial.println(int(float(analogKeys) / 20)+1); 
   analogKeys=0;  // Does not transmit keys when debugging the analog values.
#endif
}
#endif

#endif

#ifdef Beeper
void beep(int lng,byte err)  // Launch a beep sound
{
 if (err) alarmNum=err; 
 pinMode(pin_Beep, OUTPUT); 
 if (lng) digitalWrite(pin_Beep, LOW);  // active (inverted)
 beepCnt=max(beepCnt,lng);              //  if overlapping sounds, use the longest of them 
}
#endif

#ifdef E2PROM
void eeput (int n)  // records to EEPROM (only if values are validated)
{
 int eeAddress = eeStart;
 ee.eeVersion=eepromVersion;               // Write the EEPROM data version
 boolean isOk=checkValues();

 if (n==1) isOk=true;  // override (for debug testing)
 if (isOk)
  {
   EEPROM.put(eeAddress, ee);
   eeAddress += sizeof(EEPROM_Data);
#ifdef serialVerbose 
   if (term) Serial.println(F("Write EEPROM Settings"));
#else
   if (term) Serial.println(F("EE Write"));
#endif 
  }
  
}    
#endif


void eeget ()
{    
#ifdef E2PROM
 int eeAddress = eeStart;
 EEPROM.get(eeAddress, ee);
 eeAddress += sizeof(EEPROM_Data);
 // Check if the data is coherent
 boolean isOk=checkValues();
 if (!isOk) 
  {
#ifdef debug_5

  Serial.print(ee.eeVersion);
  tab();
  Serial.print(ee.reqBPM);
  tab();
  Serial.print(ee.reqVolume);
  tab();
  Serial.print(ee.reqCompression);
  tab();
  Serial.print(ee.peep);
  tab();
  Serial.print(ee.syncRatio);
  tab();
  Serial.print(ee.expirationRatio);
  tab();
  Serial.print(ee.CVmode);
  tab();
  Serial.print(ee.active);
  tab();
  Serial.print(ee.ambientPressure);
  tab();
  Serial.println(ee.apnea);
#endif 
   ee.reqBPM          = defaultBPM;
   ee.reqVolume       = defaultVolume;
   ee.reqCompression  = defaultCompression;
   ee.peep            = defaultPEEPCompression;
   ee.syncRatio       = defaultsyncRatio;
   ee.expirationRatio = defaultExpirationRatio;
   ee.CVmode          = defaultCVMode;
   ee.active          = false;
   ee.ambientPressure = defaultAmbientPressure;
   ee.apnea           = defaultApnea;
  }
#ifdef serialVerbose 
 if (term) Serial.println((isOk)?F("Read EEPROM Settings"):F("Read Default Settings"));
#else
 if (term) Serial.println((isOk)?F("EEPROM"):F("Default"));
#endif 
#else
 ee.reqBPM          = defaultBPM;
 ee.reqVolume       = defaultVolume;
 ee.reqCompression  = defaultCompression;
 ee.peep            = defaultPEEPCompression;
 ee.syncRatio       = defaultsyncRatio;
 ee.expirationRatio = defaultExpirationRatio;
 ee.CVmode          = defaultCVMode;
 ee.active          = false;
 ee.ambientPressure = defaultAmbientPressure;
 ee.apnea           = defaultApnea;
#ifdef serialVerbose 
 if (term) Serial.print(F("Read Default Settings\n"));
#endif 
#endif
}   


#ifdef USBcontrol

#define CommandMaxLength 30

byte BufferLength;
String Buffer = "*******************************";  // filler

void prstat()
{
#ifdef serialVerbose
 Serial.print  (F("\nPatient assisted breathing\tO"));
 Serial.println((ee.active)?F("N"):F("ff"));
 Serial.print  ((ee.CVmode)?F("CV"):F("CP"));
 Serial.print  (F(" mode\nVentilation speed\t\t"));
 Serial.print  (ee.reqBPM);
 Serial.print  (F(" cycles / minute\n"
                  "Ventilation volume\t\t"));
 Serial.print  (ee.reqVolume);
 Serial.print  (F(" ml\n"
                  "Ventilation pressure\t\t"));
 Serial.print  (ee.reqCompression);
 Serial.print  (F(" cm H2O\n"
                   "PEEP pressure\t\t"));
 Serial.print  (ee.peep);
 Serial.print  (F(" cm H2O\n"
                  "Patient synchonization\t\t"));
 Serial.print  (ee.syncRatio);
 Serial.print  (F(" % of cycle\n"
                "Expiration ratio\t\t"));
 Serial.print  (ee.expirationRatio);
 Serial.print  (F(" x inspiration\n"
                "Apnea Alarm\t\t"));
 Serial.print  (ee.apnea);
 Serial.println(F(" cycle(s)\n"));
#else
 Serial.print  ((ee.active)?F("ON\t"):F("Off\t"));
 Serial.print  ((ee.CVmode)?F("CV\t"):F("CP\t"));
 Serial.print  (ee.reqBPM);
 tab();
 Serial.print  (ee.reqVolume);
 tab();
 Serial.print  (ee.reqCompression);
 tab();
 Serial.print  (ee.peep);
 tab();
 Serial.print  (ee.syncRatio);
 tab();
 Serial.print  (ee.expirationRatio);
 tab();
 Serial.println (ee.apnea);
#endif 
}

void SerialCommand()
{
  commTime=millis(); 
  float r;
  int i;
  boolean ok=true;
  char ch=Buffer[1];
  char c=Buffer[0];
  Buffer[0]=32;
  r=Buffer.toFloat();
  i=int(r);
  switch ( c)
   {
    case '1': // 1 Start assisted breathing
#ifdef serialVerbose
     Serial.println(F("Breathing started"));
#endif
ee.active=true;
    break;
    case 'C': // CV / CP mode
     if (ch=='V') ee.CVmode=true; 
     if (ch=='P') ee.CVmode=false; 
#ifdef serialVerbose
     Serial.println((ee.CVmode)?F("CV mode"):F("CP mode"));
#endif
    break;
    case '0': // 0 Stop assisted breathing
     ee.active=false;
#ifdef serialVerbose
     Serial.println(F("WARNING: Breathing stopped"));
#endif
     setDisplayMenu(Stop_message,std_dly,0);
    break;
    case 'S': // S set Speed (in BPM)
     if (ok=((i>=minBPM) && (i<=maxBPM)))
      {ee.reqBPM=i; prstat();} 
#ifdef serialVerbose
        else Serial.println(F("Out of range (8-35BPM)"));  
#endif
    break;
    case 'V': // V set Volume (in ml)
     if (ok=((i>=minVolume) && (i<=maxVolume)))
      {ee.reqVolume=i; prstat();}
#ifdef serialVerbose
      else Serial.println(F("Out of range (100-800ml)"));  
#endif
    break;
    case 'P': // P Set pressure (in cm H2O)
     if (ok=((i>=minCompression) && (i<=maxCompression)))
      {ee.reqCompression=i; prstat();} 
#ifdef serialVerbose
      else Serial.println(F("Out of range (1-60 cm H2O)"));  
#endif
    break;
    case 'E': // E set PEEP (in cm H2O)
     if (ok=((i>=minPEEPCompression) && (i<=maxPEEPCompression)))
      {ee.peep=i; prstat();} 
#ifdef serialVerbose
      else Serial.println(F("Out of range (1-20 cm H2O)"));  
#endif
    break;
    case 'A': // set Apnea alarm (in cycles)
     if (ok=((i>=minApnea) && (i<=maxApnea)))
      {ee.apnea=i; prstat();} 
#ifdef serialVerbose
      else Serial.println(F("Out of range (0 to 6)"));  
#endif
    break;
    case 'X': // X set eXpiration ratio (x inspiration)
     if (ok=((r>=minExpirationRatio) && (r<=maxExpirationRatio)))
      {ee.expirationRatio=r; prstat();}
#ifdef serialVerbose
      else Serial.println(F("Out of range (1 to 3 x inspiration)"));  
#endif
    break;
    case 'Y': // Y set sYnc (in % of cycle)
     if (ok=((r>=minsyncRatio) && (r<=maxsyncRatio)))
      {ee.syncRatio=byte(r); prstat();}
#ifdef serialVerbose
      else Serial.println(F("Out of range (0-40%)"));  
#endif
    break;
    case 'M': // M show Measurements from the sensor
     meas();  
    break;
#ifdef serialTelemetry    
    case 'T': // T toggle Telemetry data on/off\n
     telemetry=!telemetry;  
    break;
#endif
#ifdef E2PROM
    case 'W': // T Terminal mode (human readable)
     eeput(0);  // Save settings to EEPROM
     menuItem=0;
#ifdef serialVerbose
     Serial.println(F("Parameters Written"));  
#endif
    break;
#endif    
    case 'D': // D debug (on / off)
     debug=!debug;
    break;
    //case 'R': // R Reset
    // if (ch=='S') { 
    // cli();
    // resetFunction();}
    //break;
    case '?': // help
     if (term)
      {
#ifndef ArduinoPlotter        
#ifdef serialVerbose
       Serial.println(F("0 Stop assisted breathing\n"
                        "1 Start assisted breathing\n"
                        "CV sets Constant Volume mode\n"
                        "CP sets Constant Pressure mode\n"
                        "S set Speed (in BPM)\n"
                        "V set Volume (in ml)\n"
                        "P set Pressure (in cm H2O)\n"
                        "E set PEEP (in cm H2O)\n"
                        "A set Apnea alarm (in cycles)\n"
                        "X set eXpiration ratio (x inspiration)\n"
                        "Y set sYnc (in % of cycle)\n"
                        "M show Measurements from the sensor\n"
                        "T toggle Telemetry data on/off\n"
#ifdef E2PROM
                        "W Write parameters to permanent memory\n"
#endif       
                        "D debug (on / off)\n"
                        //"R Reset\n"
                        "? print this\n" )); 
#else
       Serial.println(F("CMD: 0 1 CV CP S V P X Y M T"
#ifdef E2PROM
                        " W"
#endif       
                        " D"
                        //"RS Reset\n"
                        " ?")); 
#endif
#endif
      }
      break;
    default:   // Unknown command 
#ifdef serialVerbose 
     Serial.println(F("Unknown command. type '?' for instructions.")); 
#else
     Serial.println(F("?")); 
#endif
    break;
   } 
 if (!ok) Serial.println('?'); else prstat();
#ifdef watchdog
 Watchdog.reset();
#endif        
}

void SerialWait(int t)
{
 while (t--) 
  {
   delayMicroseconds(10); // 10 usec delay
   while (Serial.available())
    {       
      char inChar = (char)Serial.read();
      if (inChar != '\r') { 
      //Serial.print(inChar); 
      if (inChar == '\n') 
      {
        Buffer += (char)0; 
       SerialCommand();
       BufferLength=CommandMaxLength; // force buffer reset
      } 
      else
      {
       Buffer += inChar;
       BufferLength++;
      }
      if (BufferLength>=CommandMaxLength) {BufferLength=0;Buffer = "";}  // Overflow buffer reset
      }
    }
  }
}
#endif

/*
 *   This is the state machine that manages all the data displayed, the menus and animations.
 *   
 *   There are lots if conditional code that make it less legible but allow to change the display 
 *   controller while preserving the navigation logic
 */


#ifdef dispMenus
void displayMenu()  
{
#ifdef PCF8574LCDDisplay  
 doDisp=true;  // will update the display by default
 byte dispt=dispTick % 3;
 switch (dispPhase) {
  case 0:
     strcpy(disp,"OPEN  SOURCE\n VENTILATOR");
     dispPhase++;
    break;
  case 1:
     strcpy(disp,"Software V0.3\n             ");
     dispDelay=100;
     dispPhase=100;
    break;
  case 10:
     sprintf(disp, "ERROR %2d\n", dispParam);
     strcat(disp,(dispParam==0)?"NO SENSOR":"UNKNOWN SENSOR");     
     dispPhase=10;
  case RATE_PEEP:
     sprintf(disp,"\nSP %3d PEEP %2d  ",displayBPM,displayPeep);
     dispDelay=10;  
//     dispPhase=100;
    break;
  case VOL_PIP:
     sprintf(disp,"\nV %4d PIP  %2d  ",displayVol,displayPip);
     dispDelay=10;  
    break;

  case 100:
     sprintf(disp," S%02d V%d P%d    ",ee.reqBPM,ee.reqVolume,ee.reqCompression);
     dispDelay=100;  
     if (menuItem==0)
      {
       if (ee.active) dispPhase=(dispParam==0)?100:103; else dispPhase++;
       strcat(disp,"\n                ");     
      }
      else
       { 
        dispPhase=Menu_message+10*menuItem;
        dispDelay=10;
       } 
    break;
  case 101:
     strcpy(disp,"\n START ");
     strcat(disp,(ee.CVmode)?"VC":"PC");
     if (ee.syncRatio>0) strcat(disp,"+SV");
     dispPhase=(dispParam==0)?100:103;
     dispDelay=80;
    break;
  case 102:
     strcpy(disp,"\n STOP       ");
     dispPhase=100;
     dispDelay=50;
    break;
  case 103:
     sprintf(disp,"\n TEMP %dc     ",int(temperature)); 
     dispPhase++;
     dispDelay=80;
    break;
  case 104:
     sprintf(disp,"\n Humi %d RH   ",humidity);    
     dispPhase++;
    break;
  case 105:
     sprintf(disp,"\n PRES. %d cmH2O",int(relPressure));
     dispPhase++;
    break;
  case 106:
     sprintf(disp,"\n SYNC. %d     ",ee.syncRatio);
     dispPhase++;
    break;
  case 107:
     sprintf(disp,"\n Ratio %d.%d    ",int(ee.expirationRatio),int (ee.expirationRatio*10) %10);
     dispPhase++;
    break;
  case 108:
     sprintf(disp,"\n Amb. Pres.%d",int(ee.ambientPressure));
     dispPhase=Menu_message;
    break;
  case 110:
     sprintf(disp,"\n SET Rate  %d   ",ee.reqBPM);
     dispDelay=60;  
     dispPhase=Menu_message;
    break;
  case 120:
     sprintf(disp,"\n SET Vol. %d  ",ee.reqVolume); // character N looks better chan character M
     dispDelay=60;  
     dispPhase=Menu_message;
    break;
  case 130:
     sprintf(disp,"\n SET Press. %d  ",ee.reqCompression);
     dispDelay=60;  
     dispPhase=Menu_message;
    break;
  case 140:
     sprintf(disp,"\n SET PEEP. %d  ",ee.peep);
     dispDelay=60;  
     dispPhase=Menu_message;
    break;
  case 150:
     sprintf(disp,"\n SET Sync. %2d    ",ee.syncRatio); 
     dispDelay=60;  
     dispPhase=Menu_message;
    break;
  case 160:
     dispTick++;
     sprintf(disp,"\n SET In-Ex %d.%d",int(ee.expirationRatio),int (ee.expirationRatio*10) %10); 
     dispDelay=60;  
     dispPhase=Menu_message;
    break;
  case 170:
     sprintf(disp,"\n SET Apnea %d  ",ee.apnea);
     dispDelay=60;  
     dispPhase=Menu_message;
    break;
  case 180:
     strcpy(disp,"\n Save Setup     ");
     dispDelay=60;  
     dispPhase=Menu_message;
    break;
  default:
    if (term) Serial.println(dispPhase); 
    dispPhase=0; // Abnormal the phase should always be defined
   break;
  }
 dispCnt=dispDelay;
#endif 

#ifdef TM1638Display  
 doDisp=true;  // will update the display by default
 byte dispt=dispTick % 3;
 switch (dispPhase) {
  case 0:
     strcpy(disp,"OPEN");
     dispPhase++;
    break;
  case 1:
     strcpy(disp,"SOURCE");
     dispPhase++;
    break;
  case 2:
     strcpy(disp,"VENTILAT");
     dispDelay=30;
     dispPhase++;
    break;
  case 3:
     strcpy(disp,"ENTILATO");
     dispDelay=20;
     dispPhase++;
    break;
  case 4:
     strcpy(disp,"NTILATOR");
     dispDelay=60;
     dispPhase++;
    break;
  case 5:
     strcpy(disp,"SOFT 0.3");
     dispDelay=100;
     dispPhase=100;
    break;
  case 10:
     sprintf(disp, "ERROR %2d", dispParam);
     dispPhase+=dispParam+1; // Jump to correct error 
    break;
  case 11:
     strcpy(disp,"NO.SENSOR");
     dispPhase=10;
    break;
  case 12:
     strcpy(disp,"UNKNOWN");
     dispPhase=29;
    break;
  case 29:
     strcpy(disp,"SENSOR");
     dispPhase=10;
    break;
  case RATE_PEEP:     // no display on TM1638 (to be changed)
     //if (dispTick++ & 1) 
     // sprintf(disp,"PIP  %d  ",displayPip);
     // else
     // sprintf(disp,"PEEP %d  ",displayPeep);
     dispDelay=10;  
     dispPhase=100;
    break;
  case VOL_PIP:     // no display on TM1638 (to be changed)
     //if (dispTick++ & 1) 
     // sprintf(disp,"PIP  %d  ",displayPip);
     // else
     // sprintf(disp,"PEEP %d  ",displayPeep);
     dispDelay=10;  
     dispPhase=100;
    break;
  case 100:
     if(dispTick++ & 1)
      sprintf(disp,"%02d %d",ee.reqBPM,ee.reqVolume);
      else
      sprintf(disp,"P %d",ee.reqCompression);
     dispDelay=100;  
     if (ee.active) dispPhase=(dispParam==0)?100:103; else dispPhase++;
    break;
  case 101:
     strcpy(disp,"START  _");
     disp[6]=(ee.CVmode)?'V':'P';
     dispPhase=(dispParam==0)?100:103;
     dispDelay=80;
    break;
  case 102:
     strcpy(disp,"STOP   _");
     dispPhase=100;
     dispDelay=50;
    break;
  case 103:
     sprintf(disp,"TENP %dc",int(temperature)); // character N looks better chan character M
     dispPhase++;
     dispDelay=80;
    break;
  case 104:
     sprintf(disp,"Huni%d",humidity);    // character N looks better chan character M
     dispPhase++;
    break;
  case 105:
     sprintf(disp,"PRESS. %d",int(relPressure));
     dispPhase++;
    break;
  case 106:
     sprintf(disp,"SYNC.  %d",ee.syncRatio);
     dispPhase++;
    break;
  case 107:
     sprintf(disp,"Ratio %d.%d",int(ee.expirationRatio),int (ee.expirationRatio*10) %10);
     dispPhase++;
    break;
  case 108:
     sprintf(disp,"Atm %d",int(ee.ambientPressure));
     dispPhase=100;
    break;
      
  case 110:
     sprintf(disp,(dispTick++ & 1)?"SET   %d":"RATE  %d",int(ee.reqBPM));
     dispDelay=60;  
    break;
  case 120:
     sprintf(disp,(dispTick++ & 1)?"SET  %d":"VOL.  %d",int(ee.reqVolume)); // character N looks better chan character M
     dispDelay=60;  
    break;
  case 130:
     sprintf(disp,(dispTick++ & 1)?"SET   %d":"PRESS. %d",int(ee.reqCompression));
     dispDelay=60;  
    break;
  case 140:
     sprintf(disp,(dispTick++ & 1)?"SET  %2d":"PEEP %2d",int(ee.peep));
     dispDelay=60;  
    break;
  case 150:
     sprintf(disp,(dispTick++ & 1)?"SET  %2d":"SYNC. %2d",ee.syncRatio); 
     dispDelay=60;  
    break;
  case 160:
     dispTick++;
     sprintf(disp,(dispTick++ & 1)?"SET %d.%d":"I-E. %d.%d",int(ee.expirationRatio),int (ee.expirationRatio*10) %10); // 100.1 instead of 100 prevents rounding errors
     dispDelay=60;  
    break;
  case 170:
     sprintf(disp,(dispTick++ & 1)?"SET   %d":"APNEA %d",int(ee.apnea));
     dispDelay=60;  
    break;
  case 180:
     strcpy(disp,(dispTick++ & 1)?"SETUP  _":"SAVE   _");
     dispDelay=60;  
    break;
  default:
    if (term) Serial.println(dispPhase); 
    dispPhase=0; // Abnormal the phase should always be defined
   break;
  }
 dispCnt=dispDelay;
#endif 
}
#endif


void Timer() 
{
#ifdef Led  
#ifdef debugLed  
 digitalWrite(pin_LED, HIGH);
#endif
#endif
 timerLvl++;  
 if ((!stopMove) && (timerLvl<=interruptMaxNest)) stepper.run();
#ifdef Led  
#ifdef debugLed  
 digitalWrite(pin_LED, LOW);
#endif
#endif
 if (timerLvl>1)  // only does the motor update if nested more than 1 level
  {
   timerLvl--; 
  }
  else
  {
   sei();                        // Critical : Allows nested interrupts
   switch (timCnt % 64) {        // 12.8 ms tick period for interface handling
#ifdef watchdog
    case 0:
   Watchdog.reset();
     break;
#endif        
#ifdef dispMenus     
    case 5:
     if (--dispCnt<=0) displayMenu();
     break;
    case 10:                     // change the display
     if (doDisp)
      {
       if  (I2CBusy)
         timCnt-=4;  // retry next time
        else
         {
          doDisplay(); 
          doDisp=false;
         } 
      }
     break;
#endif     
#ifdef dispBargraph 
    case 20:                     // update the bargraph
       if  (I2CBusy)
         timCnt-=4;  // retry next time
        else
         {
          displayBargraph((breathGraphCnt>0)?barGraph-bargraphDot:barGraph);
          if (breathGraphCnt) breathGraphCnt--;
         } 
     break;
#endif     
#ifdef Beeper       
    case 30:                     // update the beep
     if (beepCnt==1)
      {
        digitalWrite(pin_Beep, HIGH);  // inactive (inverted)
        alarmNum=0;
      }
     if (beepCnt) beepCnt--;
     break;
#endif     
#ifdef isKeyboard
    case 40:                     // update the keyboard
      debounceKeyboard();
     break;
#endif     
    } 
   timCnt++;
#ifdef Led
#endif
   cli();  // no interrupts
   timerLvl--; 
  }
}


void __attribute__ ((noinline)) IIRFilter (float raw,float &flt,float pct)
{
 flt=(flt==0)?raw:flt*(1-pct)+raw*pct;
}


void readSensors() // Works no matter if sensor is present or not (returns default values if not present)

{
 float temp(NAN), hum(NAN), pres(NAN);

// Add your sensor reading routines here 

#ifdef BoschBMxSensor 
 BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
 BME280::PresUnit presUnit(BME280::PresUnit_Pa);
 I2CBusy=true;
 bme.read(pres, temp, hum, tempUnit, presUnit);
 if (!isnan(pres)) pres=pres*PaToCmH2O; 
#endif

#ifdef BoschBMP180Sensor 
float amb(NAN);
if ((sc % 8)==1)   // every 400 ms, launch a new measurement
  {
   BMP180Phase=false; 
   if (!bmp180.measureTemperature()) 
    {
     bmp180.begin(); // Restarts the sensor whenever an error is detected
#ifdef debug_3
     Serial.print('!');
#endif       
    }
  }
 if (bmp180.hasValue())
  {
    if (!BMP180Phase)
     {
      temperature2=bmp180.getTemperature();
      BMP180Phase=bmp180.measurePressure();
      if (!BMP180Phase) 
       {
        bmp180.begin(); // Restarts the sensor whenever an error is detected
#ifdef debug_3
        Serial.print('!');
#endif       
       }
      
     }
#ifdef debug_3
    Serial.print('+');
#endif  
    if (BMP180Phase)
     {     
      amb=bmp180.getPressure();
      if (!isnan(amb)) amb=amb*PaToCmH2O; 
      
      if (!isnan(amb) && (amb>minAtmosphericPressure) &&  (amb<maxAtmosphericPressure))
       {
#ifdef debug_3
         Serial.print(amb);
#endif       
        IIRFilter(amb,ee.ambientPressure,ambientPressureFilter);
        // ambientPressure=(ambientPressure==0)?amb:ambientPressure*(1-ambientPressureFilter)+amb*ambientPressureFilter; // low pass filtering
        lastAmbientPressure=millis();
       }
     }  
  }
#endif
 I2CBusy=false;

 if (isnan(temp) || isnan(pres))  // If data was not correctly acquired
 {
  temp=temperature;  // assume it did not move
  if (presM1>presM2)  // pressure was increasing 
    pres=presM1+2*(presM1-presM2); // assume the pressure increase is 2x what it was
   else
    pres=presM1;  // Pressure was decreasing, assume it stays constant
   
#ifdef debug_3
  Serial.print("E");
#endif       
 }
 else
 {
  presM2=presM1;
  presM1=pres;
 }
#ifdef debug_3
   tab();
   Serial.print((sensPressure)?"1 ":"0 ");
   tab();
   Serial.print((sensHumidity)?"1 ":"0 ");
   tab();
   Serial.print(pres);
   tab();
   Serial.print(temp);
   tab();
   Serial.print(hum);
   tab();
   Serial.println(ee.ambientPressure);
#endif
#ifdef tempSensor
  if (sensTemperature)
   {
    temperature=temp;
   }
  else 
#endif
    temperature=defaultTemperature;
#ifdef humiditySensor
  if (sensHumidity)
   {
    humidity=int(hum);
   }
  else
#endif
    humidity=defaultHumidity;
#ifdef pressureSensor
  if (sensPressure)
   {
     pressure=pres;
#ifdef TwoPressureSensors
     if ((stepper.currentPosition()==0) && !ee.active && ((checkMillis(lastAmbientPressure,delayAmbientPressure)) || !sensAmbientPressure))  
      IIRFilter(pressure,ee.ambientPressure,ambientPressureFilter);
#else
     if ((stepper.currentPosition()==0) && !ee.active)  // approximate athmospheric pressure by averaging when the bag is filled and the machine does not run.
      IIRFilter(pressure,ee.ambientPressure,ambientPressureFilter);
#endif     
      IIRFilter(pressure,avgPressure,avgPressureFilter);
     peakPressure=pressure-avgPressure;
   }
   else
#endif
   {
    pressure=defaultPressure;
    ee.ambientPressure=defaultAmbientPressure;
   }
  if (ee.ambientPressure==0) ee.ambientPressure=defaultAmbientPressure; 
  relPressure=pressure-ee.ambientPressure; 
 
#ifdef debug_4
   Serial.print(pressure);
   tab();
   //Serial.print(temperature);
   //tab();
   //Serial.print(humidity);
   //tab();
   Serial.println(ee.ambientPressure);
#endif  
#ifdef CurrentSense
 if (sensCurrent)
  {
   //set_sleep_mode(SLEEP_MODE_IDLE);
   //sleep_enable();
   //sleep_mode(); 
   //sleep_disable();
   int c=analogRead(pin_current_Sense);
   if (stepper.speed()==0) // average of idle current value when the motor does not run
   // idleCurrent=(idleCurrent==0)?c:idleCurrent*(1-idleCurrentFilter)+c*idleCurrentFilter; // low pass filtering
   IIRFilter(c,idleCurrent,idleCurrentFilter);
   currentVal=c-int(idleCurrent); 
   //currentRaw=(currentRaw==0)?analogCurrentRatio*currentVal:currentRaw*(1-currentFilter)+analogCurrentRatio*currentVal*currentFilter; // low pass filtering
   IIRFilter(analogCurrentRatio*currentVal,currentRaw,currentFilter);
   current=abs(currentRaw);
  }
  else current=0;
#endif
#ifdef debug_2  // print data for 1 in 10 measurement cycles
 if ((term) && ((sc % 10)==2) && (debug))
   meas();
#endif
sc++; 
}


void detectSensors()  // Detects the various sensors - initialisation for new sensors should be added here
{
 sensHumidity=false;
 sensTemperature=false;
 sensPressure=false;
#ifdef CurrentSense
 sensCurrent=false;
#endif

// Add your sensor detection routines here
 I2CBusy=true;
#ifdef BoschBMxSensor
 if(bme.begin())
  {
   switch(bme.chipModel())
    {
    case BME280::ChipModel_BME280:
      if (term) Serial.println(F("BME280"));
      sensHumidity=true;
      sensTemperature=true;
      sensPressure=true;
      break;
    case BME280::ChipModel_BMP280:
      if (term) Serial.println(F("BMP280"));
      sensTemperature=true;
      sensPressure=true;
      break;
    default:
      setDisplayMenu(error_message,std_dly,1);
      if (term) Serial.println(F("UNKNOWN sensor"));
    }
  }
  else
  {
   setDisplayMenu(error_message,std_dly,0);
   if (term) Serial.println(F("No BME280 sensor"));
  }
#endif  

#ifdef BoschBMP180Sensor
if (sensAmbientPressure=bmp180.begin())
  {
    if (term) Serial.println(F("BMP180"));
    bmp180.resetToDefaults(); //reset sensor to default parameters.
    bmp180.setSamplingMode(BMP180MI::MODE_UHR); //enable ultra high resolution mode for pressure measurements
  }
  else
  if (term) Serial.println(F("No BMP180 sensor"));
#endif
 I2CBusy=false;
#ifdef CurrentSense
 pinMode(pin_current_Sense,INPUT_PULLUP);
 delay (10);
 sensCurrent=(analogRead(pin_current_Sense)<750); // If the pin is left floating, it's voltage should be well above midpoint
 if (sensCurrent)
  {
   currentVal=abs(analogRead(pin_current_Sense)-current0);
   sensCurrent=(currentVal < currentMaxIdle);
  }
 if (sensCurrent)  Serial.println(F("Found current sensor!"));
#endif
}


#ifdef I2CCheck
void i2c_scanner() {    // This code is a copy of Arduino i2c_scanner example
  int nDevices = 0;
  Serial.println(F("Scanning I2C..."));
  for (byte address = 1; address < 127; ++address) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();
    if (error == 0) {
      Serial.print(F("I2C device found at address 0x"));
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println("  !");
      ++nDevices;
    } else if (error == 4) {
      Serial.print(F("Unknown error at address 0x"));
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) Serial.println(F("No I2C devices found\n"));
}
#endif

void setup() {
 // put your setup code here, to run once:
 timerLvl=0;
 Serial.begin(SERIAL_BAUD);
 while (!Serial && millis()<3000); // wait for USB Serial ready. Needed for native USB (non blocking)
 term=Serial; 
 if (term) Serial.println(F("Open Source Ventilator V 0.3"));
#ifdef jm_Wire
 twi_readFrom_wait = true; // twi_readFrom() waiting loop
 twi_writeTo_wait = true;  // twi_writeTo() waiting loop
#endif 
#ifdef I2C
 Wire.begin();
#ifdef I2CCheck
 i2c_scanner();  // checks for all devices on the bus
#endif
#endif
#ifdef PCF8574LCDDisplay
 Wire.beginTransmission(0x27);     // I2C Address of the PCF8574 chip 
 int error = Wire.endTransmission();
 if (error == 0) {
   Serial.println("LCD");
   lcd.begin(16, 2); // initialize the lcd
   lcd.setBacklight(255);
   lcd.home();
   lcd.clear();
  }
   else 
    Serial.print(error);
#endif
#ifdef TM1638
 tm.displayBegin(); // Please use Version 1.4 of the library (or commment out this line)
#endif
 Timer1.initialize(200);  
 Timer1.attachInterrupt(Timer);
#ifdef watchdog
#ifndef  watchdogProtect        
 Watchdog.enable(watchdogDelay);  // if the watchdog is enabled without delayed start, let it go
                                  // (See the warning about bricking your board before enabling this)
 Watchdog.reset();
#endif        
#endif        
 
 setDisplayMenu(startup_message); 
 
 pinMode(pin_Stepper_DIR, OUTPUT);
 digitalWrite(pin_Stepper_DIR, LOW);     // ee.active (inverted)
 pinMode(pin_Stepper_Step, OUTPUT);
 digitalWrite(pin_Stepper_Step, LOW);    // ee.active (inverted)
#ifdef disableMotorctrl
 pinMode(pin_Stepper_Disable, OUTPUT);
 digitalWrite(pin_Stepper_Disable, LOW);  // ee.active (inverted)
 motorState=true;
#endif 
 detectSensors();
 eeget ();    // read startup parameters (either from EEPROM or default value)
#ifdef USBcontrol 
 Buffer="?";  // send informations to the serial port
 SerialCommand();
 Buffer="";
#endif 
 BPM=ee.reqBPM;                 // Start from these values 
 volume=ee.reqVolume;
 compression=ee.reqCompression;
 readSensors();   
 tick=millis();
 breathLength=int(60000/BPM);
 breathe=tick-breathLength;
 stopMove=false;
// stepper.setMaxSpeed(3500);
// stepper.setAcceleration(20000);
// stepper.moveTo(0);
#ifdef Beep
 beep(20,0); 
#endif
#ifdef ArduinoPlotter
 telemetry=true;
 ee.active=true;
#endif
#ifdef debug_6  // Display raw and corrected volumes 
 int i=minVolume;
 while (i<=maxVolume)
 {
  Serial.print(i);
  tab();
  Serial.println(volume2pos(i));
  i+=10;
 }
#endif
}

#ifdef isKeyboard

void pressOk()
{
 if (menuItem==0)
   {
    ee.active=!ee.active;  // Toggle function when "Enter" key pressed
    if (!ee.active) 
     {
      setDisplayMenu(Stop_message,std_dly,0);
      lastCycleStart=0;
#ifndef debugShortEnter
#ifdef Beeper
      beep (200,3); // alert about disabling the ventilator
#endif           
#endif           
     }
     else
     breathe=millis()-int(60000/BPM); // immediate start when pressing ok
   }
  else
   {                 // Return to main screen when "Enter" key pressed
#ifdef E2PROM            
    if (menuItem==MenuItems-1) eeput(0);  // Save settings to EEPROM
#endif           
    menuItem=0;
    setDisplayMenu(Menu_message,std_dly,0);
   }
 if ((ee.active) && (stepper.distanceToGo() == 0))  // If the motor is idle, start immediately
  {
   breathe=millis()-breathLength; // will happen immediately
  }
}

void pressPrevNext()
{
 int n=0;
 if (keys[btnPrev]) n=-1;
 if (keys[btnNext]) n=1;
 if (n) 
  {
   n+=(menuItem);
   if (n<0) n=MenuItems-1;
   if (n>=MenuItems)n=0;
   menuItem=byte(n); 
   setDisplayMenu(Menu_message+10*menuItem,std_dly,0);
  }
  
// if (keys[btnPrev]) {menuItem=--menuItem % MenuItems;setDisplayMenu(Menu_message+10*menuItem,std_dly,0);}            // Previous menu item (no rollover)
//  if ((keys[btnNext]) && (menuItem<MenuItems-1)) {menuItem++;setDisplayMenu(Menu_message+10*menuItem,std_dly,0);}  // Next menu item (no rollover) 
}


void pressUpDown()
{
 int n=0;
 int syncRatio=ee.syncRatio; // needs a signed value
 if (keys[btnDn]) n=-1;
 if (keys[btnUp]) n=1;
 switch(menuItem)
  {
   case 0:
      if (n<0) dispParam=(dispParam==0)?1:0;  // button Dn triggers extended data
      if (n>0) ee.CVmode=!ee.CVmode;                // button Up triggers control mode change
      if (dispParam) setDisplayMenu(Measurement_Message);
      if (n==1) setDisplayMenu(Measurement_Start);
      dispCnt=0;
     break;
   case 1:
      ee.reqBPM=ee.reqBPM+(n*stepBPM);
     break;
   case 2:
      ee.reqVolume=ee.reqVolume+(n*stepVolume);
     break;
   case 3:
      ee.reqCompression=ee.reqCompression+(n*stepCompression);
     break;
   case 4:
      ee.peep=ee.peep+(n*stepPEEPCompression);
     break;
   case 5:
      syncRatio=ee.syncRatio+(n*stepsyncRatio); // done this way as ee.syncRatio is an (unsigned) byte
     break;
   case 6:
      ee.expirationRatio=ee.expirationRatio+(n*stepExpirationRatio);
     break;
   case 7:
      ee.apnea=ee.apnea+(n*stepApnea);
     break;
  }
 ee.reqBPM          = constrain(ee.reqBPM, minBPM, maxBPM); 
 ee.reqVolume       = constrain(ee.reqVolume, minVolume, maxVolume); 
 ee.reqCompression  = constrain(ee.reqCompression, minCompression, maxCompression); 
 ee.peep            = constrain(ee.peep, minPEEPCompression, maxPEEPCompression); 
 ee.syncRatio       = constrain(syncRatio, minsyncRatio, maxsyncRatio); 
 ee.apnea           = constrain(ee.apnea, minApnea, maxApnea); 
 ee.expirationRatio = constrain(ee.expirationRatio, minExpirationRatio, maxExpirationRatio); 
}

#endif

/*
 * The volume2pos function translates the required volume into motor travel to account for nonlinerarities
 * 
 * the formula is generated using this tool :
 * http://www.electrictactics.com/poly/polyin.html
 * 
 */


long volume2pos(int v)
{

 float x=v/100.0; 

 /*
  * Values used to find the formula (in 100ml increments) :
  *  1 => 2.8
  *  2 => 4
  *  3 => 5.1
  *  4 => 6.1
  *  5 => 7
  *  6 => 7.8
  *  7 => 8.5
  *  8 => 9.1
  *  9 => 9.6
  * 10 => 10
  */
 return (long(100*( ((-1.0/20) * x * x)  +((27.0/20) * x) +3.0/2)));
}


/*
 *  When the set pressure is reached or if the pressure gets too high (depending on the mode of operation)
 *  the patient inspiration phase is ended and the motor stopped fast.
 *  After overshoot, the motor comes back to the exact position where it was when the routine was called
 */

void motorFastStop()
{
 stepper.setAcceleration(motorMaxAcceleration);
 motorTarget=stepper.currentPosition ();
 stepper.moveTo(motorTarget);  // overshoot, then return
}

#ifdef serialTelemetry
void basicTelemetry(boolean b)
{
 if (ee.active)
  {
   if (!Mactive)
    {
     startactive=millis();
     Mactive=true;
    }
   if (telemetry)
    {
     if (breathPhase==0)
#ifdef ArduinoPlotter     
      Serial.println(F("Phase,Alarm,MotorPos,motorTarget,Pressure,Peak Press., Set press.,AmbPress,Volume,Target,BreathLng,BreathIn,BreathInSpeed,BreathInAccel,BreathOut,BreathOutSpeed,BreathOutAccel")); 

     //Serial.print(float(millis()-startactive)/1000);
     //sep();
     Serial.print(breathPhase*50);
     sep();
     Serial.print(alarmNum*20);
     sep();
     Serial.print(arduinoPlotterMotorOffset+stepper.currentPosition());
     sep();
     Serial.print(arduinoPlotterMotorOffset+stepper.targetPosition ());
     sep();
     Serial.print(arduinoPlotterPressureOffset+relPressure*10);
     sep();
     Serial.print(arduinoPlotterPressureOffset+peakPressure*10);
     sep();
     Serial.print(arduinoPlotterPressureOffset+ee.reqCompression*10);
#else
      Serial.println(F("Time,Phase,Alarm,Motor pos,Pressure,Peak Press., Set press.,Ambient press,Volume,Target,Breath lng,Breath in,Breath in speed,Breath in accel,Breath out,Breath out speed,Breath out accel")); 
     Serial.print(float(millis()-startactive)/1000);
     sep();
     Serial.print(breathPhase);
     sep();
     Serial.print(alarmNum);
     sep();
     Serial.print(stepper.currentPosition());
     sep();
     Serial.print(relPressure);
     sep();
     Serial.print(peakPressure);
     sep();
     Serial.print(ee.reqCompression);
#endif
     if (b) Serial.println();
    }
   }
  else
   Mactive=false;
}
#endif



/*
 *  This function is called once at the start of every breath.
 *  
 *  It's main role is to establish the timing for the whole cycle as well as the motor acceleration 
 *  and projected top speed.
 */

void breathingCycleStart()
{
 rebound=false; 
 BPM=ee.reqBPM;
 volume=ee.reqVolume;
 compression=ee.reqCompression;
 BPMvalue= (lastCycleStart==0)?BPM:int(60000/(millis()-lastCycleStart)); // measure the actual time it lasted
 displayBPM=BPMvalue;
 displayPeep=peepValue; 
 if (ee.active)
  {
   if (menuItem==0) setDisplayMenu(RATE_PEEP);
#ifdef rampUp 
   float r=0; 
   if (pipValue>0) r=float(compression/pipValue);
   constrain(r,1.1,2.0);
   int v=int(volumeAverage*r);
   if ((v>=minVolume) && (v<volume))  volume=v;
#endif 
  }
  else 
   {
    volumeAverage=float(volume)/3;
    pipValue=0;
   }
 peepValue=9999;
 volumePos=volume2pos(volume);
 breathPhase=breathPhaseStart;
 breathLength=int(60000/BPM); 
 breathe=millis();    // time for the next cycle start
 lastCycleStart=breathe;
 syncDelay=breathLength*(float(ee.syncRatio)/100.0);
 breathTime=breathLength;
 breathIn=breathTime/(1+ee.expirationRatio);
 breathOut=breathTime-breathIn;
 breathInSpeed=float(volumePos)/breathIn;
 breathOutSpeed=float(volumePos)/breathOut;
 breathInAcceleration=breathInSpeed*breathInSpeed*motorAcceleration;
 breathOutAcceleration=breathOutSpeed*breathOutSpeed*motorAcceleration;
 motorTarget=volumePos*motorVolumeRatio;
 motorInitialTarget=motorTarget;
 motorTargetFlt=0;
 expectedPressurebyStep==compression/motorTarget;
#ifdef serialTelemetry
 if (ee.active)
  {
   if (telemetry)
    {
     basicTelemetry(false); 
     sep();
#ifdef ArduinoPlotter     
     Serial.print(arduinoPlotterOffset+long(ee.ambientPressure)/1000);
     sep();
     Serial.print(arduinoPlotterOffset+int(volume));
     sep();
     Serial.print(arduinoPlotterOffset+long(motorTarget));
     sep();
     Serial.print(arduinoPlotterOffset+float(breathLength)/10);
     sep();
     Serial.print(arduinoPlotterOffset+float(breathIn)/10);
     sep();
     Serial.print(arduinoPlotterOffset+int(breathInSpeed*motorSpeed)/10);
     sep();
     Serial.print(arduinoPlotterOffset+float(breathInAcceleration)/50);
     sep();
     Serial.print(arduinoPlotterOffset+float(breathOut)/10);
     sep();
     Serial.print(arduinoPlotterOffset+int(breathOutSpeed*motorSpeed)/10);
     sep();
     Serial.println(arduinoPlotterOffset+float(breathOutAcceleration)/50);
#else     
     Serial.print(long(ee.ambientPressure));
     sep();
     Serial.print(int(volume));
     sep();
     Serial.print(long(motorTarget));
     sep();
     Serial.print(float(breathLength)/1000);
     sep();
     Serial.print(float(breathIn)/1000);
     sep();
     Serial.print(int(breathInSpeed*motorSpeed));
     sep();
     Serial.print(float(breathInAcceleration)/1000);
     sep();
     Serial.print(float(breathOut)/1000);
     sep();
     Serial.print(int(breathOutSpeed*motorSpeed));
     sep();
     Serial.println(float(breathOutAcceleration)/1000);
#endif     
     }
   }
#endif
#ifdef debug_0   // print data at the start of each cycle
 Serial.print(breathLength);
 tab();
 Serial.print(breathTime);
 tab();
 Serial.print(breathIn);
 tab();
 Serial.print(breathOut);
 tab();
 Serial.print(volume);
 tab();
 Serial.print(volumePos);
 tab();
 Serial.print(breathInSpeed);
 tab();
 Serial.println(breathOutSpeed);
#endif
 breathGraphCnt=10;              // blinks the LED at the start of a cycle   
 int i=int(breathInSpeed*motorSpeed);
 if (i>motorMaxSpeed) i=motorMaxSpeed;
 stepper.setMaxSpeed(i);
 stepper.setAcceleration(breathInAcceleration);
 if (ee.active)  // When a real cycle is done (with the motor actually moving)
  {
#ifdef disableMotorctrl // Optional ability to disable the motor after a time of inactivity
   if (!motorState)     // Useful to allow changing the ambubag and preserving energy on battery
    {
     digitalWrite(pin_Stepper_Disable, LOW);  // activate
     motorState=HIGH;
     delay(motorDriverRecovery);  // Allow some time for the motor driver to recover
    }
#endif     
#ifdef Beeper
  beep(2,0);
#endif        
  breathPhase=breathPhasePlanedBreathIn;
  stepper.moveTo(motorTarget);        
  }
}


/*
 * This function is called periodically during the patient inspiration phase
 * 
 * This is the place where the pressure buildup is to be measured and,
 * if required, the motor can be slowed down or stopped.
 * the inspiration phase can also be aborted prematurely if the pressure
 * gets too high.
 * 
 * This routine also manages the various errors that can happen
 * 
 * The interval between calls is determined by samplePeriod (default 50 ms)
 * 
 */

void breathingInspiration ()
{
 long cpos=stepper.currentPosition (); 
 if (relPressure>compression/4) // control once 25% of pressure achieved
  {
   float v= 1-float(peakPressure)/50; // 0.7 to 1 depending on pressure increase rate 
   constrain(v,0.7,1);
   if (relPressure>cpos * expectedPressurebyStep * v) // pressure increase too fast
    {
     long tpos=motorInitialTarget/2;
     if (relPressure>1) tpos=cpos * (compression/relPressure);
     tpos=constrain(tpos,cpos,motorInitialTarget);
     if ((motorTargetFlt>0) && (tpos>motorTargetFlt)) rebound=true;
     IIRFilter(tpos,motorTargetFlt,(rebound)?motorReboundFilter:motorTargetFilter);
     motorTarget=motorTargetFlt;
     cli();
     if (abs(stepper.currentPosition ()-motorTarget)>minMotorMove) 
       {
         
        stepper.setAcceleration((cpos>motorTarget)?motorMaxAcceleration:breathInAcceleration); 
        stepper.moveTo(motorTarget);  
       }
     sei();
    }
  }  
 if (!ee.CVmode)  // CP (Pressure Control) Mode
  {
   if ((relPressure>compressionScale) && (breathPhase==breathPhasePlanedBreathIn))  // Check for pressure
    {
     motorFastStop();
#ifdef activeBeeper     
     if (cpos<(motorTarget)*failVolumeRatio) beep(alarmDuration,1); // Insufficient volume alarm
#endif        
    }
  }
}

/*
 *  This function is called periodically at all times 
 *  
 *  It detects if the pressure is too high
 */


void breathOverpressureAlarm()
{
 if (relPressure>alarmCompressionValue*compressionScale) // over pressure detected as an error in all modes if it lasts more than 500 ms 
  {
   if (highPressureAlarmCnt<highPressureAlarmDetect)
    highPressureAlarmCnt++; 
#ifdef Beeper        
   else 
    beep(alarmDuration,2); 
#endif
   if (stepper.currentPosition()< stepper.targetPosition ())
     motorFastStop(); // stop the motor if it was moving forward (increasing the pressure)
  }  
 else
  if (highPressureAlarmCnt) highPressureAlarmCnt--;
}

/*
 * This function is called once when the patient inspiration phase is over
 * and the motor needs to return to it's zero point.
 * 
 * It sets the motor acceleration and projected top speed for the movement.
 */

void breathingExpirationStart()
{
 if (breathPhase<breathPhaseStartBreathOut)
  {breathPhase=breathPhaseStartBreathOut; 
#ifdef serialTelemetry
   basicTelemetry(true);
#endif        
   int i=int(breathOutSpeed*motorSpeed);
   if (i>motorMaxSpeed) i=motorMaxSpeed;
   long p=stepper.currentPosition()/motorVolumeRatio;
   stepper.setMaxSpeed(i);
   stepper.setAcceleration(breathOutAcceleration);
   stepper.moveTo(0);

   // dichotomy search for volume from position. (takes time, but the motor just ramps up, so it's ok).
   
   volumeValue=1024;
   int d=512;
   while (d)
    {
     volumeValue=(volume2pos(volumeValue)<p)?volumeValue+d:volumeValue-d;
     d=d/2; 
    }
   displayVol=volumeValue; 
   IIRFilter(volumeValue,volumeAverage,volumeAverageFilter);
   displayPip=pipValue; 
   if ((ee.active) && (menuItem==0)) setDisplayMenu(VOL_PIP);
   pipValue=0; 
  } 
}


void breathAssistTrigger()
{
 if ((relPressure<ee.peep-0.1*compressionScale) && (peakPressure<-0.1*compressionScale))
  {
   if (breathPhase==breathPhasePlanedBreathOut)
    {
     breathPhase=breathPhasePatientTrigger;
#ifdef serialTelemetry
     if (ee.active) basicTelemetry(true);
#endif        
    }
   breathe=millis()-breathLength-syncDelay; // Trigger a new cycle sooner.
#ifdef debug_1    // print data for user triggered cycle    
   Serial.print(relPressure);
   tab();
   Serial.print(ee.peep);
   tab();
   Serial.print(peakPressure);
   tab();
   Serial.print(compressionScale);
   Serial.println(F(" Sync"));
#endif        
  }
}


void loop() {

//    stepper.run();

  if (checkMillis(tick,samplePeriod))  // Read the sensors and acct according to the results (every 20 millisecond)
   {
    tick=tick+samplePeriod;
    readSensors();
#ifdef serialTelemetry
   if (ee.active) basicTelemetry(true);
#endif
#ifdef analogKeyboard
   analogReadKeys(); 
#endif
    compressionScale=(compression>minCompression)?compression:defaultCompression;
    volumeScale=(volume>minVolume)?volumePos:defaultVolume;   
    if (breathPhase==breathPhasePlanedBreathIn) breathingInspiration ();
    breathOverpressureAlarm();
#ifdef dispBargraph   
    updateBargraph();   
#endif      
#ifdef isKeyboard      
    // Keyboard actions
    repeatCheck(btnDn);
    repeatCheck(btnUp);
    if (keys[btnOk])
     {
      if ((menuItem==0) && ee.active)  // stop request, add delay
       {
        if (okDelay==okLong) pressOk();        // Ok button
        if (okDelay<=okLong) okDelay++;
       }
      else
       {
        if (!mkeys[btnOk])   pressOk();        // Ok button
        okDelay=0; 
       }
     } else okDelay=0;
    
    if (keys[btnPrev] && !mkeys[btnPrev]) pressPrevNext();  // previous item button
    if (keys[btnNext] && !mkeys[btnNext]) pressPrevNext();  // next item button
    if (keys[btnDn]   && !mkeys[btnDn])   pressUpDown();    // lower value button
    if (keys[btnUp]   && !mkeys[btnUp])   pressUpDown();    // increase value button
    byte ct=maxBtn;
    while (ct--) 
     {
      if (keys[ct]) lastKey=millis();
      mkeys[ct]=keys[ct];
     }
#ifdef dispMenus     
    if ((checkMillis(lastKey,keyIdleReturn)) && (menuItem!=0))
     {
      menuItem=0;
      setDisplayMenu(Menu_message,std_dly,0);
     }
#endif     
#endif     
#ifdef disableMotorctrl                  // after being inactive for a while, the motor is disabled
     if (ee.active) lastactive=millis();
     if (checkMillis(lastactive,motorOffDelay)) 
      {
       digitalWrite(pin_Stepper_Disable, HIGH);  // Deactivate
       motorState=LOW; 
      }
#endif     
   }
  if (checkMillis(breathe,breathLength+syncDelay) && ((stepper.currentPosition()>0) || stepper.isRunning())) breathe=millis()-(breathLength+syncDelay+50); // wait for the previous cycle to finish (safety)
   else
   {
    if (relPressure>pipValue)
     {
      pipValue=relPressure;  
      lastValChange=millis();
     }
    if ((breathPhase>breathPhasePlanedBreathIn) && (relPressure<peepValue))
     {
      peepValue=relPressure;  
      lastValChange=millis();
     }
    
    if (checkMillis(breathe,breathLength+syncDelay)) breathingCycleStart(); // start of a cycle
    if (stepper.distanceToGo() == 0)
     {
      if (checkMillis(breathe,breathIn))
        {
         if (stepper.isRunning())
          {
           stepper.setAcceleration(motorMaxAcceleration); // ensures complete move
           if (checkMillis(breathe,breathIn+StallTimeout)) stepper.moveTo(stepper.currentPosition()-10);  
          }
          else breathingExpirationStart(); // second half of a cycle
        }
      if (breathPhase==breathPhaseStartBreathOut) breathPhase=breathPhasePlanedBreathOut;
      if (breathPhase==breathPhasePlanedBreathOut) breathAssistTrigger();;
     }
       }  
#ifdef watchdog
#ifdef watchdogProtect        
 if (!watchdogStarted && (millis()>watchdogStartDelay))       // True when the watchdog has been activated. the first few seconds after reset are without watchdog protection
  {
   Watchdog.enable(watchdogDelay);
   watchdogStarted=true; 
  }
#endif        
 Watchdog.reset();
#endif        
#ifdef USBcontrol     
 SerialWait(1);          // wait for 10 usec looking for received characters
#else
 delayMicroseconds(10);  // wait for 10 usec
#endif 
}
