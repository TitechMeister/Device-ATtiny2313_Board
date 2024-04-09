[前回](https://github.com/TitechMeister/Device-ATmega88_Board/tree/main/docs/day3/)の続きです。今回はタイマ割り込みを使って数字を表示します。

---

# 目次

* [はじめに(ハードウェア編)](https://github.com/TitechMeister/Device-ATmega88_Board/tree/main/docs/day0/)
* [はじめに(ソフトウェア編)](https://github.com/TitechMeister/Device-ATmega88_Board/tree/main/docs/day0.5/)
* [第1回 レジスタとLチカ](https://github.com/TitechMeister/Device-ATmega88_Board/tree/main/docs/day1/)
* [第2回 7セグメントLED](https://github.com/TitechMeister/Device-ATmega88_Board/tree/main/docs/day2/)
* [第3回 ダイナミック点灯](https://github.com/TitechMeister/Device-ATmega88_Board/tree/main/docs/day3/)
* 第4回 タイマ割り込み ←ここ
* [第5回 サーミスタとAD変換](https://github.com/TitechMeister/Device-ATmega88_Board/tree/main/docs/day5/)

---

## 18. 前回のプログラムの問題点

[前回](https://github.com/TitechMeister/Device-ATmega88_Board/tree/main/docs/day3/)は7セグで4桁の数字を表示しました。しかし、前回の方法だと7セグを点灯している間は他の処理ができなくなってしまいます。これは、点灯した後に5ms待機する処理をCPU側でやってしまっているためです。今回は、**タイマ割り込み**というものを使ってこの問題を解決します。

## 19. タイマ割り込み

CPUはクロックの合図に合わせて演算を行います。前回のプログラムでは、5ms経過することをCPU側でクロックを数えていました。何もしないプログラムを実行した回数で測っていたわけです。どれだけ無駄なことをしているかというと、時計が目の前にあるのに体内時計を使って１秒ずつ数えているようなものです。
この時計を見る機能があります。それが~~大麻~~タイマカウンタです。タイマカウンタは、クロックが何回数えたかを教えてくれる部品です。
タイマカウンタが数えた値がある値になったとき、CPUの処理を中断して特定の処理が実行されます。このように、CPUの処理を中断させて特定の処理を実行することを**割り込み**といいます。
タイマ割り込みをすることによって、一定時間待つという処理をCPUとは独立に行うことができます。

### 20. タイマカウンタの使い方

タイマカウンタをしっかり使おうとすると、思いの外設定することが多いので、今回はこちらで設定内容を決めてきました。以下のように、setupの末尾にこの5文を追加してください。

```cpp
setup() {

    ...

  TCCR0A = 0b00000010;
  TCCR0B = 0b00000101;
  TIMSK = 0b00000001;

  OCR0A = 40; //(8で割った値)msごとにタイマ割り込みが発生。MAX255

  sei();
}
```

コメントにも書きましたが、OCR0Aの値を変更するとタイマ割り込みの頻度を変えることが出来ます。他の値はいじらないように注意してください。

## 21. 割り込みハンドラ

割り込みで呼び出される処理は

```cpp
ISR(TIMER0_COMPA_vect){
    /* 処理 */
}
```

に書きます。タイマ割り込みが発生すると、ここに書かれた処理が実行されます。（その前に行っていた処理は中断されます）

## 22. プログラム

では実際に、プログラムを書いてみましょう！前回と同じ「9999から１秒ごとにカウントダウンしていく」プログラムを、タイマ割り込みを使って書いてみてください。

↓プログラム例

<details>

```cpp
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
                        0b00010000, 0b00000100,
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

ISR (TIMER0_COMPA_vect) {
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

  TCCR0A = 0b00000010;
  TCCR0B = 0b00000101;
  TIMSK = 0b00000001;

  OCR0A = 40; //(8で割った値)msごとにタイマ割り込みが発生。MAX255

  sei();

}

void loop() {
  // put your main code here, to run repeatedly:

}

```

</details>

## 23. プログラムの解説

---

今回はここまでです。[次回](https://github.com/TitechMeister/Device-ATmega88_Board/tree/main/docs/day5/)はいよいよスロットマシンを作ります。
