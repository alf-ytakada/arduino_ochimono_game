// 落ち物パズルゲーム
// 使用液晶：http://www.ebay.com/itm/191866322734
// 　　　　　裏面には、「1.44' SPI 128*128 V1.1」 と記載されている。ピンは黄色
// 　　　　　コントローラーは、ST7735のようだ。
// 使用モジュール：https://github.com/alf-ytakada/TFT_ST7735/tree/1.0p1-add-display
// 　このモジュールは、fork元の物に、本液晶の_displayの設定を加えたもの。

#include <new.h>
#include <SPI.h>
#include <TFT_ST7735.h>
#include "Ochimono.h"
#include "Drawer.h"

#define __CS1 10
#define __DC  9


TFT_ST7735 tft = TFT_ST7735(__CS1, __DC);

Ochimono game = Ochimono(6, 12);
Drawer drawer = Drawer(&tft);;

void setup() {
  // put your setup code here, to run once:
  tft.begin();
  Serial.begin(38400);
  //randomSeed(analogRead(0));
  srand(analogRead(0));

  // ピン下側が画面下になるようにローテートする
  tft.setRotation(2);
  Serial.println("setup()");
  game.start();
  Serial.println("game started"); delay(20);
  Serial.println("setup done"); delay(20);
}

int cnt = 0;
int pre = millis();

void loop() {
  cnt++;
  game.mainLoop();
  if (game.redrawBoard) {
    drawer.drawBoard(game.getBoard());
    game.redrawBoard = false;
  }
  if (game.redrawCurrentBlock) {
    drawer.drawCurrentBlock(game.getCurrentBlock());
    game.redrawCurrentBlock  = false;
  }
  if (game.redrawNextBlock) {
    drawer.drawNextBlock(game.getNextBlock());
    game.redrawNextBlock  = false;
  }
  delay(100);
  int now = millis();
  if (now - pre >= 1000) {
      Serial.println(String("fps = ") + ((float)cnt / ((now - pre) / 1000)));
      Block *b  = game.getCurrentBlock();
      Serial.println(String(b->blockPiece[0].x) + "," + b->blockPiece[0].y);
      if (game.isGameOver()) {
        Serial.println("game over");
      }
      cnt = 0;
      pre = now;
    }
}
