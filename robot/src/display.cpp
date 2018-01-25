#include <Arduino.h>
#include "movement.h"

char DIN = 10;
char CS = 12;
char CLK = 11;
char maxInUse = 1;    //change this variable to set how many MAX7219's you'll use
                     // define max7219 registers
char max7219_reg_decodeMode  = 0x09;
char max7219_reg_intensity   = 0x0a;
char max7219_reg_scanLimit   = 0x0b;
char max7219_reg_shutdown    = 0x0c;
char max7219_reg_displayTest = 0x0f;

void putByte(char data) {
  char i = 8;
  char mask;
  while(i > 0) {
    mask = 0x01 << (i - 1);      // get bitmask
    digitalWrite( CLK, LOW);   // tick
    if (data & mask){            // choose bit
      digitalWrite(DIN, HIGH);// send 1
    }else{
      digitalWrite(DIN, LOW); // send 0
    }
    digitalWrite(CLK, HIGH);   // tock
    --i;                         // move to lesser bit
  }
}

void maxSingle( char reg, char col)
{
  digitalWrite(CS, LOW);       // begin
  putByte(reg);                  // specify register
  putByte(col);                  // put data
  digitalWrite(CS,HIGH);
}

void maxAll (char reg, char col) {    // initialize  all  MAX7219's in the system
  int c = 0;
  digitalWrite(CS, LOW);  // begin
  for ( c =1; c<= maxInUse; c++) {
  putByte(reg);  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
    }
  digitalWrite(CS,HIGH);
}

void initMax7219()
{
  //initiation of the max 7219
  maxAll(max7219_reg_scanLimit, 0x07);
  maxAll(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
  maxAll(max7219_reg_shutdown, 0x01);    // not in shutdown mode
  maxAll(max7219_reg_displayTest, 0x00); // no display test
  for (char e = 1; e <= 8; e++) {    // empty registers, turn all LEDs off
    maxAll(e,0);
  }
  maxAll(max7219_reg_intensity, 0x0f & 0x0f);    // the first 0x0f is the value you can set
                                                  // range: 0x00 to 0x0f
}

void setup_display ()
{
  pinMode(DIN, OUTPUT);
  pinMode(CLK,  OUTPUT);
  pinMode(CS,   OUTPUT);
  initMax7219();
}


void handle_image(Movement *movement) {
  int a= 1;
  // @todo
}
