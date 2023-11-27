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
const int latchPin2 = 9;
const int dataPin2 = 8;

const int overflowPin = 10;
const int statusPin = 7;

String sequence = "";
String number1 = "";
String number2 = "";

int num1 = 0;
int num2 = 0;

void setup() {
  pinMode(statusPin, OUTPUT);
  pinMode(overflowPin, OUTPUT);
  pinMode(clockPin1, OUTPUT);
  pinMode(latchPin1, OUTPUT);
  pinMode(dataPin1, OUTPUT);
  pinMode(clockPin2, OUTPUT);
  pinMode(latchPin2, OUTPUT);
  pinMode(dataPin2, OUTPUT);

  setRegister(0, clockPin1, latchPin1, dataPin1);
  setRegister(0, clockPin2, latchPin2, dataPin2);

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
  if (buttonAvailable) {
    keypad.updateFIFO();
    char currentKey = keypad.getButton();
    Serial.println(char(currentKey));
    //If # save the number to a variable
    if (currentKey == '*') {
      num++;  //Value to determine if it is the first or second number
      if (num == 1) {
        number1 = sequence;
        num1 = number1.toInt();
        Serial.println("Sequence 1 " + number1);
        sequence = "";
      } else if (num == 2) {
        number2 = sequence;
        num2 = number2.toInt();
        Serial.println("Sequence 2 " + number2);
        sequence = "";
      } else {
        Serial.println("Exeption100!");
      }
    } else if (currentKey == '#') {
        if((num1 + num2) > 511){
          digitalWrite(overflowPin, HIGH);
        } else {
          digitalWrite(overflowPin, LOW);
          setRegister(num1, clockPin1, latchPin1, dataPin1);
          setRegister(num2, clockPin2, latchPin2, dataPin2);
        }
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