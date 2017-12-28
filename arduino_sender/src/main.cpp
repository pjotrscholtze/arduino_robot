#include <Arduino.h>
#include <VirtualWire.h>

void setup() {
  Serial.begin(9600);
  Serial.print("Begin setup");
  vw_set_tx_pin(2);
  // Initialise the IO and ISR
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);	 // Bits per sec
  Serial.print("Done setting up");
}

void loop() {
  if (Serial.available() > 0) {
    char raw = Serial.read();
    vw_send(&raw, strlen(raw));
    vw_wait_tx();
  }
}
