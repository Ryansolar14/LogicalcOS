//Code author: Ryan Busch
//Credits: Sparkfun Library Example 5

#include <Wire.h>
#include "SparkFun_Qwiic_Keypad_Arduino_Library.h"

KEYPAD keypad;

const int interruptPin = 2;
volatile boolean buttonAvailable = false;


void setup() {
    Serial.begin(9600);
    Serial.println("Qwiic KeyPad Example 5 - InterruptRead");

    if (keypad1.begin() == false) 	// Note, using begin() like this will use default I2C address, 0x4B.
    // You can pass begin() a different address like so: keypad1.begin(Wire, 0x4A).
    {
        Serial.println("Keypad does not appear to be connected. Please check wiring. Freezing...");
        while (1);
    }
    Serial.print("Initialized. Firmware Version: ");
    Serial.println(keypad1.getVersion());

    Serial.println("Press a button and it will print here.");

    pinMode(interruptPin, INPUT); // Qwiic Keypad holds INT pin HIGH @ 3.3V, then LOW when fired.
    // Note, this means we do not want INPUT_PULLUP.

    attachInterrupt(digitalPinToInterrupt(interruptPin), intReadPrintButton, FALLING);
    // Note, INT on the Keypad will "fall" from HIGH to LOW when a new button has been pressed.
    // Also note, it will stay low while there are still button events on the stack.
    // This is useful if you want to "poll" the INT pin, rather than use a hardware interrupt.
}

void loop() {
    if(buttonAvailable){
        keypad.updateFIFO();
        Serial.print(char(keypad1.getButton()));
        buttonAvailable = false;
    }
}

void intReadPrintButton(){
    buttonAvailable = true;
}
