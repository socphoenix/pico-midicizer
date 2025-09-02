### Organ Visual Displays
In the file organ_hud.ino there is code to add a display that shows/controls transpose and memory levels. There is also code in there to control 10 total LED lights to show the 
choir/expression shoe levels (there are two sets of 5 lights on the Conn 650). I used white LEDS hooked to 1k resistors. There is an alphanumeric display with the backpack from 
Adafruit allowing for up to 4 characters controlled by i2c. the schematic is:
![organ hud_bb](https://github.com/user-attachments/assets/a2c15e7c-f992-4bb6-bf9f-63a95d42b7e1)
And an example of the finished board:
![IMG_3332](https://github.com/user-attachments/assets/de52af88-0158-450b-ab35-1989e2c1ddda)

Example of the project in action:
![LEDS](https://github.com/user-attachments/assets/40538e42-6d7b-4d25-a2e5-88569426c560)
