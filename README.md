# HCSR04_herbk
Rework of NewPing v1.8 because of licence issues of NewPing above v1.8

/*
 HCSR04_herbk library is forked from NewPing v1.8, because NewPing above v1.8 is licenced with limitations to change/extend the code.
Autor of HCSR04_herbk library is Herbert Kozuschnik
License of HCSR04_herbk library: GNU GPL v3 http://www.gnu.org/licenses/gpl.html

Disclaimer of Warranty.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

  
AUTHOR/LICENSE of NewPing Library - v1.8 - 07/30/2016 where is HCSR04_herbk library based on:
Created by Tim Eckel - teckel@leethost.com
Copyright 2016 License: GNU GPL v3 http://www.gnu.org/licenses/gpl.html
For full NewPing history go to  https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home



 FEATURES:
 * Works with many different ultrasonic sensors: SR04, SRF05, SRF06, DYP-ME007, URM37 & Parallax PING))).
 * Compatible with the entire Arduino line-up (and clones), Teensy family (including $19 96Mhz 32 bit Teensy 3.2) and non-AVR microcontrollers.
 * Interface with all but the SRF06 sensor using only one Arduino pin.
 * Doesn't lag for a full second if no ping/echo is received.
 * Ping sensors consistently and reliably at up to 30 times per second.
 * Built-in digital filter method ping_median() for easy error correction.
 * Uses port registers for a faster pin interface and smaller code size.
 * Allows you to set a maximum distance where pings beyond that distance are read as no ping "clear".
 * Ease of using multiple sensors (example sketch with 4 sensors).


 HISTORY:
 2023-10-15
 Changing output from 0 to MAX_DISTANCE if no obstacle is at range.
 Changing unsigned int to uint16_t, unsigned long to uint32_t


2023-10-14 forked from NewPing v1.8
https://bitbucket.org/teckel12/arduino-new-ping/downloads/

