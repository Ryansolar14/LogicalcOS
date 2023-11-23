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

const int clockPin1 = 3;
const int latchPin1 = 4;
const int dataPin1 = 5;

const int clockPin2 = 6;
const int latchPin2 = 7;
const int dataPin2 = 8;

const int overflowPin = 9;
const int statusPin = 7;

String sequence = "";
String number1 = "";
String number2 = "";

void setup() {
  pinMode(statusPin, OUTPUT);
  pinMode(overflowPin, OUTPUT);
  pinMode(clockPin1, OUTPUT);
  pinMode(latchPin1, OUTPUT);
  pinMode(dataPin1, OUTPUT);

  digitalWrite(statusPin, 0);
  Serial.begin(9600);
  Serial.println("Qwiic KeyPad Example 5 - InterruptRead");

  if (keypad.begin() == false)
  {
    Serial.println("Keypad does not appear to be connected. Please check wiring. Freezing...");
    while (1);
  }

  digitalWrite(statusPin, 1);
  Serial.print("Initialized. Firmware Version: ");
  Serial.println(keypad.getVersion());

  Serial.println("Press a button and it will print here.");

  pinMode(interruptPin, INPUT);  

  attachInterrupt(digitalPinToInterrupt(interruptPin), intReadPrintButton, FALLING);
}

void loop() {
  analogWrite(clockPin, 100);
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

      setRegister(num1, clockPin1, latchPin1, dataPin1);

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

//Sets the shift register.
void setRegister(int num, int clockPin, int latchPin, int dataPin) {
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, MSBFIRST, num);
  digitalWrite(latchPin, 1);
}