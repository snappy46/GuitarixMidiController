
/*
   Simple guitarix Bank and Preset switcher

   Created: 18/01/2017
   Author: Marcel Levesque <levesque.marcel@gmail.com> aka snappy46

   Code and concept heavily based on:
   http://www.arduino.cc/en/Tutorial/MidiDevice
*/

#include <MIDIUSB.h>
#define NUM_SWITCHES  8

const byte switch1 = 2;
const byte switch2 = 3;
const byte switch3 = 4;
const byte switch4 = 5;
const byte switch5 = 6;
const byte switch6 = 7;
const byte switch7 = 8;
const byte switch8 = 9;

const byte led1 = A2;
const byte led2 = A3;
const byte led3 = A4;
const byte led4 = A5;
const byte led5 = 10;
const byte led6 = 11;
const byte led7 = 12;
const byte led8 = 13;


const byte switches[NUM_SWITCHES] = {switch1, switch2, switch3, switch4, switch5, switch6, switch7, switch8};
const byte leds[NUM_SWITCHES] = {led1, led2, led3, led4, led5, led6, led7, led8};
const byte midiBankLSB[NUM_SWITCHES] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // Bank selection based on switch pressed (switch 1-8)
const byte midiProgram[NUM_SWITCHES] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07}; // Program/Preset selection based on switch pressed (switch 1-8)

const byte channel = 0x00;  // use midi channel 1

const long loopRecordingLedBlinkInterval = 500; //blink interval of 500 msec or 1/2 sec
const long switchesDebounceInterval = 500; // switch debouncer interval

int previouslyPressedSwitch = -1;
int loopRecording = 0;
int loopPlaying = 0;

int ledState = LOW;

unsigned long previousMillis = 0;  //Store the last time led was updated.
unsigned long previousBounceMillis = 0;  //Store the last time a switch was activated

void setup() {
  for (int i = 0; i < NUM_SWITCHES; i++) {
    pinMode(switches[i], INPUT_PULLUP);
    pinMode(leds[i], OUTPUT);
  }
}


void loop() {
  readSwitches();
  if (loopRecording == 1) // blinklite if loop is recording
  {
    blinkLight();
  }
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
// If switch has been pressed then check to see if same as previous one that was pressed
// to avoid trying to set bank/preset that are already set. Also check for interval between
// switch depressed to compensate for switch bouncing.

void readSwitches() {
  unsigned long currentMillis = millis();
  for (int i = 0; i < NUM_SWITCHES; i++)
  {
    if (digitalRead(switches[i]) == LOW && i != previouslyPressedSwitch && currentMillis - previousBounceMillis > switchesDebounceInterval)
    {
      previousBounceMillis = currentMillis;
      if (i == 0) {
        looperSwitchPressed();
      }
      else
      {
        presetChangeRequest(i);
      }
      
    }
  }
}


// Method to handle looperSwitch being depress (record, play, stop playing)
void looperSwitchPressed() {
   if (loopRecording == 0) {
          if (loopPlaying == 1) {
            controlChange(channel, 0X29, 0X00);
            loopPlaying = 0;
            digitalWrite(leds[0], LOW);  // extinguish loop lite to indicate no play or recording
          }
          else {
            controlChange(channel, 0X28, 0X40);
            loopRecording = 1;
            ledState = 1;
            //digitalWrite(leds[0], ledState);
          }
          MidiUSB.flush();      
        }
        else 
        {
          controlChange(channel, 0X28, 0X00);
          controlChange(channel, 0X29, 0X40);
          MidiUSB.flush();
          loopRecording = 0;
          loopPlaying = 1;
          digitalWrite(leds[0], HIGH);  // illuminate loop lite constantly to indicate loop play.
        }
        //delay(500); //small delay to cancel switch bounce 
}


// Method to handle preset change swithches
void presetChangeRequest(int i) {
  controlChange(channel, 0X00, 0X00); // Bank select MSB
  controlChange(channel, 0X20, midiBankLSB[i]);  // Bank select LSB
  programChange(channel, midiProgram[i]);  // Program/Preset change
  digitalWrite(leds[i], HIGH);
  digitalWrite(leds[previouslyPressedSwitch], LOW);
  previouslyPressedSwitch = i; // store switch that was pressed
  MidiUSB.flush();
}


//Causes the first led (loop) to continiously blink at a rate define by the interval constant
void blinkLight()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > loopRecordingLedBlinkInterval)
  {
    previousMillis =  currentMillis;

    if (ledState == LOW)
    {
      ledState = HIGH;
    }
    else
    {
      ledState = LOW;
    }
    digitalWrite(leds[0], ledState);
  }
}

