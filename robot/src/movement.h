enum Direction {
  FORWARD,
  LEFT,
  RIGHT,
  BACKWARDS,
  NEUTRAL
};

struct Movement {
  Direction directionEnum;
  char direction;
  int step;
};

void handle_movement(Movement *movement);
void set_servo_pin(int pin);
