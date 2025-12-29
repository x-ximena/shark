// include/config.h
#pragma once

#include <Arduino.h>
#include <ESP32Servo.h>

// ---------- Pins ----------
constexpr int ledPin = 22;

// ---------- Servos ----------
extern Servo servo0;
extern Servo servo1;
extern Servo servo2;
extern Servo servoHead;

// ---------- Motion state ----------
extern bool on;
extern bool stop0, stop1, stop2;

// ---------- Motion timing ----------
extern unsigned long previousMillis0;
extern unsigned long previousMillis1;
extern unsigned long previousMillis2;

constexpr long interval = 20;

// ---------- Motion params ----------
extern float angle0, offset0;
extern float angle1, offset1;
extern float angle2, offset2;
extern float speed1;
extern float speedHead;
