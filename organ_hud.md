### Organ Visual Displays
In the file organ_hud.ino there is code to add a display that shows/controls transpose and memory levels. There is also code in there to control 10 total LED lights to show the 
choir/expression shoe levels (there are two sets of 5 lights on the Conn 650). I used white LEDS hooked to 1k resistors. There is an alphanumeric display with the backpack from 
Adafruit allowing for up to 4 characters controlled by i2c. the schematic is:
![organ hud_bb](https://github.com/user-attachments/assets/f8593dc1-aa60-45a9-ab7e-2aa5ae710dcf)

And an example of the finished board:
![IMG_3332](https://github.com/user-attachments/assets/de52af88-0158-450b-ab35-1989e2c1ddda)

Example of the LED lights backside (that goes into the light slots in the lid):
![IMG_3328](https://github.com/user-attachments/assets/353c611b-6a6c-427c-8ea6-7cafa41727fe)
Example of the LED boards (reused from original incandescants board:
![IMG_3329](https://github.com/user-attachments/assets/d44e98e5-64c6-4681-9cc3-bf5861b5dd1b)

The light boards are very simple to look at, there is a negative trace that runs down the center of the board. put the shorter of the LED legs through that one, and the longer one on the other hole the original light was on. This 2nd hole connects to a solder point for a wire that will connect to the resistors.


Example of the project in action:
![LEDS](https://github.com/user-attachments/assets/40538e42-6d7b-4d25-a2e5-88569426c560)

## building notes

pinout:
1. sda
2. scl
4. transpose -
5. transpose +
6. memory level -
7. memory level +
9. Choir light 1 (don't forget the resistor! I worked left to right on these)
10. Choir light 2 (don't forget the resistor! I worked left to right on these)
11. Choir light 3 (don't forget the resistor! I worked left to right on these)
12. Choir light 4 (don't forget the resistor! I worked left to right on these)
14. Choir light 5 (don't forget the resistor! I worked left to right on these)
15. Swell light 1 (don't forget the resistor! I worked left to right on these)
16. Swell light 2 (don't forget the resistor! I worked left to right on these)
17. Swell light 3 (don't forget the resistor! I worked left to right on these)
19. Swell light 4 (don't forget the resistor! I worked left to right on these)
20. Swell light 5 (don't forget the resistor! I worked left to right on these) 
