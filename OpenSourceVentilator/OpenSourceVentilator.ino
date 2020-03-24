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

#define minBPM                     10.0   // minimum respiratory speed
#define defaultBPM                 15.0   // default respiratory speed
#define stepBPM                     1.0   // adjustment step for respiratory speed
#define maxBPM                     35.0   // maximum respiratory speed
#define maxBPMchange                0.2   // maximum respiratory speed change in proportion of final value per beat (1=100%)
#define minVolume                 100.0   // minimum respiratory volume in milliliters 
#define defaultVolume             150.0   // default respiratory volume in milliliters 
#define stepVolume                100.0   // adjustment step for respiratory volume in milliliters 
#define maxVolume                 800.0   // maximum respiratory volume in milliliters 
#define maxVolumeChange             0.25  // maximum respiratory volume change in proportion of final value per beat (1=100%) 
#define minCompression           1000.00  // minimum compression for the ambu-bag in Pa
#define stepCompression           500.00  // adjustment step for compression for the ambu-bag in Pa
#define defaultCompression       3000.00  // default compression for the ambu-bag in Pa
#define maxCompression          20000.00  // maximum compression for the ambu-bag in Pa
#define maxCompressionChange        0.5   // maximum compression for the ambu-bag change in proportion of final value per beat (1=100%)
#define minSyncRatio                0.00  // minimum portion of the cycle for breath synchronisation
#define stepSyncRatio               0.05  // adjustment step for portion of the cycle for breath synchronisation
#define defaultSyncRatio            0.15  // default portion of the cycle for breath synchronisation
#define maxSyncRatio                0.40  // maximum portion of the cycle for breath synchronisation
#define minExpirationRatio          1.00  // minimum portion of the cycle for breath synchronisation
#define stepExpirationRatio         0.2   // adjustment step for portion of the cycle for breath synchronisation
#define defaultExpirationRatio      1.2   // default portion of the cycle for breath synchronisation
#define maxExpirationRatio          3.00  // maximum portion of the cycle for breath synchronisation
#define failVolumeRatio             0.90  // In CP mode, Alarm if pressure reached before that portion of programmed volume.



#define ambientPressureFilter       0.002 // IIR filtering ratio (lower value produce longer time constant)
#define avgPressureFilter           0.1   // IIR filtering ratio (lower value produce longer time constant)
#define defaultPressure        105000.00  // Pressure in Pa returned when no sensor is found
#define defaultAmbientPressure 105000.00  // assumed ambiant pressure in Pa returned when no sensor is found
#define defaultHumidity            50.00  // humidity in % RH returned when no sensor is found
#define defaultTemperature         20.00  // temperature in °C returned when no sensor is found



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
#define motorVolumeRatio            0.8   // Ratio of distance in steps to air volume in step per milliliter.
                                          

/*******************************   HARDWARE OPTIONS   *******************************
 *    
 *    It's normal for the program to not compile if some of these are undefined as they need an alternative
 *    
 */

// #define CurrentSense     // uncomment to add motor current sensing with the Allegromicro ACS712 sensor
                          // This is not enabled by default as first test show the sensor is not sensitive
                          // enough. Another sensor should probably be used.

#define USBcontrol        // USB / serial Command line interface.
                          // This allows complete control of all parameters.

#define E2PROM            // Uses the internal EEPROM for parameter storage

#define TM1638Keyboard    // Use a TM1638 for the keyboard

#define TM1638Display     // Use a TM1638 for the display

#define TM1638bargraph     // Use a TM1638 for the Led pressure bargraph

#define ActiveBeeper      // Active beeper can be used on any pin. Passive beeper will require a PWM capable pin

#define Led               // Led debugging / Signal (should be disabled if SPI peripherals are present)

#define BoschBMxSensor    // Bosch Sensortech BMP280 or BME280

#define stepDirMotor      // Control the motor with step and Direction signals

#define disableMotorctrl  // control moter activation / desactivation      


//******************************   IMPIED DEFINITIONS  ********************************

#ifdef BoschBMxSensor
#ifndef I2C
#define I2C
#endif
#endif

#ifdef TM1638Keyboard
#define TM1638
#endif

#ifdef TM1638Display
#ifndef TM1638
#define TM1638
#endif
#endif

#ifdef TM1638bargraph
#ifndef TM1638
#define TM1638
#endif
#endif

#ifdef ActiveBeeper
#define Beeper
#endif

//********************************   CONNECTION PINS   ********************************

#ifdef TM1638
// Datasheet : https://csserver.evansville.edu/~mr63/Courses/Projects/TM1638en.pdf

#define  pin_Strobe_TM         2
#define  pin_Clock_TM          3
#define  pin_DIO_TM            4
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


//*******************************   REQUIRED LIBRARIES   *******************************

#ifdef TM1638               // Keyboard / display / LED combo board
#include <TM1638plus.h>     //  By Gavin Lyons - https://github.com/gavinlyonsrepo/TM1638plus
#endif                      

