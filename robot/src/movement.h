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
  int duration;
};

void handle_movement(Movement *movement);
