void setup() {
  // put your setup code here, to run once:
  pinMode(12, OUTPUT);    //LED有効化
  pinMode(13, INPUT);     //switch有効化
}

void loop() {
  // put your main code here, to run repeatedly:
  bool button = digitalRead(13); //buttonの値を保存
  if (button) {                  //もしbuttonが押されてたら...
    digitalWrite(12, HIGH);      //LEDをON
  }
  else {                         //そうでなければ...
    digitalWrite(12, LOW);       //LEDをOFF
  }
}