#include <ESP8266WiFi.h>
//////////////////////
// WiFi Definitions //
//////////////////////
// bridge ap credentials
const char *ssid = "espmailbox";
const char *password = "security123";

int wifiStatus;
IPAddress ip(6,6,6,2);                // this node's ap ip
IPAddress gateway(6,6,6,1);           // this node's ap default router
IPAddress subnet(255,255,255,0);    // subnet mask, this node's ap subnet addres: 6.6.6.0, broadcast: 6.6.6.255
WiFiServer server(80); 

/////////////////////
// Pin Definitions //
/////////////////////
const int REED_PIN = 0; // ESP's onboard, green LED

int hasMail = 0;

void setup() 
{
  initHardware();
  setupWiFi();
  server.begin();
}

void loop() 
{
  checkMailbox();
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
  if (req.indexOf("/hasmail") != -1) {
    val = 0;
  }
  if (req.indexOf("/getmail") != -1) {
    val = 1;
  }

  client.flush();

  // Prepare the response. Start with the common header:
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  // If we're setting the LED, print out a message saying we did
  if (val == 0)
  {
    s += "You ";
    s += (hasMail == 0)?"do not have ":"have ";
    s += "mail";
  }
  else if (val == 1)
  {
    hasMail = 0;
    s += "Getting the mail ";
  }
  else
  {
    s += "try /getmail or /hasmail";
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

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);

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
  Serial.setDebugOutput(true);
  pinMode(REED_PIN, INPUT); 
}

void checkMailbox() {
    if (digitalRead(0) == HIGH) {
        hasMail = 1;
    }
}
