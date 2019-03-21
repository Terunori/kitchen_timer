/*
    Name:       kitchen_timer.ino
    Created:    2019/3/20
    Author:     terunori
*/

#include <M5Stack.h>
#include <driver/dac.h> //Arduino-ESP32 driver

int mm = 0;
int ss = 0;

long baseTime;
long delayTime;
long setTime;

int beep_total_time;
int beep_last_time;

enum Stats {
  Initialized,
  Moving,
  Pause,
  Ended
};

enum Stats stats;

void setup() {
  M5.begin();
  //これを実行すると、M5Stackスピーカーから破裂音が出る
  dac_output_enable( DAC_CHANNEL_1 ); //DAC channel 1 is GPIO #25
  M5.Lcd.setTextFont(2);
  M5.Lcd.fillScreen(TFT_WHITE);
  stats = Initialized;
  initialize();
}

void loop() {
  M5.update();
  printTime();
  switch (stats) {
    case Initialized:
      if (M5.BtnA.isPressed() && M5.BtnB.isPressed()) {
        initialize();
        delay(200);
      } else if (M5.BtnA.wasPressed()) {
        M5.Lcd.fillRect(0, 0, 320, 180, TFT_WHITE);
        mm++;
      } else if (M5.BtnB.wasPressed()) {
        M5.Lcd.fillRect(0, 0, 320, 180, TFT_WHITE);
        if (ss < 50) {
          ss += 10;
        } else {
          ss = 0;
          mm++;
        }
      } else if (M5.BtnC.wasPressed()) {
        if (!(mm == 0 && ss == 0)) {
          moveTimer();
        }
      }
      break;
    case Moving:
      if (M5.BtnA.wasPressed()) {
        M5.Lcd.fillRect(0, 0, 320, 180, TFT_LIGHTGREY);
        mm++;
        setTime += 60000;
      } else if (M5.BtnB.wasPressed()) {
        M5.Lcd.fillRect(0, 0, 320, 180, TFT_WHITE);
        initialize();
      } else if (M5.BtnC.wasPressed()) {
        pauseTimer();
      }
      delayTime = millis() - baseTime;
      updateTime();
      if (delayTime + 1000 > setTime ) {
        endTimer();
      }
      break;
    case Pause:
      if (M5.BtnA.wasPressed()) {
        M5.Lcd.fillRect(0, 0, 320, 180, TFT_WHITE);
        mm++;
      } else if (M5.BtnB.wasPressed()) {
        initialize();
      } else if (M5.BtnC.wasPressed()) {
        if (!(mm == 0 && ss == 0)) {
          moveTimer();
        }
      }
      break;
    case Ended:
      while (!(M5.BtnA.read() || M5.BtnB.read() || M5.BtnC.read())) {
        periodicBeep(5, 1400, 80, 4, 400);
      }
      initialize();
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

  mm = 0;
  ss = 0;
  M5.Speaker.mute();

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

  baseTime = millis();
  setTime = mm * 60000 + ss * 1000 + 1000;

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

  mm = 0;
  ss = 0;
  baseTime = millis();

  beep_total_time = baseTime;
  beep_last_time = baseTime;

  stats = Ended;
}

void printTime() {
  if (stats == Pause && blinkOn()) {
    M5.Lcd.fillRect(0, 0, 320, 180, TFT_WHITE);
  } else {
    setMin();
    setColon();
    setSec();
  }
}

void setMin() {
  M5.Lcd.setTextSize(8);
  M5.Lcd.setCursor(30, 55);
  if (mm < 10) {
    M5.Lcd.print("0");
    M5.Lcd.print(mm);
  } else {
    M5.Lcd.print(mm);
  }
}

void setSec() {
  M5.Lcd.setTextSize(8);
  M5.Lcd.setCursor(190, 55);
  if (ss < 10) {
    M5.Lcd.print("0");
    M5.Lcd.print(ss);
  } else {
    M5.Lcd.print(ss);
  }
}

void setColon() {
  M5.Lcd.setTextSize(8);
  M5.Lcd.setCursor(155, 55);
  if (!(stats == Moving && blinkOn())) {
    M5.Lcd.print(":");
  }
}

void updateTime() {
  int oldss = ss;
  long remainingTime = setTime - delayTime;
  if (!(M5.BtnB.isPressed())) {
    mm = remainingTime / 60000;
    ss = (remainingTime - mm * 60000) / 1000;
    if (oldss != ss) {
      M5.Lcd.fillRect(0, 0, 320, 180, TFT_LIGHTGREY);
    }
  }
}

void periodicBeep(uint8_t volume, int hz, int duration, int times, int interval) {
  int total_duration = duration*times*2;
  if ( millis() - beep_total_time < total_duration ) {
    uint32_t b_period = millis() - beep_last_time;
    int halfCycle = 500000 / hz;
    if ( b_period < duration ) {
      dac_output_voltage(DAC_CHANNEL_1, 0);
      delayMicroseconds(halfCycle);
      dac_output_voltage(DAC_CHANNEL_1, volume);
      delayMicroseconds(halfCycle);
    } else if ( b_period >= duration && b_period < duration * 2 ) {
      dac_output_voltage(DAC_CHANNEL_1, 0);
    } else {
      beep_last_time = millis();
    }
  } else if(millis() - beep_total_time < total_duration+interval){
    dac_output_disable(DAC_CHANNEL_1);
  } else {
    beep_total_time = millis();
    dac_output_enable(DAC_CHANNEL_1);
  }
}

bool blinkOn() {
  int tmp = millis() - baseTime;
  return tmp % 1000 < 500;
}
