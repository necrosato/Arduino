#include "SR04.h"

#define GREEN 3
#define YELLOW 5
#define RED 6
#define POWER 13
#define ECHO 12
#define TRIG 11
#define VIN 9

SR04 prox_sense = SR04(ECHO, TRIG);

void setup() {
    pinMode(GREEN, OUTPUT);
    pinMode(YELLOW, OUTPUT);
    pinMode(RED, OUTPUT);
    pinMode(POWER, OUTPUT);
    pinMode(VIN, OUTPUT);
    digitalWrite(POWER, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(RED, LOW);
    digitalWrite(VIN, LOW);
    Serial.begin(9600);
}

int d = 0;
int pwm = 127;
int delay1 = 1;
int delay2 = 2;
void loop() {
    int r = prox_sense.Distance();
    if (r != 0)
        d = r;
    pwm = d;
    Serial.print("Current Distance: ");   
    Serial.println(d);   

    if (d >= 100) {
        analogWrite(GREEN, pwm);
        analogWrite(YELLOW, LOW);
        analogWrite(RED, LOW);
    }else if (d < 100 && d > 10) {
        analogWrite(GREEN, LOW);
        analogWrite(YELLOW, pwm);
        analogWrite(RED, LOW);
    }else {
        int i;
        analogWrite(GREEN, LOW);
        analogWrite(YELLOW, LOW);
        analogWrite(RED, pwm);
        for(i=0;i<250;i++) {
            digitalWrite(VIN,HIGH);
            delay(delay1);//wait for 1ms
            digitalWrite(VIN,LOW);
            delay(delay1);//wait for 1ms
        }
        //output another frequency
        Serial.println(delay2);
        for(i=0;i<250;i++) {
            digitalWrite(VIN,HIGH);
            delay(delay2);//wait for 1ms
            digitalWrite(VIN,LOW);
            delay(delay2);//wait for 1ms
        }
    }
    delay(10);
}
