#include <WiFiS3.h> // For UNO R4 WiFi
#include <WiFiSSLClient.h>
#include <ArduinoJson.h>

char ssid[] = "CopperTiger";
char pass[] = "pooppooppoop";

char* curl = "curl -d \"client_id=652766651007-0fonohibrbul5505i5cbms3avpa8eq7t.apps.googleusercontent.com&scope=email%20profile\" \
     https://oauth2.googleapis.com/device/code";

char* host = "oauth2.googleapis.com";

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Connect to the WiFi network
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  int status = WiFi.begin(ssid, pass);
  while (status != WL_CONNECTED) {
    delay(5000);
    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
  }
  Serial.println("Connected to WiFi!");
}

void loop() {
  // Create a secure client
  WiFiSSLClient client;
  // client.setInsecure(); // This is for simplicity; for production, use a root certificate.

  Serial.println("\nStarting connection to Google Apps Script...");
  if (!client.connect(host, 443)) {
    Serial.println("Connection failed!");
    return;
  }

  // Make the HTTP request
  Serial.println("Connected. Making POST request...");

  client.println("POST /device/code HTTP/1.1");
  client.println("Host: oauth2.googleapis.com");
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.println("Content-Length: 104");
  client.println();
  client.println("client_id=652766651007-0fonohibrbul5505i5cbms3avpa8eq7t.apps.googleusercontent.com&scope=email%20profile");

  client.println("Connection: close");
  client.println();

  

  Serial.println("--- Server Response ---");
  while (client.connected()) {
    if (client.available()) {
      String c = client.readString();
      Serial.print(c);
    }
  }

  client.stop(); // Close the connection
  Serial.println("Request finished. Waiting for 1 minute before next request.");
  delay(60000);
}
