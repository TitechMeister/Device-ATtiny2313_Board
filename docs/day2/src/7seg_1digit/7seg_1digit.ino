int count = 0;                                   //カウントアップ用の変数

unsigned char num[10] = {0b11111100, 0b01100000, //0b~~~~~~~~は数字の二進数表示を表す。
                        0b11011010, 0b11110010,
                        0b01100110, 0b10110110,
                        0b10111110, 0b11100000,
                        0b11111110, 0b11110110}; //7セグの各数字のパーツごとのHIGH/LOW。順にABCDEFG(DP)

unsigned char mask[8] = {0b10000000, 0b01000000,
                        0b00100000, 0b00010000,
                        0b00010000, 0b00000100,
                        0b00000010, 0b00000001}; //マスクビット

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 9; i++) {
    pinMode(i, OUTPUT);   //0~8ピンの出力機能を有効化、すなわちA~DP, digit1に出力できるように
  }
  digitalWrite(8, LOW);   //digit1を有効化

}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 8; i++) {
    unsigned char LED = num[count] & mask[i]; //countの数字のLED表示について、上i桁目のHIGH/LOWを考える
    if (LED == 0) {                          //上i桁目が0ならば、マスクビットとのAND演算によってLEDは0b00000000になっている
      digitalWrite(i, LOW);                  //よってi番目のピンをLOWに設定
    }
    else {                                   //そうでなければ、上i桁目は1
      digitalWrite(i, HIGH);                 //よってi番目のピンをHIGHに設定
    }
  }

  count = count + 1; //カウントアップ
  if (count >= 10) { //カウントアップしすぎた場合は0に戻す
    count = 0;
  }
  delay(1000);       //1秒まつ

}
