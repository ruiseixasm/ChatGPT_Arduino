#include <EtherCard.h>

// Set the network credentials
static byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
static byte ip[] = { 192, 168, 1, 50 };
static byte gateway[] = { 192, 168, 1, 1 };
static byte subnet[] = { 255, 255, 255, 0 };

// Set the UDP port to send messages
const int udpPort = 12345;

// Set the message to broadcast
const char* message = "Hello, world!";

// Create a UDP object
static UDP udp;

void setup() {
  Serial.begin(115200);

  // Initialize the Ethernet card
  if (ether.begin(sizeof Ethernet::buffer, mac) == 0) {
    Serial.println("Failed to initialize Ethernet card.");
    while (true) {}
  }

  // Set the network configuration
  ether.staticSetup(ip, gateway, subnet);

  Serial.println("Connected to Ethernet");

  // Begin UDP communication
  udp.begin(udpPort);
  Serial.print("UDP broadcasting to ");
  Serial.print(ip[0]); Serial.print("."); Serial.print(ip[1]);
  Serial.print("."); Serial.print(ip[2]); Serial.print("."); Serial.println(ip[3]);
}

void loop() {
  // Send the message to the network broadcast address
  ether.sendUdp(message, strlen(message), IPAddress(255,255,255,255), udpPort);