#include "motion.h"
#include "config.h"
#include <math.h>

// ----- Define globals -----
Servo servo0;
Servo servo1;
Servo servo2;
Servo servoHead;

bool on = false;
bool stop0 = false, stop1 = false, stop2 = false;

unsigned long previousMillis0 = 0;
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;

float angle0 = 0, offset0 = 6;
float angle1 = 0, offset1 = 13;
float angle2 = 0, offset2 = 17;
float speed1 = 3;

// ----- Init -----
void motionInit() {
  servo0.attach(14);
  servo1.attach(16);
  servo2.attach(18);
  servoHead.attach(27);

  servo0.write(90);
  servo1.write(90);
  servo2.write(90);
  servoHead.write(90);
}

// ----- Motion logic -----
void move(unsigned long currentMillis) {
  if (!stop0 && currentMillis - previousMillis0 >= interval) {
    previousMillis0 = currentMillis;
    angle0 = 90 + offset0 + 45 * sin((currentMillis + 200) * 0.001 * speed1);
    if (!on && abs(angle0 - (90 + offset0)) < 1) {
      angle0 = 90 + offset0;
      stop0 = true;
    }
    servo0.write(angle0);
  }

  if (!stop1 && currentMillis - previousMillis1 >= interval) {
    previousMillis1 = currentMillis;
    angle1 = 90 + offset1 + 45 * sin((currentMillis + 100) * 0.001 * speed1);
    if (!on && abs(angle1 - (90 + offset1)) < 2) {
      angle1 = 90 + offset1;
      stop1 = true;
    }
    servo1.write(angle1);
  }

  if (!stop2 && currentMillis - previousMillis2 >= interval) {
    previousMillis2 = currentMillis;
    angle2 = 90 + offset2 + 45 * sin((currentMillis) * 0.001 * speed1);
    if (!on && abs(angle2 - (90 + offset2)) < 2) {
      angle2 = 90 + offset2;
      stop2 = true;
    }
    servo2.write(angle2);
  }
}
