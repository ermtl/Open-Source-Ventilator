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
