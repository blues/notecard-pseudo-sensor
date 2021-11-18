/*
 * Humidity Sensing using NotecardPseudoSensor
 *
 * This example is designed to demonstrate the capability and usage of the
 * NotecardPseudoSensor library in the fewest lines of code (quick).
 *
 * NOTE: This sample is intended to compile for any MCU architecture and
 * Notecard/Notecarrier combination, but was created and tested using the
 * Notecarrier-AF kit.
 */

#include <Notecard.h>
#include <NotecardPseudoSensor.h>

#define serialDebug Serial

using namespace blues;

Notecard notecard;
NotecardPseudoSensor sensor(notecard);

void setup() {
  // Initialize Debug Output
  serialDebug.begin(115200);
  while (!serialDebug) {
    ; // wait for serial port to connect. Needed for native USB
  }
  notecard.setDebugOutputStream(serialDebug);

  // Initialize Notecard
  notecard.begin();
}

void loop() {
  // Sample NotecardPseudoSensor humidity
  float humidity = sensor.humidity();

  // Log results to Arduino IDE Serial Monitor (Ctrl+Shift+M)
  notecard.logDebugf("INFO: Notecard Humidity: %f.\n", humidity);

  // Wait 65s between loops (65535 max on Uno).
  ::delay(65535);
}