#ifdef BoschBMxSensor       // Temperature / humidity / pressure
#include <BME280I2C.h>      //  by Tyler Glenn - https://github.com/finitespace/BME280
#endif                      

#ifdef I2C
#include <Wire.h>           // I2C protocol
#endif

#ifdef currentSense
#include <avr/sleep.h>      // Sleep management library
#endif

#ifdef E2PROM
#include <EEPROM.h>         // read / write to the processor's internal EEPROM
#endif

#ifdef StepGen
#include <AccelStepper.h>  // Stepper / servo library with step pulse / dir interface
#endif                      //  By Mike McCauley - http://www.airspayce.com/mikem/arduino/AccelStepper


#include "TimerOne.h"       // Timer component
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

#ifdef TM1638Display
#define dispBufferLength 12
#endif

#ifdef TM1638
TM1638plus tm(pin_Strobe_TM, pin_Clock_TM , pin_DIO_TM);  //Constructor object
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

#ifdef StepGen
// Define the stepper and the pins it will use
AccelStepper stepper(AccelStepper::DRIVER, pin_Stepper_Step, pin_Stepper_DIR);
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

#define samplePeriod 50  // 50 ms sensor sampling period
#define highPressureAlarmDetect 10  // delay before an overpressure alarm is triggered (in samplePeriod increments)



#define startup_message       0    // power up start
#define error_message        10    // 10 + error code
#define Menu_message        100    // screen for each menu item 
#define Measurement_Start   101    // Screen with "start" message
#define Stop_message        102    // stop indicator screen  
#define Measurement_Message 103    // first of measurement messages

#define MenuItems         7
#define keyIdleReturn 16000    // returns to main menu after 16 seconds without key press
#define motorOffDelay 10000    // Motor is cut off after that time being inactive.


boolean term,                  // true if the terminal is in human mode, false in machine mmode (not fully implemented)
        stopMove;              // Immediately stop motor movements (E-Stop)
int     dispDelay,             // delay before the next display update or change
        dispPhase,             // Main variable that controls the display and menu start machine. Contains the current screen number
        dispCnt,               // Counter for dispDelay
        dispParam,             // additional parameter used on certain display screens
        timCnt,                // Timer interrupt cycle counter used to spread the load over time
        breathGraphCnt,        // Counter for the visual blinking of the bargraph at each breathing cycle start
        beepCnt,               // Counter for the delay before the end of the current beep (or 0). Time is in 10ms increments
        timerLvl,              // Count timer interrupt nesting and skips interface update when more than one
        syncDelay,             // Time (in ms) when the patient can symchronize the breathing cycle by inhaling (negative pressure)
        highPressureAlarmCnt;  // delay when the pressure is too high (>1.2x preset value) before an alarm is triggered
boolean doDisp,                // indicates if a display update is required or not.
        sensHumidity,          // True if a humidity sensor is present, false otherwise
        sensTemperature,       // True if a temperature sensor is present, false otherwise
        sensPressure,          // True if a pressure sensor is present, false otherwise
        sensFlow,              // True if an airflow sensor is present, false otherwise (not used yet)
        allSet,                // True if all actual values match the programed values, false when ramping values up or down
        active,                // True if the machine is currently doing breathing cycles, false otherwise
        motorState,            // True if the motor is activated, false otherwise, false otherwise
        CVmode,                // CV or CP mode indicator; 
        debug;                 // True if a debugging mode is used, false otherwise
char    disp[dispBufferLength];// Display data buffer
float   ambientPressure,       // Calculated ambiant air pressure (averaged)
        pressure,              // Current pressure, as read from the sensor
        peakPressure,          // high pass filtered value of the pressure. Used to detect patient initiated breathing cycles
        avgPressure,           // Averaged pressure (used to limit the motor speed with short spikes filtered out
        relPressure,           // relative pressure (measured pressure - ambiant pressure)
        temperature,           // temperature as read by the sensor, in Celsius
        humidity,              // humidity as read by the sensor, in % RH
        breathInSpeed,         // Speed of the breath inhalation in liter / second
        breathOutSpeed;        // speed of the breath exhalation in liter / second
long    tick,                  // counter used to trigger sensor measurements
        breathe,               // counter used to track the current breathing cycle
        lastKey,               // last time when a key was pressed (used to automatically return to the main menu) 
        commTime,              // Last time a serial communication was received
        lastActive,            // Last time the motor was active
        breathLength,          // duration of the current breathing cycle in milliseconds. This is 60000/BPM.
        breathTime,            // Length of cycle minus the sync period
        breathIn,              // Length of the breath inhalation
        breathOut;             // Length of the breath exhalation
float   BPM,                   // beats Per Minute. This is the minimum breathing rate, as defined by the user
        volume,                // Breathing volume per cycle
        compression,           // Max / target compression while inhaling
        compressionScale,      // Scale of the compression relative to set value
        volumeScale;           // Scale of the volume relative to set value
