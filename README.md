A project to help enable Virtual Pipe Organ consoles by connecting an Arduino to the console and translating keypresses to MIDI over USB.

There has been some work done to enable custom configurations. The basis of this project is 8x8 matrices. This allows us to have 4+ keyboards, 96+ pistons, stop tabs, etc., or other similar configurations.

The config all happens in `config.h`.

Currently only the Arduino Due is supported, but additional support will likely be added over time.
