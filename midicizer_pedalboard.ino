// Copyright (c) <2025> <Michael Bell>

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <Arduino.h>
#include <MIDI.h>
#include <Adafruit_TinyUSB.h>
#include <Adafruit_MCP23X17.h>

//initialize usb as midi device
Adafruit_USBD_MIDI usbdMidi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usbdMidi, MIDI);

//initialize mcp23017 chips (3)
Adafruit_MCP23X17 mcp;
Adafruit_MCP23X17 mcp1;
Adafruit_MCP23X17 mcp2;

//initialize variables
int noteStatus[32];
int swellShoeValue = 0;
int choirShoeValue = 0;
int thirtyTwoFoot = 1;
int bombarde = 1;

void setup() {
  Serial.begin(9600);
  //initialize 32' and bombard pins
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  //initialize array for note status
  for(int i = 0; i < 32; i++) {
  noteStatus[i] = 0;
  }
  //set midi channel to OMNI to start
  MIDI.begin(MIDI_CHANNEL_OMNI);

  Wire.setSDA(0);
  Wire.setSCL(1);
  //addresses for mcp23017 chips
  byte addresses[3] {0x24, 0x25, 0x26};
  //initialize input pins
  mcp.begin_I2C(addresses[0], &Wire);
  for(int j = 0; j < 16; j++) {
    mcp.pinMode(j, INPUT_PULLUP);
  }
  mcp1.begin_I2C(addresses[1], &Wire);
  for(int j = 0; j < 16; j++) {
    mcp1.pinMode(j, INPUT_PULLUP);
  }
  mcp2.begin_I2C(addresses[2], &Wire);
  for(int j = 0; j < 16; j++) {
    mcp2.pinMode(j, INPUT_PULLUP);
  }
  Wire.setClock(1000000);
}

void loop() {
  //check swell expression pedal
  int value = analogRead(26);
  int ranges = map(value, 840, 1023, 1, 127);
  if(ranges != swellShoeValue) {
    swellShoeValue = ranges;
    MIDI.sendControlChange(1, ranges, 1);
  }
  //check choir expression pedal
  value = analogRead(27);
  ranges = map(value, 45, 1023, 1, 127);
  if(ranges != choirShoeValue) {
    choirShoeValue = ranges;
    MIDI.sendControlChange(2, ranges, 1);
  }
  MIDI.read();
  int midiNum = 35;
  int noteNum = -1;
  //first mcp chip pedalboard
  for(int i = 0; i < 16; i++) {
    noteNum++;
    midiNum++;
    int temp = mcp.digitalRead(i);
    if(temp == 0 && noteStatus[noteNum] == 1) {
      MIDI.sendNoteOff(midiNum, 64, 4);
      noteStatus[noteNum] = 0;
    }
    if(temp == 1 && noteStatus[noteNum] == 0) {
      MIDI.sendNoteOn(midiNum, 64, 4);
      noteStatus[noteNum] = 1;
    }
  }
  //second mcp chip pedalboard
  for(int i = 0; i < 16; i++) {
    noteNum++;
    midiNum++;
    int temp = mcp1.digitalRead(i);
    if(temp == 0 && noteStatus[noteNum] == 1) {
      MIDI.sendNoteOff(midiNum, 64, 4);
      noteStatus[noteNum] = 0;
    }
    if(temp == 1 && noteStatus[noteNum] == 0) {
      MIDI.sendNoteOn(midiNum, 64, 4);
      noteStatus[noteNum] = 1;
    }
  }
  //general foot pistons
  midiNum = 63;
  for(int i = 0; i < 12; i++) {
    midiNum++;
    if(!mcp2.digitalRead(i)) {
      MIDI.sendNoteOn(midiNum, 64, 16);
    }
  }
  //divisional foot pistons
  midiNum = 75;
  for(int i = 12; i < 16; i++) {
    midiNum++;
    if(!mcp2.digitalRead(i)) {
      MIDI.sendNoteOn(midiNum, 64, 16);
    }
  }
  //32' foot piston, set up to use note on toggle!
  midiNum++;
  int temp = digitalRead(14);
  if(temp == 0 && thirtyTwoFoot == 1) { 
    MIDI.sendNoteOn(midiNum, 64, 16);
    thirtyTwoFoot = 0;
  }
  if(temp == 1 && thirtyTwoFoot == 0) {
    thirtyTwoFoot = 1;
  }
  //Bombarde foot piston, set up to use note on toggle!
  temp = digitalRead(15);
  midiNum++;
  if(temp == 0 && bombarde == 1) {
    MIDI.sendNoteOn(midiNum, 64, 16);
    bombarde = 0;
  }
  if(temp == 1 && bombarde == 0) {
    bombarde = 1;
  }
}
