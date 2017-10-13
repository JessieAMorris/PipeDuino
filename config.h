#define PrintMIDITransactions true     //True to print MIDI transactions to programming port

#define EnabledKeyboards 1
#define EnabledPistons 2

//Sizing definitions
#define MaxKeys 64
#define MaxPistons 96

const String KeyboardName[EnabledKeyboards] = { "Swell" };

//Keyboard multiplexer row addressing output pins
const byte MuxRowAddressPins[8] = { 2, 3, 4, 5, 6, 7, 8, 9 };

//Keyboard & piston multiplexer column input pins
const byte KeyboardInputPins[EnabledKeyboards][8] = {
  { 22, 23, 24, 25, 26, 27, 28, 29 },     //Swell notes 00..63
  //{ 30, 31, 32, 33, 34, 35, 36, 37 },   //Great notes 00..63
  //{ 38, 39, 40, 41, 42, 43, 44, 45 },   //Choir notes 00..63
  //{ 46, 47, 48, 49, -1, -1, -1, -1 }    // Pedals 00..31
};

const byte PistonInputPins[EnabledPistons][8] = {
  { 50, 51, 52, 53, -1, -1, -1, -1 },  //Pistons/stops 00..31
  { 14, 15, 16, 17, 18, 19, 20, 21 },  //Pistons/stops 32..95
};

//Swell pedal input pins
const byte SwellPedalPin[EnabledKeyboards] = { A0 };

//MIDI channels for each component of the organ. Channels are in the range 1..16 
const byte MIDIChannel[EnabledKeyboards] = { 2 };
const byte MIDIPistonChannel = 6;
const byte MIDISwellChannel = 7;

