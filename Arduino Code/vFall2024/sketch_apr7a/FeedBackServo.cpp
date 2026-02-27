#include <Arduino.h>
#include <Servo.h>
#include "FeedBackServo.h"

Servo FeedBackServo::Parallax;
byte FeedBackServo::feedbackPinNumber = 2;
volatile int FeedBackServo::angle;
float FeedBackServo::thetaPre;
unsigned int FeedBackServo::tHigh, FeedBackServo::tLow;
unsigned long FeedBackServo::rise, FeedBackServo::fall;
int FeedBackServo::turns = 0;
float FeedBackServo::Kp = 1.0;
const int FeedBackServo::unitsFC = 360;
const float FeedBackServo::dcMin = 0.029;
const float FeedBackServo::dcMax = 0.971;
const int FeedBackServo::dutyScale = 1;
const int FeedBackServo::q2min = unitsFC / 4;
const int FeedBackServo::q3max = q2min * 3;

FeedBackServo::FeedBackServo(byte _feedbackPinNumber)
{
    pinCheck(_feedbackPinNumber);
    feedbackPinNumber = _feedbackPinNumber;
    byte internalPinNumber = digitalPinToInterrupt(feedbackPinNumber);
    attachInterrupt(internalPinNumber, feedback, CHANGE);
}

void FeedBackServo::setServoControl(byte servoPinNumber)
{
    Parallax.attach(servoPinNumber);
}

void FeedBackServo::setKp(float _Kp)
{
    Kp = _Kp;
}

void FeedBackServo::rotate(int degree, int threshold)
{
    float output, offset, value;
    unsigned long startMillis = millis();  // Track when the rotation started
    unsigned long timeout = 1000; // Timeout after 1 seconds to avoid infinite loop

    // Continuously adjust the servo position until it is within the threshold range
    while (abs(degree - angle) > threshold) {
        output = (degree - angle) * Kp;  // Calculate error and output
        if (output > 200.0) output = 200.0;  // Cap output
        if (output < -200.0) output = -200.0;
        
        offset = (degree > angle) ? 30.0 : (degree < angle) ? -30.0 : 0.0; // Adjust for small corrections
        value = output + offset;  // Final value to set

        Parallax.writeMicroseconds(1490 - value);  // Move the servo

        // If the servo hasn't reached its target after 1 seconds, stop the rotation
        if (millis() - startMillis > timeout) {
            Serial.println("Rotation timeout reached.");
            break;
        }

        delay(15);  // Small delay to allow the servo to move
    }

    // After the rotation is complete, stop the servo at the target position
    Parallax.writeMicroseconds(1490);  // Stop the servo at the target position
}

int FeedBackServo::Angle()
{
    return angle;
}

void FeedBackServo::pinCheck(byte _feedbackPinNumber)
{
#ifdef ARDUINO_AVR_UNO
    if (_feedbackPinNumber != 2 && _feedbackPinNumber != 3)
        exit(1);
#endif
#ifdef ARDUINO_AVR_LEONARDO
    if (_feedbackPinNumber != 0 && _feedbackPinNumber != 1 && _feedbackPinNumber != 2 && _feedbackPinNumber != 3 && _feedbackPinNumber != 7)
        exit(1);
#endif
}

void FeedBackServo::feedback()
{
    if (digitalRead(feedbackPinNumber))
    {
        rise = micros();
        tLow = rise - fall;

        int tCycle = tHigh + tLow;
        if ((tCycle < 1000) || (tCycle > 1200))
            return;

        float dc = (dutyScale * tHigh) / (float)tCycle;
        float theta = ((dc - dcMin) * unitsFC) / (dcMax - dcMin);

        if (theta < 0.0)
            theta = 0.0;
        else if (theta > (unitsFC - 1.0))
            theta = unitsFC - 1.0;

        if ((theta < q2min) && (thetaPre > q3max))
            turns++;
        else if ((thetaPre < q2min) && (theta > q3max))
            turns--;

        if (turns >= 0)
            angle = (turns * unitsFC) + theta;
        else if (turns < 0)
            angle = ((turns + 1) * unitsFC) - (unitsFC - theta);

        thetaPre = theta;
    }
    else
    {
        fall = micros();
        tHigh = fall - rise;
    }
}
