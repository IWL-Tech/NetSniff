#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define LED_BUILTIN 2   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

// Set these to your desired credentials.
const char *ssid = "IWL-S1";
const char *password = "itwithlyam";

WiFiServer server(80);


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            Serial.println("scan start");

            String message = "<h1>Scan result: ";
            client.print("<h1>Scan result: ");
          
            // WiFi.scanNetworks will return the number of networks found
            int n = WiFi.scanNetworks();
            Serial.println("scan done");
            if (n == 0) {
                Serial.println("no networks found");
                message += " No networks found</h1>";
                client.println(" No networks found</h1>");
            } else {
                Serial.print(n);
                client.print(n);
                message += n;
                Serial.println(" networks found");
                client.println(" networks found</h1><br />");
                message += " networks found</h1><br />";
                for (int i = 0; i < n; ++i) {
                  if (WiFi.SSID(i).startsWith("IWL-B")) {
                    // Print SSID and RSSI for each network found
                    Serial.print(i + 1);
                    client.print(i + 1);
                    message += i+1;
                    Serial.print(": ");
                    client.print(": ");
                    message += ": ";
                    Serial.print(WiFi.SSID(i));
                    client.print(WiFi.SSID(i));
                    message += WiFi.SSID(i);
                    Serial.print(" (");
                    client.print(" (");
                    message += " (";
                    Serial.print(WiFi.RSSI(i));
                    client.print(WiFi.RSSI(i));
                    message += WiFi.RSSI(i);
                    Serial.print(")");
                    client.print(")");
                    message += ")";
                    delay(10);
                    message += "<br />";
                    client.print("<br />");
                  } else {
                    break;
                  }
                }
                client.print("<h3>Scan complete.</h3><h4>If no boards were listed, then check that your boards are connected to power.</h4>");
            }
            Serial.println("SCAN END");
            Serial.println("");

            // the content of the HTTP response follows the header:
            client.print(" ");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
