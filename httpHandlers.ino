void led_on () {
  String message = "<html> <head> <meta http-equiv='refresh' content='3;URL=/'/>  <title> Led On!  </title> " + favicon + " </head><body> ";
  message += String(millis()) + " Led On!...   ";
  message += "</body> </html>";
  server.send ( 200, "text/html", message );
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on
  Serial.println ( "Led On:  " + String(millis()) );
}


void led_off () {
  String message = "<html> <head> <meta http-equiv='refresh' content='3;URL=/'/>  <title> Led Off! </title> " + favicon + " </head><body> ";
  message += String(millis()) + " Led Off!...  ";
  message += "</body> </html>";
  server.send ( 200, "text/html", message );
  digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED off
  Serial.println ( "Led Off: " + String(millis()) );
}


void handleRoot() {
  String message = "<html> <head> <!-- <meta http-equiv='refresh' content='5'/> --> <title> Status </title> " + favicon + " </head> <body>";
  //message += " Status.... <br>";
  if (WiFi.status() == WL_CONNECTED ) {
    message += "Connected to: " + String(ssid) + "<br>";
  } else {
    message += "Unable to connect to: " + String(ssid) + "<br>";
  }
  message += "IP Address: " + String(IP[0]) + "." + String(IP[1]) + "." + String(IP[2]) + "." + String(IP[3]) + "<br>";
  message += "Device Name: " + String(deviceName) + ".local <br>";
  message += "<a href='/ConfigWIFI'> Configure Device </a><br>";
  //message += "<a href='/led_off'> Led Off </a><br>";
  //message += "<a href='/led_on'> Led On </a><br>";
  message += "</body></html>";
  server.send ( 200, "text/html", message );
}


void handleNotFound() {
  String message = "File Not Found\n\n";
  server.send ( 404, "text/plain", message );
  Serial.print ( "404 Requested: " );
  Serial.println ( server.uri() );
}


void handleConfigWIFI() {

  char __ssid[33];
  char __psk[65];
  char __deviceName[17];
  bool restartESP = false;
  if ( server.hasArg("SSID") ) {
    if (server.arg("SSID") != "" ) {
      server.arg("SSID").toCharArray(__ssid, 33);
      if (server.arg("SSID") != ssid) {
        writeFile2(LittleFS, "/wifi/ssid", __ssid);
        readFile2(LittleFS, "/wifi/ssid").toCharArray(ssid,33);
        restartESP = true;
      }
    }
  }
  if ( server.hasArg("PSK")) {
    if (server.arg("PSK") != "") {
      server.arg("PSK").toCharArray(__psk, 65);
      if (server.arg("PSK") != password) {
        writeFile2(LittleFS, "/wifi/pass", __psk);
        restartESP = true;
      }
    }
  }
    if ( server.hasArg("devicename")) {
    if (server.arg("devicename") != "") {
      server.arg("devicename").toCharArray(__deviceName, 17);
      if (server.arg("devicename") != deviceName) {
        writeFile2(LittleFS, "/wifi/name", __deviceName);
        readFile2(LittleFS, "/wifi/name").toCharArray(deviceName, 17);
        restartESP = true;
      }
    }
  }

  if ( server.hasArg("REBOOT")) {
    restartESP = true;
  } else {
    restartESP = false;
  }

  String message = " ";

  if (restartESP) {
    message += "<html> <head> <meta http-equiv='refresh' content='15;URL=/'/>  <title> Configure Wifi </title> " + favicon + " </head><body> ";
    message += " Restaring... will connect to: " + String(ssid);
    message += "</body> </html>";
  } else {
    message += "<html> <head> <!-- <meta http-equiv='refresh' content='5'/> --> <title> Configure Wifi </title> " + favicon + " </head><body> ";
    message += "<form action=\"ConfigWIFI\" method=\"post\"> SSID: <input type=\"text\" name=\"SSID\" value=\"" + String(ssid) + "\"> <br> Preshared Key: <input type=\"password\" name=\"PSK\" value=\"\"> <br> Device Name: <input type=\"text\" name=\"devicename\" value=\"" + String(deviceName) + "\"> .local <br> <input type=\"checkbox\" name=\"REBOOT\" value=\"True\" checked> Reboot? <br> <br>  <input type=\"submit\" value=\"Submit\"> </form> ";
    message += "</body> </html>";
  }

  server.send ( 200, "text/html", message );

  if (restartESP) {
    delay(100);
    ESP.restart();
  }

}

void handleRestart() {
  String message = "<html> <head> <meta http-equiv='refresh' content='15;URL=/'/>  <title> Restart </title> " + favicon + " </head><body> ";
  message += " Restarting...  ";
  message += "</body> </html>";
  server.send ( 200, "text/html", message );
  delay(100);
  ESP.restart();
}


void handleFormatLittleFS() {
  String message = "<html> <head> <meta http-equiv='refresh' content='15;URL=/'/>  <title> Clear Wifi </title> " + favicon + " </head><body> ";
  message += " Formatting LittleFS and Restarting...  ";
  message += "</body> </html>";
  server.send ( 200, "text/html", message );
  LittleFS.format();
  delay(100);
  ESP.restart();
}
