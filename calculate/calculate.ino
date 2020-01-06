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
  {'4', '5', '6', '/'},
  {'7', '8', '9', 'c'},
  {'*', '0', '.', '='}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

/* Holds the value for User inputs*/
float numOne;
float total = 0;

/* operation */
enum Operator {none, add = '+', subtract = '-', multiply = '*', divide = '/', clr = 'c', equal = '=', dot = '.', negative = '_'};
enum Operator oper = none;

/* boolean value if the value contains a decimal*/
boolean hasDecimal = false;

/* number of digits for each value */
int numDigit = 0;

/* if an operation has been entered */
boolean hasResult = false;
boolean hasError = false;
boolean keepNum = false;

// key character from key pad
char key;

/*///////////////////////////////////////////////////////////////////////////////*/

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LedControl lc = LedControl(12, 11, 10, 1);

/////////////////////////////////////////////////////////////////////////////////

// Key Listener Functions
void keyListener() {
  switch (key) {
    case '0':
      numKeySetter(0);
      break;
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
    case add:
      oper = add;
      keepNum = true;
      break;
    case subtract:
      oper = subtract;
      keepNum = true;
      break;
    case multiply:
      oper = multiply;
      keepNum = true;
      break;
    case divide:
      oper = divide;
      keepNum = true;
      break;
    case clr:
      reset();
      break;
    case equal:
      getEqual();
      break;
    case dot:
      hasDecimal = true;
      break;
    case negative:
      total *= -1;
      break;
  }
}

/////////////////////////////////////////////////////////////////////////////////
// ********** Helper Functions ************** //

// Adding the values inputed from the user
void numKeySetter(float i) {
  if (i == 0 && total == 0) {
    return;
  }
  // AHHHHHHH i can't explain this in words sooo yeahhhh.....
  if (hasResult && oper == none) {
    reset();
  }

  // This condition makes sure that the print doesn't get cleared until a new number input has been clicked.
  if (keepNum && oper != none) {
    numOne = total;
    total = 0;
    numDigit = 0;
    hasDecimal = false;
    lc.clearDisplay(0);
  }

  if ( numDigit < 7) {
    if (hasDecimal) {
      setDecimal(i);
    } else if (total == 0) {
      total = i;
    } else {
      total = (total * 10) + i;
    }

    numDigit++;
  }

  keepNum = false;
}

// Called if the EQUAL operator has been clicked. This evaluates the total and the number one value
void getEqual() {
  if (oper != none) {
    switch (oper) {
      case add:
        total = numOne + total;
        break;
      case subtract:
        total = numOne - total;
        break;
      case multiply:
        total = numOne * total;
        break;
      case divide:
        if (total != 0) {
          total = numOne / total;
        } else {
          hasError = true;
          return;
        }
        break;
    }
    hasResult = true;
    oper = none;
    //TODO: make is so this also calculates the decimal place 
    numDigit = lenHelper(total);
  }
}

// Called if the CLEAR operator has been clicked
void reset() {
  numOne = 0;
  total = 0;
  numDigit = 0;
  oper = none;
  hasResult = false;
  hasError = false;
  hasDecimal = false;
  lc.clearDisplay(0);
  lc.setDigit(0, 0, 0, false);
}

//Finding the length of digit
int lenHelper(float x) {
  if (x < 0) {
    x *= -1;
  }
//  if (x <= 0) return 0;
//  if (x >= 1000000000) return 10;
//  if (x >= 100000000)  return 9;
//  if (x >= 10000000)   return 8;
//  if (x >= 1000000)    return 7;
//  if (x >= 100000)     return 6;
//  if (x >= 10000)      return 5;
//  if (x >= 1000)       return 4;
//  if (x >= 100)        return 3;
//  if (x >= 10)         return 2;
//  return 1;
  return log10(x) + 1;
}

// TODO: implement adding decimals
// TODO: deal with this scenario : user inputs 0s as decimals 
void setDecimal(float i) {
  // Extract decimal from total
  int intpart = (int) total;
  float decimal = total - intpart;
  int len = lenHelper(decimalToFloat(decimal));
  
  //Exponential function
  if (len == 0) {
    i /= 10;
  } else if (len == 1) {
    i /= 100;
  } else if (len == 2) {
    i /= 1000;
  } else if (len == 3) {
    i /= 10000;
  } else if (len <= 4) {
    i /= 100000;
  }  else if (len <= 5) {
    i /= 1000000;
  } else if (len <= 6) {
    i /= 10000000;
  }
  //  Serial.println(i);
  total += i;
};

float decimalToFloat(float dec) {
  float temp = dec;
  while(abs(temp) <= 0.1) {
    dec *= 10;
    Serial.println(dec);
    temp *=10;
    temp -= (int)temp;
  }
  return dec;
}



//// Printing Helper
float remainder (float val) {
  return (fmod(val, 10));
}


/////////////////////////////////////////////////////////////////////////

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

  lc.setDigit(0, 0, 0, false);
}


void printNumber(float v) {

  boolean negative = false;

  if (numDigit == 0) {
    lc.setDigit(0, 0, 0, false);
    return;
  }

  if (v < -9999999 || v > 9999999) {
    return;
  }

  if (v < 0) {
    negative = true;
    v *= -1;
  }

  if (negative) {
    //print character '-' in the leftmost column
    lc.setChar(0, numDigit, '-', false);
  }

  lc.clearDisplay(0);
  float temp = v;
  //This extracts the decimal when it loops 
  boolean extractDecimal = true;
  int numberOfDecimal;

  
  //Now print the number digit by digit
  for (int i = 0; i < numDigit; i++) {
    temp = remainder(v);
    v = v / 10;
    if(i < numDigit - 1) {
      lc.setDigit(0, i, (byte)temp, false);
//      if(extractDecimal && hasDecimal) {
//         int intpart = (int) total;
//         float decimal = total - intpart;
//         v = decimal;
//         extractDecimal = false;
//      }
    } else {
      lc.setDigit(0, i, (byte)temp, hasDecimal);
    }
  }
}

void printError() {
  lc.clearDisplay(0);
  //REEEEEEEEEEEEEEEEEEE PICKLE RICK 
  lc.setChar(0, 7, 'E', false);
  lc.setChar(0, 6, 'E', false);
  lc.setChar(0, 5, 'E', false);
  lc.setChar(0, 4, 'E', false);
  lc.setChar(0, 3, 'E', false);
  lc.setChar(0, 2, 'E', false);
  lc.setChar(0, 1, 'E', false);
  lc.setChar(0, 0, 'E', false);
}


void loop() {
  //  writeArduinoOn7Segment();
  key = customKeypad.getKey();
  if (key) {
    keyListener();
    Serial.println(key);
    Serial.println(total);
    if (!hasError) {
      printNumber(total);
    } else {
      printError();
    }
  }


}
