/*
    Name:       kitchen_timer.ino
    Created:    2019/3/20
    Author:     terunori
*/

#include <M5Stack.h>

int mm = 0;
int ss = 0;

enum Stats {
  Initialized,
  Moving,
  Pause,
  Ended
};

enum Stats stats;

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.fillScreen(TFT_WHITE);
  stats = Initialized;
  initialize();
}

void loop() {
  M5.update();
  printTime();
  // put your main code here, to run repeatedly:
  switch (stats) {
    case Initialized:
      initialize();
      break;
    case Moving:
      break;
    case Pause:
      break;
    case Ended:
      break;
  }
}

void initialize() {
  M5.Lcd.setTextFont(2);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(TFT_BLACK);
  M5.Lcd.setCursor(60, 200);
  M5.Lcd.print("M");
  M5.Lcd.setCursor(150, 200);
  M5.Lcd.print("S");
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(240, 200);
  M5.Lcd.print("START");
  M5.Lcd.setCursor(240, 220);
  M5.Lcd.print("/STOP");
}

void printTime() {
  setMin();
  setColon();
  setSec();
}

void setMin() {
  M5.Lcd.setTextSize(8);
  M5.Lcd.setCursor(30, 55);
  if (mm < 10) {
    M5.lcd.print("0");
    M5.lcd.print(mm);
  } else {
    M5.lcd.print(mm);
  }
}

void setSec() {
  M5.Lcd.setTextSize(8);
  M5.Lcd.setCursor(190, 55);
  if (ss < 10) {
    M5.lcd.print("0");
    M5.lcd.print(ss);
  } else {
    M5.lcd.print(ss);
  }
}

void setColon() {
  M5.Lcd.setTextSize(8);
  M5.Lcd.setCursor(155, 55);
  if (!(stats == Moving && ss % 2 == 0)) {
    M5.lcd.print(":");
  }
}
