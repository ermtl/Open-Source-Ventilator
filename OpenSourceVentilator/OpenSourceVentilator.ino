/*
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
  Created: March 19, 2020
  Email: erv at mailpeers dot net
*/

#include "defines.h"
#include "libraries.h"
#include "ui.h"
#include "sensors.h"

void (* resetFunction)(void) = 0;  // Self reset (to be used with watchdog)

void  __attribute__ ((noinline)) tab ()
{
 if (term) Serial.print('\t');
}

void  __attribute__ ((noinline)) sep ()
{
 if (term) Serial.print(',');
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
 if (isnan(ee.syncRatio)) isOk=false;                       // Check for malformed floating point values (NaN)
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
 boolean isOk=checkValues();
 ee.eeVersion=eepromVersion;               // Write the EEPROM data version

 if (n==1) isOk=true;  // override (for debug testing)
 if (isOk)
  {
   EEPROM.put(eeAddress, ee);
   eeAddress += sizeof(EEPROM_Data);
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
 Serial.print  (int(ee.syncRatio*100.1));
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
 Serial.print  (int(ee.syncRatio*100.1));
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
     r=r/100;
     if (ok=((r>=minsyncRatio) && (r<=maxsyncRatio)))
      {ee.syncRatio=r; prstat();}
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
     strcpy(disp,"Software V0.23\n             ");
     dispDelay=100;
     dispPhase=100;
    break;
  case 10:
     sprintf(disp, "ERROR %2d\n", dispParam);
     strcat(disp,(dispParam==0)?"NO SENSOR":"UNKNOWN SENSOR");     
     dispPhase=10;
  case PIP_PEEP:
     sprintf(disp,"\n PIP %d PEEP %d    ",displayPip,displayPeep);
     dispDelay=10;  
    break;
  case 100:
     sprintf(disp," S%02d V%d P%d    \n                ",BPM,volume,compression);
     dispDelay=100;  
     if (ee.active) dispPhase=(dispParam==0)?100:103; else dispPhase++;
    break;
  case 101:
     strcpy(disp,"\nSTART ");
     strcat(disp,(ee.CVmode)?"VC":"PC");
     if (ee.syncRatio>0) strcat(disp,"+SV");
     dispPhase=(dispParam==0)?100:103;
     dispDelay=80;
    break;
  case 102:
     strcpy(disp,"\nSTOP        ");
     dispPhase=100;
     dispDelay=50;
    break;
  case 103:
     sprintf(disp,"\nTEMP %dc     ",int(temperature)); 
     dispPhase++;
     dispDelay=80;
    break;
  case 104:
     sprintf(disp,"\nHumi %d RH   ",humidity);    
     dispPhase++;
    break;
  case 105:
     sprintf(disp,"\nPRES. %d cmH2O",int(relPressure));
     dispPhase++;
    break;
  case 106:
     sprintf(disp,"\nSYNC.  %d    ",int(ee.syncRatio*100.1));
     dispPhase++;
    break;
  case 107:
     sprintf(disp,"\nRatio %d.%d    ",int(ee.expirationRatio),int (ee.expirationRatio*10) %10);
     dispPhase++;
    break;
  case 108:
     sprintf(disp,"\nAmb. Pres. %d",int(ee.ambientPressure));
     dispPhase=100;
    break;
  case 110:
     sprintf(disp,"\nSET Rate  %d  ",ee.reqBPM);
     dispDelay=60;  
    break;
  case 120:
     sprintf(disp,"\nSET Volume %d  ",ee.reqVolume); // character N looks better chan character M
     dispDelay=60;  
    break;
  case 130:
     sprintf(disp,"\nSET Press. %d  ",ee.reqCompression);
     dispDelay=60;  
    break;
  case 140:
     sprintf(disp,"\nSET PEEP. %d  ",ee.peep);
     dispDelay=60;  
    break;
  case 150:
     sprintf(disp,"\nSET Sync. %2d    ",int(ee.syncRatio*100.1)); // 100.1 instead of 100 prevents rounding errors
     dispDelay=60;  
    break;
  case 160:
     dispTick++;
     sprintf(disp,"\nSET In-Ex %d.%d",int(ee.expirationRatio),int (ee.expirationRatio*10) %10); 
     dispDelay=60;  
    break;
  case 170:
     sprintf(disp,"\nSET Apnea. %d  ",ee.apnea);
     dispDelay=60;  
    break;
  case 180:
     strcpy(disp,"\nSave Setup     ");
     dispDelay=60;  
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
     strcpy(disp,"SOFT 0.23");
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
  case PIP_PEEP:
     //if (dispTick++ & 1) 
     // sprintf(disp,"PIP  %d  ",displayPip);
     // else
     // sprintf(disp,"PEEP %d  ",displayPeep);
     //dispDelay=10;  
     dispPhase=100;
    break;
  case 100:
     if(dispTick++ & 1)
      sprintf(disp,"%02d %d",BPM,volume);
      else
      sprintf(disp,"P %d",compression);
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
     sprintf(disp,"SYNC.  %d",int(ee.syncRatio*100.1));
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
     sprintf(disp,(dispTick++ & 1)?"SET  %2d":"SYNC. %2d",int(ee.syncRatio*100.1)); // 100.1 instead of 100 prevents rounding errors
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
   switch (timCnt % 50) {        // 10 ms tick period for interface handling
#ifdef dispMenus     
    case 5:
     if (--dispCnt<=0) displayMenu();
     break;
    case 10:                     // change the display
     if (doDisp)
      {
       if  (I2CBusy)
         timCnt-=5;  // retry next time
        else
         {
          doDisplay(); 
          doDisp=false;
         } 
      }
     break;
#endif     
#ifdef dispBargraph 
    case 15:                     // update the bargraph
       if  (I2CBusy)
         timCnt-=5;  // retry next time
        else
         {
          displayBargraph((breathGraphCnt>0)?barGraph-bargraphDot:barGraph);
          if (breathGraphCnt) breathGraphCnt--;
         } 
     break;
#endif     
#ifdef Beeper       
    case 20:                     // update the beep
     if (beepCnt==1)
      {
        digitalWrite(pin_Beep, HIGH);  // inactive (inverted)
        alarmNum=0;
      }
     if (beepCnt) beepCnt--;
     break;
#endif     
#ifdef isKeyboard
    case 30:                     // update the keyboard
      debounceKeyboard();
     break;
#endif     
    } 
   if (++timCnt>9999) timCnt=0;  // counts 0 to 9999
#ifdef Led
#endif
#ifdef watchdog
   Watchdog.reset();
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
     // approximate atmospheric pressure by averaging when the bag is filled and the machine does not run.
     if ((stepper.currentPosition()==0) && !ee.active && ((millis()>lastAmbientPressure+delayAmbientPressure) || !sensAmbientPressure))  
      IIRFilter(pressure,ee.ambientPressure,ambientPressureFilter);
      //ambientPressure=(ambientPressure==0)?pressure:ambientPressure*(1-ambientPressureFilter)+pres*ambientPressureFilter; // low pass filtering
#else
     if ((stepper.currentPosition()==0) && !ee.active)  // approximate athmospheric pressure by averaging when the bag is filled and the machine does not run.
      IIRFilter(pressure,ee.ambientPressure,ambientPressureFilter);
      //ambientPressure=(ambientPressure==0)?pressure:ambientPressure*(1-ambientPressureFilter)+pres*ambientPressureFilter; // low pass filtering
#endif     
      IIRFilter(pressure,avgPressure,avgPressureFilter);
      //avgPressure=(avgPressure==0)?pressure:avgPressure*(1-avgPressureFilter)+pres*avgPressureFilter;                      // low pass filtering
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
   Serial.print(temperature);
   tab();
   Serial.print(humidity);
   tab();
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
 if (term) Serial.println(F("Open Source Ventilator V 0.23"));
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
 breathe==tick-breathLength;
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
#ifndef debugShortEnter
#ifdef Beeper
      beep (200,3); // alert about disabling the ventilator
#endif           
#endif           
     }
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
      ee.syncRatio=ee.syncRatio+(n*stepsyncRatio);
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
 ee.syncRatio       = constrain(ee.syncRatio, minsyncRatio, maxsyncRatio); 
 ee.apnea           = constrain(ee.apnea, minApnea, maxApnea); 
 ee.expirationRatio = constrain(ee.expirationRatio, minExpirationRatio, maxExpirationRatio); 
}

