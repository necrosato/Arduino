/*
 * Generated with <3 by Dckuino.js, an open source project !
 */

#include "Keyboard.h"

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

  // Wait 500ms
  delay(500);

  // esp ducky test
  // By Naookie Sato
  delay(1000);

  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press(' ');
  Keyboard.releaseAll();
  delay(100);
  Keyboard.print("terminal");
  typeKey(KEY_RETURN);

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
