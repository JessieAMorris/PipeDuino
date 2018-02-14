//Expression (swell) pedals are defined for all manuals and the pedal board.
//Arduino input range is 0V..3.3V, represented by values 0..1023
//MIDI range is 0..127, but there is no need for that level of
//granularity, so arduino value is divided by 64 then multiplied
//by 8 to give 0..127 in 16 steps of 8

void InitialiseSwellPedals() {
  for (byte S = 0; S < ENABLED_KEYBOARDS; S++) {
    pinMode(SwellPedalPin[S], INPUT);
  }
}

void ScanSwellPedals() {
  for (byte S = 0; S < ENABLED_KEYBOARDS; S++) {
    int SV = analogRead(SwellPedalPin[S]);
    int SwellValue = round(SV / 64) * 8;           //Scale to 0..120 in steps of 8
    if (SV > 1022) {
      SwellValue = 127;                            //Handle the fully open condition
    }
    if (keyboards[S].SwellPedal != SwellValue) {
      keyboards[S].SwellPedal = SwellValue;
      MIDIControlChange(MIDISwellChannel, S, SwellValue);
      if (PrintMIDITransactions) {
        Serial.print("Expression pedal for ");
        Serial.print(KeyboardName[S]);
        Serial.print(" keyboard, value ");
        Serial.println(SwellValue);
      }
    }
  }
}
