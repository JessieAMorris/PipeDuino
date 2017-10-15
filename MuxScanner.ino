//The keyboard/piston/stops multiplexers use multiple 8x8 diode matrices to select up to 64 switch positions per matrix.
//All matrices use the same set of eight row addressing pins to select the current row to scan.
//Each matrix has its own set of eight input lines, one for each column.

void InitialiseMultiplexer() {
  currentMuxSelectRow = 0;
  for (byte row = 0; row < 8; row++) {
    pinMode(MuxRowAddressPins[row], OUTPUT);
  }

  for (int keyboard = 0; keyboard < ENABLED_KEYBOARDS; keyboard++) {
    for (byte col = 0; col < 8; col++) {
      if(KeyboardInputPins[keyboard][col] != 0) {
        pinMode(KeyboardInputPins[keyboard][col], INPUT);
      }
    }
  }

  for (int piston = 0; piston < EnabledPistons; piston++) {
    for (byte col = 0; col < 8; col++) {
      if(PistonInputPins[piston][col] != 0) {
        pinMode(PistonInputPins[piston][col], INPUT);
      }
    }
  }
}

void SetRowAddressLines() {
  for (byte row = 0; row < 8; row++) {
    digitalWriteDirect(MuxRowAddressPins[row], LOW);
  }
  digitalWriteDirect(MuxRowAddressPins[currentMuxSelectRow], HIGH);
  delayMicroseconds(20);   //Address line settling time
}

void ScanKeyboardMultiplexers() {
  byte row;
  byte col;

  //Set row address lines
  for (row = 0; row < 8; row++) {
    digitalWriteDirect(MuxRowAddressPins[row], LOW);
  }
  digitalWriteDirect(MuxRowAddressPins[currentMuxSelectRow], HIGH);
  delayMicroseconds(20);   //Address line settling time

  //Read columns for each existing keyboard
  for (col = 0; col < 8; col++) {
    for (byte keyboard = 0; keyboard < ENABLED_KEYBOARDS; keyboard++) {
      if(KeyboardInputPins[keyboard][col] != 0) {
        int pinValue = digitalReadDirect(KeyboardInputPins[keyboard][col]);
        processKey(currentMuxSelectRow + col * 8, pinValue, keyboard);
      }
    }

    /*
    if (PistonMatrixExists) {
      int PinValue = digitalReadDirect(PistonsRowInputPins[col]);
      //Switches 00..63 are pistons 32..95
      ProcessPiston(currentMuxSelectRow + ((col + 4) * 8), PinValue);
    }
    */
  }

  //Step to next row
  currentMuxSelectRow++;
  if (currentMuxSelectRow > 7) {
    currentMuxSelectRow = 0;
  }
}
