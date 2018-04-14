/*

    A sketch to test my 1.5 in x 1.5 in fsr

*/


const int FSR_PIN = A0; // connected to one pin of fsr

const int GRAPH_LENGTH = 127; // length of line graph (Max MIDI value)

void setup() {
    Serial.begin(115200);
    pinMode(FSR_PIN, INPUT);
    Serial.print("Setup Finished\n");
}

void loop() {
    // read the fsr's ADC value
    int fsrADC = analogRead(FSR_PIN);
    // Map the 10 bit (0 - 1023) value to midi range (0 - 127)
    int fsrPosition = map(fsrADC, 0, 1023, 0, GRAPH_LENGTH);
    
    // print a line graph
    Serial.print("<"); // starting end
    for (int i = 0; i < GRAPH_LENGTH; i++) {
        if (i == fsrPosition) {
            Serial.print("|");
        }
        else {
            Serial.print("-");
        }
    }
    Serial.print("> (" + String(fsrADC) + ")\n");
    delay(100);
}

