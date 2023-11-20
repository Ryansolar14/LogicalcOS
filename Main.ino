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
String sequence = "";
String number1 = "";
String number2 = "";
bool add1[8];
bool add2[8];


void setup() {
    Serial.begin(9600);
    Serial.println("Qwiic KeyPad Example 5 - InterruptRead");

    if (keypad.begin() == false) 	// Note, using begin() like this will use default I2C address, 0x4B.
    // You can pass begin() a different address like so: keypad1.begin(Wire, 0x4A).
    {
        Serial.println("Keypad does not appear to be connected. Please check wiring. Freezing...");
        while (1);
    }
    Serial.print("Initialized. Firmware Version: ");
    Serial.println(keypad.getVersion());

    Serial.println("Press a button and it will print here.");

    // pinMode(interruptPin, INPUT); // Qwiic Keypad holds INT pin HIGH @ 3.3V, then LOW when fired.
    // Note, this means we do not want INPUT_PULLUP.

    // attachInterrupt(digitalPinToInterrupt(interruptPin), intReadPrintButton, FALLING);
    // Note, INT on the Keypad will "fall" from HIGH to LOW when a new button has been pressed.
    // Also note, it will stay low while there are still button events on the stack.
    // This is useful if you want to "poll" the INT pin, rather than use a hardware interrupt.
}

void loop() {
    if(buttonAvailable){
        keypad.updateFIFO();
        char key = keypad.getButton();
        if(key != 0){
          currentKey = keypad.getButton();
        }
        Serial.println(char(currentKey));
        //If # save the number to a variable
        if(currentKey == '#'){
            num++; //Value to determine if it is the first or second number
            if(num == 1){
                number1 = sequence;
                Serial.println("Sequence 1" + number1);
            } else if(num == 2){
                number2 = sequence;
                Serial.println("Sequence 2" + number2);
            }
        } else if(currentKey == '*'){
            convertDecimalToBinary(number1.toInt(), add1);
            convertDecimalToBinary(number2.toInt(), add2);
        } else {
            sequence = sequence + currentKey;
        }
        
        delay(1000);
        currentKey = 0;
        buttonAvailable = false;
    } 
}

void intReadPrintButton(){
    buttonAvailable = true;
}

void convertDecimalToBinary(int num, bool arr[8]) {
    if(num < 256 && num >= 0){
      for (int i = 0; i < 8; i++) {
      arr[7 - i] = (num >> i) & 1;
    }
  }
}

void setOutputs(bool add1[8], bool add2[8]){
    
}