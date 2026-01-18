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
float offsetHead = 0;
float speed1 = 3;

// Sine function parameters
float f_hz = 0.5f; // frequency in Hz
float amp0 = 18.0f;  // Amplitude for servo0
float amp1 = 28.0f;  // Amplitude for servo1
float amp2 = 40.0f;  // Amplitude for servo2
float dphi = 0.7f; // phase difference (radians)
// float k2 = 0.2f;


static inline float wave(float x, int offset, float amplitude)
{
    return (90 + offset + amplitude * sinf(x));
}

// ----- Init -----
void motionInit()
{
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
void move(unsigned long currentMillis)
{
    const float t = currentMillis * 0.001f;
    const float omega = 2.0f * (float)M_PI * f_hz;

    // base phase
    const float x2 = omega * t;           // tail segment
    const float x1 = x2 + dphi;           // mid lags/lead depending sign; adjust if needed
    const float x0 = x2 + 2.0f * dphi;    // base

    float angle0 = wave(x0, offset0, amp0);
    float angle1 = wave(x1, offset1, amp1);
    float angle2 = wave(x2, offset2, amp2);

    // Clamp to safe servo range (adjust as needed)
    angle0 = constrain(angle0, 0, 180);
    angle1 = constrain(angle1, 0, 180);
    angle2 = constrain(angle2, 0, 180);

    if (!stop0 && currentMillis - previousMillis0 >= interval)
    {
        previousMillis0 = currentMillis;
        // angle0 = 90 + offset0 + 45 * sin((currentMillis + 200) * 0.001 * speed1);
        if (!on && abs(angle0 - (90 + offset0)) < 1)
        {
            angle0 = 90 + offset0;
            stop0 = true;
        }
        servo0.write(angle0);
    }

    if (!stop1 && currentMillis - previousMillis1 >= interval)
    {
        previousMillis1 = currentMillis;
        // angle1 = 90 + offset1 + 45 * sin((currentMillis + 100) * 0.001 * speed1);
        if (!on && abs(angle1 - (90 + offset1)) < 2)
        {
            angle1 = 90 + offset1;
            stop1 = true;
        }
        servo1.write(angle1);
    }

    if (!stop2 && currentMillis - previousMillis2 >= interval)
    {
        previousMillis2 = currentMillis;
        // angle2 = 90 + offset2 + 45 * sin((currentMillis) * 0.001 * speed1);
        if (!on && abs(angle2 - (90 + offset2)) < 2)
        {
            angle2 = 90 + offset2;
            stop2 = true;
        }
        servo2.write(angle2);
    }
}

// void head_move()
// {
//     // angle = constrain(angle, 0, 180);
//     servoHead.write(offsetHead);
// }