byte    sc,                    // counter for displaying debugging info about the sensors 
        breathPhase,           // Current phase of the breathing cycle;
        barGraph,              // segments of the pressure bar graph
        bargraphDot,           // rate indicator blinking segment on the bargraph
        menuItem,              // Menu item currently being set by the user
        dispTick;              // Auxilary value for the dislpay. Can be used as a blink counter or similar 
byte    keys[maxBtn],          // Debounced keyboard keys
        kcnt[maxBtn],          // Keyboard debounce counter
        mkeys[maxBtn];         // Key memory (allows to detect button transitions)

// Parameters saved to / recovered from EEPROM

float   reqBPM;                // respiratory frequency
float   reqVolume;             // respiratory volume in milliliters 
float   reqCompression;        // compression for the ambu-bag in Pa
float   syncRatio;             // portion of the cycle for breath synchronisation
float   expirationRatio;       // The proportion of each breathing cycle that is spent breathing in compared to breathing out

#ifdef  CurrentSense
int     currentVal;            // ADC result of the current measurement
boolean sensCurrent;           // True if a current sensor is present, false otherwise
float   idleCurrent,           // Averaged value of the ADC when the motor is idle
        currentRaw,            // unfiltered, scaled current value 
        current;               // filtered, scaled current value 
#endif
 

void (* resetFunction) (void) = 0;  // Self reset (to be used with watchdog)

void tab ()
{
 if (term) Serial.print('\t');
}


boolean checkValues()
{
 boolean isOk=(reqBPM>=minBPM);                      // BPM in allowed range ?
 if (reqBPM>maxBPM) isOk=false;                 
 if (reqVolume<minVolume) isOk=false;                // Volume in allowed range ?
 if (reqVolume>maxVolume) isOk=false;           
 if (reqCompression<minCompression) isOk=false;      // Compression in allowed range ?
 if (reqCompression>maxCompression) isOk=false; 
 if (syncRatio<minSyncRatio) isOk=false;             // portion of the cycle for breath synchronisation in allowed range ?
 if (syncRatio>maxSyncRatio) isOk=false;
 if (expirationRatio<minExpirationRatio) isOk=false; // portion of the cycle for breath synchronisation in allowed range ?
 if (expirationRatio>maxExpirationRatio) isOk=false;
 if (isnan(reqBPM)) isOk=false;                      // Check for malformed floating point values (NaN)
 if (isnan(reqVolume)) isOk=false;  
 if (isnan(reqCompression)) isOk=false;  
 if (isnan(syncRatio)) isOk=false;  
 if (isnan(expirationRatio)) isOk=false;  
 return isOk;     
}

void meas()
{
 Serial.print(F("Temp:")); 
 Serial.print(temperature);
 Serial.print(F("°C\tHumidity:")); 
 Serial.print(humidity);
 Serial.print(F("% RH\tPressure:")); 
 Serial.print(pressure);
 Serial.print(F(" Pa\tAmbient Pressure:")); 
 Serial.print(ambientPressure);
 Serial.print(F(" Pa\tRelative Pressure:")); 
 Serial.print(relPressure);
 Serial.println(F(" Pa"));  
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
#ifdef TM1638bargraph
  for (uint8_t position = 0; position < 8; position++)
   {
    tm.setLED(position, value & 1);
    value = value >> 1;
   }
#endif  
}

void doDisplay()
{
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
}

void debounceKeyboard()
{
 uint16_t b = readKeyboard(); // returns a byte with values of button s8s7s6s5s4s3s2s1
 uint16_t c=maxBtn;
 uint16_t m=1;
 while (c--)  //Keyboard debounce
  {
   if (m & b)
     {if (kcnt[c]>debounce) keys[c]=true; else kcnt[c]++;}
    else
     {if (kcnt[c]==0) keys[c]=false; else kcnt[c]--;}
   m+=m; 
  }
}

#ifdef Beeper
void beep(int lng)  // Launch a beep sound
{
 pinMode(pin_Beep, OUTPUT); 
 if (lng) digitalWrite(pin_Beep, LOW);  // active (inverted)
 beepCnt=max(beepCnt,lng);              //  if overlapping sounds, use the longest of them 
}
#endif

#ifdef E2PROM
void eeput (int n)  // records to EEPROM (only if values are validated)
{
 int eeAddress = eeStart;
 boolean isOk=checkValues();
    
 if (n==1) isOk=true;  // override (for debug testing)
 if (isOk)
  {
   EEPROM.put(eeAddress, reqBPM);
   eeAddress += sizeof(float);
   EEPROM.put(eeAddress, reqVolume);
   eeAddress += sizeof(float);
   EEPROM.put(eeAddress, reqCompression);
   eeAddress += sizeof(float);
   byte b=(active)?1:0;
   EEPROM.put(eeAddress, b);
   eeAddress += sizeof(byte);
   EEPROM.put(eeAddress, syncRatio);
   eeAddress += sizeof(float);
   EEPROM.put(eeAddress, expirationRatio);
   eeAddress += sizeof(float);
   b=(CVmode)?1:0;
   EEPROM.put(eeAddress, b);
   eeAddress += sizeof(byte);
  }
}    
#endif

