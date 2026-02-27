#include <RPC.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDB, OUTPUT);
  pinMode(LEDG, OUTPUT);

  if(RPC.cpud_id() == CM7_CPUID) {
    blink(LEDB, 100);
  } else {
    blink(LEDG, 100);
  }
  RPC.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

void blink(int led, int delaySeconds) {
    for (int i; i<5; i++) {
      digitalWrite(led, LOW);
      delay(delaySeconds);
      digitalWrite(led, HIGH);
      delay(delaySeconds);
    }
  }
