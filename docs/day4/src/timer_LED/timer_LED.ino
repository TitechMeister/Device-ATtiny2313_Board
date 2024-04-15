int count = 9999;                           //カウントダウン用の変数
int timerdigit = 1;                         //表示桁管理用の変数
int timercount = 0;                         //タイマ割り込みが何回発生したか管理する変数

unsigned char num[10] = {0b11111100, 0b01100000, //0b~~~~~~~~は数字の二進数表示を表す。
                        0b11011010, 0b11110010,
                        0b01100110, 0b10110110,
                        0b10111110, 0b11100000,
                        0b11111110, 0b11110110}; //7セグの各数字のパーツごとのHIGH/LOW。順にABCDEFG(DP)

unsigned char mask[8] = {0b10000000, 0b01000000,
                        0b00100000, 0b00010000,
                        0b00001000, 0b00000100,
                        0b00000010, 0b00000001}; //マスクビット

void display(char digit, int dispnum) {  //digitは表示する桁(1~4), dispnumは表示する値(0~9)
  for (int i = 0; i < 8; i++) {
    unsigned char LED = num[dispnum] & mask[i]; //countの数字のLED表示について、上i桁目のHIGH/LOWを考える
    if (LED == 0) {                          //上i桁目が0ならば、マスクビットとのAND演算によってLEDは0b00000000になっている
      digitalWrite(i, LOW);                  //よってi番目のピンをLOWに設定
    }
    else {                                   //そうでなければ、上i桁目は1
      digitalWrite(i, HIGH);                 //よってi番目のピンをHIGHに設定
    }
  }

  for (int i = 8; i < 12; i++) { //何桁目を表示して何桁目を表示しないのか
    if (i == digit+7) {
      digitalWrite(digit+7, LOW); //digit1->8ピン, digit2->9ピン, digit3->10ピン, digit4->11ピンをLOW
    }
    else {
      digitalWrite(i, HIGH); //表示したい桁以外は無効化
    }

  }
}

ISR (TIMER1_COMPA_vect) {
  if (timerdigit == 1) {
    display(1, count/1000);        //上1桁目(1000の位)の表示
  } else if (timerdigit == 2) {
    display(2, (count%1000)/100);  //上2桁目(100の位)の表示
  } else if (timerdigit == 2) {
    display(3, (count%100)/10);    //上3桁目(10の位)の表示
  } else if (timerdigit == 3) {
    display(4, count%10);          //上4桁目(1の位)の表示
  }

  timerdigit = timerdigit + 1;
  if (timerdigit == 5) {
    timerdigit = 1;
  }

  timercount = timercount + 1;
  if (timercount == 100) {
    timercount = 0;
    count = count - 1;
    if (count == 0) {
      count = 9999;
    }
  }
  
}

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 12; i++) {
    pinMode(i, OUTPUT);   //0~11ピンの出力機能を有効化、すなわちA~DP, digit1~4に出力できるように
  }

  TCCR1A = 0b00000000;
  TCCR1B = 0b00001101;
  TIMSK = 0b01000000;

  OCR1AL = 40; //(8で割った値)msごとにタイマ割り込みが発生。MAX255

  sei();

}

void loop() {
  // put your main code here, to run repeatedly:

}
