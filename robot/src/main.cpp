#include <Arduino.h>
#include <VirtualWire.h>
#include "listener.h"
#include "display.h"

#define  MOVEMENT_FRONT_MIN 16
#define  MOVEMENT_FRONT_MAX 47
#define  MOVEMENT_LEFT_MIN  48
#define  MOVEMENT_LEFT_MAX  79
#define  MOVEMENT_BACK_MIN  80
#define  MOVEMENT_BACK_MAX  111
#define  MOVEMENT_RIGHT_MIN 112
#define  MOVEMENT_RIGHT_MAX 127

RemotePacketType systemState = UNKOWN;
unsigned long endTime = 0;
Movement movement = {NEUTRAL, 0, 0};

Direction get_direction(char direction) {
  /**
   *        FRONT
   *         31
   *          |
   * LEFT 63 - - 0 RIGHT
   *          |
   *         95
   *         BACK
  */
  // FRONT = 16...47
  // LEFT  = 48...79
  // BACK  = 80...111
  // RIGHT = 112..127 && 0..15

  Direction directionEnum = RIGHT;
  if (direction >= MOVEMENT_FRONT_MIN && direction <= MOVEMENT_FRONT_MAX) {
      directionEnum = FORWARD;
  } else if (direction >= MOVEMENT_LEFT_MIN && direction <= MOVEMENT_LEFT_MAX) {
      directionEnum = LEFT;
  } else if (direction >= MOVEMENT_BACK_MIN && direction <= MOVEMENT_BACK_MAX) {
      directionEnum = BACKWARDS;
  }
  return directionEnum;
}

void callback(RemotePacketType* type, char data[], byte len) {
  systemState = *type;
  switch (*type) {
    case MOVEMENT_INFORMATION:
    Serial.print("mov\n");
      // TYPE | DIRECTION 0...127 | DURATION 0..127 * 2^7  | DURATION 0...127 | FOOTER
      movement.direction = data[1];
      movement.directionEnum = get_direction(movement.direction);

      int durationTemp;
      durationTemp = (data[2] << 7) | data[3];
      endTime = millis() + durationTemp;
    break;
    case SOUND_INFORMATION:
    Serial.print("snd\n");
      // @todo
    break;
  }
}

unsigned long t = 0;
void handle_state() {
  unsigned long currTime = millis();
  if (currTime != t) {
    unsigned long step = endTime - millis();
    switch (systemState) {
      case MOVEMENT_INFORMATION:
        movement.step = step;
        handle_movement(&movement);
      break;
      case SOUND_INFORMATION:
      break;
      case UNKOWN:
        if (millis() % 300 == 0) {
          // Serial.println("unkown");
          for (char i = 0; i< 8;i++){
            maxSingle(i+1,  (step/300) % 2?0xFF:0x00);
          }
          // a++;
        }
      break;
    }

    if (endTime < millis()) {
        systemState = UNKOWN;
    }
  }
}


void setup() {
  Serial.begin(9600);	// Debugging only
  Serial.println("setup");
  set_servo_pin(6);
  remote_listening(2);
  remote_set_listener_registar(callback);
  setup_display();
  tone(8, 500, 300);
  delay(300);
  tone(8, 750, 300);
  delay(300);
  tone(8, 1000, 300);
}

void loop() {
  remote_tick();
  handle_state();
}
