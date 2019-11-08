//We always have to include the library
#include "LedControl.h"

/*
  Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
  pin 12 is connected to the DataIn
  pin 11 is connected to the CLK
  pin 10 is connected to LOAD
  We have only a single MAX72XX.
*/
LedControl lc = LedControl(12, 11, 10, 1);

/* we always wait a bit between updates of the display */
unsigned long delaytime = 100;

void setup() {

  Serial.begin(9600);
  /*
    The MAX72XX is in power-saving mode on startup,
    we have to do a wakeup call
  */
  lc.shutdown(0, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0, 5);
  /* and clear the display */
  lc.clearDisplay(0);
}


/*
  This method will display the characters for the
  word "Arduino" one after the other on digit 0.
*/
//void writeArduinoOn7Segment() {
//  lc.setChar(0, 0, '-', false);
//  delay(delaytime);
//  lc.setRow(0, 0, 0x05);
//  delay(delaytime);
//  lc.setChar(0, 0, 'd', false);
//  delay(delaytime);
//  lc.setRow(0, 0, 0x1c);
//  delay(delaytime);
//  lc.setRow(0, 0, B00010000);
//  delay(delaytime);
//  lc.setRow(0, 0, 0x15);
//  delay(delaytime);
//  lc.setRow(0, 0, 0x1D);
//  delay(delaytime);
//  lc.clearDisplay(0);
//  delay(delaytime);
//}

/*
  This method will scroll all the hexa-decimal
  numbers and letters on the display. You will need at least
  four 7-Segment digits. otherwise it won't really look that good.
*/
//void scrollDigits() {
//  for (int i = 0; i < 13; i++) {
//    lc.setDigit(0, 7, i, false);
//    lc.setDigit(0, 6, i + 1, false);
//    lc.setDigit(0, 5, i + 2, false);
//    lc.setDigit(0, 4, i + 3, false);
//    lc.setDigit(0, 3, i + 4, false);
//    lc.setDigit(0, 2, i + 5, false);
//    lc.setDigit(0, 1, i + 6, false);
//    lc.setDigit(0, 0, i + 7, false);
//    delay(delaytime);
//  }
//  lc.clearDisplay(0);
//  delay(delaytime);
//}

int decimalToInt(float n) {
  if( n < 0.000001) {
    return n * 100000000;
  } else if ( n < 0.000001) {
    return n * 10000000;
  } else if ( n < 0.00001) {
    return n * 1000000;
  } else if ( n < 0.0001) {
    return n * 100000;
  } else if ( n < 0.001) {
    return n * 10000;
  }  else if ( n < 0.01) {
    return n * 1000;
  } else if ( n < 0.01) {
    return n * 1000;
  } else if ( n < 0.1) {
    return n * 100;
  } else {
    return n * 10;
  }
}

void printNumber(float n) {
  
  int ones;
  int tens;
  int hundreds;
  int thousands;
  int tenthousands;
  int hundthousands;
  int millions;

  boolean negative;
  boolean hasDecimal;

  if (v < -9999 || v > 9999)
    return;
  if (v < 0) {
    negative = true;
    v = v * -1;
  }

  int v = (int) n;
  float temp = v - n;
  
  
  ones = v % 10;
  v = v / 10;
  tens = v % 10;
  v = v / 10;
  hundreds = v % 10;
  v = v / 10;
  thousands = v % 10;
  v = v / 10;
  tenthousands = v % 10;
  v = v / 10;
  hundthousands = v % 10;
  v = v / 10;
  millions = v;

  if (negative) {
    //print character '-' in the leftmost column
    lc.setChar(0, 7, '-', false);
  }
  else {
    //print a blank in the sign column
    lc.setChar(0, 7, ' ', false);
  }
  //Now print the number digit by digit

  lc.setDigit(0, 6, (byte)millions, false);
  lc.setDigit(0, 5, (byte)hundthousands, false);
  lc.setDigit(0, 4, (byte)tenthousands, false);
  lc.setDigit(0, 3, (byte)thousands, false);
  lc.setDigit(0, 2, (byte)hundreds, false);
  lc.setDigit(0, 1, (byte)tens, false);
  lc.setDigit(0, 0, (byte)ones, false);
}


void loop() {
  //  writeArduinoOn7Segment();
  printNumber(12);
//  delay(500);
//  /* and clear the display */
//  lc.clearDisplay(0);
//  delay(500);
  //  scrollDigits();
}
