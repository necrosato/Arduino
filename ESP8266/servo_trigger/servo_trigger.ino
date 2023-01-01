#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h>
#include <time.h>
#include <stdlib.h>

String setHtml = "<!DOCTYPE html>\
<html>\
    <head>\
        <title>Servo Trigger</title>\
    </head>\
    <body>\
        <p>Boiling Water</p>\
    </body>\
</html>";
Servo sv;
int servo_pin = D0;
int LED_PIN = BUILTIN_LED;
const char* ssid = "";
const char* password = "";
String ip;
ESP8266WebServer server(80);

std::function<void()> handleClient = []() {
  MDNS.update();
  server.handleClient();
};


class ServoContext
{
public:
    ServoContext( Servo* servo, int pin ) : servo_(servo), pin_(pin) {
        servo_->attach(pin_);
    }
    ~ServoContext() { servo_->detach(); }

    int move(int i, int timeout=-1) {
        int last = sv.read();
        int diff = abs(i - last);
        int d = timeout;
        sv.write(i);
        if ( timeout == -1 )
        {
            d = 5 * diff;
        }
        delay(d);
        return sv.read();
    }
    int pin_;
    Servo* servo_;
};

// template argument is function callback every time ip cannot be obtained
template <class T>
String waitForConnection(T fail_func) {
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println(" ...");

  String ip = "";
  while (ip == "") {
    if (WiFi.status() == WL_CONNECTED){
      Serial.println("Your ESP is connected!");
      Serial.println("Your IP address is: ");
      Serial.println(WiFi.localIP());
      Serial.println("");
      ip = WiFi.localIP().toString();
    } else {
      fail_func();
      Serial.println("WiFi not connected");
      Serial.println("");
    }
  }
  return ip;
}

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
void setupSerial(int baud, int timeout) {
  Serial.begin(baud);
  Serial.setTimeout(timeout);
  // Wait for serial to initialize.
  while(!Serial) { }
  Serial.println('\n');
}

void blinkLed(int half_time, int led_pin, bool swap = false) {
    // Low and high swapped for onboard led
    digitalWrite(led_pin, swap ? HIGH : LOW);
    delay(half_time);
    digitalWrite(led_pin, swap ? LOW : HIGH);
    delay(half_time);
}

void moveServo()
{
    Serial.println("Moving Servo");
    std::unique_ptr<ServoContext> sc(new ServoContext(&sv, servo_pin));
    sc->move(150);
    sc->move(100, 1000);
    server.send(200, "text/html", setHtml);
}

void setup() {
    setupSerial(115200, 2000);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);             // Connect to the network

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);

    ip = waitForConnection([]() { blinkLed(500, LED_PIN, false); });
     
    setClock();
    server.on("/", moveServo);
    server.begin();
    Serial.println("Server Started");

    std::unique_ptr<ServoContext> sc(new ServoContext(&sv, servo_pin));
    sc->move(100);
}

void loop() {
    handleClient();
    delay(100);
}
