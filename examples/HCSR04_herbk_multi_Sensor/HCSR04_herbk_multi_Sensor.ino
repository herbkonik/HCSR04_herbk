/*
 HCSR04_herbk library is forked from NewPing 1.8, because Newping above 1.8 is licenced with to much limits.
This example sketch pings 3 sensors continuous.
You can use MAX_DISTANCE as global and/or the distance in centimeter for each sensor.
If you want the time the ping needs to return, use sonar[i].ping() instead of sonar[i].ping_cm().
If you want inches, use sonar[i].ping_in() instead of sonar[i].ping_cm().

*/


#include <HCSR04_herbk.h>

#define SONAR_NUM 4                                                                       // Number of sensors.
#define MAX_DISTANCE 200                                                              // Maximum distance (in cm) to ping.

HCSR04_herbk sonar[SONAR_NUM] = {                                            // Sensor object array.
  HCSR04_herbk(4, 5, MAX_DISTANCE),                                             // Sensor 0 trigger pin, echo pin, and max distance to ping.
  HCSR04_herbk(6, 7, MAX_DISTANCE),                                             // Sensor 1 trigger pin, echo pin, and max distance to ping.
  HCSR04_herbk(8, 9, 50)                                                                      // sensor 2 trigger pin, echo pin, and max distance to ping.
  HCSR04_herbk(10, 11, 100)                                                                // sensor 3 trigger pin, echo pin, and max distance to ping.


};

void setup() {
  Serial.begin(115200);                                                                         // Open serial monitor at 115200 baud to see results.
}

void loop() {
  for (uint8_t i = 0; i < SONAR_NUM; i++) {                                           // Loop through each sensor and display results.
    delay(50);                                                                                          // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
   Serial.print("Distance Sensor ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.print(sonar[i].ping_cm());                                                       // Send ping, get distance in cm and print result. If distace is more then MAX_DISTANCE,  result is MAX_DISTANCE +1 cm.
    Serial.print(" cm ");
  }
  Serial.println();
}
