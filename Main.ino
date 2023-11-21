//Code author: Ryan Busch
//Credits: Sparkfun Library Example 5

#include <Wire.h>
#include "SparkFun_Qwiic_Keypad_Arduino_Library.h"

KEYPAD keypad;

const int interruptPin = 2;
volatile boolean buttonAvailable = false;

//Declare Global Variables
char currentKey = "";

int num = 0;
int part = 0;

const int aPin1 = 3;
const int aPin2 = 4;
const int bPin1 = 5;
const int bPin2 = 6;
const int statusPin = 7;

String sequence = "";
String number1 = "";
String number2 = "";

bool add1[8];
bool add2[8];


void setup() {
  pinMode(statusPin, OUTPUT);
  pinMode(aPin1, OUTPUT);
  pinMode(aPin2, OUTPUT);
  pinMode(bPin1, OUTPUT);
  pinMode(bPin2, OUTPUT);

  digitalWrite(aPin1, 0);
  digitalWrite(aPin2, 0);
  digitalWrite(bPin1, 0);
  digitalWrite(bPin2, 0);
  digitalWrite(statusPin, 0);
  Serial.begin(9600);
  Serial.println("Qwiic KeyPad Example 5 - InterruptRead");

  if (keypad.begin() == false)  // Note, using begin() like this will use default I2C address, 0x4B.
  // You can pass begin() a different address like so: keypad1.begin(Wire, 0x4A).
  {
    Serial.println("Keypad does not appear to be connected. Please check wiring. Freezing...");
    while (1)
      ;
  }
  digitalWrite(statusPin, 1);
  Serial.print("Initialized. Firmware Version: ");
  Serial.println(keypad.getVersion());

  Serial.println("Press a button and it will print here.");

  pinMode(interruptPin, INPUT);  // Qwiic Keypad holds INT pin HIGH @ 3.3V, then LOW when fired.
  // Note, this means we do not want INPUT_PULLUP.

  
  attachInterrupt(digitalPinToInterrupt(interruptPin), intReadPrintButton, FALLING);
  // Note, INT on the Keypad will "fall" from HIGH to LOW when a new button has been pressed.
  // Also note, it will stay low while there are still button events on the stack.
  // This is useful if you want to "poll" the INT pin, rather than use a hardware interrupt.
}

void loop() {
  if (buttonAvailable) {
    keypad.updateFIFO();
    char currentKey = keypad.getButton();
    Serial.println(char(currentKey));
    //If # save the number to a variable
    if (currentKey == '*') {
      num++;  //Value to determine if it is the first or second number
      if (num == 1) {
        number1 = sequence;
        Serial.println("Sequence 1 " + number1);
        sequence = "";
      } else if (num == 2) {
        number2 = sequence;
        Serial.println("Sequence 2 " + number2);
        sequence = "";
      } else {
        Serial.println("Exeption100!");
      }
    } else if (currentKey == '#') {
      convertDecimalToBinary(number1.toInt(), number2.toInt(), add1, add2);
      Serial.println("Converted");
      digitalWrite(aPin1, add1[7]);
      digitalWrite(aPin2, add1[6]);
      digitalWrite(bPin1, add2[7]);
      digitalWrite(bPin2, add2[6]);  
    } else {
      sequence = sequence + currentKey;
    }
    buttonAvailable = false;
    if (num == 2) {
      num = 0;
    }
  }
}

void intReadPrintButton() {
  buttonAvailable = true;
}

void convertDecimalToBinary(int num1, int num2, bool arr1[8], bool arr2[8]) {
   for (int i = 0; i < 8; i++) {
    arr1[7 - i] = num1 & (1 << i);
  }
  for (int i = 0; i < 8; i++) {
    arr2[7 - i] = num2 & (1 << i);
  }
}

void setOutputs(bool arr[8]) {
}