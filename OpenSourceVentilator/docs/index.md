# Important Note 
about I2C sensors and peripherals using the Arduino Wire.h library

If you compile with any I2C sensor or peripheral that uses the standard Arduino "Wire.h" library, you must be aware that this library is flawed and does contain an infinite loop that will hang the Arduino if an I2C peripheral fails to respond. The only way to exit is to reset it either manually, or using a watchdog. That's been documented since 2011 but Arduino refuses to accept the pull that would correct the situation.

Here are the lines that create the problem (in Wire/utility/twi.c) :

```
// wait for read operation to complete
while(TWI_MRX == twi_state){
continue;
}
```

This case is not supposed to happen according to the I2C protocol, but electrical glitches didn't get the memo. 

Since reliability is paramount in OpenSourceVentilator, the default "Wire.h" library can be switched for the "jm_Wire.h" library that corrects the flaw with a timeout.

However, all libraries that use "Wire.h" will reference it by name in their code, thus a function with the same name will appear to be defined twice, so the compiler will complain loudly and fail.

The solution is to replace the following line in all the library files that contains it.

```
  #include <Wire.h> 
```
with this little preprocessor code
```
  #if __has_include("jm_Wire.h")
  #include <jm_Wire.h>
  #else
  #include <Wire.h>
  #endif
```

That way, the library will use "jm_Wire.h" if it's included in the main program and "Wire.h" otherwise.
You will sometimes need to exit Arduino and restart it for the change to be effective. Unfortunately, if you upgrade a library, you'll have to do the changes all over again. If you know of a better way, please let me know.

here is a list of the libraries used by OpenSourceVentilator and the files that need to be modified:

 - "BME280I2C.h" (pressure / temperature / humidity sensor)
 
   File /libraries/BME280/src/BME280.cpp
 
   File /libraries/BME280/src/BME280I2C.cpp

 - "BMP180MI" (pressure sensor)
 
   File /libraries/BMP180MI/src/BMP180TwoWire.h
   
 - "LiquidCrystal_PCF8574"  (2x16 characters LCD with a PCF8574 "backpack")
 
   File /libraries/LiquidCrystal_PCF8574/src/LiquidCrystal_PCF8574.cpp
 

As I become aware of other libraries used in the project, they will be added to this list.
