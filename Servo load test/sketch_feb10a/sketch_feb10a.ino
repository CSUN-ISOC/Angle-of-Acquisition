#include <Servo.h>

Servo myServo;          // Create servo object
const int servoPin = 9; // Signal pin

void setup()
{
  myServo.attach(servoPin);
}

void loop()
{
  // Sweep from 0° to 180°
  for (int angle = 0; angle <= 180; angle++)
  {
    myServo.write(angle);
    delay(15);  // Controls speed (smaller = faster)
  }

  // Sweep from 180° back to 0°
  for (int angle = 180; angle >= 0; angle--)
  {
    myServo.write(angle);
    delay(15);
  }
}
