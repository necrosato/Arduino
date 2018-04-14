/*

    A sketch to test my 200 mm spectra symboll linear soft potentiometers

*/


const int SOFT_POT_PIN = A0; // connected to soft pot wiper (middle pin on soft pot)

const int GRAPH_LENGTH = 127; // length of line graph (Max MIDI value)

void setup() {
    Serial.begin(115200);
    pinMode(SOFT_POT_PIN, INPUT);
    Serial.print("Setup Finished\n");
}

void loop() {
    // read the soft pot's ADC value
    int softPotADC = analogRead(SOFT_POT_PIN);
    // Map the 10 bit (0 - 1023) value to midi range (0 - 127)
    int softPotPosition = map(softPotADC, 0, 1023, 0, GRAPH_LENGTH);
    
    // print a line graph
    Serial.print("<"); // starting end
    for (int i = 0; i < GRAPH_LENGTH; i++) {
        if (i == softPotPosition) {
            Serial.print("|");
        }
        else {
            Serial.print("-");
        }
    }
    Serial.print("> (" + String(softPotADC) + ")\n");
    delay(100);
}

