#include <ESP8266WiFi.h>
//#include "pins_arduino.h"
//////////////////////
// WiFi Definitions //
//////////////////////
// this esp's ap credentials
const char AP_NAME[] = "espnode";
const char WiFiAPPSK[] = "security123";
// other ap credentials
const char *ssid = "";
const char *password = "";

int wifiStatus;
IPAddress ip(6,6,6,1);                // this node's ap ip
IPAddress gateway(6,6,6,1);           // this node's ap default router
IPAddress subnet(255,255,255,0);    // subnet mask, this node's ap subnet addres: 6.6.6.0, broadcast: 6.6.6.255
WiFiServer server(80); 

/////////////////////
// Pin Definitions //
/////////////////////
const int REED_PIN = 0; // ESP's onboard, green LED

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
  int val = -1; // We'll use 'val' to keep track of request
  if (req.indexOf("/gpio0") != -1) {
    val = 0;
  }
  if (req.indexOf("/gpio2") != -1) {
    val = 2;
  }

  client.flush();

  // Prepare the response. Start with the common header:
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  // If we're setting the LED, print out a message saying we did
  if (val == 0)
  {
    s += "GPIO 0 is now ";
    s += (digitalRead(0) == LOW)?"LOW":"HIGH";
  }
  else if (val == 2)
  {
    s += "GPIO 2 is now ";
    s += (digitalRead(2) == LOW)?"LOW":"HIGH";
  }
  else
  {
    s += "try /gpio0 or gpio2";
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
  Serial.print("This device's MAC address is: ");
  Serial.println(WiFi.macAddress());

  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(AP_NAME, WiFiAPPSK, 6, 1);
  Serial.print("This AP's IP address is: ");
  Serial.println(WiFi.softAPIP());  

  WiFi.begin(ssid, password);
  //WiFi.config(sta_ip, sta_gateway, subnet);

  int linenum = 10, attempt = 1;
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      if (attempt % linenum == 0) {
          Serial.println(".");
      }
      else {
          Serial.print(".");
      }
      attempt++;
  }
  wifiStatus = WiFi.status();
  if(wifiStatus == WL_CONNECTED){
      Serial.print("\nConnected - Your IP address is: ");
      Serial.println(WiFi.localIP());  
  }
}

void initHardware()
{
  Serial.begin(115200);
  Serial.println();
  pinMode(REED_PIN, INPUT); 
  //delay(1000);
  // Don't need to set ANALOG_PIN as input, 
  // that's all it can be.
}
