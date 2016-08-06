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

//#define PIN_UP    2
#define PIN_LEFT  3
#define PIN_DOWN  4
#define PIN_RIGHT 5

#define PIN_ROTATE  6


TFT_ST7735 tft = TFT_ST7735(__CS1, __DC);

// 盤のサイズ
const uint8_t boardWidth  = 6;
const uint8_t boardHeight = 12;
Ochimono game = Ochimono(boardWidth, boardHeight);
Drawer drawer = Drawer(&tft, boardWidth, boardHeight);

void setup() {
  // put your setup code here, to run once:
  //pinMode(PIN_UP,   INPUT_PULLUP);
  pinMode(PIN_DOWN, INPUT_PULLUP);
  pinMode(PIN_LEFT, INPUT_PULLUP);
  pinMode(PIN_RIGHT,  INPUT_PULLUP);
  pinMode(PIN_ROTATE, INPUT_PULLUP);
  
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

// ボタンが押されたか？
int last_pressed_times[14] = {0};
bool pressing[14] = {false};
bool is_pressed(int pin) {
  int now = millis();
  int hl  = digitalRead(pin);
  if (hl == LOW) {
    if (pressing[pin] == false && now - last_pressed_times[pin] > 50) {
      // 50ms立っているならば押したとする
      pressing[pin] = true;
      last_pressed_times[pin] = now + 100;
      return true;
    }
    else {
      //last_pressed_times[pin] = now;
    }
  }
  else {
    pressing[pin] = false;
  }
  return false;
}


// ボタンが押し続けられたか？
// キーリピート的な動き
bool is_pressing(int pin) {
  int now = millis();
  int hl  = digitalRead(pin);
  if (hl == LOW && pressing[pin] && (now - last_pressed_times[pin]) > 100) {
     last_pressed_times[pin]  = now;
     return true;
  }
  return false;
}

int cnt = 0;
int pre = millis();

void loop() {
  cnt++;
  if (is_pressed(PIN_LEFT) || is_pressing(PIN_LEFT)) {
    game.moveBlock(dir_left);
  }
  if (is_pressed(PIN_DOWN) || is_pressing(PIN_DOWN)) {
    game.moveBlock(dir_down);
  }
  if (is_pressed(PIN_RIGHT) || is_pressing(PIN_RIGHT)) {
    game.moveBlock(dir_right);
  }
  if (is_pressed(PIN_ROTATE)) {
    game.rotateBlock(dir_right);
  }
  
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
  if (game.isGameOver()) {
    drawer.drawGameOver();
  }
  if (game.isErasing()) {
    drawer.drawChain(game.currentChain());
  }
  delay(10);
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
