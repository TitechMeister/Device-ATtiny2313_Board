void setup() {
  // put your setup code here, to run once:
  pinMode(12, OUTPUT);    //LED有効化
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(12, HIGH); //LED ON
  delay(1000);            //1秒まつ
  digitalWrite(12, LOW);  //LED OFF
  delay(1000);            //1秒まつ
}
