espardwificonfig
====

espardwificonfig provides a web interface for configuration of WiFi settings for an ESP32 using Arduino.

The SSID, Password and mDNS values are stored in LittleFS. 

If the SSID and Password combination retrieved from LittleFS result in a successful WiFi connection an IP address is obtained via DHCP and the mDNS name retrieved from LittleFS is initialized.

If the SSID and Password combination retrieved from LittleFS fail to connect to a local WiFi Access Point the ESP32 is put into Access Point mode using SSID "ESPap" and Pre-Shared Key "thereisnospoon". Connecting to "ESPap" will serve your client a DHCP address and make the ESP8255 available at 192.168.4.1

Connecting to http://192.168.4.1/ will provide a simple status page and a link to configure the device at http://192.168.4.1/ConfigWIFI

Alone this project simply allows an operator to configure the SSID, Pre-Share Key and mDNS name. This is ideal for projects that will be distributed to users without the ability to re-program the ESP32 with hard coded values. 

