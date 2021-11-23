

#include <WiFi.h>
#include <WebServer.h>
#include <EEPROM.h>
#include <ESPmDNS.h>


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
  Serial.begin ( 115200 );
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  eeprom_read_string(0, ssid, sizeof(ssid));
  eeprom_read_string(128, password, sizeof(password));
  eeprom_read_string(99, deviceName, sizeof(deviceName));
  Serial.println(" ");
  Serial.println(ssid);
  Serial.println(password);
  WiFi.mode(WIFI_STA);
  WiFi.begin ( ssid, password );
  Serial.println(" ");
  Serial.print("Connecting to: ");
  Serial.print(ssid);
  Serial.print(" ");
  for (int i = 0; i <= 20; i++) {
    if (WiFi.status() == WL_CONNECTED ) {
      Serial.println("connected");
      mDNSIP = WiFi.localIP();
      break;
    }
    //Serial.println(WiFi.status());
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() != WL_CONNECTED ) {
    Serial.println("connection failed.");
    Serial.print("Starting AP: ");
    Serial.println(apssid);
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
  Serial.println ( "HTTP server started" );
  if (!MDNS.begin(deviceName)) {
    Serial.println("Error setting up MDNS responder!");
    while(1) {
      delay(1000);
    }
  }
  Serial.print(deviceName);
  Serial.println(".local MDNS responder started");
  MDNS.addService("http", "tcp", 80);
  Serial.print("IP address: ");
  Serial.println(mDNSIP);
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