void eeget ()
{    
#ifdef E2PROM
 int eeAddress = eeStart;
 byte tac,cvm;

 EEPROM.get(eeAddress, reqBPM);
 eeAddress += sizeof(float);
 EEPROM.get(eeAddress, reqVolume);
 eeAddress += sizeof(float);
 EEPROM.get(eeAddress, reqCompression);
 eeAddress += sizeof(float);
 EEPROM.get(eeAddress, tac);
 eeAddress += sizeof(byte);
 EEPROM.get(eeAddress, syncRatio);
 eeAddress += sizeof(float);
 EEPROM.get(eeAddress, expirationRatio);
 eeAddress += sizeof(float);
 EEPROM.get(eeAddress, cvm);
 eeAddress += sizeof(byte);
 // Check if the data is coherent
 boolean isOk=checkValues();
 if (tac>1) isOk=false; // is the value 0 or 1 ?
 if (cvm>1) isOk=false; // is the value 0 or 1 ?
 if (!isOk) 
  {
   reqBPM          = defaultBPM;
   reqVolume       = defaultVolume;
   reqCompression  = defaultCompression;
   syncRatio       = defaultSyncRatio;
   expirationRatio = defaultExpirationRatio;
   tac=0;
   cvm=1;
  }
 active=(tac>0);  
 CVmode=(cvm>0);  
 if (term) Serial.print((isOk)?F("Read EEPROM Settings\n"):F("Read Default Settings\n"));
#else
 reqBPM          = defaultBPM;
 reqVolume       = defaultVolume;
 reqCompression  = defaultCompression;
 syncRatio       = defaultSyncRatio;
 expirationRatio = defaultExpirationRatio;
 active=false;
 CVmode=true;
 if (term) Serial.print(F("Read Default Settings\n"));
#endif
}   


#ifdef USBcontrol

#define CommandMaxLength 30

byte BufferLength;
String Buffer = "*******************************";  // filler

void prstat()
{
 Serial.print(F("\nPatient assisted breathing\tO"));
 Serial.println((active)?F("N"):F("ff"));
 Serial.println((CVmode)?F("CV mode"):F("CP mode"));
 Serial.print(F("Ventilation speed\t\t"));
 Serial.print(reqBPM);
 Serial.println(F(" cycles / minute"));
 Serial.print(F("Ventilation volume\t\t"));
 Serial.print(reqVolume/1000);
 Serial.println(F(" liter"));
 Serial.print(F("Ventilation pressure\t\t"));
 Serial.print(reqCompression/1000);
 Serial.println(F(" KPa"));
 Serial.print(F("Patient synchonization\t\t"));
 Serial.print(int(syncRatio*100.1));
 Serial.print(F(" % of cycle\n"));
 Serial.print(F("Expiration ratio\t\t"));
 Serial.print(expirationRatio);
 Serial.println(F(" x inspiration\n"));
}

