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
        <p>Kettle Started</p>\
    </body>\
</html>";

String unsetHtml = "<!DOCTYPE html>\
<html>\
    <head>\
        <title>Servo Trigger</title>\
    </head>\
    <body>\
        <p>Kettle Stopped</p>\
    </body>\
</html>";

Servo sv;
int servo_pin = D0;
int LED_PIN = BUILTIN_LED;
const char* ssid = "";
const char* password = "";
String ip;
ESP8266WebServer server(80);

bool stop_reached = false;
void ICACHE_RAM_ATTR isr()
{
    stop_reached = true;
    Serial.println("interrupted!"); 
}

std::function<void()> handleClient = []() {
  MDNS.update();
  server.handleClient();
};

class ServoContext
{
public:
    ServoContext( Servo* servo, int pin ) : servo_(servo), pin_(pin) {
        servo_->attach(pin_, 1000, 2000);
    }
    ~ServoContext() { servo_->detach(); }

    int cmove( int timeout, bool check_isr=false )
    {
        stop_reached = false;
        servo_->writeMicroseconds( timeout > 0 ? 2000 : 1000 );
        if ( check_isr )
        {
            while ( ! stop_reached ) { delay(1); }
        }
        else
        {
            delay( abs( timeout ) );
        }
        servo_->writeMicroseconds( 1500 );
        return servo_->read();
    }

    int ctoggle( int timeout )
    {
        cmove( timeout, false );
        cmove( ( timeout * -1 ), true );
        return 0;
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
  while(!Serial) { }
  Serial.println('\n');
}

void blinkLed(int half_time, int led_pin, bool swap = false) {
    digitalWrite(led_pin, swap ? HIGH : LOW);
    delay(half_time);
    digitalWrite(led_pin, swap ? LOW : HIGH);
    delay(half_time);
}

bool lastMove = true;
void moveServo()
{
    Serial.println("Moving Servo");
    std::unique_ptr<ServoContext> sc(new ServoContext(&sv, servo_pin));
    int rotation_time = 170 * 5 * -1;
    if ( lastMove )
    {
        rotation_time = 130 * 5 * -1;
    }
    sc->ctoggle( rotation_time );
    server.send(200, "text/html", setHtml);
    lastMove = true;
}

void moveServoOpposite()
{
    Serial.println("Moving Servo");
    std::unique_ptr<ServoContext> sc(new ServoContext(&sv, servo_pin));
    int rotation_time = 170 * 5;
    if ( ! lastMove )
    {
        rotation_time = 130 * 5;
    }
    sc->ctoggle( rotation_time );
    server.send(200, "text/html", unsetHtml);
    lastMove = false;
}

void setup() {
    setupSerial(115200, 2000);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);             // Connect to the network
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
    ip = waitForConnection([]() { blinkLed(500, LED_PIN, false); });
    attachInterrupt( digitalPinToInterrupt(D2), isr, FALLING);
    setClock();
    server.on("/", moveServo);
    server.on("/stop", moveServoOpposite);
    server.begin();
    std::unique_ptr<ServoContext> sc(new ServoContext(&sv, servo_pin));
    sc->ctoggle( -500 );
    Serial.println("Server Started");
}

void loop() {
    handleClient();
    delay(100);
}
