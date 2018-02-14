# PipeDuino

A project to help enable Virtual Pipe Organ consoles by connecting an Arduino to the console and windchest and translating keypresses to MIDI over USB. It also takes MIDI over USB and turns on and off solenoids based on the key presses.


## Keyboard
There has been some work done on the keyboard side to enable custom configurations. The basis of this project is 8x8 matrices. This allows us to have 4+ keyboards, 96+ pistons, stop tabs, etc., or other similar configurations.

The config all happens in `config.h`.

Currently only the Arduino Due is supported, but additional support will likely be added over time.

## Chest
The windchest controller is made to use SN74HC595 chips to drive mosfets to actually turn on and off the different solenoids. Basic support has been added, but there's much more to come!
