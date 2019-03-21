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
  M5.Lcd.setTextFont(2);
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
      if (M5.BtnA.wasPressed()) {
        M5.Lcd.fillRect(0, 0, 320, 180, TFT_WHITE);
        mm++;
      } else if (M5.BtnB.wasPressed()) {
        M5.Lcd.fillRect(0, 0, 320, 180, TFT_WHITE);
        M5.Lcd.fillScreen(TFT_WHITE);
        ss++;
      } else if (M5.BtnC.wasPressed()) {
        moveTimer();
      }
      break;
    case Moving:
      if (M5.BtnC.wasPressed()) {
        pauseTimer();
      }
      break;
    case Pause:
      if (M5.BtnC.wasPressed()) {
        endTimer();
      }
      break;
    case Ended:
      if (M5.BtnC.wasPressed()) {
        initialize();
      }
      break;
  }
}

void initialize() {
  M5.Lcd.clear(TFT_WHITE);
  M5.Lcd.setTextColor(TFT_BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(60, 200);
  M5.Lcd.print("M");
  M5.Lcd.setCursor(150, 200);
  M5.Lcd.print("S");
  M5.Lcd.setCursor(215, 200);
  M5.Lcd.print("START");
  stats = Initialized;
}

void moveTimer() {
  M5.Lcd.clear(TFT_LIGHTGREY);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(45, 200);
  M5.Lcd.print("+1M");
  M5.Lcd.setCursor(120, 200);
  M5.Lcd.print("RESET");
  M5.Lcd.setCursor(215, 200);
  M5.Lcd.print("PAUSE");
  stats = Moving;
}

void pauseTimer() {
  M5.Lcd.clear(TFT_WHITE);
  M5.Lcd.setTextColor(TFT_BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(45, 200);
  M5.Lcd.print("+1M");
  M5.Lcd.setCursor(120, 200);
  M5.Lcd.print("RESET");
  M5.Lcd.setCursor(210, 200);
  M5.Lcd.print("RESUME");
  stats = Pause;
}

void endTimer() {
  M5.Lcd.clear(TFT_OLIVE);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(40, 200);
  M5.Lcd.print("___");
  M5.Lcd.setCursor(120, 200);
  M5.Lcd.print("RESET");
  M5.Lcd.setCursor(230, 200);
  M5.Lcd.print("___");
  stats = Ended;
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
