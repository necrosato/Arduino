//
// pollen_counter.ino
// Naookie Sato
//
// Created by Naookie Sato on 04/26/2020
// Copyright © 2020 Naookie Sato. All rights reserved.
//

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <CertStoreBearSSL.h>
#include <stdlib.h>
#include <Regexp.h>
#include "config.h"
#include "utils.h"
#include <time.h>
#include <FS.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// NOTE: TO ACTUALLY CONNECT 1306 128x64 LCD Display I2C - 
/*
    arduino I2C standards define the following:
        SDA = GPIO4
        SCL = GPIO5

    so to connect an I2C Display to NodeMCU: 
        SDA = GPIO4 = D2 (NodeMCU Pin - see ./pins_arduino.h)
        SCL = GPIO5 = D1 (NodeMCU Pin - see ./pins_arduino.h)
    
*/

#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);

String ip;
int LED_PIN = BUILTIN_LED;
char grassCount[16] = "----";
char grassLabel[16] = "GRASS  ";
char treeCount[16] = "----";
char treeLabel[16] = "TREE  ";
char date[16] = "";
String errorMsg = "";
String url = "https://www.oregonallergyassociates.com/pollen-counts.html";
//String fingerprint = "1C:3D:E2:00:71:C5:94:82:71:BC:4A:A7:5D:B2:6B:98:97:4D:A7:B9";
const uint8_t fingerprint[20] = {0x1C, 0x3D, 0xE2, 0x00, 0x71, 0xC5, 0x94, 0x82, 0x71, 0xBC, 0x4A, 0xA7, 0x5D, 0xB2, 0x6B, 0x98, 0x97, 0x4D, 0xA7, 0xB9};


// Set time via NTP, as required for x.509 validation
void setClock() {
  configTime(-7 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

void initOled() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  display.setTextColor(WHITE);
}

template <class T>
void oledPrintAt(const T& t, int x, int y, int size) {
  display.setTextSize(size);
  display.setCursor(x, y);
  display.print(t);
}

template <class T>
void oledPrint(const T& t, int size) {
  display.setTextSize(size);
  display.print(t);
}

void setup() {
  initOled();
  setupSerial(115200, 2000);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);             // Connect to the network

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  display.clearDisplay();
  oledPrintAt("Connecting", 0, 0, 2);
  display.display();
  ip = waitForConnection([]() { blinkLed(500, LED_PIN, false); });
  setClock();
  SPIFFS.begin();
}

void drawIp() {
  oledPrintAt("IP: ", 0, 0, 1);
  oledPrint(ip.c_str(), 1);
}

void drawDate() {
  oledPrintAt(date, 0, 0, 2);
}

void drawTree() {
  int startx = 64;
  int starty = 20;
  oledPrintAt(treeLabel, startx, starty, 1);
  oledPrintAt(treeCount, startx, starty + 15, 2);
}

void drawGrass() {
  int startx = 0;
  int starty = 20;
  oledPrintAt(grassLabel, startx, starty, 1);
  oledPrintAt(grassCount, startx, starty + 15, 2);
}

void drawError() {
  if (errorMsg != "") {
    oledPrintAt(errorMsg.c_str(), 0, 55, 1);
    errorMsg = "";
  }
}

void drawScreen() {
  display.clearDisplay();
  //drawIp();
  drawDate();
  drawGrass();
  drawTree();
  drawError();
  display.display();
}

void loop() {
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  BearSSL::CertStore certStore;
  int numCerts = certStore.initCertStore(SPIFFS, "/certs.idx", "/certs.ar");
  client->setCertStore(&certStore);
  //client->setFingerprint(fingerprint);

  HTTPClient http;
  http.begin(*client, url);
  http.setReuse(false);
  http.addHeader("Content-Type", "text/plain");
  int delay_t = 10000;
  int ret = http.GET();
  if (ret == 200) {
    String payload = http.getString();
    //Serial.println(payload);
    if (payload != "") {
      MatchState ms;
      ms.Target((char *) payload.c_str());
      auto match = ms.Match("Grass Pollen Count:<br>([0-9]+)", 0);
      if (match > 0) {
        ms.GetCapture(grassCount, 0);
      } else {
        errorMsg = "No grass ";
        Serial.println("Could not match grass count");
      }
      match = ms.Match("Tree Pollen Count:<br>([0-9]+)", 0);
      if (match > 0) {
        ms.GetCapture(treeCount, 0);
      } else {
        errorMsg += "No tree ";
        Serial.println("Could not match tree count");
      }
      match = ms.Match("Last Counted: <b>([a-zA-Z0-9]+)", 0);
      if (match > 0) {
        ms.GetCapture(date, 0);
      } else {
        errorMsg += "No date";
        Serial.println("Could not match date");
      }
    } else {
      errorMsg = "No Payload";
      Serial.println("Could not retreive payload!");
    }
  } else { errorMsg = "http code (" + String(ret) + ")"; };

  if (errorMsg != "") { delay_t = 1000; }
  drawScreen(); 
  Serial.print("HTTP code: ");
  Serial.println(ret);
  Serial.print("Grass Count: ");
  Serial.println(grassCount);
  Serial.print("Tree Count: ");
  Serial.println(treeCount);
  Serial.print("Date Collected: ");
  Serial.println(date);
  delay(delay_t);
}
