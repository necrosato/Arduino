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
  // trollface mac
  // By Naookie Sato
/*
  delay(100);
  typeKey(KEY_RETURN);
  delay(100);
  typeKey(KEY_RETURN);
  delay(100);
  typeKey(KEY_RETURN);
  delay(100);
  typeKey(KEY_RETURN);
  delay(100);
  typeKey(KEY_RETURN);
*/
  delay(500);
  typeKey(KEY_ESC);
  delay(500);
  typeKey(KEY_ESC);
  delay(500);
  typeKey(KEY_RETURN);
  delay(500);
  typeKey(KEY_ESC);
  delay(500);
  Keyboard.press(KEY_LEFT_GUI);
  delay(100);
  Keyboard.press(' ');
  delay(100);
  Keyboard.releaseAll();
  delay(100);
  Keyboard.print("terminal");
  delay(100);
  typeKey(KEY_RETURN);
  delay(500);
  Keyboard.print("wget -P ~/Desktop/ http://i0.kym-cdn.com/photos/images/newsfeed/000/096/044/trollface.jpg");
  delay(100);
  typeKey(KEY_RETURN);
  delay(100);
  Keyboard.print("open ~/Desktop/trollface.jpg");
  delay(100);
  typeKey(KEY_RETURN);
  delay(100);
  // Ending stream
  Keyboard.end();
}

/* Unused endless loop */
void loop() {delay(10000);}
