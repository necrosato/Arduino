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

  // trollface linux
  // By Naookie Sato
  delay(1000);

  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press('t');
  Keyboard.releaseAll();

  delay(500);
  Keyboard.print("wget -P ~/Desktop/ http://i0.kym-cdn.com/photos/images/newsfeed/000/096/044/trollface.jpg");
  delay(100);
  typeKey(KEY_RETURN);
  delay(100);
  Keyboard.print("display ~/Desktop/trollface.jpg");
  delay(100);
  typeKey(KEY_RETURN);
  delay(100);
  // Ending stream
  Keyboard.end();
}

/* Unused endless loop */
void loop() {delay(10000);}
