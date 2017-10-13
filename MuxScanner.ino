//The keyboard/piston/stops multiplexers use multiple 8x8 diode matrices to select up to 64 switch positions per matrix.
//All matrices use the same set of eight row addressing pins to select the current row to scan.
//Each matrix has its own set of eight input lines, one for each column.

void InitialiseMultiplexer() {
  CurrentMuxSelectRow = 0;
  for (byte row = 0; row < 8; row++) {
    pinMode(MuxRowAddressPins[row], OUTPUT);
  }

  for (byte col = 0; col < 8; col++) {
    for (int keyboard = 0; keyboard < EnabledKeyboards - 1; keyboard++) {
      if(KeyboardInputPins[keyboard][col] != -1) {
        pinMode(KeyboardInputPins[keyboard][col], INPUT);
      }
    }

    for (int piston = 0; piston < EnabledPistons; piston++) {
      if(PistonInputPins[piston][col] != -1) {
        pinMode(KeyboardInputPins[piston][col], INPUT);
      }
    }
  }
}

void SetRowAddressLines() {
  for (byte row = 0; row < 8; row++) {
    digitalWriteDirect(MuxRowAddressPins[row], LOW);
  }
  digitalWriteDirect(MuxRowAddressPins[CurrentMuxSelectRow], HIGH);
  delayMicroseconds(20);   //Address line settling time
}

void ScanKeyboardMultiplexers() {
  byte row;
  byte col;

  //Set row address lines
  for (row = 0; row < 8; row++) {
    digitalWriteDirect(MuxRowAddressPins[row], LOW);
  }
  digitalWriteDirect(MuxRowAddressPins[CurrentMuxSelectRow], HIGH);
  delayMicroseconds(20);   //Address line settling time

  //Read columns for each existing keyboard
  for (col = 0; col < 8; col++) {
    for (byte keyboard = 0; keyboard < EnabledKeyboards - 1; keyboard++) {
      int pinValue = digitalReadDirect(KeyboardInputPins[keyboard][col]);
      processKey(CurrentMuxSelectRow + col * 8, pinValue, keyboard);
    }

    /*
    if (PistonMatrixExists) {
      int PinValue = digitalReadDirect(PistonsRowInputPins[col]);
      //Switches 00..63 are pistons 32..95
      ProcessPiston(CurrentMuxSelectRow + ((col + 4) * 8), PinValue);
    }
    */
  }

  //Step to next row
  CurrentMuxSelectRow++;
  if (CurrentMuxSelectRow > 7) {
    CurrentMuxSelectRow = 0;
  }
}
