// 落ち物パズルゲーム
// 使用液晶：http://www.ebay.com/itm/191866322734
// 　　　　　裏面には、「1.44' SPI 128*128 V1.1」 と記載されている。ピンは黄色
// 　　　　　コントローラーは、ST7735のようだ。
// 使用モジュール：https://github.com/alf-ytakada/TFT_ST7735/tree/1.0p1-add-display
// 　このモジュールは、fork元の物に、本液晶の_displayの設定を加えたもの。


#include <SPI.h>
#include <TFT_ST7735.h>
#include "Ochimono.h"

#define __CS1 10
#define __DC  9


TFT_ST7735 tft = TFT_ST7735(__CS1, __DC);

Ochimono *game;

void setup() {
  // put your setup code here, to run once:j
  tft.begin();
  Serial.begin(38400);
  randomSeed(analogRead(0));

  // ピン下側が画面下になるようにローテートする
  tft.setRotation(2);

  game  = new Ochimono(6, 12);
  game->start();
}

void loop() {

  game->mainLoop();
  delay(20);
    int now = millis();
    if (now - pre >= 1000) {
      Serial.println(String("fps = ") + ((float)cnt / ((now - pre) / 1000)));
      cnt = 0;
      pre = now;
    }
}
