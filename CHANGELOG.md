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
       

