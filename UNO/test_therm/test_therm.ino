void setup() {
      
      Serial.begin(115200);\
      delay(200);
     
      // We start by connecting to a WiFi network
     
      Serial.println();
      Serial.println();
 }   
     
void loop() {
        Serial.print("Reading analog bits: ");
        Serial.println(analogRead(A0));
        Serial.print(getTemp(A0));
        Serial.println(" F");
    delay(1000); // check for connection every once a second
}

float getTemp(int pin) {
    int tempReading = analogRead(pin);
    // This is OK
    double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
    tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
    float tempC = tempK - 273.15;            // Convert Kelvin to Celcius
    float tempF = (tempC * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
    return tempF;
}
