//Up to 96 pistons are available and are simple momentary action on/off switches
//Pistons 00..31 are columns 4..7 of the pedal multiplexer
//Pistons 32..95 are on the Pistons multiplexor
//The pistons may be used for any momentary switching action, including stops

void InitialisePistons() {
  for (byte P = 0; P < MaxPistons; P++) {
    Pistons[P].Value = 0;
    Pistons[P].LastChanged = 0;
  }
}

void ProcessPiston(byte PistonNumber, byte Value) {
  if (Pistons[PistonNumber].Value == 1) {
    MIDIControlChange(MIDIPistonChannel, PistonNumber, 0);
    Pistons[PistonNumber].Value = 0;
    if (PrintMIDITransactions) {
      Serial.print("Piston off: ");
      Serial.println(PistonNumber);
    }
  }
  else {
    MIDIControlChange(MIDIPistonChannel, PistonNumber, 127);
    Pistons[PistonNumber].Value = 1;
    if (PrintMIDITransactions) {
      Serial.print("Piston on:  ");
      Serial.println(PistonNumber);
    }
  }
}
