/// 暴走時退避ゾーン
/*
 * 
 * 
 * 
 * 
*/
/// 暴走時退避ゾーン

#include "Keyboard.h"

const int inputPin = 3; //入力ピンを定義
const int ledPin = 6;  //LED を接続するピンを定義
const int emargencyPin = 2; //暴走防止ピン

bool myrect = false; //疑似矩形波生成用変数
bool myrectbef = myrect;

void setup()
{
  pinMode(inputPin, INPUT_PULLUP); //入力ピンプルアップに初期化
  pinMode(ledPin, OUTPUT);  //出力ピンに初期化
  pinMode(emargencyPin, INPUT_PULLUP);  //入力ピンプルアップに設定

  Keyboard.begin(); //Keyboard を初期化
}

void loop()
{
  if(digitalRead(emargencyPin) && !digitalRead(inputPin)) //入力が LOW になったら
  {
    digitalWrite(ledPin, myrect); //LED をつける
    Keyboard.press('a'); //a を入力する
  }
  else if(!digitalRead(emargencyPin) || digitalRead(inputPin))
  {
    digitalWrite(ledPin, LOW);  //LED を消す
    Keyboard.release('a'); //a を離す
  }
  delay(3 / 4);
}
