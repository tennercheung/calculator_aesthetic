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
  {'7', '8', '9', 'c'},
  {'*', '0', '/', '='}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

/* Holds the value for User inputs*/
float numOne;
float total = 0;

// array for printing it with a for loop, stores each num input
//char digit[7];

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

// key character from key pad
char key;

/*///////////////////////////////////////////////////////////////////////////////*/

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LedControl lc = LedControl(12, 11, 10, 1);

/////////////////////////////////////////////////////////////////////////////////
// Key Listener Functions
void keyListener() {
  switch (key) {
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

void operKeySetter() {
  switch (key) {
    case add:
      oper = add;
      break;
    case subtract:
      oper = subtract;
      break;
    case multiply:
      oper = multiply;
    case divide:
      oper = divide;
      break;
    case clr:
      reset();
      break;
    case equal:
      getEqual();
      break;
    case dot:
      setDecimal();
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
  // AHHHHHHH i can't explain this in words sooo yeahhhh.....
  if (hasResult && oper == none) {
    reset();
  }

  // This condition makes sure that the print doesn't get cleared until a new number input has been clicked.
  if (oper != none) {
    numOne = total;
    total = 0;
  }

  if ( numDigit <= 7) {
    if (total == 0) {
      total = i;
    } else {
      total = (total * 10) + i;
    }

//    digit[numDigit] = i;
    numDigit++;
  }
}


// Called if the CLEAR operator has been clicked
void reset() {
  numOne = 0;
  total = 0;
  numDigit = 0;
  oper = none;
  hasResult = false;
}

// TODO: implement adding decimals
void setDecimal() {

};

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
          reset();
        }
        break;
    }
    hasResult = true;
    oper = none;
  }
}


//// Printing Helper
void reverse(char* str, int len) 
{ 
    int i = 0, j = len - 1, temp; 
    while (i < j) { 
        temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; 
        j--; 
    } 
} 
  
int intToStr(int x, char str[], int d) 
{ 
    int i = 0; 
    while (x) { 
        str[i++] = (x % 10) + '0'; 
        x = x / 10; 
    } 
  
    // If number of digits required is more, then 
    // add 0s at the beginning 
    while (i < d) 
        str[i++] = '0'; 
  
    reverse(str, i); 
    str[i] = '\0'; 
    return i; 
} 
  
// Converts a floating-point/double number to a string. 
void ftoa(float n, char* res, int afterpoint) 
{ 
    // Extract integer part 
    int ipart = (int)n; 
  
    // Extract floating part 
    float fpart = n - (float)ipart; 
  
    // convert integer part to string 
    int i = intToStr(ipart, res, 0); 
  
    // check for display option after point 
    if (afterpoint != 0) { 
        res[i] = '.'; // add dot 
  
        // Get the value of fraction part upto given no. 
        // of points after dot. The third parameter  
        // is needed to handle cases like 233.007 
        fpart = fpart * pow(10, afterpoint); 
  
        intToStr((int)fpart, res + i + 1, afterpoint); 
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
}


void printNumber(float v) {

  boolean negative;

  if (v < -9999 || v > 9999)
    return;
  if (v < 0) {
    negative = true;
    v *= -1;
  }


  if (negative) {
    //print character '-' in the leftmost column
    lc.setChar(0, numDigit - 1, '-', false);
  }
  else {
    //print a blank in the sign column
    lc.setChar(0, numDigit - 1, ' ', false);
  }

  //Now print the number digit by digit
  char digit[7];
  int iterator = numDigit;
  ftoa(total, digit, 4);
  for (int i = 0; i < 7; i++) {
    if ( i <= numDigit) {
      lc.setDigit(0, i, (byte) digit[iterator], false);
      iterator--;
    } else {
      lc.setDigit(0, i, ' ', false);
    }
  }
}

// TODO: print an error message when an arithmetic error appears
void printError() {

}


void loop() {
  //  writeArduinoOn7Segment();
  key = customKeypad.getKey();

  if (key) {
    keyListener();
    Serial.println(key);
  }

  if (!hasError) {
    printNumber(total);
  } else {
    printError();
  }


}
