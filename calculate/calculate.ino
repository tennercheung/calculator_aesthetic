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
  {'4', '5', '6', '-'},
  {'7', '8', '9', 'c'},
  {'*', '0', '.', '='}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

/* Holds the value for User inputs*/
float numOne;
char buff[9] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'};

/* operation */
enum Operator {none, add = '+', subtract = '-', multiply = '*', divide = '/', clr = 'c', equal = '=', dot = '.', negative = '_'};
enum Operator oper = none;

/* boolean value if the value contains a decimal*/
boolean hasDecimal = false;

/* number of digits for each value */
int numDigit = 0;
int availableDigit = 7;
int decNumOne;
int decAcc;

/* if an operation has been entered */
boolean hasResult = false;
boolean hasError = false;
boolean keepNum = false;
boolean isNegative = false;

// key character from key pad
char key;

/*///////////////////////////////////////////////////////////////////////////////*/

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LedControl lc = LedControl(12, 11, 10, 1);

/////////////////////////////////////////////////////////////////////////////////

// Key Listener Functions
void keyListener() {
  switch (key) {
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
    case negative:
      isNegative = true;
      break;
    default:
      numKeySetter(key);
  }
}

/////////////////////////////////////////////////////////////////////////////////
// ********** Helper Functions ************** //

// Adding the values inputed from the user
void numKeySetter(char i) {
  //  if (i == '0' && buff == '0) {
  //    return;
  //  }
  // AHHHHHHH i can't explain this in words sooo yeahhhh.....
  if (hasResult && oper == none) {
    reset();
  }

  // This condition makes sure that the print doesn't get cleared until a new number input has been clicked.
  if (keepNum && oper != none) {
    numOne = strtod(buff, NULL);

    if (isNegative) {
      numOne *= -1;
    }

    for (int i = 0; i < 9; i++) {
      buff[i] = ' ';
    }
    numDigit = 0;
    availableDigit = 7;
    decNumOne = decAcc;
    decAcc = 0;
    hasDecimal = false;
    lc.clearDisplay(0);
    Serial.println(numOne);
  }

  if ( numDigit < availableDigit) {
    if (hasDecimal) {
      decAcc++;
    }

    if ( i == dot) {
      if (!hasDecimal) {
        hasDecimal = true;
        availableDigit = 8;
      } else {
        return;
      }
    }
    buff[numDigit] = i;
    numDigit++;
  }

  keepNum = false;
}

// Called if the EQUAL operator has been clicked. This evaluates the buff and the number one value
void getEqual() {
  if (oper != none) {
    float numTwo = strtod(buff, NULL);
    float result;

    if (oper == multiply) {
      decNumOne = decNumOne + decAcc;
    } else if ( oper == divide) {
      decNumOne = 6;
    } else if (decNumOne < decAcc) {
      decNumOne = decAcc;
    }

    Serial.println("Decimal Precision");
    Serial.println(decNumOne);

    switch (oper) {
      case add:
        result = numOne + numTwo;
        break;
      case subtract:
        result = numOne - numTwo;
        break;
      case multiply:
        result = numOne * numTwo;
        break;
      case divide:
        if (numTwo != 0) {
          result = numOne / numTwo;
        } else {
          hasError = true;
          return;
        }
        break;
    }
    Serial.println(numTwo);
    Serial.println(result);

    if (result < 0) {
      isNegative = true;
    }
    decAcc = 0;
    hasDecimal = false;
    dtostrf(result, 1, decNumOne, buff);
    digitCounter();
    Serial.println(numDigit);
    hasResult = true;
    oper = none;
  }
}

// Called if the CLEAR operator has been clicked
void reset() {
  for (int i = 0; i < 9; i++) {
    buff[i] = ' ';
  }
  numOne = 0;
  numDigit = 0;
  decNumOne = 0;
  decAcc = 0;
  availableDigit = 7;
  oper = none;
  hasResult = false;
  hasDecimal = false;
  hasError = false;
  hasDecimal = false;
  lc.clearDisplay(0);
  lc.setDigit(0, 0, 0, false);
}

void digitCounter() {
  int acc = 0;

  // Counts from the end of array and stops either at the first encounter
  //of a decimal place or the at the encounter of the decimal point
  for (int i = 8; i > 0; i--) {
    if (buff[i] == dot) {
      break;
    }

    if (buff[i] > '0' && buff[i] <= '9') {
      acc--;
      hasDecimal = true;
      break;
    }
    acc++;
  }

  if(8 - acc == 0) {
    numDigit = 1;
  } else {
    numDigit = 8 - acc;
  }
  
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


void printNumber() {
  if (numDigit == 0) {
    lc.setDigit(0, 0, 0, false);
    return;
  }

  if (isNegative) {
    //print character '-' in the leftmost column
    lc.setChar(0, numDigit, '-', false);
  }

  lc.clearDisplay(0);
  int temp = numDigit - 1;

  for (int i = 0; i < numDigit; i++) {
    if (hasDecimal && buff[temp] == dot) {
      temp--;
      lc.setChar(0, i, buff[temp], true);
    } else {
      lc.setChar(0, i, buff[temp], false);
    }

    if(temp <= 0) {
      break;
    }
    
    temp--;
  }
  Serial.println(buff);

}

void printError() {
  lc.clearDisplay(0);

  lc.setChar(0, 4, 'E', false);
  //  delay(100);
  lc.setChar(0, 3, 'r', false);
  //  delay(100);
  lc.setChar(0, 2, 'r', false);
  //  delay(100);
  lc.setChar(0, 1, '0', false);
  //  delay(100);
  lc.setChar(0, 0, 'r', false);
  //  delay(100);
}


void loop() {
  //  writeArduinoOn7Segment();
  key = customKeypad.getKey();
  if (key) {
    keyListener();
    Serial.println(key);
    if (!hasError) {
      printNumber();
    } else {
      printError();
    }
  }
}
