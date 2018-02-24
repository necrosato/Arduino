static const uint8_t D4   = 2;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(D4, OUTPUT);
  digitalWrite(D4, LOW);   // turn the LED off (LOW is the voltage level)
    delay(5000);
  digitalWrite(D4, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(5000);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(D4, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(D4, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
