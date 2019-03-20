#include <M5Stack.h>

enum Stats {
  Initialized,
  Moving,
  Pause,
  Ended
};

enum CountModes {
  up,
  down
};

enum Stats stats;

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  stats = Initialized;
}

void loop() {
  // put your main code here, to run repeatedly:

  if (M5.BtnA.isPressed()) {
    M5.Lcd.fillScreen(TFT_OLIVE);
    stats = Moving;
  }

  if (stats == Moving) {
    M5.Lcd.printf("hello world");
  }
  
}
