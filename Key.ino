void InitialiseKeys() {
  for (byte keyboard = 0; keyboard < ENABLED_KEYBOARDS; keyboard++) {
    byte Note = 36;   //Bottom C
    for (byte key = 0; key < MaxKeys; key++) {
      keyboards[keyboard].keys[key].Value = 0;
      keyboards[keyboard].keys[key].Sample = 0;
      keyboards[keyboard].keys[key].Note = Note;
      Note++;
    }
  }
}

void processKey(byte key, byte Value, byte keyboard) {
  if (Value != keyboards[keyboard].keys[key].Value) {
    //key has changed position
    if (Value == 0) {
      //key off
      MIDINoteOff(MIDIChannel[keyboard], keyboards[keyboard].keys[key].Note, 64);
      if (PrintMIDITransactions) {
        Serial.print(KeyboardName[keyboard]);
        Serial.print(", note off: ");
        Serial.println(keyboards[keyboard].keys[key].Note);
      }
    }
    else {
      //key on
      MIDINoteOn(MIDIChannel[keyboard], keyboards[keyboard].keys[key].Note, 64);
      if (PrintMIDITransactions) {
        Serial.print(KeyboardName[keyboard]);
        Serial.print(", note on:  ");
        Serial.println(keyboards[keyboard].keys[key].Note);
      }
    }
    keyboards[keyboard].keys[key].Value = Value;
  }
}
