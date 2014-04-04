#include "Arduino.h"

void touchDisplay (long touchMe, int ledPin);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  touchDisplay (touchRead(23), 3);
  touchDisplay (touchRead(22), 4);
  touchDisplay (touchRead(19), 5);
  Serial.println("Values: ");
  delay(100);
}

void touchDisplay (long touchMe, int ledPin)
{
  Serial.print(touchMe);
  Serial.print("\t");
  if (touchMe > 1000) {
    analogWrite(ledPin, map (touchMe, 500, 12000, 0, 255));
  } else {
    analogWrite(ledPin, 0);
  }
}
