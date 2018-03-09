#include <ESP8266WiFi.h>
#include "pins_arduino.h"
//////////////////////
// WiFi Definitions //
//////////////////////
const char WiFiAPPSK[] = "super_secure";
const char AP_NAME[] = "esp_ap";

/////////////////////
// Pin Definitions //
/////////////////////
const int LED_PIN = D4; // ESP's onboard, green LED
const int ANALOG_PIN = A0; // The only analog pin on the ESP
const int DIGITAL_PIN = D3; // Digital pin to be read

WiFiServer server(80);

void setup() 
{
  initHardware();
  setupWiFi();
  server.begin();
}

void loop() 
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Match the request
  int val = -1; // We'll use 'val' to keep track of both the
                // request type (read/set) and value if set.
  if (req.indexOf("/led/0") != -1)
    val = 1; // Will write LED high
  else if (req.indexOf("/led/1") != -1)
    val = 0; // Will write LED low
  else if (req.indexOf("/read") != -1)
    val = -2; // Will print pin reads
  // Otherwise request will be invalid. We'll say as much in HTML

  // Set GPIO5 according to the request
  if (val >= 0)
    digitalWrite(LED_PIN, val);

  client.flush();

  // Prepare the response. Start with the common header:
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  // If we're setting the LED, print out a message saying we did
  if (val >= 0)
  {
    s += "LED is now ";
    s += (val)?"off":"on";
  }
  else if (val == -2)
  { // If we're reading pins, print out those values:
    s += "Analog Pin = ";
    s += String(analogRead(ANALOG_PIN));
    s += "<br>"; // Go to the next line.
    s += "Digital Pin 3 = ";
    s += String(digitalRead(DIGITAL_PIN));
  }
  else
  {
    s += "Invalid Request.<br> Try /led/1, /led/0, or /read.";
  }
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

void setupWiFi()
{
  WiFi.mode(WIFI_AP);
  IPAddress ip(6,6,6,1);
  IPAddress gateway(6,6,6,1); 
  IPAddress subnet(255,255,255,0); 
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(AP_NAME, WiFiAPPSK, 6, 0);
}

void initHardware()
{
  Serial.begin(115200);
  pinMode(DIGITAL_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT); 
  digitalWrite(LED_PIN, HIGH);//on Lolin ESP8266 v3 dev boards, the led is active low
  //delay(1000);
  // Don't need to set ANALOG_PIN as input, 
  // that's all it can be.
}
