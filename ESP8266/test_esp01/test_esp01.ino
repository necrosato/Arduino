void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(0, INPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(2, INPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  Serial.begin(115200);
  delay(1000);
  if (digitalRead(0) == HIGH) {
    Serial.println("GPIO0 is pulled high");
  }
  else {
    Serial.println("GPIO0 is pulled low");
  }
  if (digitalRead(2) == HIGH) {
    Serial.println("GPIO2 is pulled high");
  }
  else {
    Serial.println("GPIO2 is pulled low");
  }
  Serial.println(LED_BUILTIN);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  delay(1000);                      // Wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)
}
