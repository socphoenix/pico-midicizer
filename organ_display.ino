// Copyright (c) <2025> <Michael Bell>

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <Arduino.h>
#include <MIDI.h>
#include <Adafruit_TinyUSB.h>
#include <Adafruit_MCP23X17.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();


Adafruit_USBD_MIDI usbdMidi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usbdMidi, MIDI);
int lastState[] = {1,1,1,1};
int transpose = 0;
char memoryLetter = 'A';

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  //pins 6-10 choir, 11-15 swell expression shoes.
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  for(int i = 6; i < 16; i++) {
    digitalWrite(i, LOW);
  }
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOff();
  Wire.setSDA(0);
  Wire.setSCL(1);
  alpha4.begin(0x70);
  // while (!Serial) {
  //   digitalWrite(LED_BUILTIN, HIGH);
  //   delay(1000);
  //   digitalWrite(LED_BUILTIN, LOW);
  //   delay(1000);
  // }
  alpha4.writeDigitAscii(0, 'M');
  alpha4.writeDigitAscii(1, 'L');
  alpha4.writeDigitAscii(3, 'A');
  alpha4.writeDisplay();
}

void loop() {
  //MIDI.sendContinue();
  if(MIDI.read()) {
    byte* arr = (byte*)MIDI.getSysExArray();
    // for(int i = 0; i < 16; i++) {
    //   Serial.println(char(arr[i]));
    // }
    if(arr[3] == 1) {
      Serial.println(char(arr[5]));
      alpha4.writeDigitAscii(0, 'M');
      alpha4.writeDigitAscii(1, 'L');
      alpha4.writeDigitRaw(2, 0x0);
      alpha4.writeDigitAscii(3, char(arr[5]));
      alpha4.writeDisplay();
    }
    else if(arr[3] == 2 && char(arr[5]) != '-') {
      Serial.println(char(arr[5]));
      alpha4.writeDigitAscii(0, 'T');
      alpha4.writeDigitAscii(1, 'R');
      alpha4.writeDigitRaw(2, 0x0);
      alpha4.writeDigitAscii(3, char(arr[5]));
      alpha4.writeDisplay();
    }
    else if(arr[3] == 2 && char(arr[5]) == '-') {
      alpha4.writeDigitAscii(0, 'T');
      alpha4.writeDigitAscii(1, 'R');
      alpha4.writeDigitAscii(2, char(arr[5]));
      alpha4.writeDigitAscii(3, char(arr[6]));
      alpha4.writeDisplay();
    }
    //choir lights
    else if(arr[3] == 3) {
      String percent = "";
      percent += char(arr[5]);
      for(int i = 6; i < 8; i++) {
        if(arr[i] != 32 && arr[i] != 46) {
          percent += char(arr[i]);
        }
        else if(arr[i] == 32 || arr[i] == 46) {
          i = 10;
        }
      }
      int numPercent = percent.toInt();
      if(numPercent < 20) {
        digitalWrite(6, LOW);
        digitalWrite(7, LOW);
        digitalWrite(8, LOW);
        digitalWrite(9, LOW);
        digitalWrite(10, LOW);
      }
      else if(numPercent < 40 && numPercent > 19) {
        digitalWrite(6, HIGH);
        digitalWrite(7, LOW);
        digitalWrite(8, LOW);
        digitalWrite(9, LOW);
        digitalWrite(10, LOW);
      }
      else if(numPercent < 60 && numPercent > 39) {
        digitalWrite(6, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        digitalWrite(9, LOW);
        digitalWrite(10, LOW);
      }
      else if(numPercent < 80 && numPercent > 59) {
        digitalWrite(6, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(8, HIGH);
        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
      }
      else if(numPercent > 80) {
        digitalWrite(6, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(8, HIGH);
        digitalWrite(9, HIGH);
        digitalWrite(10, HIGH);
      }
    }
    //swell lights
    else if(arr[3] == 4) {
      String percent = "";
      percent += char(arr[5]);
      for(int i = 6; i < 8; i++) {
        if(arr[i] != 32 && arr[i] != 46) {
          percent += char(arr[i]);
        }
        else if(arr[i] == 32 || arr[i] == 46) {
          i = 10;
        }
      }
      int numPercent = percent.toInt();
      if(numPercent < 20) {
        digitalWrite(11, LOW);
        digitalWrite(12, LOW);
        digitalWrite(13, LOW);
        digitalWrite(14, LOW);
        digitalWrite(15, LOW);
      }
      else if(numPercent < 40 && numPercent > 19) {
        digitalWrite(11, HIGH);
        digitalWrite(12, LOW);
        digitalWrite(13, LOW);
        digitalWrite(14, LOW);
        digitalWrite(15, LOW);
      }
      else if(numPercent < 60 && numPercent > 39) {
        digitalWrite(11, HIGH);
        digitalWrite(12, HIGH);
        digitalWrite(13, LOW);
        digitalWrite(14, LOW);
        digitalWrite(15, LOW);
      }
      else if(numPercent < 80 && numPercent > 59) {
        digitalWrite(11, HIGH);
        digitalWrite(12, HIGH);
        digitalWrite(13, HIGH);
        digitalWrite(14, HIGH);
        digitalWrite(15, LOW);
      }
      else if(numPercent > 80) {
        digitalWrite(11, HIGH);
        digitalWrite(12, HIGH);
        digitalWrite(13, HIGH);
        digitalWrite(14, HIGH);
        digitalWrite(15, HIGH);
      }
    }
    else {
      //do nothing as God intended
    }
    //arr[3] is ID number, can be used to differentiate what is coming in!
    //use ID 1 for memory level, 2 for transpose, 3 for choir, 4 for swell percent.
    //if arr[5] == '-' transpose is in the negative, need to use all led screens.
    //on box open %, there is numbers starting at arr[5]. terminates with a space character ' ' then the percentage sign, read until space.
  }
  //GP2 for tr -, GP3 for tr +, GP4 for ml -, GP5 for ml +
  int temp = digitalRead(2);
  if(temp == 0 && lastState[0] == 1) {
    lastState[0] = 0;
    MIDI.sendNoteOn(1, 64, 15);
    delay(500);
  }
  if(temp == 1 && lastState[0] == 0) {
    lastState[0] = 1;
  }
  temp = digitalRead(3);
  if(temp == 0 && lastState[1] == 1) {
    lastState[1] = 0;
    MIDI.sendNoteOn(2, 64, 15);
    delay(500);
  }
  if(temp == 1 && lastState[1] == 0) {
    lastState[1] = 1;
  }
  temp = digitalRead(4);
  if(temp == 0 && lastState[2] == 1) {
    lastState[2] = 0;
    MIDI.sendNoteOn(3, 64, 15);
    delay(500);
  }
  if(temp == 1 && lastState[2] == 0) {
    lastState[2] = 1;
  }
  temp = digitalRead(5);
  if(temp == 0 && lastState[3] == 1) {
    lastState[3] = 0;
    MIDI.sendNoteOn(4, 64, 15);
    delay(500);
  }
  if(temp == 1 && lastState[3] == 0) {
    lastState[3] = 1;
  }
}

