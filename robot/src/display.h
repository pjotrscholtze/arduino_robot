#include <Arduino.h>
#include "movement.h"

void putByte(char data);
void maxSingle( char reg, char col);
void maxAll (char reg, char col);
void initMax7219();
void setup_display ();
void handle_image(Movement *movement);
