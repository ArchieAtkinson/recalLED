# recalLED

recalLED is a hand held PCB Simon Says Game. The PCB has four different coloured LEDs and four buttons. Each round a sequence of lights will flash. Once the sequence is over you must use the buttons to input sequence back. If you get it right the green LED flashes and you go to the next level which increase the number of flashes in a sequences. If you lose the red LED will flash and you will go back to the beginning. The game remembers your score and flashes the number of levels you have won when you turn it on. For more information, see the article I wrote about its creation on [my website](https://www.archieatkinson.com/recalled).

![Gif of the game in action](https://www.archieatkinson.com/assets/recalled/demo.gif)

This repo contains the Eagle PCB and Schematic Files, Soildworks files for a case and the firmware. The firmware is written using the Arduino IDE and can be uploaded to the MCU using an Arduino as a programmer. It runs off a single CR2032 battery.
