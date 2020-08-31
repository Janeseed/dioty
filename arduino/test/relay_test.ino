 int relay = 13; // 릴레이가 연결된 핀 번호
int onoff = 14;
int val = 0;

void setup() {
Serial.begin(115200);
pinMode(relay, OUTPUT);
pinMode(onoff, INPUT);
digitalWrite(relay, LOW);
}

void loop() {
  val = digitalRead(onoff);
  if (val == 0) {
    digitalWrite(relay, HIGH);
  } else if(val == 1) {
    digitalWrite(relay, LOW);
  }
}
