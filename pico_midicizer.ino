// Copyright (c) <2025> <Michael Bell>

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <Arduino.h>
#include <MIDI.h>
#include <Adafruit_TinyUSB.h>
#include <Adafruit_MCP23X17.h>


//initialize usb output midi only
Adafruit_USBD_MIDI usbdMidi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usbdMidi, MIDI);
//create array of mcp units
constexpr uint8_t mcps {16};
Adafruit_MCP23X17 mcp[mcps];
byte addresses[8] {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27};

//analog shoes lookback value
int swellShoeValue = 0;
int choirShoeValue = 0;
int cncl = 1;
int set = 1;
//all keyboards and pedal. Pedal starts at i=183.
int noteStatus[215];

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  //set midi channel to OMNI to start
  MIDI.begin(MIDI_CHANNEL_OMNI);
  //set up main general pistons
  pinMode(12, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);
  pinMode(20, INPUT_PULLUP);
  pinMode(21, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(22, INPUT_PULLUP);

  //cancel button
  pinMode(4, INPUT_PULLUP);
  //set button
  pinMode(5, INPUT_PULLUP);
  for(int i = 0; i < 183; i++) {
    noteStatus[i] = 1;
  }
  for(int i = 183; i < 215; i++) {
    noteStatus[i] = 0;
  }
  //declare I2C lanes for both sets of chips
  Wire.setSDA(0);
  Wire.setSCL(1);
  Wire1.setSDA(2);
  Wire1.setSCL(3);
  //iterate through I2C lane 0
  for(int i = 0; i < 8; i++) {
    mcp[i].begin_I2C(addresses[i], &Wire);

    for(int j = 0; j < 16; j++) {
      mcp[i].pinMode(j, INPUT_PULLUP);
    }
  }
  Wire.setClock(1000000);
  //set up 2nd i2c bank
  for(int i =0; i < 4; i++) {
    int mcpNumber = i + 8;
    mcp[mcpNumber].begin_I2C(addresses[i], &Wire1);
    if(i != 4 && i != 6){
      for(int j = 0; j < 16; j++) {
        mcp[mcpNumber].pinMode(j, INPUT_PULLUP);
      }
    }
    if(i == 4) {
      for(int j = 0; j < 7; j++) {
        mcp[mcpNumber].pinMode(j, INPUT_PULLUP);
      }
    }
    if(i == 6) {
      for(int j = 0; j < 12; j++) {
        mcp[mcpNumber].pinMode(j, INPUT_PULLUP);
      }
    }
  }
  Wire1.setClock(1000000);
}

