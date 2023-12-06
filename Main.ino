//LogicalcOS - Arduino program to read a keypad and output the two numbers to be on two shift registers for a 6bit addition circuit.
//Date: 12/03/2023
//Code author: Ryan Busch
//Credits: Sparkfun Owiic Keypad Library Example 5

#include <Wire.h>
#include "SparkFun_Qwiic_Keypad_Arduino_Library.h"

KEYPAD keypad;
//Declare Global Variables

volatile boolean buttonAvailable = false;

char currentKey = "";

int num = 0;
int part = 0;

const int interruptPin = 2;

const int clockPin1 = 3;
const int latchPin1 = 4;
const int dataPin1 = 5;

const int clockPin2 = 6;
const int latchPin2 = 9;
const int dataPin2 = 8;

const int overflowPin = 11;
const int statusPin = 7;
const int buzzerPin = 10;

String sequence = "";
String number1 = "";
String number2 = "";

int num1 = 0;
int num2 = 0;

void setup() {
  //Set pin modes
  pinMode(statusPin, OUTPUT);
  pinMode(overflowPin, OUTPUT);
  pinMode(clockPin1, OUTPUT);
  pinMode(latchPin1, OUTPUT);
  pinMode(dataPin1, OUTPUT);
  pinMode(clockPin2, OUTPUT);
  pinMode(latchPin2, OUTPUT);
  pinMode(dataPin2, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(interruptPin, INPUT);  

  //Set all shift register output to LOW so the display is initially zero
  setRegister(0, clockPin1, latchPin1, dataPin1);
  setRegister(0, clockPin2, latchPin2, dataPin2);
 
  digitalWrite(statusPin, 0); //Only high if keypad is active

  //Initialize Serial Monitor for debug
  Serial.begin(9600);
  Serial.println("*****LogicalcOS Debug Terminal*****");

  //If keypad does not initialize, wait 500ms and try again. If it fails again, freeze the program.
  if (keypad.begin() == false)
  {
    Serial.println("Keypad does not appear to be connected...");
    delay(500);
    if(keypad.begin() == false)
    {
      Serial.println("Keypad still does not appear to be connected. Freezing...");
      while(1);
    }
  }

  digitalWrite(statusPin, 1); //Display is active
  
  attachInterrupt(digitalPinToInterrupt(interruptPin), intReadPrintButton, FALLING); //Attach interrupt pin
}

void loop() {
  if (buttonAvailable) {
    keypad.updateFIFO();
    char currentKey = keypad.getButton();
    Serial.println(char(currentKey));
    //If a number save it to a variable
    if (currentKey == '*') {
      num++;  //Value to determine if it is the first or second number
      if (num == 1) {
        //Save current sequence to number 1
        number1 = sequence;
        num1 = number1.toInt();
        Serial.println("Sequence 1 " + number1);
        sequence = "";
      } else if (num == 2) {
        //Save current sequence to number 2
        number2 = sequence;
        num2 = number2.toInt();
        Serial.println("Sequence 2 " + number2);
        sequence = "";
      } else {
        //Error, this should never happen
        Serial.println("Exeption100!");
      }
      //If key is #, check for an overflow and output the two numbers on the shift registers.
    } else if (currentKey == '#') {
        //Check for overflow, if there is an overflow, set the overflow pin high.
        if (((num1 + num2) == 1001)){
          analogWrite(buzzerPin, 100);
        } else if ((num1 > 63) || (num2 > 63)){
          digitalWrite(overflowPin, HIGH);
          analogWrite(buzzerPin, 100);
          delay(1500);
          analogWrite(buzzerPin, 0);
        } else {
          digitalWrite(overflowPin, LOW);
          setRegister(num1, clockPin1, latchPin1, dataPin1);
          setRegister(num2, clockPin2, latchPin2, dataPin2);
        }
    } else {
      //Update sequence
      sequence = sequence + currentKey;
    }
    buttonAvailable = false;
    //Reset the counter that determines if the value is number 1 or number 2.
    if (num == 2) {
      num = 0;
    }
  }
}

//Interrupt function to read the button.
void intReadPrintButton() {
  buttonAvailable = true;
}

//Sets the shift register.
void setRegister(int num, int clockPin, int latchPin, int dataPin) {
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, MSBFIRST, num);
  digitalWrite(latchPin, 1);
}
