# Copyright (c) 2023 Michael Bell
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.


import board
import digitalio
from digitalio import DigitalInOut, Direction, Pull
from adafruit_mcp230xx.mcp23017 import MCP23017
import usb_midi
import adafruit_midi
from adafruit_midi.control_change import ControlChange
from adafruit_midi.note_on import NoteOn
from adafruit_midi.note_off import NoteOff
import gc
import busio
from analogio import AnalogIn

#set up expression pedals
swellShoe = AnalogIn(board.GP26)
swellValue = swellShoe.value // 512
choirShoe = AnalogIn(board.GP27)
choirValue = choirShoe.value // 512

usb_midi = adafruit_midi.MIDI(midi_out=usb_midi.ports[1], out_channel=10)
#set up lists
presets = [0,0,0,0,0,0,0,0,0,0,0,0]
noteStatus = []
notePins = []
midiNum = 35
mcpAddresses = []
mcpCodes = [0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27]

#board.what? Should be pin number from what I can see,
#run dir(board) from serial to check for button numbers.

#i2c codes:
#using format i2c = busio.I2C(board.SCL, board.SDA)
i2c0 = busio.I2C(board.GP1, board.GP0)
i2c1 = busio.I2C(board.GP3, board.GP2)

#cancel button
cncl = DigitalInOut(board.GP4)
cncl.direction = Direction.INPUT
cncl.pull = Pull.UP

#Set button
st = DigitalInOut(board.GP5)
st.direction = Direction.INPUT
st.pull = Pull.UP

#mem-
memMinus = DigitalInOut(board.GP6)
memMinus.direction = Direction.INPUT
memMinus.pull = Pull.UP

#mem+
memPlus = DigitalInOut(board.GP7)
memPlus.direction = Direction.INPUT
memPlus.pull = Pull.UP



#for loop for presets 0-11:
pinsPresets = [board.GP8,board.GP9,board.GP10,board.GP11,board.GP12,board.GP13,board.GP14,board.GP18,board.GP19,board.GP20,board.GP21,board.GP22]

for x in range(12):
	presets[x] = DigitalInOut(pinsPresets[x])
	presets[x].direction = Direction.INPUT
	presets[x].pull = Pull.UP
#free memory by removing pinsPresets
del pinsPresets

#pin setup for mcp23017’s looks like we need 14 for this.
a = 0
z = 0
mcp = MCP23017(i2c0, address = mcpCodes[a])
for b in range(16):
	mcpAddresses.append(mcp.get_pin(b))
	mcpAddresses[z].direction = digitalio.Direction.INPUT
	mcpAddresses[z].pull = digitalio.Pull.UP
	z = z + 1

a = a + 1
mcp = MCP23017(i2c0, address = mcpCodes[a])
for b in range(16):
	mcpAddresses.append(mcp.get_pin(b))
	mcpAddresses[z].direction = digitalio.Direction.INPUT
	mcpAddresses[z].pull = digitalio.Pull.UP
	z = z + 1

a = a + 1
mcp = MCP23017(i2c0, address = mcpCodes[a])
for b in range(16):
	mcpAddresses.append(mcp.get_pin(b))
	mcpAddresses[z].direction = digitalio.Direction.INPUT
	mcpAddresses[z].pull = digitalio.Pull.UP
	z = z + 1

a = a + 1
mcp = MCP23017(i2c0, address = mcpCodes[a])
for b in range(16):
	mcpAddresses.append(mcp.get_pin(b))
	mcpAddresses[z].direction = digitalio.Direction.INPUT
	mcpAddresses[z].pull = digitalio.Pull.UP
	z = z + 1

a = a + 1
mcp = MCP23017(i2c0, address = mcpCodes[a])

for b in range(16):
	mcpAddresses.append(mcp.get_pin(b))
	mcpAddresses[z].direction = digitalio.Direction.INPUT
	mcpAddresses[z].pull = digitalio.Pull.UP
	z = z + 1

a = a + 1
mcp = MCP23017(i2c0, address = mcpCodes[a])

for b in range(16):
	mcpAddresses.append(mcp.get_pin(b))
	mcpAddresses[z].direction = digitalio.Direction.INPUT
	mcpAddresses[z].pull = digitalio.Pull.UP
	z = z + 1

a = a + 1
mcp = MCP23017(i2c0, address = mcpCodes[a])

for b in range(16):
	mcpAddresses.append(mcp.get_pin(b))
	mcpAddresses[z].direction = digitalio.Direction.INPUT
	mcpAddresses[z].pull = digitalio.Pull.UP
	z = z + 1

a = a + 1
mcp = MCP23017(i2c0, address = mcpCodes[a])
for b in range(16):
	mcpAddresses.append(mcp.get_pin(b))
	mcpAddresses[z].direction = digitalio.Direction.INPUT
	mcpAddresses[z].pull = digitalio.Pull.UP
	z = z + 1

#begin second i2c bank
a = 0
mcp = MCP23017(i2c1, address = mcpCodes[a])

for b in range(16):
	mcpAddresses.append(mcp.get_pin(b))
	mcpAddresses[z].direction = digitalio.Direction.INPUT
	mcpAddresses[z].pull = digitalio.Pull.UP
	z = z + 1

a = a + 1
mcp = MCP23017(i2c1, address = mcpCodes[a])

