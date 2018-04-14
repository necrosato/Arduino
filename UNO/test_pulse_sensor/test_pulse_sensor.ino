/* Pulse Sensor test sketch to read raw 10 bit values */

int PULSE_DATA = A0;
int GRAPH_LENGTH = 127; // Midi max value
int thresh = 520; //threshold for a hearbeat

void setup() {
    Serial.begin(115200);
    pinMode(PULSE_DATA, INPUT);
    Serial.print("Setup Finished\n");
}

int signal;

void loop() {
    // read the pulse sensors's ADC value
    signal = analogRead(PULSE_DATA);
    // Map the 10 bit (0 - 1023) value to midi range (0 - 127)
    int signal_midi = map(signal, 0, 1023, 0, GRAPH_LENGTH);
    
    /*
    // print a line graph
    Serial.print("<"); // starting end
    for (int i = 0; i < GRAPH_LENGTH; i++) {
        if (i == signal_midi) {
            Serial.print("|");
        }
        else {
            Serial.print("-");
        }
    }
    Serial.print("> (" + String(signal) + ")\n");
    */
    Serial.println(signal);
    delay(10);
}

