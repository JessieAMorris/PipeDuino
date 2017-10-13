void InitialiseKeys() {
  for (byte Keyboard = 0; Keyboard<EnabledKeyboards; Keyboard++) {
    byte Note = 36;   //Bottom C
    for (byte Key = 0; Key < MaxKeys; Key++) {
      Keyboards[Keyboard].Keys[Key].Value = 0;
      Keyboards[Keyboard].Keys[Key].Sample = 0;
      Keyboards[Keyboard].Keys[Key].Note = Note;
      Note++;
    }
  }
}

void processKey(byte Key, byte Value, byte Keyboard) {
  if (Value != Keyboards[Keyboard].Keys[Key].Value) {
    //Key has changed position
    if (Value == 0) {
      //Key off
      MIDINoteOff(MIDIChannel[Keyboard], Keyboards[Keyboard].Keys[Key].Note, 64);
      if (PrintMIDITransactions) {
        Serial.print(KeyboardName[Keyboard]);
        Serial.print(", note off: ");
        Serial.println(Keyboards[Keyboard].Keys[Key].Note);
      }
    }
    else {
      //Key on
      MIDINoteOn(MIDIChannel[Keyboard], Keyboards[Keyboard].Keys[Key].Note, 64);
      if (PrintMIDITransactions) {
        Serial.print(KeyboardName[Keyboard]);
        Serial.print(", note on:  ");
        Serial.println(Keyboards[Keyboard].Keys[Key].Note);
      }
    }
    Keyboards[Keyboard].Keys[Key].Value = Value;
  }
}
