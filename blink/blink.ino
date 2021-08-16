#define activelow 0 //source を使いたいときは 1 にする

#if !activelow
const int source = 10; //マイコンから流出
#else
#define sink source
const int sink = 10;  //マイコンに流入
#endif

const int rate = 9;
unsigned long int value;

void setup()
{
//  pinMode(source, OUTPUT);  //出力に設定
  pinMode(rate, INPUT); //入力に設定
  Serial.begin(9600);
}

void loop()
{
  value = analogRead(rate);
  Serial.print("analogWrite(");
  #if !activelow
  analogWrite(source, value * 255 / 1023); //PWM 波形の出力
  Serial.print(value * 255 / 1023);
  #else
  analogWrite(source, 255 - value * 255 / 1023); //PWM 波形の出力（H, L 逆転）
  Serial.print(255 - value * 255 / 1023);
  #endif
  Serial.println(");");
}

///実験結果メモ///
//電流制限抵抗 5kΩ としたとき。
//
// value = 255 でも死なない。（PWM デューティー比 100%）
// value = 170 くらいが、安全でありかつちゃんとバックライトらしい明るさになる。
//（PWM デューティー比 2/3）
// これより大幅に小さくするとちょっとバックライトとしては暗いかなって印象になる。
//
////////////////