void SerialCommand()
{
  commTime=millis(); 
  float r;
  char ch=Buffer[1];
  switch ( Buffer[0])
   {
    case '1': // 1 Start assisted breathing
     Serial.println(F("Breathing started"));
     active=true;
    break;
    case 'C': // CV / CP mode
     if (ch='V') CVmode=true; 
     if (ch='P') CVmode=false; 
     Serial.println((CVmode)?F("CV mode"):F("CP mode"));
    break;
    case '0': // 0 Stop assisted breathing
     active=false;
     Serial.println(F("WARNING: Breathing stopped"));
     setDisplayMenu(Stop_message,std_dly,0);
    break;
    case 'S': // S set Speed (in BPM)
     Buffer[0]=32;
     r=Buffer.toFloat();
     if ((r>=minBPM) && (r<=maxBPM))
      {reqBPM=r; prstat();} else Serial.println(F("Out of range (6-40BPM)"));  
    break;
    case 'V': // V set Volume (in liters)
     Buffer[0]=32;
     r=Buffer.toFloat()*1000;
     if ((r>=minVolume) && (r<=maxVolume))
      {reqVolume=r; prstat();} else Serial.println(F("Out of range (0.1-1.5l)"));  
    break;
    case 'P': // P Set pressure (in KPa)
     Buffer[0]=32;
     r=Buffer.toFloat()*1000;
     if ((r>=minCompression) && (r<=maxCompression))
      {reqCompression=r; prstat();} else Serial.println(F("Out of range (1-20KPa)"));  
    break;
    case 'E': // E set expiration ratio (x inspiration)
     Buffer[0]=32;
     r=Buffer.toFloat();
     if ((r>=minExpirationRatio) && (r<=maxExpirationRatio))
      {expirationRatio=r; prstat();} else Serial.println(F("Out of range (1 to 3 x inspiration)"));  
    break;
    case 'Y': // Y set sYnc (in % of cycle)
     Buffer[0]=32;
     r=Buffer.toFloat()/100;
     if ((r>=minSyncRatio) && (r<=maxSyncRatio))
      {syncRatio=r; prstat();} else Serial.println(F("Out of range (0-40%)"));  
    break;
    case 'M': // M show Measurements from the sensor
     meas();  
    break;
#ifdef EEPROM
    case 'W': // T Terminal mode (human readable)
     eeput(0);  // Save settings to EEPROM
     menuItem=0;
     Serial.println(F("Parameters Written"));  
    break;
#endif    
    case 'D': // D debug (on / off)
     debug=!debug;
    break;
    case 'R': // R Reset
     cli();
     resetFunction();
    break;
    case '?': // help
     if (term)
      {
       prstat();  
       Serial.println(F("1 Start assisted breathing")); 
       Serial.println(F("CV sets Constant Volume mode")); 
       Serial.println(F("CP sets Constant Pressure mode")); 
       Serial.println(F("0 Stop assisted breathing")); 
       Serial.println(F("S set Speed (in BPM)")); 
       Serial.println(F("V set Volume (in liters)")); 
       Serial.println(F("P set Pressure (in KPa)")); 
       Serial.println(F("E set expiration ratio (x inspiration)")); 
       Serial.println(F("Y set sYnc (in % of cycle)")); 
       Serial.println(F("M show Measurements from the sensor")); 
#ifdef E2PROM
       Serial.println(F("W Write parameters to permanent memory")); 
#endif       
       Serial.println(F("D debug (on / off)")); 
       Serial.println(F("R Reset")); 
       Serial.println(F("? print this")); 
      }
      break;
    default:   // Unknown command 
     Serial.println(F("Unknown command. type '?' for instructions.")); 
    break;
   }      
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


void displayMenu()  
{
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
     strcpy(disp,"SOFT 0.14");
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
     dispPhase=99;
    break;
  case 99:
     strcpy(disp,"SENSOR");
     dispPhase=10;
    break;
  case 100:
     if (compression<10000) 
       sprintf(disp,"%02d %d.%d %d.%d",int(BPM),int(volume/1000),int (volume/100) %10,int(compression/1000),int (compression/100) %10);
      else
       sprintf(disp,"%02d %d.%d %d",int(BPM),int(volume/1000),int (volume/100) %10,int(compression/1000));
     dispDelay=100;  
     if (active) dispPhase=(dispParam==0)?100:103; else dispPhase++;
    break;
  case 101:
     strcpy(disp,"START  _");
     disp[6]=(CVmode)?'V':'P';
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
     sprintf(disp,"Huni%d",int(humidity));    // character N looks better chan character M
     dispPhase++;
    break;
  case 105:
     sprintf(disp,"PRESS. %dH",int(relPressure/100));
     dispPhase++;
    break;
  case 106:
     sprintf(disp,"SYNC.  %d",int(syncRatio*100.1));
     dispPhase++;
    break;
  case 107:
     sprintf(disp,"Ratio %d.%d",int(expirationRatio),int (expirationRatio*10) %10);
     dispPhase=100;
    break;
      
  case 110:
     sprintf(disp,(dispTick++ & 1)?"SET   %d":"RATE  %d",int(reqBPM));
     dispDelay=60;  
    break;
  case 120:
     sprintf(disp,(dispTick++ & 1)?"SET   %d.%d":"VOLUN. %d.%d",int(reqVolume/1000),int (reqVolume/100) %10); // character N looks better chan character M
     dispDelay=60;  
    break;
  case 130:
     sprintf(disp,(dispTick++ & 1)?"SET   %d.%d":"PRESS. %d.%d",int(reqCompression/1000),int (reqCompression/100) %10);
     dispDelay=60;  
    break;
  case 140:
     sprintf(disp,(dispTick++ & 1)?"SET   %2d":"SYNC.  %2d",int(syncRatio*100.1)); // 100.1 instead of 100 prevents rounding errors
     dispDelay=60;  
    break;
  case 150:
     dispTick++;
     sprintf(disp,(dispt==2)?"SET   %d.%d":"EXP.  %d.%d",int(expirationRatio),int (expirationRatio*10) %10); 
     if (dispt==1) sprintf(disp,"Ratio %d.%d",int(expirationRatio),int (expirationRatio*10) %10); 
     dispDelay=60;  
    break;
  case 160:
     strcpy(disp,(dispTick++ & 1)?"SETUP  _":"SAVE   _");
     dispDelay=60;  
    break;
  default:
    if (term) Serial.println(dispPhase); 
    dispPhase=0; // Abnormal the phase should always be defined
   break;
  }
 dispCnt=dispDelay;
}


