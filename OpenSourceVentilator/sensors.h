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

#define startup_message       0    // power up start
#define error_message        10    // 10 + error code
#define Menu_message        100    // screen for each menu item 
#define Measurement_Start   101    // Screen with "start" message
#define Stop_message        102    // stop indicator screen  
#define Measurement_Message 103    // first of measurement messages
#define  PIP_PEEP            99    // Display current values for PIP and PEEP

#define MenuItems             9
#define keyIdleReturn     16000    // returns to main menu after 16 seconds without key press
#define motorOffDelay     10000    // Motor is cut off after that time being inactive.
#define interruptMaxNest      4    // delay steps to prevent a stack runaway

#define PaToCmH2O 0.010197162129779282 // Conversion coefficient


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
        sensAmbientPressure,   // True if a separate ambient pressure sensor is present, false otherwise
        sensFlow,              // True if an airflow sensor is present, false otherwise (not used yet)
        Mactive,               // active memory (transition detection  
        motorState,            // True if the motor is activated, false otherwise, false otherwise
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
        breathOutAcceleration; // Acceleration in step squared/second      
long    tick,                  // counter used to trigger sensor measurements
        breathe,               // counter used to track the current breathing cycle
        lastKey,               // last time when a key was pressed (used to automatically return to the main menu) 
        lastValChange,         // Last time the PIP or PEEP changed (used for display)
        commTime,              // Last time a serial communication was received
        lastactive,            // Last time the motor was active
        startactive,           // last time the motor was started
        lastAmbientPressure,   // last time a valid ambient pressure was collected
        breathLength,          // duration of the current breathing cycle in milliseconds. This is 60000/BPM.
        breathTime,            // Length of cycle minus the sync period
        breathIn,              // Length of the breath inhalation
        breathOut;             // Length of the breath exhalation
int     motorTarget,           // Exprected motor position at the end of the cycle
        pipValue,              // Peak pressure
        peepValue,             // End of cycle pressure 
        displayPip,            // Peak pressure
        displayPeep,           // End of cycle pressure 
        volume,                // Breathing volume per cycle in ml
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
   float   syncRatio;             // portion of the cycle for breath synchronisation
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
