int blue = 3;
int green = 5;
int red = 6;

void setup() {
  // put your setup code here, to run once:
    pinMode(blue, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(red, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
    //test
    for (int i = 1; i < 15; i++) {
    analogWrite(blue, i);
    analogWrite(red, i);
    delay(20);
    analogWrite(red, 0);
    delay(20);
    analogWrite(green, i);
    delay(20);
    analogWrite(blue, 0);
    delay(20);
    analogWrite(red, i);
    delay(20);
    analogWrite(green, 0);
    delay(20);
    }
    for (int i = 15; i > 0; i--) {
    analogWrite(blue, i);
    analogWrite(red, i);
    delay(20);
    analogWrite(red, 0);
    delay(20);
    analogWrite(green, i);
    delay(20);
    analogWrite(blue, 0);
    delay(20);
    analogWrite(red, i);
    delay(20);
    analogWrite(green, 0);
    delay(20);
    }
}
