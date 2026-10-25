#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm(0x40);

struct joint {
  uint8_t channel; // unsigned interger to prevent negative numbers. Uses 8-bit memory, saves RAM.
  uint16_t minUs, maxUs;    // pulse widths for 0 to 180 degrees
  float safemin, safemax;   // mechanical limits for the arm
  float current, target;    // degrees to track current posistion and target posistion
  float speed;              // degrees per second
};

void writejoint(const joint &j);

joint motor[]{ // Array to determine the joint structure variables
  {0, 600,  2400, 10,  170,  90,  90, 60}, // base motor
  {1, 600,  2400, 10,  160,  90,  90, 40}, // shoulder motor
  {2, 600,  2400, 10,  160,  90,  90, 40}, // elbow motor
  {3, 600,  2400, 10,  170,  90,  90, 60}, // wrist motor
  {4, 600,  2400, 10,  120,  90,  90, 60}, // gripper motor
};

const uint8_t NUM_JOINTS = sizeof(joint) / sizeof(motor[0]);

void writejoint(const joint &j){ // Clamping function to prevents the motor from making movements that will damage the motor
  float a = constrain(j.current, j.safemin, j.safemax);
  float us = j.minUs + (a / 180.00f) * (j.maxUs - j.minUs);
  pwm.writeMicroseconds(j.channel, (uint8_t)us);
}