void Timer() 
{
 timerLvl++;  
 if (!stopMove) stepper.run();
 if (timerLvl>1)  // only does the motor update if nested more than 1 level
  {
   timerLvl--; 
  }
  else
  {
#ifdef Led  
   digitalWrite(pin_LED, HIGH);
#endif
   sei();  // Allows nested interrupts
   switch (timCnt % 50) {        // 10 ms tick period for interface handling
    case 0:
     if (--dispCnt<=0) displayMenu();
     break;
    case 10:      // change the display
     if (doDisp)
      {
       doDisplay(); 
       doDisp=false;
      }
     break;
    case 15:      // update the beep
#ifdef Beeper    
     if (beepCnt==1) digitalWrite(pin_Beep, HIGH);  // inactive (inverted)
     if (beepCnt) beepCnt--;
#endif     
     break;
    case 20:      // update the bargraph
     displayBargraph((breathGraphCnt>0)?barGraph-bargraphDot:barGraph);
     if (breathGraphCnt) breathGraphCnt--;
     break;
    case 30:      // update the keyboard
      debounceKeyboard();
     break;
    } 
   if (++timCnt>9999) timCnt=0;  // counts 0 to 9999
#ifdef Led
   digitalWrite(pin_LED, LOW);
#endif
   cli();  // no interrupts
   timerLvl--; 
  }
}


void readSensors() // Works no matter if sensor is present or not (returns default values if not present)

{
 float temp(NAN), hum(NAN), pres(NAN);
 BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
 BME280::PresUnit presUnit(BME280::PresUnit_Pa);
 bme.read(pres, temp, hum, tempUnit, presUnit);
 if (sensPressure)
  {
    pressure=pres;
    if (stepper.currentPosition()==0)  // approximate athmospheric pressure by averaging when the bag is filled
     ambientPressure=(ambientPressure==0)?pressure:ambientPressure*(1-ambientPressureFilter)+pres*ambientPressureFilter; // low pass filtering
    avgPressure=(avgPressure==0)?pressure:avgPressure*(1-avgPressureFilter)+pres*avgPressureFilter;                      // low pass filtering
    peakPressure=pressure-avgPressure;
  }
  else
  {
   pressure=defaultPressure;
   ambientPressure=defaultAmbientPressure;
  }
 if (sensHumidity)
  {
   humidity=hum;
  }
  else humidity=defaultHumidity;
 if (sensTemperature)
  {
   temperature=temp;
  }
  else temperature=defaultTemperature;
 relPressure=pressure-ambientPressure; 

#ifdef CurrentSense
 if (sensCurrent)
  {
   set_sleep_mode(SLEEP_MODE_IDLE);
   sleep_enable();
   sleep_mode(); 
   sleep_disable();
   int c=analogRead(pin_current_Sense);
   if (stepper.speed()==0) // average of idle current value when the motor does not run
    idleCurrent=(idleCurrent==0)?c:idleCurrent*(1-idleCurrentFilter)+c*idleCurrentFilter; // low pass filtering
   currentVal=c-int(idleCurrent); 
   currentRaw=(currentRaw==0)?analogCurrentRatio*currentVal:currentRaw*(1-currentFilter)+analogCurrentRatio*currentVal*currentFilter; // low pass filtering
   current=abs(currentRaw);
  }
  else current=0;
#endif
 if ((term) && (sc++>10) && (debug))
  {
   sc=0;
   meas();
  }
}

void detectBMEsensor()  // Detects the pressure sensor correct type or the absence of a functional pressure sensor
{
 sensHumidity=false;
 sensTemperature=false;
 sensPressure=false;
 if(bme.begin())
  {
   switch(bme.chipModel())
    {
    case BME280::ChipModel_BME280:
      if (term) Serial.println(F("Found BME280 sensor!"));
      sensHumidity=true;
      sensTemperature=true;
      sensPressure=true;
      break;
    case BME280::ChipModel_BMP280:
      if (term) Serial.println(F("Found BMP280 sensor! No Humidity available"));
      sensTemperature=true;
      sensPressure=true;
      break;
    default:
      setDisplayMenu(error_message,std_dly,1);
      if (term) Serial.println(F("Found UNKNOWN sensor! Error!"));
    }
  }
  else
  {
   setDisplayMenu(error_message,std_dly,0);
   if (term) Serial.println(F("Could not find BME280 sensor"));
  }
}

#ifdef CurrentSense
void detectCurrentSensor()     // detects if a Allegromicro current sensor is connected to pin_current_Sense or not.
{
 pinMode(pin_current_Sense,INPUT_PULLUP);
 delay (10);
 sensCurrent=(analogRead(pin_current_Sense)<750); // If the pin is left floating, it's voltage should be well above midpoint
 if (sensCurrent)
  {
   currentVal=abs(analogRead(pin_current_Sense)-current0);
   sensCurrent=(currentVal < currentMaxIdle);
  }
 if (sensCurrent)  Serial.println(F("Found current sensor!"));
}
#endif


