#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#define FORMAT_LITTLEFS_IF_FAILED true

char ssid[33] = "";
char password[65] = "";
char deviceName[17] = "";

const char *apssid = "ESPap";
const char *appassword = "thereisnospoon";

const String favicon = "<link rel=\"shortcut icon\" href=\"data:image/vnd.microsoft.icon;base64,AA ==\"></link>";

bool ledState = LOW;
unsigned long previousBlinkMillis = 0;
int blinkInterval = 500;

IPAddress IP;

WebServer server( 80 );

void setup ( void ) {
  Serial.begin ( 115200 );
  // wait at least 5 seconds for Serial
  for (int i = 0; i <= 5; i++) {
    if (Serial) {
      Serial.println("start.");
      break;
    } else {
      delay(1000);
    }
  }
  if(!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)){
    Serial.println("LittleFS Mount Failed");
  }
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  readFile2(LittleFS, "/wifi/ssid").toCharArray(ssid, 33);
  readFile2(LittleFS, "/wifi/pass").toCharArray(password, 65);
  readFile2(LittleFS, "/wifi/name").toCharArray(deviceName, 17);
  Serial.println(" ");
  //Serial.println(ssid);
  //Serial.println(password);
  WiFi.mode(WIFI_STA);
  WiFi.begin ( ssid, password );
  Serial.println(" ");
  Serial.print("Connecting to: ");
  Serial.print(ssid);
  Serial.print(" ");
  for (int i = 0; i <= 20; i++) {
    if (WiFi.status() == WL_CONNECTED ) {
      Serial.println("connected");
      IP = WiFi.localIP();
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
    IP = WiFi.softAPIP();
    blinkInterval = 250;
  }



  server.on ( "/", handleRoot );
  server.on ( "/ConfigWIFI", handleConfigWIFI);
  server.on ( "/restart", handleRestart);
  server.on ( "/formatLittleFS", handleFormatLittleFS);
  //server.on ( "/led_on", led_on);
  //server.on ( "/led_off", led_off);
  server.onNotFound ( handleNotFound );
  server.begin();
  Serial.println ( "HTTP server started" );
  if (!MDNS.begin(deviceName)) {
    Serial.println("Error setting up MDNS responder!");
  } else {
    Serial.print(deviceName);
    Serial.println(".local MDNS responder started");
    MDNS.addService("http", "tcp", 80);
  }
  Serial.print("IP address: ");
  Serial.println(IP);

  digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED off
} //setup()

void loop ( void ) {
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
