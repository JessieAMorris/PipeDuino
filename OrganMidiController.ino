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

#include "MIDIUSB.h"

#define PrintMIDITransactions true     //True to print MIDI transactions to programming port

//Organ keyboard/piston matrices
//Only the keyboards/piston matrices that are set to true will be scanned
//All eight input pins for each matrix that is set to true must be pulled down to 
//ground with a 10k resistor to prevent spurious note on/off messages due to noise.
#define SwellMatrixExists   true
#define GreatMatrixExists   false
#define ChoirMatrixExists   false
#define PedalMatrixExists   false
#define PistonMatrixExists  false

//Sizing definitions
#define MaxKeys 64
#define MaxKeyboards 4
#define MaxPistons 96

//Indexes into Organ structure
#define SwellKeyboard 0
#define GreatKeyboard 1
#define ChoirKeyboard 2
#define PedalKeyboard 3

const String KeyboardName[MaxKeyboards] = { "Swell", "Great", "Choir", "Pedal" };

//Keyboard multiplexer row addressing output pins
const byte MuxRowAddressPins[8] = { 2, 3, 4, 5, 6, 7, 8, 9 };

//Keyboard & piston multiplexer column input pins
const byte SwellRowInputPins[8]   = { 22, 23, 24, 25, 26, 27, 28, 29 };  //Swell notes 00..63
const byte GreatRowInputPins[8]   = { 30, 31, 32, 33, 34, 35, 36, 37 };  //Great notes 00..63
const byte ChoirRowInputPins[8]   = { 38, 39, 40, 41, 42, 43, 44, 45 };  //Choir notes 00..63
const byte PedalRowInputPins[8]   = { 46, 47, 48, 49, 50, 51, 52, 53 };  //Pedals 00..31 and pistons/stops 00..31
const byte PistonsRowInputPins[8] = { 14, 15, 16, 17, 18, 19, 20, 21 };  //Pistons/stops 32..95

//Swell pedal input pins
const byte SwellPedalPin[MaxKeyboards] = { A0, A1, A2, A3 };

//MIDI channels for each component of the organ. Channels are in the range 1..16 
const byte MIDIChannel[MaxKeyboards] = { 2, 3, 4, 5 };  //Swell, Great, Choir, Pedals
const byte MIDIPistonChannel         = 6;
const byte MIDISwellChannel          = 7;

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
