#include <Arduino.h>
#include <EtherCard.h>
#include <LiquidCrystal.h>
#include <Versatile_RotaryEncoder.h>

// Set the network credentials
static byte mymac[] = {0x74,0x69,0x69,0x2D,0x30,0x31};  // Replace with your MAC address
static byte myip[] = {192, 168, 31, 200};  // Replace with your IP address
static byte gwip[] = {192, 168, 31, 7};  // Replace with your gateway IP address
static byte dnsip[] = {192, 168, 31, 7};  // Replace with your DNS IP address
static byte mask[] = {255, 255, 255, 0};  // Replace with your Network mask

// Set the UDP port to send messages
static uint16_t udpPort = 3108;

byte Ethernet::buffer[400]; // Buffer for Ethernet communication

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Set here your encoder reading pins (Ex.: EC11 with breakout board)
#define clk 17  // (A3)
#define dt 16   // (A2)
#define sw 18   // (A4)

// Buzzer pin
#define buzzer_pin 3

void buzz(int duration) {
    digitalWrite(buzzer_pin, HIGH);
    delay(duration); 
    digitalWrite(buzzer_pin, LOW);
}

static char lcd_last_line[17];

void copy_line(const char * const source_data, char * const destination_data, uint16_t size) {
  for (uint16_t char_column = 0; char_column < 16; char_column++) {
    if (char_column < size) {
      destination_data[char_column] = source_data[char_column];
    } else {
      destination_data[char_column] = ' ';
    }
  }
}

void getPacket(uint16_t port, uint8_t ip[4], uint16_t src_port, const char *data, uint16_t len) {
  Serial.print("Packet received from: ");

  Serial.print(ip[0]);
  Serial.print(".");
  Serial.print(ip[1]);
  Serial.print(".");
  Serial.print(ip[2]);
  Serial.print(".");
  Serial.print(ip[3]);
  Serial.print(" with ");
  Serial.println(data);
  
  lcd.setCursor(0, 0);
  lcd.print(lcd_last_line);
  copy_line(data, lcd_last_line, len);
  lcd.setCursor(0, 1);
  lcd.print(lcd_last_line);
  buzz(100);
}

void setup() {
  Serial.begin(9600);

  pinMode(buzzer_pin, OUTPUT);
  digitalWrite(buzzer_pin, LOW);
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
    
  ether.begin(sizeof Ethernet::buffer, mymac);
  ether.staticSetup(myip, gwip, dnsip, mask);
  ether.udpServerListenOnPort(&getPacket, udpPort);
  Serial.println("Ready!");
  lcd.print("Ready!");
}

void loop() {
  ether.packetLoop(ether.packetReceive());
}