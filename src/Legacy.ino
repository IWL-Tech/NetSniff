// hi

#include <WiFi.h>
#include "time.h"
#include "heltec.h"
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "Arduino.h"

const char* ssid = "VM6611532";
const char* password = "xtn7rppfHzqj";

int counter = 1;

WebServer server(80);

const int led = 13;

const char *longString = R""""(
<!DOCTYPE html>
<html>
<head><title>Test</title></head>
<body>
  <h1>Hello</h1>
</body>
</html>
)"""";

void handleRoot() {
  server.send(200, "text/html", longString);
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void restart(void) {
  Serial.println("Restarting in 10 seconds");
 
  delay(10000);
 
  ESP.restart();
 
}

void scan(void) {
  Serial.println("scan start");

  String message = "Scan result: ";

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0) {
        Serial.println("no networks found");
        message += "\nNo networks found";
    } else {
        Serial.print(n);
        message += n;
        Serial.println(" networks found");
        message += " networks found\n";
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            message += i+1;
            Serial.print(": ");
            message += ": ";
            Serial.print(WiFi.SSID(i));
            message += WiFi.SSID(i);
            Serial.print(" (");
            message += " (";
            Serial.print(WiFi.RSSI(i));
            message += WiFi.RSSI(i);
            Serial.print(")");
            message += ")";
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
            message += (WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*";
            delay(10);
            message += "\n";
        }
    }
    Serial.println("SCAN END");
    Serial.println("");

    Serial.println(message);

    server.send(200, "text/plain", message);
}

void setup(void) {
  




  
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("awaiting.. ");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  
  server.on("/", scan);

  server.on("/restart", restart);

  server.on("/scan", scan);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  
  delay(2);//allow the cpu to switch to other tasks


  
}
