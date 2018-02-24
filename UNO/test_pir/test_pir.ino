const int pir_out = 8;
const int led_pin = 10;

void setup() {
    pinMode(pir_out, INPUT);
    pinMode(led_pin, OUTPUT);
    digitalWrite(pir_out, LOW);
    digitalWrite(led_pin, LOW);
    Serial.begin(9600);
}
int pir_trigger;
void loop() {
    pir_trigger = digitalRead(pir_out);
    if (pir_trigger == HIGH) {
        Serial.println("Motion Detected");
        digitalWrite(led_pin, HIGH);
    }
    else {
        Serial.println("No Motion");
        digitalWrite(led_pin, LOW);
    }
    delay(10);
}
