//
// battery-meter.ino
// Naookie Sato
//
// Created by Naookie Sato on 05/08/2020
// Copyright © 2020 Naookie Sato. All rights reserved.
//

void setup()
{
  Serial.begin(115200);
  delay(1000);
}

void loop()
{
  auto analog = analogRead(0);
  Serial.print("Analog: ");
  Serial.println(analog);
  
  float voltage = map(analog, 0, 1024, 0, 420);
  Serial.print("Voltage: ");
  Serial.println(voltage / 100.0);

  int min_voltage = 300;
  int percent = voltage < min_voltage ? 0 : map(voltage, min_voltage, 420, 0, 100);
  Serial.print("Percent: ");
  Serial.println(percent);
  delay(1000);
}
