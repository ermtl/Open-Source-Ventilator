/*

  Open source Ventilator

  Version 0.10

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


// Connecting the BME280 Sensor:
// Sensor              ->  Board
// -----------------------------
// Vin (Voltage In)    ->  3.3V
// Gnd (Ground)        ->  Gnd
// SDA (Serial Data)   ->  A4 on Uno/Pro-Mini, 20 on Mega2560/Due, 2 Leonardo/Pro-Micro
// SCK (Serial Clock)  ->  A5 on Uno/Pro-Mini, 21 on Mega2560/Due, 3 Leonardo/Pro-Micro

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
                            

BME280I2C bme;    // Default : forced mode, standby time = 1000 ms
                  // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,


// GPIO I/O pins on the Arduino connected to strobe, clock, data,
// pick on any I/O you want.
#define  pin_Strobe_TM 2
#define  pin_Clock_TM 3
#define  pin_DIO_TM 4
//Constructor object
TM1638plus tm(pin_Strobe_TM, pin_Clock_TM , pin_DIO_TM);

#define pin_Beep  8
#define pin_LED  13
#define eeStart  48  // EEPROM Offset for data


#define pin_Stepper_DIR    6
#define pin_Stepper_Step   7
#define pin_Stepper_Enable 5
// Define the stepper and the pins it will use
AccelStepper stepper(AccelStepper::DRIVER, pin_Stepper_Step, pin_Stepper_DIR);


#define SERIAL_BAUD 115200
#define std_dly 70  // display standard delay

#define samplePeriod 100 // 100 ms sensor sampling period
#define debounce 3       // Keyboard debouncing


#define ambientPressureFilter       0.03  // IIR filtering ratio (lower value produce longer time constant)
#define defaultPressure        105000.00  // Pressure in Pa returned when no sensor is found
#define defaultAmbientPressure 105000.00  // assumed ambiant pressure in Pa returned when no sensor is found
#define defaultHumidity            50.00  // humidity in % RH returned when no sensor is found
#define defaultTemperature         20.00  // temperature in °C returned when no sensor is found


// WARNING : When changing min and max value, manually change the text in the SerialCommand procedure accordingly

#define minBPM                    6.0   // minimum respiratory speed
#define defaultBPM               15.0   // default respiratory speed
#define stepBPM                   1.0   // adjustment step for respiratory speed
#define maxBPM                   40.0   // maximum respiratory speed
#define maxBPMchange              0.2   // maximum respiratory speed change in proportion of final value per beat (1=100%)
#define minVolume               100.0   // minimum respiratory volume in milliliters 
#define defaultVolume           150.0   // default respiratory volume in milliliters 
#define stepVolume              100.0   // adjustment step for respiratory volume in milliliters 
#define maxVolume              1500.0   // maximum respiratory volume in milliliters 
#define maxVolumeChange           0.25  // maximum respiratory volume change in proportion of final value per beat (1=100%) 
#define minCompression         1000.00  // minimum compression for the ambu-bag in Pa
#define stepCompression         500.00  // adjustment step for compression for the ambu-bag in Pa
#define defaultCompression     3000.00  // default compression for the ambu-bag in Pa
#define maxCompression        20000.00  // maximum compression for the ambu-bag in Pa
#define maxCompressionChange      0.5   // maximum compression for the ambu-bag change in proportion of final value per beat (1=100%)

#define startup_message   0   // power up start
#define error_message    10   // 10 + error code
#define Menu_message    100   // screen for each menu item 
#define Stop_message    105   // stop indicator screen  

#define btnPrev 7
#define btnNext 6
#define btnDn   5
#define btnUp   4
#define btnOk   0

#define MenuItems 5
#define keyIdleReturn 12000  // returns to main menu after 12 seconds without key press


boolean term,killMove;
int     dispDelay,dispPhase,dispCnt,dispParam,timCnt,breathGraphCnt,timerLvl;
boolean doDisp, sensHumidity, sensTemperature, sensPressure, allSet,active,debug;
char    disp[11];
float   ambientPressure,pressure,relPressure,temperature,humidity;
long    tick,breathe,breathe2,lastKey,commTime;
float   BPM,volume,compression;
byte    sc,barGraph,menuItem;
byte    keys[8],kcnt[8],mkeys[8];

// Parameters saved to / recovered from EEPROM

float reqBPM;          // set respiratory frequency
float reqVolume;       // set respiratory volume in milliliters 
float reqCompression;  // set compression for the ambu-bag in Pa
 

void(* resetFunction) (void) = 0;  // Self reset (to be used with watchdog)

void tab ()
{
 if (term) Serial.print('\t');
}


void eeput (int n)  // records to EEPROM (only if values are validated)
{
    int eeAddress = eeStart;
    boolean isOk=(reqBPM>=minBPM);         
    if (reqBPM>maxBPM) isOk=false;                 // BPM in allowed range ?
    if (reqVolume<minVolume) isOk=false;       
    if (reqVolume>maxVolume) isOk=false;           // Volume in allowed range ?
    if (reqCompression<minCompression) isOk=false; 
    if (reqCompression>maxCompression) isOk=false; // Compression in allowed range ?
    
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
    }
}    

void eeget ()
{    
    int eeAddress = eeStart;
    float treqBPM,treqVolume,treqCompression;
    byte tac;

    EEPROM.get(eeAddress, treqBPM);
    eeAddress += sizeof(float);
    EEPROM.get(eeAddress, treqVolume);
    eeAddress += sizeof(float);
    EEPROM.get(eeAddress, treqCompression);
    eeAddress += sizeof(float);
    EEPROM.get(eeAddress, tac);
    eeAddress += sizeof(byte);

    // Check if the data is coherent
    boolean isOk=(treqBPM>=minBPM);         
    if (treqBPM>maxBPM) isOk=false;                 // BPM in allowed range ?
    if (treqVolume<minVolume) isOk=false;       
    if (treqVolume>maxVolume) isOk=false;           // Volume in allowed range ?
    if (treqCompression<minCompression) isOk=false; 
    if (treqCompression>maxCompression) isOk=false; // Compression in allowed range ?
    if (tac>1) isOk=false; // is the value 0 or 1 ?
    
    if (isOk) 
      {
       reqBPM=treqBPM;
       reqVolume=treqVolume;
       reqCompression=treqCompression;
       active=(tac>0);
      } else
      {
       reqBPM=defaultBPM;
       reqVolume=defaultVolume;
       reqCompression=defaultCompression;
       active=false;
      }
    if (term)
     {
      Serial.print((isOk)?F("Read EEPROM Settings: "):F("Read Default Settings: "));
      Serial.print(reqBPM);
      tab();
      Serial.print(reqVolume);
      tab();
      Serial.println(reqCompression);
     }
}   

#define CommandMaxLength 80

byte BufferLength;
String Buffer= "*********************************************************************************";  // filler

void prstat()
{
 Serial.print(F("\nPatient assisted breathing\tO"));
 Serial.println((active)?F("N"):F("ff"));
 Serial.print(F("Ventilation speed\t\t"));
 Serial.print(reqBPM);
 Serial.println(F(" cycles / minute"));
 Serial.print(F("Ventilation volume\t\t"));
 Serial.print(reqVolume/1000);
 Serial.println(F(" liter"));
 Serial.print(F("Ventilation pressure\t\t"));
 Serial.print(reqCompression/1000);
 Serial.println(F(" KPa\n"));
}


void meas()
{
 Serial.print(F("Sensor Temp:")); 
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

void setDisplay(int ph, int dly=std_dly, int param=0) 
{
 dispDelay=dly;
 dispCnt=0;
 dispPhase=ph;
 dispParam=param;
}

void SerialCommand()
{
  commTime=millis(); 
  float r;
  // char ch=Buffer[1];
  switch ( Buffer[0])
   {
    case '1': // 1 Start assisted breathing
     Serial.println(F("Breathing started"));
     active=true;
    break;
    case '0': // 0 Stop assisted breathing
     active=false;
     Serial.println(F("WARNING: Breathing stopped"));
     setDisplay(Stop_message,std_dly,0);
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
    case 'M': // M show Measurements from the sensor
     meas();  
    break;
    case 'W': // T Terminal mode (human readable)
     eeput(0);  // Save settings to EEPROM
     menuItem=0;
     Serial.println(F("Parameters Written"));  
    break;
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
       Serial.println(F("0 Stop assisted breathing")); 
       Serial.println(F("S set Speed (in BPM)")); 
       Serial.println(F("V set Volume (in liters)")); 
       Serial.println(F("P Set pressure (in KPa)")); 
       Serial.println(F("M show Measurements from the sensor")); 
       Serial.println(F("W Write parameters to permanent memory")); 
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


// scans the individual bits of value sets a LED based on which button pressed
void doLEDs(uint8_t value) {
  for (uint8_t position = 0; position < 8; position++) {
    tm.setLED(position, value & 1);
    value = value >> 1;
  }
}


void Timer() 
{
timerLvl++;  
if (!killMove) stepper.run();
if (timerLvl>1)  // only does the motor update if nested more than 1 level
{
 timerLvl--; 
}
else
{
digitalWrite(pin_LED, HIGH);

sei();  // Allows nested interrupts
switch (timCnt % 50) {        // 10 ms tick period for interface handling
 case 0:
  if (--dispCnt<=0)
  {
   doDisp=true;  // will update the display by default
   switch (dispPhase) {
    case 0:
       strcpy(disp,"OPEN");
      break;
    case 1:
       strcpy(disp,"SOURCE");
      break;
    case 2:
       strcpy(disp,"VENTILAT");
       dispDelay=30;
      break;
    case 3:
       strcpy(disp,"ENTILATO");
       dispDelay=20;
      break;
    case 4:
       strcpy(disp,"NTILATOR");
       dispDelay=60;
      break;
    case 5:
       strcpy(disp,"SOFT 0.11");
       dispDelay=100;
       dispPhase=99;
      break;
    case 10:
       sprintf(disp, "ERROR %02d", dispParam);
       dispPhase+=dispParam; // Jump to correct error 
      break;
    case 11:
       strcpy(disp,"NO.SENSOR");
       dispPhase=9;
      break;
    case 12:
       strcpy(disp,"UNKNOWN");
       dispPhase=98;
      break;
    case 99:
       strcpy(disp,"SENSOR");
       dispPhase=9;
      break;
    case 100:
       if (compression<10000) 
         sprintf(disp,"%02d %d.%d %d.%d",int(BPM),int(volume/1000),int (volume/100) %10,int(compression/1000),int (compression/100) %10);
        else
         sprintf(disp,"%02d %d.%d %d",int(BPM),int(volume/1000),int (volume/100) %10,int(compression/1000));
       dispDelay=100;  
       if (active) dispPhase=(dispParam==0)?99:101;
      break;
    case 101:
       strcpy(disp,"START  _");
       dispPhase=99;
       dispDelay=80;
      break;
    case 102:
       sprintf(disp,"TENP %dC",int(temperature)); // character N looks better chan character M
       
       dispDelay=80;
      break;
    case 103:
       sprintf(disp,"HUN %dRH",int(humidity)); // character N looks better chan character M
       
       dispDelay=80;
      break;
    case 104:
       //char chr=' ';
       //if (relPressure<0) chr='-';  
       sprintf(disp,"PRS %dH",int(relPressure/100));
       dispPhase=99;
       dispDelay=50;
      break;
    case 105:
       strcpy(disp,"STOP   _");
       dispPhase=99;
       dispDelay=50;
      break;
      
    case 110:
       sprintf(disp,"RATE  %d",int(reqBPM));
       dispDelay=60;  
      break;
    case 111:
       sprintf(disp,"SET   %d",int(reqBPM));
       dispDelay=60;  
       dispPhase=109;
      break;
    case 120:
       sprintf(disp,"VOLUN. %d.%d",int(reqVolume/1000),int (reqVolume/100) %10); // character N looks better chan character M
       dispDelay=60;  
      break;
    case 121:
       sprintf(disp,"SET   %d.%d",int(reqVolume/1000),int (reqVolume/100) %10);
       dispDelay=60;  
       dispPhase=119;
      break;
    case 130:
       sprintf(disp,"PRESS. %d.%d",int(reqCompression/1000),int (reqCompression/100) %10);
       dispDelay=60;  
      break;
    case 131:
       sprintf(disp,"SET   %d.%d",int(reqCompression/1000),int (reqCompression/100) %10);
       dispDelay=60;  
       dispPhase=129;
      break;
    case 140:
       strcpy(disp,"SAVE   _");
       dispDelay=60;  
      break;
    case 141:
       strcpy(disp,"SETUP  _");
       dispDelay=60;  
       dispPhase=139;
      break;
    default:
      if (term) Serial.println(dispPhase); 
      dispPhase=0; // Abnormal the phase should always be defined
     break;
    }
   dispCnt=dispDelay;
  }
  break;
  case 10:      // change the display
  if (doDisp)
   {
    tm.displayText(disp);   
    byte l=strlen(disp);   // Fills the display
    while (l<8)      
     {
      tm.displayASCII(l++,' ');
     }
    doDisp=false;
    dispPhase++;
   }
   break;
  case 20:      // update the bargraph
   doLEDs((breathGraphCnt>0)?barGraph-4:barGraph);
   if (breathGraphCnt) breathGraphCnt--;
   break;
  case 30:      // update the keyboard
   uint8_t b = tm.readButtons(); // returns a byte with values of button s8s7s6s5s4s3s2s1
   byte c=8;
   byte m=1;
   while (c--)
    {
     if (m & b)
       {if (kcnt[c]>debounce) keys[c]=true; else kcnt[c]++;}
      else
       {if (kcnt[c]==0) keys[c]=false; else kcnt[c]--;}
     m+=m; 
    }
   break;
 } 
if (++timCnt>9999) timCnt=0;  // counts 0 to 9999
digitalWrite(pin_LED, LOW);
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
      ambientPressure=(ambientPressure==0)?pressure:ambientPressure*(1-ambientPressureFilter)+pres*ambientPressureFilter; // low pass filtering
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
   if ((term) && (sc++>10) && (debug))
    {
     sc=0;
     meas();
    }
}


void setup() {
  // put your setup code here, to run once:
  timerLvl=0;
  Timer1.initialize(200);  
  Timer1.attachInterrupt(Timer);
  setDisplay(startup_message); 
  pinMode(pin_Beep, OUTPUT);
  digitalWrite(pin_Beep, LOW);  // active (inverted)
  pinMode(pin_Stepper_DIR, OUTPUT);
  digitalWrite(pin_Stepper_DIR, LOW);     // active (inverted)
  pinMode(pin_Stepper_Step, OUTPUT);
  digitalWrite(pin_Stepper_Step, LOW);    // active (inverted)
  pinMode(pin_Stepper_Enable, OUTPUT);
  digitalWrite(pin_Stepper_Enable, LOW);  // active (inverted)

  Serial.begin(SERIAL_BAUD);
  int c=1000;
  while (!Serial && c--) delay(1); // wait for serial port to connect. Needed for native USB
  term=Serial; 
  if (term) Serial.println(F("Open Source Ventilator Ver 0.11"));
  delay (100);
  digitalWrite(pin_Beep, HIGH);  // inactive (inverted)
  Wire.begin();
  sensHumidity=false;
  sensTemperature=false;
  sensPressure=false;
  if(bme.begin())
   {
    switch(bme.chipModel())
    {
     case BME280::ChipModel_BME280:
       if (term) Serial.println(F("Found BME280 sensor! Success."));
       sensHumidity=true;
       sensTemperature=true;
       sensPressure=true;
       break;
     case BME280::ChipModel_BMP280:
       if (term) Serial.println(F("Found BMP280 sensor! No Humidity available."));
       sensTemperature=true;
       sensPressure=true;
       break;
     default:
       setDisplay(error_message,std_dly,1);
       if (term) Serial.println(F("Found UNKNOWN sensor! Error!"));
    }
   }
   else
   {
    setDisplay(error_message,std_dly,0);
    if (term) Serial.println(F("Could not find BME280 sensor!"));
   }
 ambientPressure=0; // force value initialisation
 eeget ();
 BPM=reqBPM;                 // Start from these values without sweep
 volume=reqVolume;
 compression=reqCompression;
 Buffer="?";  // send informations to the serial port
 SerialCommand();
 Buffer="";
 readSensors();   
 tick=millis();
 breathe=tick-int(60000/BPM);
 killMove=false;
 stepper.setMaxSpeed(3500);
 stepper.setAcceleration(20000);
 stepper.moveTo(0);
}


void loop() {

//    stepper.run();

  if (millis()>tick+samplePeriod)
   {
    tick=tick+samplePeriod;
    readSensors();
    allSet=((reqBPM==BPM) && (reqVolume==volume) && (reqCompression==compression));

    float c=(compression>minCompression)?compression:defaultCompression;
    byte b=1*(relPressure<-0.2*c)+2*(relPressure<-0.1*c)+4+8*(relPressure>0.1*c)+16*(relPressure>0.2*c)+
             32*(relPressure>0.4*c)+64*(relPressure>0.8*c)+128*(relPressure>c);
    barGraph=b; 

    byte ct=8;
    while (ct--) if (keys[ct]) lastKey=millis();
    if ((millis()>lastKey+keyIdleReturn) && (menuItem!=0))
     {
      menuItem=0;
      setDisplay(Menu_message,std_dly,0);
     }
    
    if (keys[btnOk] != mkeys[btnOk])
     {
      if (keys[btnOk]) 
       { 
        if (menuItem==0)
          {
           active=!active;  // Toggle function when "Enter" key pressed
           if (!active) setDisplay(Stop_message,std_dly,0);
          }
         else
          {                 // Return to main screen when "Enter" key pressed
           if (menuItem==4) eeput(0);  // Save settings to EEPROM
           menuItem=0;
           setDisplay(Menu_message,std_dly,0);
          }
        if ((active) && (stepper.distanceToGo() == 0))  // If the motor is idle, start immediately
         {
          breathe=millis()-int(60000/BPM); // will happen immediately
         }
       } 
      mkeys[btnOk]=keys[btnOk];
     }

    if (keys[btnPrev] != mkeys[btnPrev])
     {
      if ((keys[btnPrev]) && (menuItem>0)) {menuItem--;setDisplay(Menu_message+10*menuItem,std_dly,0);}  // Previous menu item (no rollover)
      mkeys[btnPrev]=keys[btnPrev];
     }
    if (keys[btnNext] != mkeys[btnNext])
     {
      if ((keys[btnNext]) && (menuItem<MenuItems-1)) {menuItem++;setDisplay(Menu_message+10*menuItem,std_dly,0);}  // Next menu item (no rollover)
      mkeys[btnNext]=keys[btnNext];
     }
    if (keys[btnDn] != mkeys[btnDn])  // Lower the value
     {
      if (keys[btnDn])
        {
         switch(menuItem)
          {
           case 0:
              dispParam=(dispParam==0)?1:0;
             break;
           case 1:
              reqBPM=reqBPM-stepBPM;
              if (reqBPM<minBPM) reqBPM=minBPM;
             break;
           case 2:
              reqVolume=reqVolume-stepVolume;
              if (reqVolume<minVolume) reqVolume=minVolume;
             break;
           case 3:
              reqCompression=reqCompression-stepCompression;
              if (reqCompression<minCompression) reqCompression=minCompression;
             break;
           }
        }
      mkeys[btnDn]=keys[btnDn];
     }
     
    if (keys[btnUp] != mkeys[btnUp])  // Increase the value
     {
      if (keys[btnUp])
        {
         switch(menuItem)
          {
           case 0:
              dispParam=(dispParam==0)?1:0;
             break;
           case 1:
              reqBPM=reqBPM+stepBPM;
              if (reqBPM>maxBPM) reqBPM=maxBPM;
             break;
           case 2:
              reqVolume=reqVolume+stepVolume;
              if (reqVolume>maxVolume) reqVolume=maxVolume;
             break;
           case 3:
              reqCompression=reqCompression+stepCompression;
              if (reqCompression>maxCompression) reqCompression=maxCompression;
             break;
           }
       
        }
      mkeys[btnUp]=keys[btnUp];
     }
   }
    if (millis()>breathe+int(60000/BPM))  // start of a cycle
     {
      if (!allSet) // Manage the ramp up / ramp down of parameters
       {
        if (BPM<minBPM) BPM=minBPM;
        if (BPM>maxBPM) BPM=maxBPM;
        float f=reqBPM*maxBPMchange;  // slowly change BPM value to reach requested BPM value
        if (BPM>reqBPM)
          {
           BPM=BPM-f;
           if (BPM<reqBPM) BPM=reqBPM;
          } else
          {
           BPM=BPM+f;
           if (BPM>reqBPM) BPM=reqBPM;
          }
        if (volume<minVolume) volume=minVolume;
        if (volume>maxVolume) volume=maxVolume;
        f=reqVolume*maxVolumeChange;  // slowly change volume value to reach requested volume value
        if (volume>reqVolume)
          {
           volume=volume-f;
           if (volume<reqVolume) volume=reqVolume;
          } else
          {
           volume=volume+f;
           if (volume>reqVolume) volume=reqVolume;
          }  
        if (compression<minCompression) compression=minCompression;
        if (compression>maxCompression) compression=maxCompression;
        f=reqCompression*maxCompressionChange;  // slowly change ompression value to reach requested pressure value
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
      breathe=breathe+int(60000/BPM);
      breathGraphCnt=10; // blinks at the start of a cycle   
      
      // Values are just a starting point
      stepper.setMaxSpeed(volume*1.5);
      stepper.setAcceleration(volume*4);
      if (active) stepper.moveTo(volume*0.6);        
     }
    if (millis()>breathe+int(30000/BPM))  // second half of a cycle
     {
      stepper.moveTo(0);
     }
 SerialWait(10); // wait for 100 usec
}
