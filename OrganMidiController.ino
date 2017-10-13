/*
********************************************************************************
*                  The Virtual Pipe Organ MIDI encoder project                 *
*                      An Arduino-based open source project                    *
********************************************************************************

********** COPRIGHT AND USAGE INFORMATION

Copyright (C) 2017 John Linford. All right reserved

This project is free software; you can redistribute it and/or modify it under the terms of
the CC BY-NC-SA 3.0 license. https://creativecommons.org/licenses/by-nc-sa/3.0/

The license applies to all part of the project including the examples and tools supplied with
the project.

********** NOTICES

This software is provided "as is". No warranty is provided regarding fitness for purpose.
No support rights exist nor are any rights implied by your use of this software.

********** ARDUINO MIDI ENCODER SOFTWARE

In keeping with the Arduino tradition, by proposing to make use of this software you must
understand and accept that this is not a "turnkey"  project. A reasonable understanding of
hardware and  software engineering will be required together with a willingness to develop
them both to meet your requirements.

********** DEPENDENCIES
Requires the MIDIUSB library, available from https://github.com/arduino-libraries/MIDIUSB

********** LIMITATIONS
Code, as written, only works on the Arduino Due 

********** VERSION INFORMATION

Version   Date       Information
--------  --------   -----------
0.001.02  2017-01-22 * Improvements to expression pedal behaviour (fewer MIDI updates, granularity)
                     * Initial work on de-bounce logic
0.001.01  2017-01-19 * Numerous coding loose ends tidied up
0.001.00  2017-01-18 * Initial development of Arduino code completed
0.000.01  2017-01-14 * Commenced development of MIDI controller
*/

//Program identification
const String ProgramName    = "Virtual Pipe Organ MIDI encoder";
const String Copyright      = "Copyright (C) 2017 John Linford";
const String ContactInfo    = "John Linford, JohnL@g3wgv.com";
const String ProgramVersion = "0.001.02";
const String ProgramDate    = "2017-01-22";

#define MaxKeyboards 8

#include "config.h"
#include "MIDIUSB.h"

//Keyboard structures and table
struct KeyValueStruct {
  byte Note;
  byte Value;
  byte Sample;
};

struct KeyboardStruct{
  KeyValueStruct Keys[MaxKeys];
  int SwellPedal;
};

KeyboardStruct Keyboards[MaxKeyboards];

//Pistons/stops structure and table
struct PistonValueStruct {
  byte Value;
  int  LastChanged;
};

PistonValueStruct Pistons[MaxPistons];

byte MuxKeyboard;
//byte MuxRow;
byte MuxCol;
byte KeyNumber;
byte SampleNumber;


//General variables
byte CurrentMuxSelectRow;
unsigned long Milliseconds;

void setup()
{
  Serial.begin(115200);
  InitialiseMultiplexer();
  InitialiseSwellPedals();
  InitialiseKeys();
  InitialisePistons();
  Serial.println("*********************************************");
  Serial.println("*  Arduino MIDI encoder for Hauptwerk VPO   *");
  Serial.println("*  (C) 2017, John Linford, JohnL@g3wgv.com  *");
  Serial.println("*********************************************");
}

void loop()
{
  unsigned long M = millis();
  if (M != Milliseconds) {
    Milliseconds = M;
    ScanKeyboardMultiplexers();
    if (Milliseconds % 100 == 0) {
      ScanSwellPedals();
    }
  }
}
