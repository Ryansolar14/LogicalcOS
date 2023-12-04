# LogicalcOS
Arduino program for a 6 bit addition calculator circuit implemented with 7400 series logic chips.

## About
This program reads data from a Sparkfun Qwiic (I2C) keypad, processes it, ensures the data is not over bounds, and then outputs it on two 8 Bit 74HC595N Shift Registers. The program also has an overflow indicator output and a indicator to determine if the keypad has initialized.

## Hardware
This project has only been tested and run on the Arduino Nano Every. Pinouts are as follows:

|Purpose           |Pin#      |
|------------------|----------|
|Clock Pin 1       |3         |
|Latch Pin 1       |4         |
|Data Pin 1        |5         |
|Clock Pin 2       |6         |
|Latch Pin 2       |9         |
|Data Pin 2        |8         |
|Overflow Pin      |10        |
|Interrupt Pin     |2         |
|Keypad Status     |7         |
