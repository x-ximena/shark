#include <Arduino.h>
#include "config.h"
#include "motion.h"
#include "web.h"

void setup()
{
  Serial.begin(115200);
  delay(200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  motionInit();
  webInit();
}

void loop()
{
  unsigned long currentMillis = millis();

  webLoop();

  // Move servos
  if (on || !stop0 || !stop1 || !stop2)
  {
    move(currentMillis);
  }

  // head_move(); 
}
