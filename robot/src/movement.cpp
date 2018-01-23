#include <Arduino.h>
#include "movement.h"
#include <SoftwareServo.h>

#define LEFT_MOTOR 6
#define RIGHT_MOTOR 8

int lastStep = 0;
SoftwareServo myservo;

void set_servo_pin(int pin) {
    myservo.attach(pin);
}

void handle_movement(Movement *movement) {
  Serial.println("mov");
  if (lastStep == movement->step){
    return;
  }
  lastStep = movement->step;
  if (lastStep < 100) {
    movement->directionEnum = NEUTRAL;
  }

  switch (movement->directionEnum) {
    case LEFT:
    case RIGHT:
    case FORWARD:
    Serial.println("mov>thing");
      myservo.write(179);
    break;
    case BACKWARDS:
    case NEUTRAL:
    Serial.println("mov>neutral");
    myservo.write(0);
    break;
  }

    SoftwareServo::refresh();
}
