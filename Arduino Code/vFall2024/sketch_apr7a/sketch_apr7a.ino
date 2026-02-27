#include "globals.h"
#include "matrix.h"
#include "muxControl.h"
#include "lightArray.h"
#include "FeedBackServo.h"
#include <RPC.h>

// Define pins
#define FEEDBACK_PIN 6   // Feedback pin connected to the feedback signal from the servo
#define SERVO_PIN 4      // Control pin for the servo

uint32_t activeSensor;
float* testIntensityReadings;

// set feedback signal pin number and constants
FeedBackServo feedbackServo(FEEDBACK_PIN);  // Create a FeedBackServo object

void setup() {
  /*
  // put your setup code here, to run once:
  //check to see which core is current
  pinMode(LEDB, OUTPUT); //blue led for M7
  pinMode(LEDG, OUTPUT); //green led for M4
   if(RPC.cpu_id() == CM7_CPUID){
    blink(LEDB, 100); //blink  blue led (M7 Core)
   } else {
    blink(LEDG, 100); //blink green led(M4 core)
   }
  RPC.begin(); //initialize 2nd core
  //end of core check
  */

  pinMode(LED_BUILTIN, OUTPUT);
	activeSensor= 0; //currently selected sensor
	testIntensityReadings = nullptr; //initialize test intensity readings
	lightArrayInit(activeSensor); //initialize mux controls
  Serial.begin(9600);
  analogReadResolution(12);
  pinMode(D5, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D1, OUTPUT);
  lightArrayBaseline(); //take baseline
  
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(100);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(100);                      // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(100);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(100);                      // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(100);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(100);                      // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(100);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  

  //servo part, sets motor pin
  feedbackServo.setServoControl(SERVO_PIN);  // Attach the servo control to the pin
  feedbackServo.setKp(1.0);  // Optional: Set a custom proportional gain for rotation
}

void loop() {
  lightArraySampleSensors();

  for (int i = 0; i < g_sensorCount; i++) {
    Serial.print("Sensor[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.print(100 * g_sensorIntensityVector.element[i][0], 5);
    Serial.print("% - ");
    Serial.println(g_muxADCReadings[i]);
  }

  auto [azimuth, elevation] = lightArrayCalculateAoA3D();

  Serial.print("Azimuth: ");
  Serial.print(azimuth, 2);
  Serial.print("°, Elevation: ");
  Serial.print(elevation, 2);
  Serial.println("°");
  delay(500);

  // Rotate to Azimuth
  Serial.print("Rotating to AoA Azimuth: ");
  Serial.println(static_cast<int>(azimuth));
  feedbackServo.rotate(static_cast<int>(azimuth), 4);

  delay(1000);  // Wait 1s between readings
}

  /*
  //leds that turn on trying to show that it is being setup
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(200);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(200);                      // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(200);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  */

  // Serial.println("Sensor[1]: ");
  // Serial.println(testIntensityReadings[1],32);
  // Serial.println("Sensor[2]: ");
  // Serial.println(testIntensityReadings[2],32);
  // Serial.println("Sensor[3]: ");
  // Serial.println(testIntensityReadings[3],32);
  // Serial.println("Sensor[4]: ");
  // Serial.println(testIntensityReadings[4],32);
  // Serial.println("Sensor[5]: ");
  // Serial.println(testIntensityReadings[5],32);
  // Serial.println("Sensor[6]: ");
  // Serial.println(testIntensityReadings[6],32);
  // Serial.println("Sensor[7]: ");
  // Serial.println(testIntensityReadings[7],32);
  // Serial.println("Sensor[8]: ");
  // Serial.println(testIntensityReadings[8],32);
  // Serial.println("Sensor[9]: ");
  // Serial.println(testIntensityReadings[9],32);
  // Serial.println("Sensor[10]: ");
  // Serial.println(testIntensityReadings[10],32);
  // Serial.println("Sensor[11]: ");
  // Serial.println(testIntensityReadings[11],32);
  // Serial.println("Sensor[12]: ");
  // Serial.println(testIntensityReadings[12],32);
  // Serial.println("Sensor[13]: ");
  // Serial.println(testIntensityReadings[13],32);
  // Serial.println("Sensor[14]: ");
  // Serial.println(testIntensityReadings[14],32);
  // Serial.println("Sensor[15]: ");
  // Serial.println(testIntensityReadings[15],32);
  



/*
//code core check
void blink(int led, int delaySeconds) {
    for(int i; i < 3; i++) {
      digitalWrite(led, LOW);
      delay(delaySeconds);
      digitalWrite(led, HIGH);
      delay(delaySeconds);
    }
  
  }
  //end of 2nd core blink
*/