void setup() {
 // put your setup code here, to run once:
 timerLvl=0;
 Timer1.initialize(200);  
 Timer1.attachInterrupt(Timer);
 setDisplayMenu(startup_message); 
 
 pinMode(pin_Stepper_DIR, OUTPUT);
 digitalWrite(pin_Stepper_DIR, LOW);     // active (inverted)
 pinMode(pin_Stepper_Step, OUTPUT);
 digitalWrite(pin_Stepper_Step, LOW);    // active (inverted)
#ifdef disableMotorctrl
 pinMode(pin_Stepper_Disable, OUTPUT);
 digitalWrite(pin_Stepper_Disable, LOW);  // active (inverted)
 motorState=true;
#endif 
#ifdef Beep
 beep(20); 
#endif 
 Serial.begin(SERIAL_BAUD);
 int c=1000;
 while (!Serial && c--) delay(1); // wait for serial port to connect. Needed for native USB (non blocking)
 term=Serial; 
 if (term) Serial.println(F("Open Source Ventilator Ver 0.14"));
 Wire.begin();
 detectBMEsensor();
#ifdef CurrentSense
 detectCurrentSensor()
#endif
 eeget ();    // read startup parameters (either from EEPROM or default value)
#ifdef USBcontrol 
 Buffer="?";  // send informations to the serial port
 SerialCommand();
 Buffer="";
#endif 
 BPM=reqBPM;                 // Start from these values without sweep
 volume=reqVolume;
 compression=reqCompression;
 readSensors();   
 tick=millis();
 breathLength=int(60000/BPM);
 breathe==tick-breathLength;
 stopMove=false;
// stepper.setMaxSpeed(3500);
// stepper.setAcceleration(20000);
// stepper.moveTo(0);
}

void limitValues(float &B,float &V,float &C,float &D,float &E) // prevent values from going outside of their respective limits 
{
 B = constrain(B, minBPM, maxBPM); 
 V = constrain(V, minVolume, maxVolume); 
 C = constrain(C, minCompression, maxCompression); 
 D = constrain(D, minSyncRatio, maxSyncRatio); 
 E = constrain(E, minExpirationRatio, maxExpirationRatio); 
}




void pressOk()
{
 if (menuItem==0)
   {
    active=!active;  // Toggle function when "Enter" key pressed
    if (!active) setDisplayMenu(Stop_message,std_dly,0);
   }
  else
   {                 // Return to main screen when "Enter" key pressed
#ifdef E2PROM            
    if (menuItem==MenuItems-1) eeput(0);  // Save settings to EEPROM
#endif           
    menuItem=0;
    setDisplayMenu(Menu_message,std_dly,0);
   }
 if ((active) && (stepper.distanceToGo() == 0))  // If the motor is idle, start immediately
  {
   breathe=millis()-breathLength; // will happen immediately
  }
}

void pressPrevNext()
{
 if ((keys[btnPrev]) && (menuItem>0)) {menuItem--;setDisplayMenu(Menu_message+10*menuItem,std_dly,0);}            // Previous menu item (no rollover)
 if ((keys[btnNext]) && (menuItem<MenuItems-1)) {menuItem++;setDisplayMenu(Menu_message+10*menuItem,std_dly,0);}  // Next menu item (no rollover) 
}

void pressUpDown()
{
 int n=0;
 if (keys[btnDn]) n=-1;
 if (keys[btnUp]) n=1;
 switch(menuItem)
  {
   case 0:
      if (n<0) dispParam=(dispParam==0)?1:0;  // button Dn triggers extended data
      if (n>0) CVmode=!CVmode;                // button Up triggers control mode change
      if (dispParam) setDisplayMenu(Measurement_Message);
      if (n==1) setDisplayMenu(Measurement_Start);
      dispCnt=0;
     break;
   case 1:
      reqBPM=reqBPM+(n*stepBPM);
     break;
   case 2:
      reqVolume=reqVolume+(n*stepVolume);
     break;
   case 3:
      reqCompression=reqCompression+(n*stepCompression);
     break;
   case 4:
      syncRatio=syncRatio+(n*stepSyncRatio);
     break;
   case 5:
      expirationRatio=expirationRatio+(n*stepExpirationRatio);
     break;
  }
 limitValues(reqBPM,reqVolume,reqCompression,syncRatio,expirationRatio); 
}

/*
 *  When set values are changed, actual values are progressively changed from the current to the new value
 */
void sweepValues()  
{
 float f=max(reqBPM,BPM)*maxBPMchange;  // slowly change BPM value to reach requested BPM value
 limitValues(BPM,volume,compression,syncRatio,expirationRatio);
 if (BPM>reqBPM)
   {
    BPM=BPM-f;
    if (BPM<reqBPM) BPM=reqBPM;
   } else
   {
    BPM=BPM+f;
    if (BPM>reqBPM) BPM=reqBPM;
   }
 f=max(reqVolume,volume)*maxVolumeChange;  // slowly change volume value to reach requested volume value        
 if (volume>reqVolume)
   {
    volume=volume-f;
    if (volume<reqVolume) volume=reqVolume;
   } else
   {
    volume=volume+f;
    if (volume>reqVolume) volume=reqVolume;
   }  
 f=max(reqCompression,compression)*maxCompressionChange;  // slowly change ompression value to reach requested pressure value
 if (compression>reqCompression)
   {
    compression=compression-f;
    if (compression<reqCompression) compression=reqCompression;
   } else
   {
    compression=compression+f;
    if (compression>reqCompression) compression=reqCompression;
   }  
}


