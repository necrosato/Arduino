#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <string.h>
#include <stdlib.h>
#include "pins_arduino.h"

#define OLED_RESET D6

// NOTE: TO ACTUALLY CONNECT 1306 128x64 LCD Display I2C - 
/*
    arduino I2C standards define the following:
        SDA = GPIO4
        SCL = GPIO5

    so to connect an I2C Display to NodeMCU: 
        SDA = GPIO4 = D2 (NodeMCU Pin - see ./pins_arduino.h)
        SCL = GPIO5 = D1 (NodeMCU Pin - see ./pins_arduino.h)
    
*/

Adafruit_SSD1306 display(OLED_RESET);

// wifi credentials
// uncomment only one of these

const char *ssid = "moo";
const char *password = "HCMOO_7120";
/*
const char *ssid = "Naookie's iPhone";
const char *password = "432testpoint";
*/

/*
only use when at home
// static ip info
IPAddress ip(10,0,0,211);
IPAddress gateway(10,0,0,1); 
IPAddress subnet(255,255,255,0); 
 
//server address
IPAddress server(73,240,200,192);
*/
IPAddress server(10,0,0,210);
String req = "GET /temp HTTP/1.1";


int wifiStatus;
WiFiClient client;
WiFiServer control(80);
char buf[1024];
char k_buf[8];
char c_buf[8];
char f_buf[8];

boolean lcd_on = true;
char unit = 'F';

void setup() {
    strcpy(buf, "");
    strcpy(k_buf, "");
    strcpy(c_buf, "");
    strcpy(f_buf, "");

    Serial.begin(115200);
    delay(200);

    Serial.println("\n");
    Serial.print("This device's MAC address is: ");
    Serial.println(WiFi.macAddress());

    //WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    //WiFi.config(ip, gateway, subnet);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    wifiStatus = WiFi.status();
    if(wifiStatus == WL_CONNECTED){
        Serial.println("");
        Serial.print("Connected - Your IP address is: ");
        Serial.println(WiFi.localIP());  
    }
 
    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
    // init done

    // Show image buffer on the display hardware.
    // Since the buffer is intialized with an Adafruit splashscreen
    // internally, this will display the splashscreen.
    display.display();
    delay(2000);

    // Clear the buffer.
    display.clearDisplay();

    // draw a single pixel
    display.drawPixel(10, 10, WHITE);
    // Show the display buffer on the hardware.
    // NOTE: You _must_ call display after making any drawing commands
    // to make them visible on the display hardware!
    display.display();
    delay(2000);
    display.clearDisplay();
    display.display();
    
    control.begin();
}

int loop_count = 0;

void loop() {

    // check to see if controller connected
    check_controller();

    // if we do not have a current connection with the server
    if (!client.connected()) {
        // connect to server
        if (client.connect(server, 11000)) {
            Serial.println("Established connection to server\n");
        }
    }
    // else client is already connected
    else {
        // Make HTTP request
        client.println(req);
        client.println();
    }
    
    // if client is still connected OR we have a response buffered
    if (client.connected()) {
        // if response recieved, print it
        int i = 0;
        while(client.available() > 0) {
            char r = (char) client.read();
            buf[i] = r;
            i++;
        }
        client.flush();

        if (strip_temps() == 0) {
            show_temps();
        }
        strcpy(buf, "");
    }
    else {
        Serial.println("Terminating connection to server\n");
        client.stop();
    }
    
    delay(100);
}

