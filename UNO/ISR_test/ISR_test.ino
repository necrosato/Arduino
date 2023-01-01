//
// ISR_test.ino
// Naookie Sato
//
// Created by Naookie Sato on 04/30/2022
// Copyright © 2022 Naookie Sato. All rights reserved.
//

int tpin = 2;

void isr()
{
    Serial.println("interrupted!"); 
}

void setup()
{
    Serial.begin(9600);\
    delay(200);
    Serial.println();
    pinMode(tpin, INPUT_PULLUP);
    attachInterrupt( digitalPinToInterrupt(tpin), isr, FALLING);
}

void loop()
{
    Serial.println("begin wait");
    delay(2000);
    Serial.println("end wait");
}
