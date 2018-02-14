#include <pitchToNote.h>
#include <pitchToFrequency.h>
#include <frequencyToNote.h>
#include <MIDIUSB.h>

//**************************************************************//
//  Name    : shiftOutCode, Hello World
//  Author  : Carlyn Maw,Tom Igoe, David A. Mellis
//  Date    : 25 Oct, 2006
//  Modified: 23 Mar 2010
//  Version : 2.0
//  Notes   : Code for using a 74HC595 Shift Register           //
//          : to count from 0 to 255
//****************************************************************

//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

byte outputStates = 0x00;


void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  Serial.begin(115200);
  Serial.println("Starting loop");
}

byte keyOn = 0x09;
byte keyOff = 0x08;

void loop() {
  midiEventPacket_t rx;
  do {
    rx = MidiUSB.read();
    if (rx.header != 0) {
      byte key = rx.byte2 - 0x48;
      byte mask = 0xff << key;

      if(rx.header == keyOn || rx.header == keyOff) {
        byte channel;
        if(rx.header == keyOn) {
          channel = rx.byte1 ^ 0x90;
        } else {
          channel = rx.byte1 ^ 0x80;
        }
        
        byte mask = 1 << (key);
        
        Serial.print("Got a keyOn for ");
        Serial.print(key, HEX);
        Serial.print(" with mask ");
        Serial.println(mask, BIN);

        Serial.print("Going to change the output state bit. Current state: ");
        Serial.println(outputStates, BIN);
        
        outputStates = outputStates ^ mask;

        Serial.print("After state: ");
        Serial.println(outputStates, BIN);
      }
//      Serial.print("Received: ");
//      Serial.print(rx.header, HEX);
//      Serial.print("-");
//      Serial.print(rx.byte1, HEX);
//      Serial.print("-");
//      Serial.print(rx.byte2, HEX);
//      Serial.print("-");
//      Serial.println(rx.byte3, HEX);
    }
  } while (rx.header != 0);


    // take the latchPin low so
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    
    // shift out the bits:
    shiftOut(dataPin, clockPin, MSBFIRST, outputStates);

    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
}