#endif


/*
 *  When the set pressure is reached or if the pressure gets too high (depending on the mode of operation)
 *  the patient inspiration phase is ended and the motor stopped fast.
 *  After overshoot, the motor comes back to the exact position where it was when the routine was called
 */

void motorFastStop()
{
 stepper.setAcceleration(motorMaxAcceleration);
 stepper.moveTo(stepper.currentPosition ());  // overshoot, then return
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
      Serial.println("Phase,Alarm,MotorPos,motorTarget,Pressure,AmbPress,Volume,Target,BreathLng,BreathIn,BreathInSpeed,BreathInAccel,BreathOut,BreathOutSpeed,BreathOutAccel"); 

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
#else
      Serial.println("Time,Phase,Alarm,Motor pos,Pressure,Ambient press,Volume,Target,Breath lng,Breath in,Breath in speed,Breath in accel,Breath out,Breath out speed,Breath out accel"); 
     Serial.print(float(millis()-startactive)/1000);
     sep();
     Serial.print(breathPhase);
     sep();
     Serial.print(alarmNum);
     sep();
     Serial.print(stepper.currentPosition());
     sep();
     Serial.print(relPressure);
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
 pipValue=0; 
 peepValue=9999;
 BPM=ee.reqBPM;
 volume=ee.reqVolume;
 compression=ee.reqCompression;
 breathPhase=0;
 breathLength=int(60000/BPM); 
 breathe=millis();    // time for the next cycle start
 syncDelay=breathLength*ee.syncRatio;
 breathTime=breathLength;
 breathIn=breathTime/(1+ee.expirationRatio);
 breathOut=breathTime-breathIn;
 breathInSpeed=float(volume)/breathIn;
 breathOutSpeed=float(volume)/breathOut;
 breathInAcceleration=breathInSpeed*breathInSpeed*motorAcceleration;
 breathOutAcceleration=breathOutSpeed*breathOutSpeed*motorAcceleration;
 motorTarget=volume*motorVolumeRatio;
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
     Serial.print(arduinoPlotterOffset+long(volume*motorVolumeRatio));
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
     Serial.print(long(volume*motorVolumeRatio));
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
  breathPhase=1;
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
 if (!ee.CVmode)  // CP (Pressure Control) Mode
  {
   if ((relPressure>compressionScale) && (breathPhase==1))  // Check for pressure
    {
     motorFastStop();
#ifdef activeBeeper     
     if (stepper.currentPosition ()<(motorTarget)*failVolumeRatio) beep(300,1); // Insufficient volume alarm
#endif        
    }
  }
}

