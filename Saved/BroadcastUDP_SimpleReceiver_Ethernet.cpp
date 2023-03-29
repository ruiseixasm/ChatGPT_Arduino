#include <Arduino.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // MAC address of Ethernet shield
IPAddress ip(192, 168, 1, 177); // IP address of Ethernet shield
unsigned int localPort = 8888; // Local port to listen on

EthernetUDP udp;

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip); // Initialize Ethernet connection
  udp.begin(localPort); // Initialize UDP
}

void loop() {
  byte packetBuffer[UDP_TX_PACKET_MAX_SIZE];
  int packetSize = udp.parsePacket();
  
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    Serial.write(packetBuffer, packetSize);
  }
}
