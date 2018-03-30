/*
 * Generated with <3 by Dckuino.js, an open source project !
 */

#include "Keyboard.h"

void load_delay() {
    while(1) {
        delay(1000);
        Serial.println("Waiting for upload...");
    }
}

  // pushbutton setup, check for new script

  pinMode(8, INPUT);
  delay(1000);
  if(digitalRead(8) == HIGH) {
    Serial.begin(115200);
    load_delay();
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

  // pushbutton setup, check for new script

  pinMode(8, INPUT);
  delay(1000);
  if(digitalRead(8) == HIGH) {
    Serial.begin(115200);
    load_delay();
  }

  // Wait 500ms
  delay(500);

  // esp ducky test
  // By Naookie Sato
  delay(1000);

  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press('t');
  Keyboard.releaseAll();

  delay(1000);
  Keyboard.print("echo \"hello world\"");
  //delay(100);

  typeKey(KEY_RETURN);
  delay(100);
  Keyboard.print("ls -al; pwd");
  typeKey(KEY_RETURN);

  // Ending stream
  Keyboard.end();
}

/* Unused endless loop */
void loop() {}
