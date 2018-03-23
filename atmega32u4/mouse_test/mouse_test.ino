#include "Keyboard.h"
#include "Mouse.h"

void load_delay() {
    while(1) {
        delay(1000);
        Serial.println("Waiting for upload...");
    }
}

void typeKey(int key)
{
  Keyboard.press(key);
  delay(50);
  Keyboard.release(key);
}

/* Init function */
void setup()
{
  // Begining the Keyboard stream
  Keyboard.begin();

  // Begin the mouse
  Mouse.begin();

  // micro ducky mouse test
  // By Naookie Sato
  delay(1000);

  // pushbutton setup, check for new script
  pinMode(8, INPUT);
  delay(1000);
  if(digitalRead(8) == HIGH) {
    Serial.begin(115200);
    load_delay();
  }
}

void loop() {
    Mouse.move(100, 0);
    delay(500);
    Mouse.move(0, 100);
    delay(500);
    Mouse.move(-100, 0);
    delay(500);
    Mouse.move(0, -100);
    delay(500);
}
