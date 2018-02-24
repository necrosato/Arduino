#define BUTTON 2
#define LED 4

void setup() {
    pinMode(BUTTON, INPUT_PULLUP);
    pinMode(LED, OUTPUT);
}

int button_status = 0;
int pow_status = 0;
void loop() {
    if (digitalRead(BUTTON) == LOW && button_status == 0) {
        digitalWrite(LED, HIGH);
        pow_status = 1;
        button_status = 2;
    }
    if (digitalRead(BUTTON) == LOW && button_status == 1) {
        digitalWrite(LED, LOW);
        pow_status = 0;
        button_status = 2;
    }
    if (button_status == 2 && digitalRead(BUTTON) == HIGH) {
        button_status = pow_status;
    }
}
