#include <ESP8266WiFi.h>
//#include "pins_arduino.h" //uncomment when building on mac
//////////////////////
// WiFi Definitions //
//////////////////////
// this esp's ap credentials
const char AP_NAME[] = "espnode2";
const char WiFiAPPSK[] = "security123";
// other esp ap credentials
const char *ssid = "espnode";
const char *password = "security123";

int wifiStatus;
IPAddress ip(6,6,6,129);              // this node's ap ip
IPAddress gateway(6,6,6,129);         // this node's ap default router
IPAddress sta_ip(6,6,6,2);          // this node's station ip
IPAddress sta_gateway(6,6,6,1);     // this node's station default router
IPAddress subnet(255,255,255,128);    // subnet mask, this node's ap subnet address: 6.6.6.128, broadcast: 6.6.6.255

IPAddress other(6,6,6,130);

/////////////////////
// Pin Definitions //
/////////////////////
const int LED_PIN = D4; // ESP's onboard, green LED
const int ANALOG_PIN = A0; // The only analog pin on the ESP
const int DIGITAL_PIN = D3; // Digital pin to be read

void setup() 
{
  initHardware();
  setupWiFi();
}

int led_status = 0;
char buf[1024];
String req_on = "GET /led/1 HTTP/1.1";
String req_off = "GET /led/0 HTTP/1.1";
void loop() 
{
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("lost connection to ap, waiting for reconnection...");
        //setupWiFi();
        delay(3000);
        return;
    }
    WiFiClient ap_client;
    Serial.println("Attempting to connect to ap server (port 80):");
    // connect to server
    if (ap_client.connect(other, 80)) {
        Serial.println("Established client connection to ap server:");
    }
    else {
        Serial.println("Connection to ap server failed, retrying:");
        return;
    }
    // Make HTTP request
    Serial.print("Sending http request to ap server: ");
    if (led_status == 0) {
        Serial.println(req_on);
        ap_client.println(req_on);
        ap_client.println();
        led_status = 1;
    }
    else {
        Serial.println(req_off);
        ap_client.println(req_off);
        ap_client.println();
        led_status = 0;
    }
    delay(1000);

    int timeout = 500;
    int count = 0;
    while (ap_client.available() == 0 && count < timeout) { 
        count++;
        delay(1);
    }
    if (ap_client.available() > 0) {
        Serial.println("Response recieved from ap server: ");
        int i = 0;
        while(ap_client.available() > 0) {
            char r = (char) ap_client.read();
            buf[i] = r;
            i++;
        }
        buf[i] = '\0';
        ap_client.flush();
        Serial.println(buf);
        strcpy(buf, "");
    }
    Serial.println("Terminating connection to server\n");
}

void setupWiFi()
{
  Serial.print("This device's MAC address is: ");
  Serial.println(WiFi.macAddress());

  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(AP_NAME, WiFiAPPSK, 6, 0);
  Serial.print("This AP's IP address is: ");
  Serial.println(WiFi.softAPIP());  

  WiFi.begin(ssid, password);
  WiFi.config(sta_ip, sta_gateway, subnet);

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
  pinMode(DIGITAL_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT); 
  digitalWrite(LED_PIN, HIGH);//on Lolin ESP8266 v3 dev boards, the led is active low
  //delay(1000);
  // Don't need to set ANALOG_PIN as input, 
  // that's all it can be.
}
