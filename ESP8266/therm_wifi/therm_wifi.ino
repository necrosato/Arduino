#include <ESP8266WiFi.h>
#include <stdlib.h>
#include "pins_arduino.h"
 
const char* ssid     = "moo";
const char* password = "HCMOO_7120";     

IPAddress ip(10,0,0,210);
IPAddress gateway(10,0,0,1); 
IPAddress subnet(255,255,255,0); 
 
int wifiStatus;
WiFiServer server(11000); 

void setup() {
      
        Serial.begin(115200);
        delay(200);
       
        // We start by connecting to a WiFi network
       
        Serial.println();
        Serial.println();
        Serial.print("Your are connecting to: ");
        Serial.println(ssid);
        Serial.print("With ip: ");
        Serial.println(ip);
        
//        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
//        WiFi.config(ip, gateway, subnet);
        
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        server.begin();
       
    }   
     
void loop() {
    wifiStatus = WiFi.status();

    if(wifiStatus == WL_CONNECTED){
        Serial.println("");
        Serial.println("Your ESP is connected!");  
        Serial.println("Your IP address is: ");
        Serial.println(WiFi.localIP());  

        Serial.print("Reading analog bits: ");
        Serial.println(analogRead(A0));
        Serial.print(getTemp(A0));
        Serial.println(" K");
        // Check if a client has connected
        WiFiClient client = server.available();
        if (!client) {
            delay(1000);
            return;
        }

        // Read the first line of the request
        String req = client.readStringUntil('\r');
        Serial.println(req);
        client.flush();

        // Match the request
        int val = -1; // We'll use 'val' to keep track of both the
                // request type (read/set) and value if set.
        if (req.indexOf("/temp") != -1)
            val = 1; // Will display temperature

        client.flush();

        // Prepare the response. Start with the common header:
        String s = "HTTP/1.1 200 OK\r\n";
        s += "Content-Type: text/html\r\n\r\n";
        s += "<!DOCTYPE HTML>\r\n<html>\r\n";
        // If we're setting the LED, print out a message saying we did
        char tstr[7] = "";
        if (val >= 0) {
            double tempK = getTemp(A0);
            double tempC = tempK - 273.15;            // Convert Kelvin to Celcius
            double tempF = (tempC * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
            s += dtostrf(tempK, 6, 2, tstr);
            s += " deg K<br>";
            s += dtostrf(tempC, 6, 2, tstr);
            s += " deg C<br>";
            s += dtostrf(tempF, 6, 2, tstr);
            s += " deg F<br>";

        }
        else {
            s += "Invalid Request.<br> Try /temp";
        }
        s += "</html>\n";

        // Send the response to the client
        client.print(s);
        delay(10);
        Serial.println("Client disonnected");
        // The client will actually be disconnected 
        // when the function returns and 'client' object is detroyed
    }
    else {
        Serial.println("");
        Serial.println("WiFi not connected");
    }
    delay(1000); // check for connection every once a second
}

double getTemp(int pin) {
    int tempReading = analogRead(pin);
    // This is OK
    double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
    tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
    //double tempC = tempK - 273.15;            // Convert Kelvin to Celcius
    //double tempF = (tempC * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
    return tempK;
}
