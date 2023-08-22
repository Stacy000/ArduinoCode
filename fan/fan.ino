int fanIn1=43;
int fanIn2=45;
int fanIn3=47;
int fanIn4=49;;

void setup() {
  // put your setup code here, to run once:
  pinMode(fanIn1, OUTPUT);
  pinMode(fanIn2, OUTPUT);
  pinMode(fanIn3, OUTPUT);
  pinMode(fanIn4, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(fanIn1, HIGH);
  digitalWrite(fanIn2, LOW);
  digitalWrite(fanIn3, HIGH);
  digitalWrite(fanIn4, LOW);
}
