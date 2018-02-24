
// Arduino pin numbers
const int SW_pin = 7; // digital pin connected to switch output
const int X_pin = A0; // analog pin connected to X output
const int Y_pin = A1; // analog pin connected to Y output

const int rled = 9;
const int gled = 10;
const int bled = 11;

void setup() {
    pinMode(SW_pin, INPUT);
    pinMode(rled, INPUT);
    pinMode(gled, INPUT);
    pinMode(bled, INPUT);
    digitalWrite(rled, LOW);
    digitalWrite(gled, LOW);
    digitalWrite(bled, LOW);
    digitalWrite(SW_pin, HIGH);
    Serial.begin(115200);
}


int xval = 0, yval = 0;
int sw_stat = 0, combo = 0;
void loop() {
    xval = analogRead(X_pin); //these are on range [0,1023]
    yval = analogRead(Y_pin);
    sw_stat = digitalRead(SW_pin);// LOW or HIGH
    if (sw_stat == LOW) {
        combo = (combo + 1) % 6;
        return;
    }

    int delta1 = xval / 4;
    int delta2 = yval / 4;
    int dpin1, dpin2, clearpin;
    
    switch (combo) {
    case 0:
        dpin1 = rled;
        dpin2 = gled;
        clearpin = bled;
        break;
    case 1:
        dpin1 = rled;
        dpin2 = bled;
        clearpin = gled;
        break;
    case 2:
        dpin1 = bled;
        dpin2 = gled;
        clearpin = rled;
        break;
    case 3:
        dpin1 = bled;
        dpin2 = rled;
        clearpin = gled;
        break;
    case 4:
        dpin1 = gled;
        dpin2 = rled;
        clearpin = bled;
        break;
    case 5:
        dpin1 = gled;
        dpin2 = bled;
        clearpin = rled;
        break;
    }
   
    analogWrite(dpin1, delta1);
    analogWrite(dpin2, delta2);
    analogWrite(clearpin, 0);
    delay(10);
}
