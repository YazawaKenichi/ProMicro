//2x2 を 5x6 に拡張
//参考：https://eucalyn.hatenadiary.jp/entry/original-keyboard-07
//
//////////////////////////暴走時退避場所//////////////////////////
//
//
//
//
//
////////////////////////////////////////////////////////////////

#include <Keyboard.h>

#define debug 0
#define pushdebug 1
#define mapdebug 0
#define keydebug 1

const int rownum = 5; //行の数
const int colnum = 6; //列の数

const int rowpin[rownum] = {2, 3, 4, 5, 6};  //赤が 2 ピン
const int colpin[colnum] = {7, 8, 9, 15, 14, 16};  //赤が 7 ピン
const byte keymap[rownum][colnum] = {{KEY_ESC, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5}, {KEY_TAB, 'q', 'w', 'e', 'r', 't'}, {KEY_CAPS_LOCK, 'a', 's', 'd', 'f', 'g'}, {KEY_LEFT_SHIFT, 'z', 'x', 'c', 'v', 'b'}, {KEY_LEFT_CTRL, KEY_LEFT_GUI, KEY_LEFT_ALT, '\n', ' '}};  //出力したい ASCII CODE をマッピングする

bool currentstate[rownum][colnum];  //現在の入力状態を記録する
bool beforestate[rownum][colnum]; //前回の入力状態を記憶する

int i, j;

void printall()
{
  Serial.println("");
  Serial.println("Printing Map Status");
  for (i = 0; i < colnum; i++)
  {
    for (j = 0; j < colnum; j++)
    {
      Serial.print(currentstate[i][j], BIN);
      if (i < colnum - 1)
        Serial.print(", ");
      else
        Serial.println("");
    }
  }
  Serial.println("");
}

void setup()
{
  for (i = 0; i < rownum; i++)
    pinMode(rowpin[i], OUTPUT); //行を OUTPUT に初期化
  for (i = 0; i < colnum; i++)
    pinMode(colpin[i], INPUT_PULLUP);  //列を INPUT に初期化
  for (i = 0; i < rownum; i++)
  {
    for (j = 0; j < colnum; j++)
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
  for (i = 0; i < rownum; i++)
  {
    digitalWrite(rowpin[i], LOW); //欲しい行を LOW にする
#if debug
    Serial.print("rowpin[");
    Serial.print(i);
    Serial.println("] is LOW.");
#endif
    for (j = 0; j < colnum; j++)
    {
      currentstate[i][j] = !digitalRead(colpin[j]);  //行を次々と読んでいく
#if debug
      Serial.print("colpin[");
      Serial.print(j);
      Serial.println("] being read.");
#endif
      if (currentstate[i][j] != beforestate[i][j]) //今の状態と前回の状態が違う時
      {
#if pushdebug
        //その座標を表示
        Serial.print("key(");
        Serial.print(i);
        Serial.print(", ");
        Serial.print(j);
        Serial.print(") ");
        Serial.print(char(keymap[i][j]));
        Serial.print(" is ");
#endif
        if (currentstate[i][j])
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
      if (i == rownum - 1 && j == colnum - 1)
        printall();
#endif
    }
    digitalWrite(rowpin[i], HIGH);
#if debug
    Serial.print("rowpin[");
    Serial.print(i);
    Serial.println("] is HIGH");
#endif
    delay(0.1);
  }
}

//////////////////////////////////わかったこと///////////////////////////
/*

   「push と release が区別されていて、push した後は release するまで送信される」
   この規則のおかげで、ショートカットを実現できる。
   ただし、release する必要が出てくる。（これの利点は後述）
   Raspberry Pi Pico より良いところとして、
   CircuitPython に用意されている keyboard.send() メソッドでは、
   メソッドが呼び出された一瞬しかコードを送信しない性質上。
   ショートカットする時、引数に二つの送信コードを記述する必要があり、
   押されたキーの数も考慮してコーディングする必要が出てきて面倒。
   （脳筋プログラムになる）

   わざわざ release しなきゃいけない利点として、
   キースキャン二週目の時にスイッチがオフなら、そこでリリースすることで、
   スキャニングを一周するまでに押されているほかのキーも同時に push された状態になっている。
   これにより理論上無限の数のキーを同時押しできる。

   Arduino の Keyboard.h の、KEY_RETURN は「Enter キー」という意味での Enter として使うのは良くない。
   ∵ここにおける KEY_RETURN はガチで改行しかしない。
   i.e. 日本語入力で、変換する文字を確定するための Enter として利用することはできない。
   '\n' i.e. ASCII Code における LF を指定すれば KEY_RETURN ではなく、
   Enter キーとして出力されるようだ。

*/
///////////////////////////////////////////////////////////////////////
