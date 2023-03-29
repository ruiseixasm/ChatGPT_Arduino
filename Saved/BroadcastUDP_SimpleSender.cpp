#include <Arduino.h>
#include <EtherCard.h>

// Set the network credentials
static byte mymac[] = {0x74,0x69,0x69,0x2D,0x30,0x31};  // Replace with your MAC address
static byte myip[] = {192, 168, 31, 200};  // Replace with your IP address
static byte broadcastip[] = {192, 168, 31, 255};  // Set the network broadcast address
static byte gwip[] = {192, 168, 31, 7};  // Replace with your gateway IP address
static byte dnsip[] = {192, 168, 31, 7};  // Replace with your DNS IP address
static byte mask[] = {255, 255, 255, 0};  // Replace with your Network mask

// Set the UDP port to send messages
static uint16_t udpPort = 3108;

// Set the message to broadcast
static char message[] = "Hello, world!";

// Create an EtherCard object
//EtherCard ether_card; // Already declared

// Define the buffer size
byte Ethernet::buffer[400]; // tcp/ip send and receive buffer

void setup() {
  Serial.begin(9600);
  ether.begin(sizeof Ethernet::buffer, mymac);
  ether.staticSetup(myip, gwip, dnsip, mask);

  Serial.print("Ethernet broadcasting to ");
  Serial.print(broadcastip[0]);
  Serial.print(".");
  Serial.print(broadcastip[1]);
  Serial.print(".");
  Serial.print(broadcastip[2]);
  Serial.print(".");
  Serial.println(broadcastip[3]);
}

void loop() {
  // Send the message to the network broadcast address
  //ether.packetLoop(ether.packetSend(sizeof message - 1, udpPort, broadcastip, message));
  //ether_card.packetLoop(ether_card.packetSend(sizeof message - 1, (const byte*)message));
  ether.sendUdp(message, sizeof(message)-1, udpPort, broadcastip, udpPort);
  //ether.packetSend(sizeof message -1);

  delay(1000);
}
