# CSGOBombTimer
DEMO:https://youtu.be/6le5cuakBh4

A simple python script to start a timer with an arduino when a signal is sent from CSGO
#Instructions
The code is basically split up into 3 sections
- CS config
- Python Listen Server
- Arduino Timer

##CS Config
This is the easiest to set up. Just drop the file `gamestate_integration_consolesample.cfg` into the csgo/cfg/ folder. If you can't run the listen server on port 3000, then you also need to change that in this file, otherwise just ignore it.

##Python Listen server
This was run on python 2.7, it can probably run on python 3.X with some slight modification. If you need to change the listen port, change that here and in CS. You will also need to set the Serial port in here, by default it is COM3, but yours may be different.

## Arduino code & Hardware
This harware uses five 74HC595 shift registers in daisy chain confirguration.
 * 595's don't control seven segment displays directly because 7 seg displays have high current consumption, therefore Q0,Q1..Q3 outputs of 74hc595 use an extra BCD to 7-segment display chip called HEF4543
 * Using HEF4543 only lets me to use numeric combinations on 7-seg displays, therefore I'm limited in using only digits. 
 * Two important features about the HEF4543 are: 
 * first it will display 0 if BCD inputs are all low
 * second it will display blank 7-segments if value >9 is at BCD inputs
 * Arduino also uses ds3231 RTC timer chip for displaying time when CSGO is not launched