void loop() {

//    stepper.run();

  if (millis()>tick+samplePeriod)
   {
    tick=tick+samplePeriod;
    readSensors();
    
    
    allSet=((reqBPM==BPM) && (reqVolume==volume) && (reqCompression==compression));
    compressionScale=(compression>minCompression)?compression:defaultCompression;
    volumeScale=(volume>minVolume)?volume:defaultVolume;   
    if (CVmode)  // CV Mode - bargraph needs to show compression
     {
      bargraphDot=4;
      byte b=1*(relPressure<-0.2*compressionScale)+2*(relPressure<-0.1*compressionScale)+4+  // Calculate the bargraph content from the pressure
             8*(relPressure>0.1*compressionScale)+16*(relPressure>0.2*compressionScale)+
             32*(relPressure>0.4*compressionScale)+64*(relPressure>0.8*compressionScale)+
             128*(relPressure>compressionScale);
      barGraph=b; 
     } else
     {        // CP Mode - bargraph needs to show volume 
      if ((relPressure>compressionScale) && (breathPhase==1))  // Check for pressure
       {
        stepper.setAcceleration(motorMaxAcceleration);
        stepper.moveTo(stepper.currentPosition ());  // overshoot, then return
        if (stepper.currentPosition ()<(volume*motorVolumeRatio)*failVolumeRatio) beep(300); // Insufficient volume alarm
       }
      bargraphDot=1;
      float vol=stepper.currentPosition()/motorVolumeRatio;
      int c=int ((7*vol)/volumeScale+1); // Always make sure at least the leftmost position is active
      barGraph=0;
      while (c--) barGraph+=barGraph+1;
      // There is no need for an overvolume alarm as we control the steps.
     }

    if (relPressure>1.1*compressionScale)
      {
       if (highPressureAlarmCnt<highPressureAlarmDetect)
        highPressureAlarmCnt++; 
#ifdef Beeper        
       else 
        beep(300); 
#endif
      }  
     else
      if (highPressureAlarmCnt) highPressureAlarmCnt--;
    // Keyboard actions
    if (keys[btnOk]   && !mkeys[btnOk])   pressOk();        // Ok button
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
    if ((millis()>lastKey+keyIdleReturn) && (menuItem!=0))
     {
      menuItem=0;
      setDisplayMenu(Menu_message,std_dly,0);
     }
#ifdef disableMotorctrl
     if (active) lastActive=millis();
     if (millis()>lastActive+motorOffDelay) 
      {
       digitalWrite(pin_Stepper_Disable, HIGH);  // Deactivate
       motorState=LOW; 
      }
#endif     
   }
  if ((millis()>breathe+breathLength) && (stepper.currentPosition()>0)) breathe+=50; // wait for the previous cycle to finish (safety)
  if (millis()>breathe+breathLength)  // start of a cycle
     {
      if (!allSet) sweepValues(); // Manage the ramp up / ramp down of parameters
      breathLength=int(60000/BPM); 
      breathe=breathe+breathLength; // time for the next cycle start
      syncDelay=breathLength*syncRatio;
      breathTime=breathLength-syncDelay;
      breathIn=breathTime/(1+expirationRatio);
      breathOut=breathTime-breathIn;
      breathInSpeed=volume/breathIn;
      breathOutSpeed=volume/breathOut;
      /*  DEBUG
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
      Serial.print(breathInSpeed);
      tab();
      Serial.println(breathOutSpeed);
      */
      breathGraphCnt=10;              // blinks the LED at the start of a cycle   
      // Values are just a starting point
      stepper.setMaxSpeed(int(breathInSpeed*motorSpeed));
      stepper.setAcceleration(breathInSpeed*breathInSpeed*motorAcceleration);
      breathPhase=0;
      if (active)
       {
#ifdef disableMotorctrl
        if (!motorState) 
         {
          digitalWrite(pin_Stepper_Disable, LOW);  // activate
          motorState=HIGH;
          delay(motorDriverRecovery);  // Allow some time for the motor driver to recover
         }
#endif     
#ifdef Beeper
        beep(2);
#endif        
        breathPhase=1;
        stepper.moveTo(volume*motorVolumeRatio);        
       }
     }
    if (millis()>breathe+breathIn)  // second half of a cycle
     {
      breathPhase=2;
      stepper.setMaxSpeed(int(breathOutSpeed*motorSpeed));
      stepper.setAcceleration(breathOutSpeed*breathOutSpeed*motorAcceleration);
      stepper.moveTo(0);
     }
    if ((syncDelay) && ((millis()>breathe+breathLength-syncDelay+20) || ((breathPhase==2) && (stepper.distanceToGo() == 0))))  // Synchronization period
     {
      if ((relPressure<-0.1*compressionScale) && (peakPressure<-0.1*compressionScale))
       {
        breathe=millis()-breathLength; // Trigger a new cycle sooner.
        Serial.print(relPressure);
        tab();
        Serial.print(peakPressure);
        tab();
        Serial.print(compressionScale);
        Serial.println(F(" Sync"));
       }
     }
#ifdef USBcontrol     
 SerialWait(10); // wait for 100 usec
#endif 
}
