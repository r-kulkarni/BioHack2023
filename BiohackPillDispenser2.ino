#include <Servo.h>

Servo myservo;

int posServ = 2;
int testInput = 4;
int testOutput = 5;
int communicate = 0;

void setup() {
  myservo.attach(3);
  pinMode(testInput, INPUT);
  pinMode(testOutput, OUTPUT);
}

void loop() {
  digitalWrite(testOutput, LOW);
  communicate = digitalRead(testInput);
  if (communicate == HIGH) {
    for (posServ = 5; posServ <= 85; posServ += 1) {
      myservo.write(posServ);
      delay(15);
    }
    for (posServ = 85; posServ >= 5; posServ -= 1) {
      myservo.write(posServ);
      delay(15);
    }
    digitalWrite(testOutput, HIGH);
    delay(200);
  }
}
