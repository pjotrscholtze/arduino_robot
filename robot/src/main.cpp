#include <Arduino.h>
#include <VirtualWire.h>
#include "listener.h"
#include "movement.h"

#define  MOVEMENT_FRONT_MIN 16
#define  MOVEMENT_FRONT_MAX 47
#define  MOVEMENT_LEFT_MIN 48
#define  MOVEMENT_LEFT_MAX 79
#define  MOVEMENT_BACK_MIN 80
#define  MOVEMENT_BACK_MAX 111
#define  MOVEMENT_RIGHT_MIN 112
#define  MOVEMENT_RIGHT_MAX 127


Direction get_direction(char direction) {
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
void callback(RemotePacketType* type, char* data, byte len) {
  switch (*type) {
    case MOVEMENT_INFORMATION:
      // TYPE | DIRECTION 0...127 | DURATION 0..127 * 2^7  | DURATION 0...127 | FOOTER
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
      char direction = data[1];
      Direction directionEnum = get_direction(direction);

      int duration = data[3] << 7;
      duration = duration | data[2];

      Movement movement = {
        directionEnum,
        direction,
        duration
      };
      handle_movement(&movement);
    break;
    case SOUND_INFORMATION:
      // @todo
    break;
  }
}

void setup() {
  Serial.begin(9600);	// Debugging only
  Serial.println("setup");
  remote_listening(2);
  remote_set_listener_registar(callback);
}


void loop() {
  remote_tick();
}
