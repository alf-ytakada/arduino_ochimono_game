// 落ち物パズルゲーム
// 使用液晶：http://www.ebay.com/itm/191866322734
// 　　　　　裏面には、「1.44' SPI 128*128 V1.1」 と記載されている。ピンは黄色
// 　　　　　コントローラーは、ST7735のようだ。
// 使用モジュール：https://github.com/alf-ytakada/TFT_ST7735/tree/1.0p1-add-display
// 　このモジュールは、fork元の物に、本液晶の_displayの設定を加えたもの。

#include <new.h>
#include <SPI.h>
#include <TFT_ST7735.h>
#include <EEPROM.h>
#include "Ochimono.h"
#include "Drawer.h"

#define __CS1 10
#define __DC  9

//#define PIN_UP    2
#define PIN_LEFT  5
#define PIN_DOWN  6
#define PIN_RIGHT 7

#define PIN_ROTATE  8


TFT_ST7735 tft = TFT_ST7735(__CS1, __DC);

// 盤のサイズ
const uint8_t boardWidth  = 6;
const uint8_t boardHeight = 12;
// ゲーム本体
Ochimono game = Ochimono(boardWidth, boardHeight);
Drawer drawer = Drawer(&tft, boardWidth, boardHeight);
// 最大スコア
long maxScore = 0;

// EEPROM書き込み
void writeEEPROM(long addr, long val) {
  uint8_t chksum  = 0;
  for (int i = 0 ; i < 4 ; i++) {
    Serial.println(String("val = ") + val);
    uint8_t writeValue  = (val & 0xff);
    Serial.println(String("write value = ") + writeValue);
    EEPROM.write(addr + i, writeValue);
    val >>= 8;
    chksum  += writeValue;
  }
  EEPROM.write(addr + 4, chksum);
  delay(11111);
}

// EEPROM読み出し
long readEEPROM(long addr) {
  uint8_t chksum  = 0;
  long ret = 0;
  for (int i = 3 ; i >= 0 ; i--) {
    ret <<= 8;
    uint8_t val = EEPROM.read(addr + i);
    ret |= val;
    Serial.println(String("ret = ") + ret);
    chksum  += val;
  }
  if (chksum == EEPROM.read(addr + 4)) {
    return ret;
  }
  return -1;
}


void setup() {
  pinMode(PIN_DOWN, INPUT_PULLUP);
  pinMode(PIN_LEFT, INPUT_PULLUP);
  pinMode(PIN_RIGHT,  INPUT_PULLUP);
  pinMode(PIN_ROTATE, INPUT_PULLUP);
  
  tft.begin();
  Serial.begin(38400);
  srand(analogRead(0));

  // ピン下側が画面下になるようにローテートする
  tft.setRotation(2);
  game.start();
  // 最大スコアロード
  maxScore = readEEPROM(0);
  if (maxScore == -1) {
    maxScore  = 0;
  }
  drawer.drawScore(game.currentScore());
  drawer.drawMaxScore(maxScore);
}

// ボタンが押されたか？
int32_t last_pressed_times[14] = {0};
bool pressing[14] = {false};
bool pressed[14]  = {false};
bool is_pressed(int pin) {
  int32_t now = millis();
  int hl  = digitalRead(pin);
  if (hl == LOW) {
    if (pressed[pin] == false && pressing[pin] == true && now - last_pressed_times[pin] > 30) {
      // 30ms立っているならば押したとする
      last_pressed_times[pin] = now + 200;
      pressed[pin]  = true;
      return true;
    }
    else if (pressing[pin] == false) {
      last_pressed_times[pin] = now;
      pressing[pin] = true;
    }
  }
  else {
    pressing[pin] = false;
    pressed[pin]  = false;
  }
  return false;
}


// ボタンが押し続けられたか？
// waitRepeat: キーリピート的な動き。
bool is_pressing(int pin, bool waitRepeat = true) {
  int32_t now = millis();
  int hl  = digitalRead(pin);
  if (hl == LOW && pressed[pin]) {
    if (waitRepeat && now - last_pressed_times[pin] > 100) {
      last_pressed_times[pin] = now + 30;
      return true;
    }
    if (!waitRepeat) {
      return true;
    }
  }
  return false;
}


///////////////////////////////
// loop()
int cnt = 0;
int pre = millis();
bool isSavedScore = false;

void loop() {
  cnt++;

  /////////////////////////////////
  // キー入力処理
  bool pressed_left   = is_pressed(PIN_LEFT);
  bool pressing_left  = is_pressing(PIN_LEFT);
  bool pressed_right  = is_pressed(PIN_RIGHT);
  bool pressing_rignt = is_pressing(PIN_RIGHT);
  bool pressed_down   = is_pressed(PIN_DOWN);
  bool pressing_down  = is_pressing(PIN_DOWN);
  
  if (pressed_left || pressing_left) {
    game.moveBlock(dir_left);
  }
  if (pressed_down || pressing_down) {
    game.moveBlock(dir_down);
  }
  if (pressed_right || pressing_rignt) {
    game.moveBlock(dir_right);
  }
  if (is_pressed(PIN_ROTATE)) {
    game.rotateBlock(dir_right);
  }

  /////////////////////////////////
  // リセット
  if (is_pressing(PIN_LEFT, false) && is_pressing(PIN_DOWN, false) && is_pressing(PIN_RIGHT, false)) {
    Serial.println("reset");
    game.init();
    game.start();
    tft.clearScreen();
    isSavedScore  = false;
    drawer.drawScore(game.currentScore());
    drawer.drawMaxScore(maxScore);
    delay(300);
    return;
  }
  
  /////////////////////////////////
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
  
  // ゲームオーバー？
  if (game.isGameOver()) {
    drawer.drawGameOver();
    // 最大スコア記録ならば記録
    if ((!isSavedScore) && game.currentScore() > maxScore) {
      maxScore  = game.currentScore();
      writeEEPROM(0, maxScore);
      isSavedScore  = true;
    }
  }

  // ブロック消去中？
  if (game.isErasing()) {
    drawer.drawChain(game.currentChain());
    drawer.drawScore(game.currentScore());
  }
  delay(10);
  int now = millis();
  if (now - pre >= 1000) {
      //Serial.println(String("fps = ") + ((float)cnt / ((now - pre) / 1000)));
      //Block *b  = game.getCurrentBlock();
      //Serial.println(String(b->blockPiece[0].x) + "," + b->blockPiece[0].y);
      if (game.isGameOver()) {
        Serial.println("game over");
      }
      cnt = 0;
      pre = now;
    }
}
