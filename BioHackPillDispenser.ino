#include <LiquidCrystal.h>
#include "Arduino.h"
#include "uRTCLib.h"

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
uRTCLib rtc(0x68);
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tues", "Wed", "Thur", "Fri", "Sat"};

int pills = 0;
int dose = 0;
int alarmHour = 0;
int alarmMin = 0;
int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;
int k = 0;
int m = 0;
int testOutput = 10;
int testInput = 13;
int s = 0;
int com = 0;
int check1 = 0;
int check2 = 1;

const int buttonRight = 7;
const int buttonLeft = 8;
const int buttonEnter = 6;
const int beeperPin = 9;

byte pill[8] = {
  0b01110,
  0b10001,
  0b10001,
  0b10001,
  0b11111,
  0b11111,
  0b11111,
  0b01110
};

void setup() {
  pinMode(buttonRight, INPUT);
  pinMode(buttonLeft, INPUT);
  pinMode(buttonEnter, INPUT);
  pinMode(beeperPin, OUTPUT);
  pinMode(testOutput, OUTPUT);
  pinMode(testInput, INPUT);
  lcd.createChar(0, pill);
  lcd.begin(16, 2);
  URTCLIB_WIRE.begin();
  rtc.set(0, 14, 9, 1, 21, 5, 23);
}

void loop() {
  buttonState1 = digitalRead(buttonEnter);
  if (buttonState1 == HIGH) {
    mainMenu();
    k = 0;
  } else {
    rtc.refresh();
    lcd.setCursor(0,0);
    lcd.write(byte(0));
    lcd.print("Pill Supply:");
    lcd.print(pills);
    lcd.setCursor(0,1);
    lcd.print(rtc.month());
    lcd.print("/");
    lcd.print(rtc.day());
    lcd.print(" ");
    lcd.print(daysOfTheWeek[rtc.dayOfWeek()-1]);
    lcd.print(" ");
    if (rtc.hour() > 12) {
      lcd.print(rtc.hour()-12);
    } else {
      lcd.print(rtc.hour());
    }
    lcd.print(":");
    if (rtc.minute() < 10) {
      lcd.print(0);
      lcd.print(rtc.minute());
    } else {
      lcd.print(rtc.minute());
    }
    if (rtc.hour() >= 12) {
      lcd.print("pm");
    } else {
      lcd.print("am");
    }
    if (rtc.hour() == alarmHour && rtc.minute() == (alarmMin - 1) && check1 != check2) {
      com = 0;
      while (s < dose) {
        digitalWrite(testOutput, HIGH);
        while (com == 0) {
          if (digitalRead(testInput) == HIGH) {
            com = 1;
          }
        }
        digitalWrite(testOutput, LOW);
        delay(200);
        s = s + 1;
        pills = pills - 1;
      }
      check1 = check2;
    }
    if (rtc.hour() == alarmHour && rtc.minute() == alarmMin) {
      tone(beeperPin, 200);
      delay(250);
      noTone(beeperPin);
      delay(250);
      tone(beeperPin, 200);
      delay(350);
      noTone(beeperPin);
      delay(150);
      check1 = 0;
      check2 = 1;
      s = 0;
    }
  }
}

void mainMenu() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Menu:");
  lcd.setCursor(0,1);
  lcd.print("Dose");
  lcd.print(" ");
  lcd.print("Alarm");
  lcd.print(" ");
  lcd.print("Pills");
  int pos = 0;
  delay(1000);
  while (k == 0) {
    lcd.setCursor(pos, 1);
    lcd.cursor();
    buttonState1 = digitalRead(buttonEnter);
    buttonState2 = digitalRead(buttonLeft);
    buttonState3 = digitalRead(buttonRight);
    if (buttonState2 == HIGH) {
      pos = pos - 1;
    } else if (buttonState3 == HIGH) {
      pos = pos + 1;
    }
    if (buttonState1 == HIGH) {
      if (pos < 4){
        doseMenu();
        k = 1;
        lcd.clear();
      } else if (pos > 4 && pos < 10) {
        alarmMenu();
        k = 1;
        lcd.clear();
      } else if (pos > 10) {
        pillMenu();
        k = 1;
        lcd.clear();
      }
    }
    delay(175);
  }
}

