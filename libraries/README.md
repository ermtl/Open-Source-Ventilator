To properly compile the OpenSourceVentilator.ino controller with the #define LCDReset option, the AccelStepper and LiquidCrystal_PCF8574 libraries have to use a patched version.

AccelStepper will be further modified to allow finer microstepping.

Simply overwrite the files with the same name in your arduino libraries folder/subdirectory. This is a quickfix, a proper solution will be used shortly.
