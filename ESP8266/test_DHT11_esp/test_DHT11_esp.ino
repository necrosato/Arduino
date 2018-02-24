#include "nodeMCU_GPIO.h"
#include "SimpleDHT.h"
//#include "LiquidCrystal.h"
#define DHT11 D3

SimpleDHT11 sensor;
//LiquidCrystal lcd(D1, D2, D4, D5, D6, D7);

void setup() {
    Serial.begin(9600);
//    lcd.begin(16, 2);
//    lcd.print("Current Temp:");
}

void loop() {
//    lcd.setCursor(0, 1);

    byte temp = 0;
    byte humid = 0;
    byte data[40] ={0};
    if (sensor.read(DHT11, &temp, &humid, data)) {
        Serial.print("Read DHT11 failed");
        return;
    }
    Serial.print("Current temp is "); Serial.print((int)temp); Serial.println(" C");
    delay(500);
}
