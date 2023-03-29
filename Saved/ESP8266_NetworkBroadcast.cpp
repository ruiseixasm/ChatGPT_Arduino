#include <Arduino.h>
#include <ESP8266WiFi.h> // NEED TO SELECT THE ESP8266 BOARD FIRST!
#include <WiFiUdp.h> // NEED TO SELECT THE ESP8266 BOARD FIRST!

// Set the network credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Set the UDP port to send messages
const int udpPort = 12345;

// Set the message to broadcast
const char* message = "Hello, world!";

// Create a UDP object
WiFiUDP udp;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  // Begin UDP communication
  udp.begin(udpPort);
  Serial.print("UDP broadcasting to ");
  Serial.print(WiFi.localIP());
  Serial.print(":");
  Serial.println(udpPort);
}

void loop() {
  // Send the message to the network broadcast address
  udp.beginPacket(WiFi.localIP(), udpPort);
  udp.write(message);
  udp.endPacket();

  delay(1000);
}
