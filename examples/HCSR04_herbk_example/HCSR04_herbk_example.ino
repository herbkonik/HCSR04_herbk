/*
 HCSR04_herbk library is forked from NewPing 1.8, because Newping above 1.8 is licenced with to much limits.
This example sketch pings 3 sensors continuous.
You can use MAX_DISTANCE as global and/or the distance in centimeter for each sensor.
If you want the time the ping needs to return, use sonar[i].ping() instead of sonar[i].ping_cm().
If you want inches, use sonar[i].ping_in() instead of sonar[i].ping_cm().

*/

#include <HCSR04_herbk.h>

#define TRIGGER_PIN  2                                                                      // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     4                                                                        // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 100                                                              // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated about 400 cm.

HCSR04_herbk sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // Setup of pins and maximum distance.

void setup() {
  Serial.begin(115200);                                                                          // Open serial monitor at 115200 baud to see  results.
}

void loop() {
   delay(50);                                                                                           // Wait 50ms between pings. 29ms should be the shortest delay between pings.
  Serial.print("Distance: ");
  Serial.print(sonar.ping_cm());                                                            // Send ping, get distance in cm and print result. If distace is more then MAX_DISTANCE,  result is MAX_DISTANCE +1 cm.
  Serial.println(" cm");
 }
