//1x1 キーマトリックスでデバウンス回路の実験
//参考：https://eucalyn.hatenadiary.jp/entry/original-keyboard-07

#include <Keyboard.h>

#define debug 0
#define pushdebug 0
#define mapdebug 0
#define keydebug 1

const int rownum = 1; //行の数
const int colnum = 1; //列の数

const int rowpin[rownum] = {2};  //行 0 が 2 ピン、行 1 が 3 ピン
const int colpin[colnum] = {3};  //列 0 が 4 ピン、列 1 が 5 ピン
const byte keymap[rownum][colnum] = {{'q'}};  //出力したい ASCII CODE をマッピングする

static const float debouncetime = 3 / 4;  //unit: ms  //デバウンス回路を組んだなら (3 / 4) //デバウンス回路を組んでないなら 0 とか 0.1 でも十分大丈夫な希ガス
bool currentstate[rownum][colnum];  //現在の入力状態を記録する
bool beforestate[rownum][colnum]; //前回の入力状態を記憶する

int i, j;

void printall()
{
  Serial.println("");
  Serial.println("Printing Map Status");
  for(i = 0; i < colnum; i++)
  {
    for(j = 0; j < colnum; j++)
    {
      Serial.print(currentstate[i][j], BIN);
      if(i < colnum - 1)
        Serial.print(", ");
      else
        Serial.println("");
    }
  }
  Serial.println("");
}

void setup()
{
  for(i = 0; i < rownum; i++)
    pinMode(rowpin[i], OUTPUT); //行を OUTPUT に初期化
  for(i = 0; i < colnum; i++)
    pinMode(colpin[i], INPUT_PULLUP);  //列を INPUT に初期化
  for(i = 0; i < rownum; i++)
  {
    for(j = 0; j < colnum; j++)
    {
      currentstate[i][j] = LOW; //状態を全て LOW に初期化
      beforestate[i][j] = LOW;  //状態を全て LOW に初期化
    }
    digitalWrite(rowpin[i], HIGH); //行を全て HIGH に初期化
  }
  Serial.begin(9600);
  Keyboard.begin();
}

void loop()
{
  for(i = 0; i < rownum; i++)
  {
    digitalWrite(rowpin[i], LOW); //欲しい行を LOW にする
    #if debug
    Serial.print("rowpin[");
    Serial.print(i);
    Serial.println("] is LOW.");
    #endif
    for(j = 0; j < colnum; j++)
    {
      currentstate[i][j] = !digitalRead(colpin[j]);  //行を次々と読んでいく
      #if debug
      Serial.print("colpin[");
      Serial.print(j);
      Serial.println("] being read.");
      #endif
      if(currentstate[i][j] != beforestate[i][j]) //今の状態と前回の状態が違う時
      {
        #if pushdebug
        //その座標を表示
        Serial.print("key(");
        Serial.print(i);
        Serial.print(", ");
        Serial.print(j);
        Serial.print(")");
        #endif
        if(currentstate[i][j])
        {
          #if pushdebug
          Serial.println("Push!");
          #endif
          #if keydebug
          Keyboard.press(keymap[i][j]);
          #endif
        }
        else
        {
          #if pushdebug
          Serial.println("Release!");
          #endif
          #if keydebug
          Keyboard.release(keymap[i][j]);
          #endif
        }
        beforestate[i][j] = currentstate[i][j];
      }
      #if mapdebug
      if(i == rownum - 1 && j == colnum - 1)
        printall();
      #endif
    }
    digitalWrite(rowpin[i], HIGH);
    #if debug
    Serial.print("rowpin[");
    Serial.print(i);
    Serial.println("] is HIGH");
    #endif
    delay(debouncetime);
  }
}
