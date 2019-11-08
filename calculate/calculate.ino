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

#include <Keypad.h>

#include <stdio.h>

#include <stdlib.h>

// Globar Variables
const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', '+'},
  {'4', 'c', '6', '-'},
  {'7', '8', '9', '.'},
  {'*', '0', '/', '='}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

/* Holds the value for User inputs*/
float numOne;
//float numTwo;
float total;

/* operation */
enum operators {plus = '+', minus = '-', multiply = '*', divide = '/', clr = 'c', equal = '='};
enum operators oper;

/* boolean value if the value contains a decimal*/
boolean hasDecimal = false;
//boolean hasDecimalTwo = false;

/* number of digits for each value */
int numDigit = 0;

/* if an operation has been entered */
boolean hasOperator = false;
boolean result = false;

// key character from key pad 
char key;

/*///////////////////////////////////////////////////////////////////////////////*/

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LedControl lc = LedControl(12, 11, 10, 1);

///////////////////////////////////////////////////////////////////////////////// 
// Listens for key
void keyListener() {
  switch(key) {
    case '1':
      numKeySetter(1);
      break;
    case '2':
      numKeySetter(2);
      break;
    case '3':
      numKeySetter(3);
      break;
    case '4':
      numKeySetter(4);
      break;
    case '5':
      numKeySetter(5);
      break;
    case '6':
      numKeySetter(6);
      break;
    case '7':
      numKeySetter(7);
      break;
    case '8':
      numKeySetter(8);
      break;
    case '9':
      numKeySetter(9);
      break;
    default:
      operKeySetter();
  }
}

void numKeySetter(float i) {
  if( numDigit <= 7) {
    if(total == 0) {
      total = i;
    } else {
      total = (total*10) + i;
    }
    numDigit++;
  }
}

void operKeySetter() {
  swtich(key):
    case plus:
          
      
}

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

void printNumber(int v) {

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
  key = customKeypad.getKey();

  if (customKey) {
    char inputCopy[2] = {customKey};
    int result = atoi(inputCopy);
    printNumber(result);


    Serial.println(customKey);
  }

  //  delay(500);
  //  /* and clear the display */
  //  lc.clearDisplay(0);
  //  delay(500);
  //  scrollDigits();
}
