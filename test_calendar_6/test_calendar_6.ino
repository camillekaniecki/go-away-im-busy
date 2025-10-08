#include <WiFiS3.h> // For UNO R4 WiFi
#include <WiFiSSLClient.h>
#include <R4HttpClient.h>
#include <ArduinoJson.h>

char ssid[] = "XXXXX";
char pass[] = "YYYYYYY";

// The URL of your Google Apps Script Web App
const char* googleWebAppUrl = "https://script.google.com/macros/s/AKfycbx9XzV7zwqDjjNmUf5UEyU_K8FHIQ8V3heL5BGpOTdudIXL5Yb7WWUCOENia1u84MWnaQ/exec";
const char* host = "script.google.com";

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
  Serial.println("Connected. Making GET request...");
  client.print("GET ");
  client.print(googleWebAppUrl);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(host);
  client.println("Connection: close");
  client.println();

  String redirectUrl = "";
  bool redirect = false;
  // --- Temporary Debugging Code ---
  // The following code will print the entire server response to the Serial Monitor.
  // This helps us see the raw headers and body to diagnose connection issues.
  Serial.println("--- Server Response ---");
  while (client.connected() || client.available()) {
    if (client.available()) {
      // Original parsing code is commented out for debugging
      // Find the start of the JSON body by skipping the HTTP headers
      if (!client.find("\r\n\r\n")) {
        Serial.println("Invalid response: Could not find header/body separator.");
        return;
      }

      if (client.find("HREF=\"")) {
        redirect = true;
        redirectUrl = client.readStringUntil('"');
      }
      redirectUrl.trim(); // Remove any trailing characters
      redirectUrl = redirectUrl.substring(0, redirectUrl.length());
    }
  }
  // Serial.println("redirectUrl: " + redirectUrl);
  Serial.println("--- End of Response ---");
  client.stop();


  Serial.println("Redirecting to: " + redirectUrl);
  client.connect("script.googleusercontent.com", 443);
  // Make the second request to the new URL
  // Make the HTTP request
  Serial.println("Connected. Making 2nd GET request...");
  client.print("GET ");
  client.print(String(redirectUrl));
  client.println(" HTTP/1.1");
  client.println("Host: script.googleusercontent.com");
  client.println("Connection: close");
  client.println();
  // --- End of Debugging Code ---
  

  Serial.println("--- Server 2nd Response ---");
  while (client.connected() || client.available()) {
    if (client.available()) {
      // Original parsing code is commented out for debugging
      // Find the start of the JSON body by skipping the HTTP headers
      // if (!client.find("\r\n\r\n")) {
      //   Serial.println("Invalid response: Could not find header/body separator.");
      //   return;
      // }
      String c = client.readString();
      Serial.print(c);
    }
  }

  client.stop(); // Close the connection
  Serial.println("Request finished. Waiting for 1 minute before next request.");
  delay(60000);
}
