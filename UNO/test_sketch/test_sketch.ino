#include "SR04.h"
// Define Pins
#define BUTTON 2
#define BLUE 3
#define GREEN 5
#define RED 6
#define VIN 8
#define ECHO 12
#define TRIG 11
SR04 sr04 = SR04(ECHO, TRIG);

void setup()
{
    pinMode(BUTTON, INPUT_PULLUP);
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
    pinMode(VIN, OUTPUT);
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
    digitalWrite(VIN, HIGH);
    Serial.begin(9600);
}

// define variables
int redValue;
int greenValue;
int blueValue;

int button_status = 0;
int pow_status = 0;

// main loop
void loop()
{
    #define delayTime 10 // fading time between colors
    Serial.print("about to loop, distance in cm: ");
    Serial.println(sr04.Distance());

    redValue = 127; // choose a value between 1 and 127 to change the color.
    greenValue = 0;
    blueValue = 0;

    for(int i = 0; i < 127; i += 1) // fades out red bring green full when i=127
    {
        if (digitalRead(BUTTON) == LOW && button_status == 0) {
            pow_status = 1;
            button_status = 2;
        }
        if (digitalRead(BUTTON) == LOW && button_status == 1) {
            pow_status = 0;
            button_status = 2;
        }
        if (button_status == 2 && digitalRead(BUTTON) == HIGH) {
            button_status = pow_status;
        }
        redValue -= 1;
        greenValue += 1;
        if (pow_status == 1) {
            analogWrite(RED, redValue);
            analogWrite(GREEN, greenValue);
        }
        else {
            analogWrite(RED, LOW);
            analogWrite(GREEN, LOW);
        }
        delay(delayTime);
    }

    redValue = 0;
    greenValue = 127;
    blueValue = 0;

    for(int i = 0; i < 127; i += 1) // fades out green bring blue full when i=127
    {
        if (digitalRead(BUTTON) == LOW && button_status == 0) {
            pow_status = 1;
            button_status = 2;
        }
        if (digitalRead(BUTTON) == LOW && button_status == 1) {
            pow_status = 0;
            button_status = 2;
        }
        if (button_status == 2 && digitalRead(BUTTON) == HIGH) {
            button_status = pow_status;
        }
        greenValue -= 1;
        blueValue += 1;
        // The following was reversed, counting in the wrong directions
        // analogWrite(GREEN, 127 - greenValue);
        // analogWrite(BLUE, 127 - blueValue);
        if (pow_status == 1) {
            analogWrite(GREEN, greenValue);
            analogWrite(BLUE, blueValue);
        }
        else {
            analogWrite(GREEN, LOW);
            analogWrite(BLUE, LOW);
        }
        delay(delayTime);
    }

    redValue = 0;
    greenValue = 0;
    blueValue = 127;

    for(int i = 0; i < 127; i += 1) // fades out blue bring red full when i=127
    {
        if (digitalRead(BUTTON) == LOW && button_status == 0) {
            pow_status = 1;
            button_status = 2;
        }
        if (digitalRead(BUTTON) == LOW && button_status == 1) {
            pow_status = 0;
            button_status = 2;
        }
        if (button_status == 2 && digitalRead(BUTTON) == HIGH) {
            button_status = pow_status;
        }
        // The following code has been rearranged to match the other two similar sections
        blueValue -= 1;
        redValue += 1;
        // The following was reversed, counting in the wrong directions
        // analogWrite(BLUE, 127 - blueValue);
        // analogWrite(RED, 127 - redValue);
        if (pow_status == 1) {
            analogWrite(BLUE, blueValue);
            analogWrite(RED, redValue);
        }
        else {
            analogWrite(BLUE, LOW);
            analogWrite(RED, LOW);
        }
        delay(delayTime);
    }
}
