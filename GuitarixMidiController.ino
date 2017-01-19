
/*
   Simple guitarix Bank and Preset switcher

   Created: 18/01/2017
   Author: Marcel Levesque <levesque.marcel@gmail.com> aka snappy46

   Code and concept heavily based on:
   http://www.arduino.cc/en/Tutorial/MidiDevice
*/

#include <MIDIUSB.h>
#define NUM_SWITCHES  7

const byte switch1 = 2;
const byte switch2 = 3;
const byte switch3 = 4;
const byte switch4 = 5;
const byte switch5 = 6;
const byte switch6 = 7;
const byte switch7 = 8;

const byte switches[NUM_SWITCHES] = {switch1, switch2, switch3, switch4, switch5, switch6, switch7};
const byte midiBankLSB[NUM_SWITCHES] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06}; // Bank selection based on switch pressed (switch 1-7)
const byte midiProgram[NUM_SWITCHES] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06}; // Program/Preset selection based on switch pressed (switch 1-7)

const byte channel = 0x00;  // use midi channel 1

int previouslyPressedSwitch = -1;

void setup() {
  for (int i = 0; i < NUM_SWITCHES; i++)
    pinMode(switches[i], INPUT_PULLUP);
}


void loop() {
  readSwitches();
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control value (0-127).

void programChange(byte channel, byte value) {
  midiEventPacket_t event = {0x0C, 0xC0 | channel, value};
  MidiUSB.sendMIDI(event);
}

// Read all the switches to check if any switch has been pressed.
// If switch has been pressed then check to see if same as previous one that was pressed.
// This act as a switch debouncer and avoid trying to set bank/preset that are already set.

void readSwitches()
{
  for (int i = 0; i < NUM_SWITCHES; i++)
  {
    if (digitalRead(switches[i]) == LOW  && i != previouslyPressedSwitch)
    {
      controlChange(channel, 0X00, 0X00); // Bank select MSB
      controlChange(channel, 0X20, midiBankLSB[i]);  // Bank select LSB
      programChange(channel, midiProgram[i]);  // Program/Preset change
      MidiUSB.flush();
      previouslyPressedSwitch = i; // store switch that was pressed
    }
  }
}

