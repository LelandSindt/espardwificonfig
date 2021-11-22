

#include <WiFi.h>
#include <WebServer.h>
#include <EEPROM.h>
#include <ESPmDNS.h>

#define SERIALDEBUG

char ssid[33] = "";
char password[65] = "";
char deviceName[17] = "";

const char *apssid = "ESPap";
const char *appassword = "thereisnospoon";

const String favicon = "<link rel=\"shortcut icon\" href=\"data:image/vnd.microsoft.icon;base64,AA ==\"></link>";

const int EEPROM_MIN_ADDR = 0;
const int EEPROM_MAX_ADDR = 511;

bool ledState = LOW;
unsigned long previousBlinkMillis = 0;
int blinkInterval = 500;

IPAddress mDNSIP;

WebServer server( 80 );

void setup ( void ) {
#ifdef SERIALDEBUG
  Serial.begin ( 115200 );
#endif
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  eeprom_read_string(0, ssid, sizeof(ssid));
  eeprom_read_string(128, password, sizeof(password));
  eeprom_read_string(99, deviceName, sizeof(deviceName));
#ifdef SERIALDEBUG
  Serial.println(" ");
  Serial.println(ssid);
  Serial.println(password);
#endif
  WiFi.mode(WIFI_STA);
  WiFi.begin ( ssid, password );
#ifdef SERIALDEBUG
  Serial.println(" ");
  Serial.print("Connecting to: ");
  Serial.print(ssid);
  Serial.print(" ");
#endif
  for (int i = 0; i <= 20; i++) {
    if (WiFi.status() == WL_CONNECTED ) {
      Serial.println("connected");
      mDNSIP = WiFi.localIP();
      break;
    }
    //Serial.println(WiFi.status());
#ifdef SERIALDEBUG
    Serial.print(".");
#endif
    delay(500);
  }

  if (WiFi.status() != WL_CONNECTED ) {
#ifdef SERIALDEBUG
    Serial.println("connection failed.");
    Serial.print("Starting AP: ");
    Serial.println(apssid);
#endif
    WiFi.mode(WIFI_AP);
    WiFi.softAP(apssid, appassword);
    mDNSIP = WiFi.softAPIP();
    blinkInterval = 250;
  }



  server.on ( "/", handleRoot );
  server.on ( "/ConfigWIFI", handleConfigWIFI);
  server.on ( "/restart", handleRestart);
  server.on ( "/cleareeprom", handleClearEEPROM);
  server.on ("/led_on", led_on);
  server.on ("/led_off", led_off);
  server.onNotFound ( handleNotFound );
  server.begin();
#ifdef SERIALDEBUG
  Serial.println ( "HTTP server started" );
#endif
  if (!MDNS.begin(deviceName)) {
#ifdef SERIALDEBUG
    Serial.println("Error setting up MDNS responder!");
#endif
  } else {
#ifdef SERIALDEBUG
    Serial.print(deviceName);
    Serial.println(".local MDNS responder started");
#endif
    MDNS.addService("http", "tcp", 80);
  }
#ifdef SERIALDEBUG
  Serial.print("IP address: ");
  Serial.println(mDNSIP);
#endif
  digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED off
} //setup()

void loop ( void ) {
  //MDNS.update();
  server.handleClient();

  unsigned long currentMillis = millis();

  if (currentMillis - previousBlinkMillis > blinkInterval) {
    // save the last time you blinked the LED
    previousBlinkMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
    digitalWrite(LED_BUILTIN, ledState);
  }


} //loop()





