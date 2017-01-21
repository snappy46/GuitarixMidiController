# GuitarixMidiController
Guitarix midi controller using an Arduino Leonardo to allow easy bank and preset changes using foot switches or any other switches for that matter. Other Arduino model can also work as long as they provide HID capability.

Currently the bank and preset are hardcoded into the program and changes to the bank and preset requires reprogramming the Arduino.  The bank and preset work in pair; see program array below for explanation.  I am only using 7 switches if you want more switches simply modifiy NUM_SWITCHES to a higher number in the code and adjust the bank and preset array size as well as the const byte with the right switches accordingly.

![bankPreset.png](https://github.com/snappy46/GuitarixMidiController/blob/master/Screenshot/bankPreset.png)

As shown in the image midiBankLSB select the bank wanted and midiProgram select the preset.  First element in the array is used by switch1, second element switch2 etc...  For example if I wanted to select the first bank and third preset when momentary switch3 is pressed then I would set the third element in the midiBankLSB to 0x00 and the third element in midiProgram to 0x03.  It should also be noted that guitarix bank0 refer to the top item in the Preset Selection list and preset/program0 correspond to the first Preset in the Presetlist. For example in the image below arizona is bank0 and arizona-low is preset1. If arizona was deleted then kokoko3K would become bank0. For ease of use I simply create my own bank and make sure I keep on top of the bank list (bank0) and then insert all the preset that I want to use in preset list. Keep in mind that anytime you had a new bank to guitarix it is inserted on top therefore bank0. To keep your bank on top you then may need to organize your bank/preset list and move your bank back to the top after you downloaded a bank.

![guitarixBankPreset.png](https://github.com/snappy46/GuitarixMidiController/blob/master/Screenshot/guitarixBankPreset.png)

Other caveat....

Once programmed and connected to the USB port the Arduino midi controller only shows in the ALSA tab. (see below)

![JackAlsa.png](https://github.com/snappy46/GuitarixMidiController/blob/master/Screenshot/JackAlsa.png)

In other to connect with guitarix it need to be available in the MIDI tab.  To make it available in the midi tab you need to run the a2midid bridge program an then the Arduino midi controller will show on the MIDI tab.

![JackMidi.png](https://github.com/snappy46/GuitarixMidiController/blob/master/Screenshot/JackMidi.png)

The easiest way to make sure that it run everytime you start guitarix is to run it when jack (QJackCtl) is run by adding it to Jack options menu as a script to start after jack has started.  (see image below)

![JackSetup.png](https://github.com/snappy46/GuitarixMidiController/blob/master/Screenshot/JackSetup.png)


###Arduino Schematic

![midicontroller_schem.jpg](https://github.com/snappy46/GuitarixMidiController/blob/master/Schematic/midicontroller_schem.jpg)

###Arduino Board

![midicontroller_bb.jpg](https://github.com/snappy46/GuitarixMidiController/blob/master/Schematic/midicontroller_bb.jpg)


Keep on Rocking !!!
