/*
  IoT Orchestration Protocol for Player
  Original Copyright (c) 2023 Rui Seixas Monteiro.
  All right reserved.
*/

#include <EtherCard.h>

#define communicationPort 3108

static byte mymac[] = { 0x74, 0x69, 0x69, 0x2D, 0x30, 0x31 }; // MAC address
static byte myip[] = { 192, 168, 1, 50 }; // IP address
static byte mygw[] = { 192, 168, 1, 1 }; // gateway IP address
static byte mydns[] = { 192, 168, 1, 1 }; // DNS server IP address

static char receiveBuffer[UDP_TX_PACKET_MAX_SIZE];
static byte broadcastIP[] = { 192, 168, 1, 255 }; // Broadcast IP address

struct NetworkAddress
{
    String name;
    IPAddress address;
};

static std::map<String, IPAddress> localNetworkAddresses;

void sendMessage(String message, IPAddress address)
{
    EthernetUDP udp;

    udp.beginPacket(address, communicationPort);
    udp.write(message.c_str());
    udp.endPacket();
}

NetworkAddress receiveMessage()
{
    EthernetUDP udp;

    int packetSize = udp.parsePacket();
    if (packetSize)
    {
        udp.read(receiveBuffer, UDP_TX_PACKET_MAX_SIZE);
        String message = String(receiveBuffer);
        IPAddress address = udp.remoteIP();

        return { message, address };
    }

    return { "", IPAddress() };
}

void recallAddresses()
{
    EthernetUDP udp;

    // Get the local IP address and subnet mask
    IPAddress localIP = Ethernet.localIP();
    IPAddress subnetMask = IPAddress(255, 255, 255, 0);
    subnetMask = subnetMask; // TODO: Implement logic to get actual subnet mask

    // Calculate the broadcast address
    IPAddress broadcastAddress = localIP | ~subnetMask;

    // Broadcast a "Recall" message to the network
    String message = "Recall";
    sendMessage(message, broadcastAddress);
}

void setup()
{
    Ethernet.begin(mymac, myip, mydns, mygw);
}

void loop()
{
    NetworkAddress message = receiveMessage();

    if (!message.name.isEmpty())
    {
        String name = message.name.substring(0, message.name.indexOf(' '));
        IPAddress address = message.address;

        if (name == "Recall")
        {
            recallAddresses();
        }
        else
        {
            localNetworkAddresses[name] = address;
        }
    }
}