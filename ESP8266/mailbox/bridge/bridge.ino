#include <ESP8266WiFi.h>
//////////////////////
// WiFi Definitions //
//////////////////////
// this esp's ap credentials
const char AP_NAME[] = "espmailbox";
const char WiFiAPPSK[] = "security123";
// other ap credentials
const char *ssid = "";
const char *password = "";

int wifiStatus;
IPAddress ip(6,6,6,1);                // this node's ap ip
IPAddress gateway(6,6,6,1);           // this node's ap default router
IPAddress subnet(255,255,255,0);      // subnet mask, this node's ap subnet addres: 6.6.6.0, broadcast: 6.6.6.255
WiFiServer server(80); 
IPAddress mailbox(6,6,6,2);

/////////////////////
// Pin Definitions //
/////////////////////

void setup() 
{
  initHardware();
  setupWiFi();
  server.begin();
}

void loop() {
    check_mailbox();
    handle_server();
}

String req_get = "GET /getmail HTTP/1.1";
String req_has = "GET /hasmail HTTP/1.1";
char buf[1024];
int mail_status = 0;

void reset_mailbox() {
    WiFiClient client;
    Serial.println("Attempting to connect to mailbox server (port 80):");
    // connect to server
    if (client.connect(mailbox, 80)) {
        Serial.println("Established connection to mailbox server:");
    }
    else {
        Serial.println("Connection to mailbox server failed:");
        return;
    }
    // Make HTTP request
    Serial.print("Sending http request to mailbox server: ");
    Serial.println(req_get);
    client.println(req_get);
    delay(1000);

    int timeout = 500;
    int count = 0;
    while (client.available() == 0 && count < timeout) { 
        count++;
        delay(1);
    }
    if (client.available() > 0) {
        Serial.println("Response recieved from mailbox server: ");
        int i = 0;
        while(client.available() > 0) {
            char r = (char) client.read();
            buf[i] = r;
            i++;
        }
        buf[i] = '\0';
        client.flush();
        Serial.println(buf);
        strcpy(buf, "");
    }
    Serial.println("Terminating connection to server\n");


}
void check_mailbox() {
    WiFiClient client;
    Serial.println("Attempting to connect to mailbox server (port 80):");
    // connect to server
    if (client.connect(mailbox, 80)) {
        Serial.println("Established connection to mailbox server:");
    }
    else {
        Serial.println("Connection to mailbox server failed:");
        return;
    }
    // Make HTTP request
    Serial.print("Sending http request to mailbox server: ");
    Serial.println(req_has);
    client.println(req_has);
    delay(1000);

    int timeout = 500;
    int count = 0;
    while (client.available() == 0 && count < timeout) { 
        count++;
        delay(1);
    }
    if (client.available() > 0) {
        Serial.println("Response recieved from mailbox server: ");
        int i = 0;
        while(client.available() > 0) {
            char r = (char) client.read();
            buf[i] = r;
            i++;
        }
        buf[i] = '\0';
        client.flush();
        Serial.println(buf);
        if (String(buf).indexOf("do not") != -1) {
            mail_status = 0;
        }
        else {
            mail_status = 1;
        }
        strcpy(buf, "");
    }
    Serial.println("Terminating connection to server\n");

}

void handle_server() {
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
  if (req.indexOf("/hasmail") != -1) {
    val = 0;
  }
  else if (req.indexOf("/getmail") != -1) {
    val = 1;
  }

  client.flush();

  // Prepare the response. Start with the common header:
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  // If we're setting the LED, print out a message saying we did
  if (val == 0) {
    if (mail_status == 1) {
        s += "<p>You have mail!</p>";
    }
    else {
        s += "<p>You do NOT have mail</p>";
    }
  }
  else if (val == 1) {
    s += "<p>sending getmail request</p>";
    reset_mailbox();
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
  Serial.setDebugOutput(true);
  Serial.println();
}