for b in range(16):
	mcpAddresses.append(mcp.get_pin(b))
	mcpAddresses[z].direction = digitalio.Direction.INPUT
	mcpAddresses[z].pull = digitalio.Pull.UP
	z = z + 1

a = a + 1
mcp = MCP23017(i2c1, address = mcpCodes[a])

for b in range(16):
	mcpAddresses.append(mcp.get_pin(b))
	mcpAddresses[z].direction = digitalio.Direction.INPUT
	mcpAddresses[z].pull = digitalio.Pull.UP
	z = z + 1

a = a + 1
mcp = MCP23017(i2c1, address = mcpCodes[a])

#for chip 12 we need 7 inputs, the rest are unused.
for b in range(7):
	mcpAddresses.append(mcp.get_pin(b))
	mcpAddresses[z].direction = digitalio.Direction.INPUT
	mcpAddresses[z].pull = digitalio.Pull.UP
	z = z + 1

a = a + 1
mcp = MCP23017(i2c1, address = mcpCodes[a])

for b in range(16):
	mcpAddresses.append(mcp.get_pin(b))
	mcpAddresses[z].direction = digitalio.Direction.INPUT
	mcpAddresses[z].pull = digitalio.Pull.UP
	z = z + 1
a = a + 1
mcp = MCP23017(i2c1, address = mcpCodes[a])

for b in range(16):
	mcpAddresses.append(mcp.get_pin(b))
	mcpAddresses[z].direction = digitalio.Direction.INPUT
	mcpAddresses[z].pull = digitalio.Pull.UP
	z = z + 1

#free memory deleting unneeded variable for hex addresses
del mcpCodes

#set initial values for noteStatus using pin
for x in range(183):
	noteStatus.append(1)
	#st.value if needed? 1 should be correct
	#noteStatus.append(st.value)
for x in range(183, 215):
	noteStatus.append(0)

#main program loop
while True:
	#garbage collecting
	gc.collect()
	#reset noteNum at start of loop
	noteNum = 1
	midiNum = 36
	#set midi channel to 16 for cancel/set/memlevel
	USB_MIDI_channel = 16
	usb_midi.out_channel = USB_MIDI_channel - 1

	#expression pedals
	if swellShoe.value // 512 != swellValue and swellShoe.value >= swellValue+3 or swellShoe.value <= swellValue-3:
		swellValue = swellShoe.value // 512
		usb_midi.send(ControlChange(1, swellValue))
	if choirShoe.value // 512 != choirValue and choirShoe.value >=swellValue+3 or swellShoe.value <= swellValue-3:
		choirValue = choirShoe.value // 512
		usb_midi.send(ControlChange(2, choirValue))

#cancel then set then mem minus then memplus buttons.

	if cncl.value == 0:
		usb_midi.send(NoteOn(60, 64))
	if st.value == 0:
		usb_midi.send(NoteOn(61, 64))
	if memMinus.value == 0:
		usb_midi.send(NoteOn(62, 64))
	if memPlus.value == 0:
		usb_midi.send(NoteOn(63, 64))
	#check presets and increment z to send unique midi signal for each preset button
	z = 64
	for x in range(len(presets)):
		if presets[x].value == 0:
			usb_midi.send(NoteOn(z, 64))
		z = z + 1

#begin looping through keyboards going swell great choir pedals:
	USB_MIDI_channel = 1
	midiNum = 36
	usb_midi.out_channel = USB_MIDI_channel - 1
	for x in range(61):
		y = mcpAddresses[x].value
		if y == 0 and y != noteStatus[x]:
			noteStatus[x] = y
			usb_midi.send(NoteOn(midiNum, 64))
		elif y == 1 and y != noteStatus[x]:
			noteStatus[x] = y
			usb_midi.send(NoteOff(midiNum, 64))
		midiNum = midiNum + 1
	#begin great
	USB_MIDI_channel = 2
	midiNum = 36
	usb_midi.out_channel = USB_MIDI_channel - 1
	for x in range(61, 122):
		y = mcpAddresses[x].value
		if y == 0 and y != noteStatus[x]:
			noteStatus[x] = y
			usb_midi.send(NoteOn(midiNum, 64))
		elif y == 1 and y != noteStatus[x]:
			noteStatus[x] = y
			usb_midi.send(NoteOff(midiNum, 64))
		midiNum = midiNum + 1
	#begin choir
	USB_MIDI_channel = 3
	midiNum = 36
	usb_midi.out_channel = USB_MIDI_channel - 1
	for x in range(122, 183):
		y = mcpAddresses[x].value
		if y == 0 and y != noteStatus[x]:
			noteStatus[x] = y
			usb_midi.send(NoteOn(midiNum, 64))
		elif y == 1 and y != noteStatus[x]:
			noteStatus[x] = y
			usb_midi.send(NoteOff(midiNum, 64))
		midiNum = midiNum + 1
	#begin pedals
	USB_MIDI_channel = 4
	midiNum = 36
	usb_midi.out_channel = USB_MIDI_channel - 1
	for x in range(183, 215):
		y = mcpAddresses[x].value
		if y == 1 and y != noteStatus[x]:
			noteStatus[x] = y
			usb_midi.send(NoteOn(midiNum, 64))
		elif y == 0 and y != noteStatus[x]:
			noteStatus[x] = y
			usb_midi.send(NoteOff(midiNum, 64))
		midiNum = midiNum + 1
