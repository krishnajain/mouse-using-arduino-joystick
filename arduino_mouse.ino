#include "Mouse.h"
#include <EEPROM.h>

const int mouseLeftButton = 4;    
const int mouseRightButton = 2;    
const int mouseMiddleButton = 8;   
const int scrollUpButton = 5; 
const int scrollDownButton = 3; 
const int dpi = 7; 
const int minMax = 6; 
const int xAxis = A0;         
const int yAxis = A1;         
const int dpiPin = 13;         

int senstivity[4] = {2, 4, 8, 10};
int range = senstivity[0];               
byte i = 0;
int responseDelay = 10;     
int scrollDelay = 50;
float threshold = range / 4;    
float center = range / 2;      
int scrollVal = 0;

void setDPI()
{
  switch(i)
    {
      case 0: analogWrite(dpiPin, 0); break;
      case 1: analogWrite(dpiPin, 84); break;
      case 2: analogWrite(dpiPin, 128); break;
      case 3: analogWrite(dpiPin, 255); break;
      default: i = 0; EEPROM.write(0, i); break;
    }
  range = senstivity[i];
  threshold = range / 4;    
  center = range / 2;       
}

void setup() {

  Mouse.begin();
  pinMode(dpiPin, OUTPUT);
  pinMode(mouseLeftButton, INPUT);
  pinMode(mouseRightButton, INPUT);
  pinMode(mouseMiddleButton, INPUT);
  pinMode(dpi, INPUT);
  pinMode(scrollUpButton, INPUT);
  pinMode(scrollDownButton, INPUT);
  i = EEPROM.read(0);
  setDPI();
}

void loop() {
  
  float xReading = readAxis(A1);
  float yReading = readAxis(A0);

  if (digitalRead(mouseLeftButton) == LOW) {
    if (!Mouse.isPressed(MOUSE_LEFT)) {
      Mouse.press(MOUSE_LEFT);
    }
  }
  else {
    if (Mouse.isPressed(MOUSE_LEFT)) {
      Mouse.release(MOUSE_LEFT);
    }
  }

  if (digitalRead(mouseRightButton) == LOW) {
    if (!Mouse.isPressed(MOUSE_RIGHT)) {
      Mouse.press(MOUSE_RIGHT);
    }
  }
  else {

    if (Mouse.isPressed(MOUSE_RIGHT)) {
      Mouse.release(MOUSE_RIGHT);
    }
  }

  if (digitalRead(mouseMiddleButton) == LOW) {
    if (!Mouse.isPressed(MOUSE_MIDDLE)) {
      Mouse.press(MOUSE_MIDDLE);
    }
  }
  else {
    if (Mouse.isPressed(MOUSE_MIDDLE)) {
      Mouse.release(MOUSE_MIDDLE);
    }
  }

  if (digitalRead(scrollUpButton) == LOW) {
    scrollVal = 1;
    delay(scrollDelay);
  }
  else if (digitalRead(scrollDownButton) == LOW) {
    scrollVal = -1;
    delay(scrollDelay);
  }
  else {
    scrollVal = 0;
  }

  Mouse.move(xReading, yReading, scrollVal);

  if (digitalRead(dpi) == LOW) {
    i = (i==(sizeof(senstivity)/sizeof(int))-1) ? (0) : (i+1) ;
    EEPROM.write(0, i);
    setDPI();
    delay(500);
  }

  if (digitalRead(minMax) == LOW) {
    switch(i)
    {
      case 0: i = EEPROM.read(0); break;
      default: i = 0; break;
    }
    setDPI();
    delay(500);
  }

  delay(responseDelay);
}


float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float readAxis(int thisAxis) {
  // read the analog input:
  float reading = analogRead(thisAxis);

  reading = mapfloat(reading, 0, 1023, 0, range);

  float distance = reading - center;

  if (abs(distance) < threshold) {
    distance = 0;
  }

  return distance;
}
