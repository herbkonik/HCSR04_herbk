/*
 HCSR04_herbk library is forked from NewPing v1.8, because NewPing above v1.8 is licenced with limitationsto change/extend the code.
Autor of HCSR04_herbk library is Herbert Kozuschnik hkozuschnik2@leonardod2.de  (remove all 2 if you want to get in contact)
License of HCSR04_herbk library: GNU GPL v3 http://www.gnu.org/licenses/gpl.html

This software is furnished "as is", without technical support, and with no warranty, express or implied, as to its usefulness for any purpose.

 AUTHOR/LICENSE of NewPing Library - v1.8 - 07/30/2016 where is HCSR04_herbk library based on:
 Created by Tim Eckel - teckel@leethost.com
 Copyright 2016 License: GNU GPL v3 http://www.gnu.org/licenses/gpl.html
For full NewPing history go to  https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home

*/

/*
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


 CONSTRUCTOR:
   HCSR04_herbk sonar(trigger_pin, echo_pin [, max_cm_distance])
     trigger_pin & echo_pin - Arduino pins connected to sensor trigger and echo.
     To use the same Arduino pin for trigger and echo, specify the same pin for both values.
    max_cm_distance - [Optional] Maximum distance you wish to sense. Default=500cm.

 METHODS:
   sonar.ping([max_cm_distance]) - Send a ping and get the echo time (in microseconds) as a result. [max_cm_distance] allows you to optionally set a new max distance.
   sonar.ping_in([max_cm_distance]) - Send a ping and get the distance in whole inches. [max_cm_distance] allows you to optionally set a new max distance.
   sonar.ping_cm([max_cm_distance]) - Send a ping and get the distance in whole centimeters. [max_cm_distance] allows you to optionally set a new max distance.
   sonar.ping_median(iterations [, max_cm_distance]) - Do multiple pings (default=5), discard out of range pings and return median in microseconds. [max_cm_distance] allows you to optionally set a new max distance.
   HCSR04_herbk::convert_in(echoTime) - Convert echoTime from microseconds to inches (rounds to nearest inch).
   HCSR04_herbk::convert_cm(echoTime) - Convert echoTime from microseconds to centimeters (rounds to nearest cm).
  sonar.ping_timer(function [, max_cm_distance]) - Send a ping and call function to test if ping is complete. [max_cm_distance] allows you to optionally set a new max distance.
   sonar.check_timer() - Check if ping has returned within the set distance limit.
  HCSR04_herbk::timer_us(frequency, function) - Call function every frequency microseconds.
   HCSR04_herbk::timer_ms(frequency, function) - Call function every frequency milliseconds.
   HCSR04_herbk::timer_stop() - Stop the timer.

 HISTORY:
 2023-10-15
 Changing output from 0 to MAX_DISTANCE if no obstacle is at range.
 Changing unsigned int to uint16_t, unsigned long to uint32_t


2023-10-14 forked from NewPing v1.8
https://bitbucket.org/teckel12/arduino-new-ping/downloads/

*/


#ifndef HCSR04_herbk_h
#define HCSR04_herbk_h
#include <Arduino.h>

#if defined (__AVR__)
	#include <avr/io.h>
	#include <avr/interrupt.h>
#endif

// Shouldn't need to change these values unless you have a specific need to do so.
#define MAX_SENSOR_DISTANCE 500 // Maximum sensor distance can be as high as 500cm, no reason to wait for ping longer than sound takes to travel this distance and back. Default=500
#define US_ROUNDTRIP_CM 57      // Microseconds (uS) it takes sound to travel round-trip 1cm (2cm total), uses integer to save compiled code space. Default=57
#define US_ROUNDTRIP_IN 146     // Microseconds (uS) it takes sound to travel round-trip 1 inch (2 inches total), uses integer to save compiled code space. Defalult=146
#define ONE_PIN_ENABLED true    // Set to "false" to disable one pin mode which saves around 14-26 bytes of binary size. Default=true
#define ROUNDING_ENABLED false  // Set to "true" to enable distance rounding which also adds 64 bytes to binary size. Default=false
#define URM37_ENABLED false     // Set to "true" to enable support for the URM37 sensor in PWM mode. Default=false
#define TIMER_ENABLED true      // Set to "false" to disable the timer ISR (if getting "__vector_7" compile errors set this to false). Default=true

