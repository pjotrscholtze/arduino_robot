#include <Arduino.h>
#include <VirtualWire.h>

void setup() {
  // start serial port at 9600 bps:
  Serial.begin(9600);
  vw_set_tx_pin(2);
  // Initialise the IO and ISR
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);	 // Bits per sec
}

void loop() {
  // if we get a valid byte, read analog ins:
  if (Serial.available() > 0) {
    // send sensor values:
    char t = Serial.read();
    vw_send(&t, 1);
  }
}
