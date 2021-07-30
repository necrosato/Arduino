//
// multiplexer.ino
// Naookie Sato
//
// Created by Naookie Sato on 05/06/2020
// Copyright © 2020 Naookie Sato. All rights reserved.
//

int pinA = D0;
int pinB = D1;
int pinC = D2;

void setup()
{
  Serial.begin(115200);
  delay(1000);
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
}

void loop()
{
  digitalWrite(pinA, LOW);
  digitalWrite(pinB, LOW);
  digitalWrite(pinC, LOW);
  delay(10);
  auto analog1 = analogRead(0);
  digitalWrite(pinA, LOW);
  digitalWrite(pinB, LOW);
  digitalWrite(pinC, HIGH);
  delay(10);
  auto analog2 = analogRead(0);
  Serial.print("Analog 1: ");
  Serial.println(analog1);
  Serial.print("Analog 2: ");
  Serial.println(analog2);
  delay(1000);
}