int strip_temps() {
    boolean body = false;
    boolean in_tag = false;
    char tag[128];
    strcpy(tag, "");
    int i = 0, j = 0;
    int res_len = strlen(buf);

    if (strcmp(buf, "") == 0) {
        return -1;
    }

    strcpy(k_buf, "");
    strcpy(c_buf, "");
    strcpy(f_buf, "");

    /* else {
        Serial.print("res_len: ");
        Serial.println(res_len);
    } */

    while(!body && i < res_len) {
        if (buf[i] == '<') {
            in_tag = true;
        }
        if (in_tag) {
            //Serial.print(buf[i]);
            //delay(100);
            tag[j] = buf[i];
            j++;
            if (buf[i] == '>') {
                in_tag = false;
                tag[j] = '\0';
                if (strcmp(tag, "<html>") == 0) {
                    //Serial.print("TAG MATCH: ");
                    //Serial.println(tag);
                    body = true;
                }
                else {
                    //Serial.print("TAG MISMATCH: ");
                    //Serial.println(tag);
                    strcpy(tag, "");
                    j = 0;
                }
            }
        }
        i++;
    }
    if (body) {
        char cur;
        boolean k_start = false;
        boolean k_done = false;
        boolean c_start = false;
        boolean c_done = false;
        boolean f_start = false;
        boolean f_done = false;
        j = 0;
        while (!k_done) {
            cur = buf[i];
            if ((cur >= '0' && cur <= '9') || cur == '.') {
                k_start = true;
                k_buf[j] = cur;
                j++;
            }
            else if (k_start) {
                k_buf[j] = '\0';
                k_done = true;
            }
            i++;
        }
        j = 0;
        while (!c_done) {
            cur = buf[i];
            if ((cur >= '0' && cur <= '9') || cur == '.') {
                c_start = true;
                c_buf[j] = cur;
                j++;
            }
            else if (c_start) {
                c_buf[j] = '\0';
                c_done = true;
            }
            i++;
        }
        j = 0;
        while (!f_done) {
            cur = buf[i];
            if ((cur >= '0' && cur <= '9') || cur == '.') {
                f_start = true;
                f_buf[j] = cur;
                j++;
            }
            else if (f_start) {
                f_buf[j] = '\0';
                f_done = true;
            }
            i++;
        }
    }
    return 0;
}

void show_temps() {
    display.clearDisplay();
    if (!lcd_on) {
        display.display();
        return;
    }
    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.setCursor(10,0);
    display.print("Current Temperature");

    display.setTextSize(3);
    display.setCursor(0,17);
    switch (unit) {
    case 'K':
        display.print(k_buf);
        display.setCursor(0,42);
        display.print(" DEG K");
        break;
    case 'C':
        display.print(" ");
        display.print(c_buf);
        display.setCursor(0,42);
        display.print(" DEG C");
        break;
    case 'F':
        display.print(" ");
        display.print(f_buf);
        display.setCursor(0,42);
        display.print(" DEG F");
        break;
    }
    display.display();
}

void check_controller() {
    WiFiClient controller = control.available();
    if (controller) {
         // Read the first line of the request
        String c_req = controller.readStringUntil('\r');
        Serial.println(c_req);
        controller.flush();

        // Match the request
        int val = -1; // We'll use 'val' to keep track of both the
                // request type (read/set) and value if set.
        if (c_req.indexOf("/temp") != -1) {
            val = 1; // Will display temperature
        } else if (c_req.indexOf("/k") != -1) {
            val = 2;
        } else if (c_req.indexOf("/c") != -1) {
            val = 3;
        } else if (c_req.indexOf("/f") != -1) {
            val = 4;
        } else if (c_req.indexOf("/on") != -1) {
            val = 5;
        } else if (c_req.indexOf("/off") != -1) {
            val = 6;
        }



        controller.flush();

        // Prepare the response. Start with the common header:
        String c_res = "HTTP/1.1 200 OK\r\n";
        c_res += "Content-Type: text/html\r\n\r\n";
        c_res += "<!DOCTYPE HTML>\r\n<html>\r\n";

        char tstr[7] = "";
        switch (val) {
        case 1:
            c_res += k_buf;
            c_res += " K<br>";
            c_res += c_buf;
            c_res += " C<br>";
            c_res += f_buf;
            c_res += " F<br>";
            break;
        case 2:
            c_res += "Unit changed to Kelvin (K)<br>";
            unit = 'K';
            break;
        case 3:
            c_res += "Unit changed to Celsius (C)<br>";
            unit = 'C';
            break;
        case 4:
            c_res += "Unit changed to Farenheit (F)<br>";
            unit = 'F';
            break;
        case 5:
            c_res += "Turning on lcd<br>";
            lcd_on = true;
            break;
        case 6:
            c_res += "Turning off lcd<br>";
            lcd_on = false;
            break;
        default:
            c_res += "Invalid Request.<br>";
            break;
        }
        c_res += "</html>\n";

        // Send the response to the client
        controller.print(c_res);
        Serial.print(c_res);

    }
}
