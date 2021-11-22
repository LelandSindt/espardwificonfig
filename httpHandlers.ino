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
  message += "IP Address: " + String(mDNSIP[0]) + "." + String(mDNSIP[1]) + "." + String(mDNSIP[2]) + "." + String(mDNSIP[3]) + "<br>";
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
#ifdef SERIALDEBUG
  Serial.print ( "404 Requested: " );
  Serial.println ( server.uri() );
#endif
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
        //eeprom_write_chars(0, __ssid, 33);
        //eeprom_read_chars(0, ssid, 33);
        eeprom_write_string(0, __ssid);
        eeprom_read_string(0, ssid, sizeof(ssid));
        restartESP = true;
      }
    }
  }
  if ( server.hasArg("PSK")) {
    if (server.arg("PSK") != "") {
      server.arg("PSK").toCharArray(__psk, 65);
      if (server.arg("PSK") != password) {
        //eeprom_write_chars(34, __psk, 65);
        eeprom_write_string(128, __psk);
        restartESP = true;
      }
    }
  }
    if ( server.hasArg("devicename")) {
    if (server.arg("devicename") != "") {
      server.arg("devicename").toCharArray(__deviceName, 17);
      if (server.arg("devicename") != deviceName) {
        eeprom_write_string(99, __deviceName);
        eeprom_read_string(99, deviceName, sizeof(deviceName));
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

void handleClearEEPROM() {
  String message = "<html> <head> <meta http-equiv='refresh' content='15;URL=/'/>  <title> Clear Wifi </title> " + favicon + " </head><body> ";
  message += " Clearing EEPROM and Restarting...  ";
  message += "</body> </html>";
  server.send ( 200, "text/html", message );
  clear_EEPROM();
  delay(100);
  ESP.restart();
}

