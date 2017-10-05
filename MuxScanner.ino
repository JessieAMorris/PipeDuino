//The keyboard/piston/stops multiplexers use multiple 8x8 diode matrices to select up to 64 switch positions per matrix.
//All matrices use the same set of eight row addressing pins to select the current row to scan. 
//Each matrix has its own set of eight input lines, one for each column.

void InitialiseMultiplexer() {
  CurrentMuxSelectRow = 0;
  for (byte Row = 0; Row < 8; Row++) {
    pinMode(MuxRowAddressPins[Row], OUTPUT);
  }
  for (byte Col = 0; Col < 8; Col++) {
    if (SwellMatrixExists) {
      pinMode(SwellRowInputPins[Col], INPUT);
    }
    if (GreatMatrixExists) {
      pinMode(GreatRowInputPins[Col], INPUT);
    }
    if (ChoirMatrixExists) {
      pinMode(ChoirRowInputPins[Col], INPUT);
    }
    if (PedalMatrixExists) {
      pinMode(PedalRowInputPins[Col], INPUT);
    }
    if (PistonMatrixExists) {
      pinMode(PistonsRowInputPins[Col], INPUT);
    }
  }
}

void NewInitialiseMultiplexer() {
  for (byte Row = 0; Row < 8; Row++) {
    pinMode(MuxRowAddressPins[Row], OUTPUT);
  }
  for (byte Col = 0; Col < 8; Col++) {
    if (SwellMatrixExists) {
      pinMode(SwellRowInputPins[Col], INPUT);
    }
    if (GreatMatrixExists) {
      pinMode(GreatRowInputPins[Col], INPUT);
    }
    if (ChoirMatrixExists) {
      pinMode(ChoirRowInputPins[Col], INPUT);
    }
    if (PedalMatrixExists) {
      pinMode(PedalRowInputPins[Col], INPUT);
    }
    if (PistonMatrixExists) {
      pinMode(PistonsRowInputPins[Col], INPUT);
    }
  }
  MuxKeyboard = 0;
  CurrentMuxSelectRow = 0;
  MuxCol = 0;
  SetRowAddressLines();
}

void SetRowAddressLines() {
  for (byte Row = 0; Row < 8; Row++) {
    digitalWriteDirect(MuxRowAddressPins[Row], LOW);
  }
  digitalWriteDirect(MuxRowAddressPins[CurrentMuxSelectRow], HIGH);
  delayMicroseconds(20);   //Address line settling time
}

void ScanKeyboardMultiplexers() {
  byte Row;
  byte Col;

  //Set row address lines
  for (Row = 0; Row < 8; Row++) {
    digitalWriteDirect(MuxRowAddressPins[Row], LOW);
  }
  digitalWriteDirect(MuxRowAddressPins[CurrentMuxSelectRow], HIGH);
  delayMicroseconds(20);   //Address line settling time

  //Read columns for each existing keyboard
  for (Col = 0; Col < 8; Col++) {
    if (SwellMatrixExists) {
      int PinValue = digitalReadDirect(SwellRowInputPins[Col]);
      ProcessKey(CurrentMuxSelectRow + Col * 8, PinValue, SwellKeyboard);
    }
    if (GreatMatrixExists) {
      int PinValue = digitalReadDirect(GreatRowInputPins[Col]);
      ProcessKey(CurrentMuxSelectRow + Col * 8, PinValue, GreatKeyboard);
    }
    if (ChoirMatrixExists) {
      int PinValue = digitalReadDirect(ChoirRowInputPins[Col]);
      ProcessKey(CurrentMuxSelectRow + Col * 8, PinValue, ChoirKeyboard);
    }
    if (PedalMatrixExists) {
      int PinValue = digitalReadDirect(PedalRowInputPins[Col]);
      if (Col < 4) {
        //Switches 00..31 are pedals 0..31 (columns 0..3)
        ProcessKey(CurrentMuxSelectRow + Col * 8, PinValue, PedalKeyboard);
      }
      else
      {
        //Switches 32..63 are pistons 0..31 (columns 4..7)
        ProcessPiston(CurrentMuxSelectRow + ((Col - 4) * 8), PinValue);
      }
    }
    if (PistonMatrixExists) {
      int PinValue = digitalReadDirect(PistonsRowInputPins[Col]);
      //Switches 00..63 are pistons 32..95
      ProcessPiston(CurrentMuxSelectRow + ((Col + 4) * 8), PinValue);
    }
  }

  //Step to next row
  CurrentMuxSelectRow++;
  if (CurrentMuxSelectRow > 7) {
    CurrentMuxSelectRow = 0;
  }
}

void NewScanKeyboardMultiplexers() {
  KeyNumber = MuxCol * 8 + CurrentMuxSelectRow;
  if (SwellMatrixExists) {
    int PinValue = digitalReadDirect(SwellRowInputPins[MuxCol]);
    Keyboards[SwellKeyboard].Keys[KeyNumber].Value = Keyboards[SwellKeyboard].Keys[KeyNumber].Value + PinValue;
  }

  MuxCol++;
  if (MuxCol > 7) {
    MuxCol = 0;
    SampleNumber++;
    if (SampleNumber >7) {
      SampleNumber = 0;
      ProcessKey(KeyNumber, 0, MuxKeyboard);
      MuxKeyboard++;
      if (MuxKeyboard > MaxKeyboards - 1) {
        MuxKeyboard = 0;
        CurrentMuxSelectRow++;
        if (CurrentMuxSelectRow > 7) {
          CurrentMuxSelectRow = 0;
        }
        SetRowAddressLines();
      }
    }
  }
}