void doseMenu() {
  lcd.noCursor();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Pill Dosage (#):");
  lcd.setCursor(0,1);
  lcd.print("1");
  lcd.print(" ");
  lcd.print("2");
  lcd.print(" ");
  lcd.print("3");
  lcd.print(" ");
  lcd.print("4");
  lcd.print(" ");
  lcd.print("5");
  lcd.print(" ");
  lcd.print("6");
  lcd.print(" ");
  lcd.print("7");
  lcd.print(" ");
  lcd.print("8");
  int pos = 0;
  delay(1000);
  while (k == 0) {
    lcd.setCursor(pos, 1);
    lcd.cursor();
    buttonState1 = digitalRead(buttonEnter);
    buttonState2 = digitalRead(buttonLeft);
    buttonState3 = digitalRead(buttonRight);
    if (buttonState2 == HIGH) {
      pos = pos - 1;
    } else if (buttonState3 == HIGH) {
      pos = pos + 1;
    }
    if (buttonState1 == HIGH) {
      if (pos == 0){
        dose = 1;
      } else if (pos == 2){
        dose = 2;
      } else if (pos == 4){
        dose = 3;
      } else if (pos == 6){
        dose = 4;
      } else if (pos == 8){
        dose = 5;
      } else if (pos == 10){
        dose = 6;
      } else if (pos == 12){
        dose = 7;
      } else if (pos == 14){
        dose = 8;
      }
      lcd.clear();
      lcd.print("Dose = ");
      lcd.print(dose);
      lcd.print(" Pills");
      delay(1500);
      lcd.setCursor(0,1);
      lcd.print("   Loading...   ");
      delay(2000);
      lcd.clear();
      k = 1;
    }
    delay(175);
  }
}

void alarmMenu() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Alarm Time:");
  delay(1000);
  int m = 0;
  while (m == 0) {
    buttonState1 = digitalRead(buttonEnter);
    buttonState2 = digitalRead(buttonLeft);
    buttonState3 = digitalRead(buttonRight);
    int pos = 5;
    lcd.setCursor(pos, 1);
    lcd.noCursor();
    if (alarmHour < 10) {
      lcd.print(0);
      lcd.print(alarmHour);
      lcd.print(":");
    } else {
      lcd.print(alarmHour);
      lcd.print(":");
    }
    if (buttonState2 == HIGH) {
      alarmHour = alarmHour - 1;
    } else if (buttonState3 == HIGH) {
      alarmHour = alarmHour + 1;
    }
    if (buttonState1 == HIGH) {
      delay(1000);
      while (m == 0) {
        buttonState1 = digitalRead(buttonEnter);
        buttonState2 = digitalRead(buttonLeft);
        buttonState3 = digitalRead(buttonRight);
        pos = 8;
        lcd.setCursor(pos, 1);
        lcd.noCursor();
        if (alarmMin < 10) {
          lcd.print(0);
          lcd.print(alarmMin);
        } else {
          lcd.print(alarmMin);
        }
        if (buttonState2 == HIGH) {
          alarmMin = alarmMin - 1;
        } else if (buttonState3 == HIGH) {
          alarmMin = alarmMin + 1;
        }
        if (buttonState1 == HIGH) {
          delay(2000);
          m = 1;
        }
        delay(175);
      }
    }
    delay(175);
  }
}
  
void pillMenu() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Pill Supply:");
  lcd.setCursor(0,1);
  delay(1000);
  lcd.noCursor();
  int m = 0;
  while (m == 0) {
    buttonState1 = digitalRead(buttonEnter);
    buttonState2 = digitalRead(buttonLeft);
    buttonState3 = digitalRead(buttonRight);
    lcd.setCursor(0,1);
    lcd.print(pills);
    lcd.print(" Total Pills");
    if (buttonState2 == HIGH) {
      pills = pills - 1;
    } else if (buttonState3 == HIGH) {
      pills = pills + 1;
    }
    if (buttonState1 == HIGH) {
      m = 1;
    }
    delay(175);
  }
}
