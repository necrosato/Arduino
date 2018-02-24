#include <Servo.h>

Servo sv;

int pos = 0;

void setup() {
    sv.attach(3);
}
void loop() {
    sv.write(15);
    delay(3000);
    sv.write(150);
    delay(3000);
}