/*
 *  This function is called periodically at all times 
 *  
 *  It detects if the pressure is high
 */


void breathOverpressureAlarm()
{
 if (relPressure>alarmCompressionValue*compressionScale) // over pressure detected as an error in all modes if it lasts more than 500 ms 
  {
   if (highPressureAlarmCnt<highPressureAlarmDetect)
    highPressureAlarmCnt++; 
#ifdef Beeper        
   else 
    beep(300,2); 
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
 if (breathPhase<2)
  {breathPhase++; 
#ifdef serialTelemetry
   basicTelemetry(true);
#endif        
   int i=int(breathOutSpeed*motorSpeed);
   if (i>motorMaxSpeed) i=motorMaxSpeed;
   stepper.setMaxSpeed(i);
   stepper.setAcceleration(breathOutAcceleration);
   stepper.moveTo(0);
  } 
}


void breathAssistTrigger()
{
 if ((relPressure<-0.1*compressionScale) && (peakPressure<-0.1*compressionScale))
  {
   if (breathPhase==3)
    {
     breathPhase=4;
#ifdef serialTelemetry
     if (ee.active) basicTelemetry(true);
#endif        
    }
   breathe=millis()-breathLength-syncDelay; // Trigger a new cycle sooner.
#ifdef debug_1    // print data for user triggered cycle    
   Serial.print(relPressure);
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

  if (millis()>tick+samplePeriod)  // Read the sensors and acct according to the results (every 50 millisecond)
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
    volumeScale=(volume>minVolume)?volume:defaultVolume;   
    if (breathPhase==1) breathingInspiration ();
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
    if ((millis()>lastKey+keyIdleReturn) && (menuItem!=0))
     {
      menuItem=0;
      setDisplayMenu(Menu_message,std_dly,0);
     }
#endif     
#endif     
#ifdef disableMotorctrl                  // after being inactive for a while, the motor is disabled
     if (ee.active) lastactive=millis();
     if (millis()>lastactive+motorOffDelay) 
      {
       digitalWrite(pin_Stepper_Disable, HIGH);  // Deactivate
       motorState=LOW; 
      }
#endif     
   }
  if ((menuItem==0) && (millis()>lastValChange+200) && ((pipValue!=displayPip) || (peepValue!=displayPeep)))
   {
    if (!ee.active)
     {
       setDisplayMenu(Menu_message,std_dly,0);
     }
     else
     {
      displayPip=pipValue;
      if (peepValue<9999) displayPeep=peepValue;
      setDisplayMenu(PIP_PEEP);
     }
   }
  if ((millis()>breathe+breathLength+syncDelay) && (stepper.currentPosition()>0)) breathe=millis()-breathLength+50; // wait for the previous cycle to finish (safety)
   else
   {
    if (relPressure>pipValue)
     {
      pipValue=relPressure;  
      lastValChange=millis();
     }
    if ((breathPhase>1) && (relPressure<peepValue))
     {
      peepValue=relPressure;  
      lastValChange=millis();
     }
    
    if (millis()>breathe+breathLength+syncDelay) breathingCycleStart(); // start of a cycle
    if ((millis()>breathe+breathIn) && (stepper.distanceToGo() == 0)) breathingExpirationStart(); // second half of a cycle

    if ((breathPhase==2) && (stepper.distanceToGo() == 0))  // Synchronization period
     {
      breathPhase=3;
      breathAssistTrigger();
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