// Probably shouldn't change these values unless you really know what you're doing.
#define NO_ECHO _maxEchoTime     // Value returned if there's no ping echo within the specified MAX_SENSOR_DISTANCE or max_cm_distance. Default=maxEchoTime
#define MAX_SENSOR_DELAY 5800   // Maximum uS it takes for sensor to start the ping. Default=5800
#define ECHO_TIMER_FREQ 24      // Frequency to check for a ping echo (every 24uS is about 0.4cm accuracy). Default=24
#define PING_MEDIAN_DELAY 29000 // Microsecond delay between pings in the ping_median method. Default=29000
#define PING_OVERHEAD 5         // Ping overhead in microseconds (uS). Default=5
#define PING_TIMER_OVERHEAD 13  // Ping timer overhead in microseconds (uS). Default=13
#if URM37_ENABLED == true
	#undef  US_ROUNDTRIP_CM
	#undef  US_ROUNDTRIP_IN
	#define US_ROUNDTRIP_CM 50  // Every 50uS PWM signal is low indicates 1cm distance. Default=50
	#define US_ROUNDTRIP_IN 127 // If 50uS is 1cm, 1 inch would be 127uS (50 x 2.54 = 127). Default=127
#endif

// Conversion from uS to distance (round result to nearest cm or inch).
#define HCSR04_herbkConvert(echoTime, conversionFactor) (max(((uint16_t)echoTime + conversionFactor / 2) / conversionFactor, (echoTime ? 1 : 0)))

// Detect non-AVR microcontrollers (Teensy 3.x, Arduino DUE, etc.) and don't use port registers or timer interrupts as required.
#if (defined (__arm__) && defined (TEENSYDUINO))
	#undef  PING_OVERHEAD
	#define PING_OVERHEAD 1
	#undef  PING_TIMER_OVERHEAD
	#define PING_TIMER_OVERHEAD 1
	#define DO_BITWISE true
#elif !defined (__AVR__)
	#undef  PING_OVERHEAD
	#define PING_OVERHEAD 1
	#undef  PING_TIMER_OVERHEAD
	#define PING_TIMER_OVERHEAD 1
	#undef  TIMER_ENABLED
	#define TIMER_ENABLED false
	#define DO_BITWISE false
#else
	#define DO_BITWISE true
#endif

// Disable the timer interrupts when using ATmega128 and all ATtiny microcontrollers.
#if defined (__AVR_ATmega128__) || defined (__AVR_ATtiny24__) || defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__) || defined (__AVR_ATtiny25__) || defined (__AVR_ATtiny45__) || defined (__AVR_ATtiny85__) || defined (__AVR_ATtiny261__) || defined (__AVR_ATtiny461__) || defined (__AVR_ATtiny861__) || defined (__AVR_ATtiny43U__)
	#undef  TIMER_ENABLED
	#define TIMER_ENABLED false
#endif

// Define timers when using ATmega8, ATmega16, ATmega32 and ATmega8535 microcontrollers.
#if defined (__AVR_ATmega8__) || defined (__AVR_ATmega16__) || defined (__AVR_ATmega32__) || defined (__AVR_ATmega8535__)
	#define OCR2A OCR2
	#define TIMSK2 TIMSK
	#define OCIE2A OCIE2
#endif

class HCSR04_herbk {
	public:
		HCSR04_herbk(uint8_t trigger_pin, uint8_t echo_pin, uint16_t max_cm_distance = MAX_SENSOR_DISTANCE);
		uint16_t ping(uint16_t max_cm_distance = 0);
		uint32_t ping_cm(uint16_t max_cm_distance = 0);
		uint32_t ping_in(uint16_t max_cm_distance = 0);
		uint32_t ping_median(uint8_t it = 5, uint16_t max_cm_distance = 0);
		static uint16_t convert_cm(uint16_t echoTime);
		static uint16_t convert_in(uint16_t echoTime);
#if TIMER_ENABLED == true
		void ping_timer(void (*userFunc)(void), uint16_t max_cm_distance = 0);
		boolean check_timer();
		uint32_t ping_result;
		static void timer_us(uint16_t frequency, void (*userFunc)(void));
		static void timer_ms(uint32_t frequency, void (*userFunc)(void));
		static void timer_stop();
#endif
	private:
		boolean ping_trigger();
		void set_max_distance(uint16_t max_cm_distance);
#if TIMER_ENABLED == true
		boolean ping_trigger_timer(uint16_t trigger_delay);
		boolean ping_wait_timer();
		static void timer_setup();
		static void timer_ms_cntdwn();
#endif
#if DO_BITWISE == true
		uint8_t _triggerBit;
		uint8_t _echoBit;
		volatile uint8_t *_triggerOutput;
		volatile uint8_t *_echoInput;
		volatile uint8_t *_triggerMode;
#else
		uint8_t _triggerPin;
		uint8_t _echoPin;
#endif
		uint16_t _maxEchoTime;
		uint32_t _max_time;
};


#endif
