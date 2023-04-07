# pico-midicizer
This guide/code is for creating a midi controller to digitize an organ, with a guide on how it was installed into a Conn 650 theater organ. The code relies
on Circuitpython 8 (tested with 8.0.4), which can be downloaded [here](https://circuitpython.org/board/raspberry_pi_pico/), and uses additional libraries
downloadable [here](https://circuitpython.org/libraries).

## Building the microcontroller:
It is assumed that you have some basic knowledge of these chips, if you are unfamiliar with mcp23017, there is plenty of documentation online explaining
the wiring.

The following parts are needed to build the controller:
> Pi [Pico](https://www.adafruit.com/product/5525)

> 3 full sized [breadboards](https://www.adafruit.com/product/239)

> 2 half sized [breadboards](https://www.adafruit.com/product/64)

> 14 mcp23017 [chips](https://www.adafruit.com/product/732) (these can be found cheaper on ebay)

> between 6-700 feet of wire, I used 20 gauge [doorbell wire](https://www.homedepot.com/p/Cerrowire-500-ft-20-2-Twisted-Copper-Bell-Wire-206-0102J1/202206413)

> buttons for the pedals, I used [7mm tactile buttons](https://www.amazon.com/Momentary-Tactile-Button-Switch-black/dp/B0796Q3PRB/ref=sr_1_3?crid=2EAVE0CUDA4KD&keywords=7mm+tactile+buttons&qid=1680872519&sprefix=7mm+tactile+buttons%2Caps%2C139&sr=8-3)

> 4 2.2k resistors, one attached to sda and scl of the first mcp chip of each i2c lane.

The breadboard layout is pretty simple. the first board should have the pi pico and 3 mcp23017 chips:
![board1](https://user-images.githubusercontent.com/18272432/230613525-190ecc4e-7fe2-4a7b-b822-f753d7ef565a.png)
sda/scl on the first chip should be connected to pins 2, 1 respectively on the pico as shown in the diagram.

The second and third boards each have 4 mcp23017 chips:
![board2](https://user-images.githubusercontent.com/18272432/230613637-fadcc066-0b5f-42d4-a550-cb0bcef9e13d.png)

![board3](https://user-images.githubusercontent.com/18272432/230613670-bf99930f-9f8a-4f1c-820b-421056ad2405.png)

On the third board, chip 2 starts the new i2c line, these need to be connected to pins 5, 4 respectively on the pico. This was not in the diagram. 

The fourth board should be one mcp23017 but itself:

![board4](https://user-images.githubusercontent.com/18272432/230613765-7d5fe86d-3275-4988-b678-c53c7d2df84c.png)

The final 5th board has 2 mcp23017 chips, and was placed behind the expression shoes to lessen the wire lengths running the length of the organ:

![board5](https://user-images.githubusercontent.com/18272432/230613886-0c882cb8-a3ca-4e55-a9a4-8e850bdf2384.png)

The boards will need power/ground as well as i2c lines run between them as shown below:
![allboards](https://user-images.githubusercontent.com/18272432/230613981-a6c50181-7e46-45b3-aa45-ea47b7d842c3.png)


### installing circuitpython:
Hold the bootsel button on the pico and plug it into your computer. drag the circuitpython.uf2 file to the removable drive that shows up. It will
disappear, then reappear once the install is done. There should be a folder marked library. From the .zip bundle of libraries extract the files
> adafruit_mcp230xx.mpy

> adafruit_bus_device

to this folder. Using an editor like [Mu](https://codewith.mu) that will write files completely on saving, copy the code.py file from this repository
to the root of the drive as code.py

At this point you can move to wiring the organ

### Wiring of the Conn 650
**Note there are other ways to do this. Other guides available online show to solder wires to the underside of each keyboard. This could be done by
soldering a ground wire to one of the contact rails, and positive wires to the back of one of the thin metal wires that will contact the grounded rail
when a key is depressed. The route shown in this guide was picked in order to minimize the amount of time needed to do this install, it is not necessarily
the best way to do this by any stretch of the imagination.**

It is likely best to start with the bottom manual and work your way up. There is space to stick the breadboards down behind the top manual.
![23-04-03 09-04-02 1389](https://user-images.githubusercontent.com/18272432/230615686-6dd6e459-9fff-43b1-90ab-0590b181dee3.jpg)

This project will also use the buttons on each keyboard for presets/cancel and set, and mem+/mem- to change memory levels.
Lift up all three keyboards so you can see underneath the keyboard. On the bottom manual, trace the wire bundle that runs off to the right up to the top
of the console and underneath the breadboards. Note the wiring color coming from each of the buttons, I've kept notes on color but can't guarantee that
all 650s have the same color coding. On our organ the colors were as follows running from 0-5:
> brown: 11

> red: 12

> orange: 14

> yellow: 15

> green: 16

> blue: 17

> black: ground on breadboard

Connect these to the pins (actuall pin #s listed see [here](https://land-boards.com/blwiki/images/thumb/5/56/Raspberry-Pi-Pico-Pinout.jpg/1095px-Raspberry-Pi-Pico-Pinout.jpg) for the picos pinout.

Then place the manual down and remove all the keys by removing the springs from the back, pressing down on the key and pulling it out.
![23-04-03 17-42-46 1404](https://user-images.githubusercontent.com/18272432/230616703-a969ebe7-0441-43f2-8bf0-d8c726575d84.jpg)

With the keys removed, look on the bottom of one of the keys and note the rearmost bent metal piece protruding down. You will need this to mark where the
wire should be placed to make contact to complete the circuit. place the wood down with some glue (we used super glue), and then mark where the wires 
should go.
![23-04-03 12-52-02 1397](https://user-images.githubusercontent.com/18272432/230617040-b1ade34b-af58-471c-bc56-97debba4ecc0.jpg)
Using glue, glue down the wire with a longer stripped end to the board where marked, then run it up to the mcp23017 pins. starting with board four if 
starting with the lowest manual, look at the top of the chip and go eight pins to the left of the end of the chip (start at the top key of the manual). Then work rightwards to the end of the chip, and then work right to left doing the same for the 8 pins on the bottom. Continue in this pattern working towards the pi pico.
![23-04-03 13-18-52 1393](https://user-images.githubusercontent.com/18272432/230617370-325678fb-7811-4272-9427-0b3c63895a67.jpg)

Once completed take one of the springs and tie some stripped wire to the end, then use some heat shrink tube to secure it down. Connect this wire to ground
on one of the breadboards.
![23-04-03 11-52-10 1391](https://user-images.githubusercontent.com/18272432/230617499-62bd83ef-1114-4fb6-b424-3bb9acb18edb.jpg)
![23-04-03 11-53-26 1392](https://user-images.githubusercontent.com/18272432/230617507-9c52dce8-280e-4a48-9eeb-201360af74cb.jpg)

Then reattach the keys and springs. Depress each key and make sure it contacts the exposed wire. If it doesn't reach use a pair of needle nose pliers to 
push the spring/depressed piece of metal down a bit and recheck.

Repeat for the second manual. To the right of the presets (0-5 buttons), there is a group of three buttons. We used the first two as memory - and +.
Colors/pins for these on our console were:
> Violet: 9

> Grey: 10

Repeat the keys as above.

The third manual we used C and S buttons for cancel/set. we cut the wires underneath and spliced as it was much harder to get the wires traced. Pins for
these are:
> Cancel: 6

> set: 7

> black wire from each: to ground on breadboard.

Repeat steps as shown for the bottom manual. At this point, run power/ground and SDA/SCL down to the pedalboard. We soldered and heat shrink tubed the
buttons for the pedals, and then glued them under the pedal contact on the rear of the organ as shown below:
![23-04-04 22-18-17 1406](https://user-images.githubusercontent.com/18272432/230618269-2c07fa46-bcca-4722-ad59-c7ca86e4e2ba.jpg)
These are wired up in the reverse order of the manuals. Working left to right, insert one wire from each button to the bottom eight, then right to left
the top eight of each mcp chip. The other wire goes to ground on the breadboard.

Finally the expression pedals. Remove these from the organ and cut out the wire used. Connect (solder) onto these a positive, output, and ground. 
On our pedals these were as follows:
> ground: second from the top pin

> output: third from the top pin

> positive: fourth from the top pin
![IMG_1413](https://user-images.githubusercontent.com/18272432/230619008-1de65011-f7b0-41ce-b4ac-78bccb005906.jpg)

Do this to one of the potentiometers on each pedal, and run the output wire from each back up to the pico. One of these will go in pin 31, the other in pin
32 (the one in pin 31 will be midi controller 1, the other 2 both on channel 16 when setting this up in vpo software).

### Software Setup
I used [GrandOrgue](https://github.com/GrandOrgue/grandorgue) though this should be similar to Hauptwerk for setup. For a sample set [Freisach](https://piotrgrabowski.pl/friesach/) was used.

> swell manual: midi channel 1 9x note velocity 1-127, notes 36-127

> great manual: midi channel 2 9x note velocity 1-127, notes 36-127

> choir manual: midi channel 3 9x note velocity 1-127, notes 36-127

> pedals: midi channel 4 9x note velocity 1-127, notes 36-127

> cancel: midi channel 16, 9xnote value: 60

> set: midi channel 16, 9x note value: 61

> memory minus: midi channel 16 9x note value: 62

> memory plus: midi channel 16 9x note value: 63

> presets (all 12 in ascending order 0-5, 0-5): midi channel 16 9x note value: 64-76

> expression shoes: midi channel 16 bx controller, controller 1/2, range 127-1

This should be all you need to start playing!
