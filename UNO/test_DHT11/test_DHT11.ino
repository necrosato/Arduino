#include "SimpleDHT.h"
#define DHT11 2
SimpleDHT11 sensor;

void setup() {
    Serial.begin(9600);
}

void loop() {
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