void loop() {
  //turn on built-in LED light on rp2040
  digitalWrite(LED_BUILTIN, HIGH);
  MIDI.read();
  
  //read cancel and set, send midi event if changed
  if(!digitalRead(4) && cncl == 1) {
    MIDI.sendNoteOn(60, 64, 16);
    cncl = 0;
  }
  if(digitalRead(4) == 1 && cncl == 0) {
    MIDI.sendNoteOff(60, 64, 16);
    cncl = 1;
  }
  if(!digitalRead(5) && set == 1) {
    MIDI.sendNoteOn(61, 64, 16);
    set = 0;
  }
  if(digitalRead(5) == 1 && set == 0) {
    MIDI.sendNoteOff(61, 64, 16);
    set = 1;
  }

  //look for depressed general pistons
  int temp[12];
  temp[0] = digitalRead(12);
  temp[1] = digitalRead(8);
  temp[2] = digitalRead(9);
  temp[3] = digitalRead(14);
  temp[4] = digitalRead(10);
  temp[5] = digitalRead(11);
  temp[6] = digitalRead(13);
  temp[7] = digitalRead(19);
  temp[8] = digitalRead(20);
  temp[9] = digitalRead(21);
  temp[10] = digitalRead(15);
  temp[11] = digitalRead(22);
  int midiNum = 64;
  for(int i = 0; i < 12; i++) {
    if(temp[i] == 0) {
      MIDI.sendNoteOn(midiNum, 64, 16);
    }
    midiNum++;
  } 

  //begin looping through keyboards going swell great choir pedals, midi channel 1 for swell:
  midiNum = 35;
  int noteNum = -1;
  for(int i = 0; i < 16; i++) {
    midiNum++;
    noteNum++;
    int temp = mcp[0].digitalRead(i);
    if(temp == 0 && noteStatus[noteNum] != 0) {
      MIDI.sendNoteOn(midiNum, 64, 1);
      noteStatus[noteNum] = 0;
    }
    if(temp == 1 && noteStatus[noteNum] == 0) {
      MIDI.sendNoteOff(midiNum, 64, 1);
      noteStatus[noteNum] = 1;
    }
  }
  for(int i = 0; i < 16; i++) {
    midiNum++;
    noteNum++;
    int temp = mcp[1].digitalRead(i);
    if(temp == 0 && noteStatus[noteNum] == 1) {
      MIDI.sendNoteOn(midiNum, 64, 1);
      noteStatus[noteNum] = 0;
    }
    if(temp == 1 && noteStatus[noteNum] == 0) {
      MIDI.sendNoteOff(midiNum, 64, 1);
      noteStatus[noteNum] = 1;
    }
  }
  for(int i = 0; i < 16; i++) {
    midiNum++;
    noteNum++;
    int temp = mcp[2].digitalRead(i);
    if(temp == 0 && noteStatus[noteNum] == 1) {
      MIDI.sendNoteOn(midiNum, 64, 1);
      noteStatus[noteNum] = 0;
    }
    if(temp == 1 && noteStatus[noteNum] == 0) {
      MIDI.sendNoteOff(midiNum, 64, 1);
      noteStatus[noteNum] = 1;
    }
  }
  for(int i = 0; i < 13; i++) {
    midiNum++;
    noteNum++;
    int temp = mcp[3].digitalRead(i);
    if(temp == 0 && noteStatus[noteNum] == 1) {
      MIDI.sendNoteOn(midiNum, 64, 1);
      noteStatus[noteNum] = 0;
    }
    if(temp == 1 && noteStatus[noteNum] == 0) {
      MIDI.sendNoteOff(midiNum, 64, 1);
      noteStatus[noteNum] = 1;
    }
  }
  //begin great manual
  midiNum = 35;
  for(int i = 13; i < 16; i++) {
    midiNum++;
    noteNum++;
    int temp = mcp[3].digitalRead(i);
    if(temp == 0 && noteStatus[noteNum] == 1) {
      MIDI.sendNoteOn(midiNum, 64, 2);
      noteStatus[noteNum] = 0;
    }
    if(temp == 1 && noteStatus[noteNum] == 0) {
      MIDI.sendNoteOff(midiNum, 64, 2);
      noteStatus[noteNum] = 1;
    }
  }
  for(int i = 0; i < 16; i++) {
    noteNum++;
    midiNum++;
    int temp = mcp[4].digitalRead(i);
    if(temp == 0 && noteStatus[noteNum] == 1) {
      MIDI.sendNoteOn(midiNum, 64, 2);
      noteStatus[noteNum] = 0;
    }
    if(temp == 1 && noteStatus[noteNum] == 0) {
      MIDI.sendNoteOff(midiNum, 64, 2);
      noteStatus[noteNum] = 1;
    }
  }
  for(int i = 0; i < 16; i++) {
    noteNum++;
    midiNum++;
    int temp = mcp[5].digitalRead(i);
    if(temp == 0 && noteStatus[noteNum] == 1) {
      MIDI.sendNoteOn(midiNum, 64, 2);
      noteStatus[noteNum] = 0;
    }
    if(temp == 1 && noteStatus[noteNum] == 0) {
      MIDI.sendNoteOff(midiNum, 64, 2);
      noteStatus[noteNum] = 1;
    }
  }
  for(int i = 0; i < 16; i++) {
    noteNum++;
    midiNum++;
    int temp = mcp[6].digitalRead(i);
    if(temp == 0 && noteStatus[noteNum] == 1) {
      MIDI.sendNoteOn(midiNum, 64, 2);
      noteStatus[noteNum] = 0;
    }
    if(temp == 1 && noteStatus[noteNum] == 0) {
      MIDI.sendNoteOff(midiNum, 64, 2);
      noteStatus[noteNum] = 1;
    }
  }
  for(int i = 0; i < 10; i++) {
    noteNum++;
    midiNum++;
    int temp = mcp[7].digitalRead(i);
    if(temp == 0 && noteStatus[noteNum] == 1) {
      MIDI.sendNoteOn(midiNum, 64, 2);
      noteStatus[noteNum] = 0;
    }
    if(temp == 1 && noteStatus[noteNum] == 0) {
      MIDI.sendNoteOff(midiNum, 64, 2);
      noteStatus[noteNum] = 1;
    }
  }
  //begin choir, 6 left on mcp[8]
  midiNum = 35;
  for(int i = 10; i < 16; i++) {
    noteNum++;
    midiNum++;
    int temp = mcp[7].digitalRead(i);
    if(temp == 0 && noteStatus[noteNum] == 1) {
      MIDI.sendNoteOn(midiNum, 64, 3);
      noteStatus[noteNum] = 0;
    }
    if(temp == 1 && noteStatus[noteNum] == 0) {
      MIDI.sendNoteOff(midiNum, 64, 3);
      noteStatus[noteNum] = 1;
    }
  }
  for(int i = 0; i < 16; i++) {
    noteNum++;
    midiNum++;
    int temp = mcp[8].digitalRead(i);
    if(temp == 0 && noteStatus[noteNum] == 1) {
      MIDI.sendNoteOn(midiNum, 64, 3);
      noteStatus[noteNum] = 0;
    }
    if(temp == 1 && noteStatus[noteNum] == 0){
      MIDI.sendNoteOff(midiNum, 64, 3);
      noteStatus[noteNum] = 1;
    }
  }
  for(int i = 0; i < 16; i++) {
    noteNum++;
    midiNum++;
    int temp = mcp[9].digitalRead(i);
    if(temp == 0 && noteStatus[noteNum] == 1) {
      MIDI.sendNoteOn(midiNum, 64, 3);
      noteStatus[noteNum] = 0;
    }
    if(temp == 1 && noteStatus[noteNum] == 0) {
      MIDI.sendNoteOff(midiNum, 64, 3);
      noteStatus[noteNum] = 1;
    }
  }
  for(int i = 0; i < 16; i++) {
    noteNum++;
    midiNum++;
    int temp = mcp[10].digitalRead(i);
    if(temp == 0 && noteStatus[noteNum] == 1) {
      MIDI.sendNoteOn(midiNum, 64, 3);
      noteStatus[noteNum] = 0;
    }
    if(temp == 1 && noteStatus[noteNum] == 0) {
      MIDI.sendNoteOff(midiNum, 64, 3);
      noteStatus[noteNum] = 1;
    }
  }
  for(int i = 0; i < 7; i++) {
    noteNum++;
    midiNum++;
    int temp = mcp[11].digitalRead(i);
    if(temp == 0 && noteStatus[noteNum] == 1) {
      MIDI.sendNoteOn(midiNum, 64, 3);
      noteStatus[noteNum] = 0;
    }
    if(temp == 1 && noteStatus[noteNum] == 0) {
      MIDI.sendNoteOff(midiNum, 64, 3);
      noteStatus[noteNum] = 1;
    }
  }
  
}