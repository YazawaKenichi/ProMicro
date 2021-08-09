//キーパッド
//参考：https://okiraku-camera.tokyo/blog/?p=7988

const uint8_t line[] = {2, 3, 4, 5, 6, 7};  //出力ピン
const uint8_t col[] = {8, 9, 10, 11, 12}; //入力ピン

void setup()
{
  Serial.begin(115200); //通信速度の設定
  for(int i = 0; i < sizeof(line) / sizeof(uint8_t); i++) //
  {
    pinMode(line[i], OUTPUT); //出力に設定
    digitalWrite(line[i], HIGH);  //HIGH に初期化
  }
  for(int i = 0; i < sizeof(col) / sizeof(uint8_t); i++)
    pinMode(col[i], INPUT_PULLUP);  //プルアップ入力に設定
}

int key_scan()  //
{
  int matrix_status = 0;
  for(int i = 0; i < sizeof(line) / sizeof(uint8_t); i++)
  {
    digitalWrite(line[i], LOW);
    int col_data = 0;  //キー全体の状態を格納
    for(int j = 0; j < sizeof(col) / sizeof(uint8_t); j++)
      col_data += ((!digitalRead(col[j]) ? 1 : 0) << j);  //列ごとに状態を計算
    matrix_status += (col_data << (sizeof(col) / sizeof(uint8_t) * i)); //キー全体の状態を計算
    digitalWrite(line[i], HIGH);
  }
  return matrix_status;
}

const uint8_t key_codes[] = {}; //ここにキーコードを入力していく。
void loop()
{
  static int last_key_pressed = 0;
  int key_pressed = key_scan();
  int change = key_pressed ^ last_key_pressed;
  if(change)
  {
    int mask = 1;
    uint8_t code;
    for(int i = 0; i < sizeof(key_codes) / sizeof(uint8_t); i++)
    {
      if(change & mask)
      {
//        code = key_codes[i] | (key_pressed & mask ? 0 : 0x80);  //ここ数字限定のプログラムになってるから改良した方がいい。
//        char tmp[4];
//        sprintf(&tmp, "%02x ", code);  //文字列を合成して tmp 配列に格納？
//        Serial.print(tmp);  //文字列の表示
      }
      mask = mask << 1;
    }
    last_key_pressed = key_pressed;
    Serial.println();
  }
  delay(75);  //チャタリング対策。

}
/*
    line[0] の col[0] が下位ビット
    col[1] が次のビット
    col[2] が次のビット
    col[3] が次のビット
    col[sizeof(col) / sizeof(uint8_t)] が line[0] の最後のビット
    line[1] の col[0] がその次のビット
*/
