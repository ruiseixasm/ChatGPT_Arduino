// include the library code:
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Versatile_RotaryEncoder.h>
#include <EtherCard.h>

#define STATIC 1 // set to 1 to disable DHCP (adjust myip/gwip values below)

// Set the message to broadcast
static char message[] = "Player1";

// Set the UDP port to send messages
static uint16_t udpPort = 3108;
static byte mymac[] = {0x74, 0x69, 0x69, 0x2D, 0x30, 0x31}; // Replace with your MAC address
static byte broadcastip[] = {192, 168, 31, 255};			// Set the network broadcast address

#if STATIC
static byte myip[] = {192, 168, 31, 200}; // Replace with your IP address
static byte gwip[] = {192, 168, 31, 7};	  // Replace with your gateway IP address
static byte dnsip[] = {192, 168, 31, 7};  // Replace with your DNS IP address
static byte mask[] = {255, 255, 255, 0};  // Replace with your Network mask
#endif

byte Ethernet::buffer[500]; // tcp/ip send and receive buffer

const char page[] PROGMEM =
	"HTTP/1.0 503 Service Unavailable\r\n"
	"Content-Type: text/html\r\n"
	"Retry-After: 600\r\n"
	"\r\n"
	"<html>"
	"<head><title>"
	"Service Temporarily Unavailable"
	"</title></head>"
	"<body>"
	"<h3>This service is currently unavailable</h3>"
	"<p><em>"
	"The main server is currently off-line.<br />"
	"Please try again later."
	"</em></p>"
	"</body>"
	"</html>";

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Set here your encoder reading pins (Ex.: EC11 with breakout board)
#define clk 17 // (A3)
#define dt 16  // (A2)
#define sw 18  // (A4)

// Buzzer pin
#define buzzer_pin 3

// Functions prototyping to be handled on each Encoder Event
void handleRotate(int8_t rotation);
void handlePressRotate(int8_t rotation);
void handleHeldRotate(int8_t rotation);
void handlePress();
void handlePressRelease();
void handleLongPress();
void handleLongPressRelease();
void handlePressRotateRelease();
void handleHeldRotateRelease();

// Create a global pointer for the encoder object
Versatile_RotaryEncoder *versatile_encoder;

void buzzer(int duration) {

    static unsigned long lastTime = 0;
    static int last_duration = 0;

    if (duration > 0) {
	    digitalWrite(buzzer_pin, HIGH);
        lastTime = millis();
        last_duration = duration;
    } else if (last_duration > 0 && millis() - lastTime > last_duration) {
	    digitalWrite(buzzer_pin, LOW);
        last_duration = 0;
    }
}

void setup()
{

	Serial.begin(9600);

	pinMode(buzzer_pin, OUTPUT);

	// set up the LCD's number of columns and rows:
	lcd.begin(16, 2);

	versatile_encoder = new Versatile_RotaryEncoder(clk, dt, sw);

	// Load to the encoder all nedded handle functions here (up to 9 functions)
	versatile_encoder->setHandleRotate(handleRotate);
	versatile_encoder->setHandlePressRotate(handlePressRotate);
	versatile_encoder->setHandleHeldRotate(handleHeldRotate);
	versatile_encoder->setHandlePress(handlePress);
	versatile_encoder->setHandlePressRelease(handlePressRelease);
	versatile_encoder->setHandleLongPress(handleLongPress);
	versatile_encoder->setHandleLongPressRelease(handleLongPressRelease);
	versatile_encoder->setHandlePressRotateRelease(handlePressRotateRelease);
	versatile_encoder->setHandleHeldRotateRelease(handleHeldRotateRelease);

	Serial.println("\n[ETHERNET]");

	// Change 'SS' to your Slave Select pin, if you arn't using the default pin
	if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
		Serial.println("Failed to access Ethernet controller");
	else
		Serial.println("Success in accessing the Ethernet controller");

#if STATIC
	ether.staticSetup(myip, gwip, dnsip, mask);
#else
	if (!ether.dhcpSetup())
		Serial.println("DHCP failed");
#endif

	ether.printIp("IP:  ", ether.myip);
	ether.printIp("GW:  ", ether.gwip);
	ether.printIp("DNS: ", ether.dnsip);

	// Sounding readiness
	buzzer(250);

	Serial.println("Ready!");
	lcd.print("Ready!");

	Serial.print("Ethernet broadcasting to ");
	Serial.print(broadcastip[0]);
	Serial.print(".");
	Serial.print(broadcastip[1]);
	Serial.print(".");
	Serial.print(broadcastip[2]);
	Serial.print(".");
	Serial.println(broadcastip[3]);

	// set your own defualt values (optional)
	// versatile_encoder->setInvertedSwitch(true); // inverts the switch behaviour from HIGH to LOW to LOW to HIGH
	// versatile_encoder->setReadIntervalDuration(1); // set 2ms as long press duration (default is 1ms)
	// versatile_encoder->setShortPressDuration(35); // set 35ms as short press duration (default is 50ms)
	// versatile_encoder->setLongPressDuration(550); // set 550ms as long press duration (default is 1000ms)
}

void loop()
{

	// Update buzzer state
	buzzer(0);

	// Do the encoder reading and processing
	if (versatile_encoder->ReadEncoder())
	{
		// Do something here whenever an encoder action is read
	}

	// wait for an incoming TCP packet, but ignore its contents
	if (ether.packetLoop(ether.packetReceive()))
	{
		Serial.println("\n[ACCESS DETECTED]");
		lcd.setCursor(0, 0);
		lcd.print("ACCESS DETECTED ");
		memcpy_P(ether.tcpOffset(), page, sizeof page);
		ether.httpServerReply(sizeof page - 1);
		buzzer(100);
	}

    static unsigned long lastTime = 0;
    if (millis() - lastTime > 10*1000) {
        ether.sendUdp(message, sizeof(message) - 1, udpPort, broadcastip, udpPort);
        
		Serial.print("SENT: ");
		Serial.print(message);
		Serial.print("\n");
        lastTime = millis();
	    //buzzer(10);
    }
}

// Implement your functions here accordingly to your needs

void handleRotate(int8_t rotation)
{
	Serial.print("#1 Rotated: ");
	lcd.setCursor(0, 0);
	lcd.print("#1 Rotated:     ");
	lcd.setCursor(0, 1);
	if (rotation > 0)
	{
		Serial.println("Right");
		lcd.print("Right           ");
	}
	else
	{
		Serial.println("Left");
		lcd.print("Left            ");
	}
}

void handlePressRotate(int8_t rotation)
{
	Serial.print("#2 Pressed and rotated: ");
	lcd.setCursor(0, 0);
	lcd.print("#2 Press & rot.:");
	lcd.setCursor(0, 1);
	if (rotation > 0)
	{
		Serial.println("Right");
		lcd.print("Right           ");
	}
	else
	{
		Serial.println("Left");
		lcd.print("Left            ");
	}
}

void handleHeldRotate(int8_t rotation)
{
	Serial.print("#3 Held and rotated: ");
	lcd.setCursor(0, 0);
	lcd.print("#3 Held & rot.: ");
	lcd.setCursor(0, 1);
	if (rotation > 0)
	{
		Serial.println("Right");
		lcd.print("Right           ");
	}
	else
	{
		Serial.println("Left");
		lcd.print("Left            ");
	}
}

void handlePress()
{
	Serial.println("#4 Pressed");
	lcd.setCursor(0, 0);
	lcd.print("#4 Pressed      ");
	lcd.setCursor(0, 1);
	lcd.print("                ");
}

void handlePressRelease()
{
	Serial.println("#5 Press released");
	lcd.setCursor(0, 0);
	lcd.print("#5 Press        ");
	lcd.setCursor(0, 1);
	lcd.print("released        ");
}

void handleLongPress()
{
	Serial.println("#6 Long pressed");
	lcd.setCursor(0, 0);
	lcd.print("#6 Long pressed ");
	lcd.setCursor(0, 1);
	lcd.print("                ");
}

void handleLongPressRelease()
{
	Serial.println("#7 Long press released");
	lcd.setCursor(0, 0);
	lcd.print("#7 Long press   ");
	lcd.setCursor(0, 1);
	lcd.print("released        ");
}

void handlePressRotateRelease()
{
	Serial.println("#8 Press rotate released");
	lcd.setCursor(0, 0);
	lcd.print("#8 Press rotate ");
	lcd.setCursor(0, 1);
	lcd.print("released        ");
}

void handleHeldRotateRelease()
{
	Serial.println("#9 Held rotate released");
	lcd.setCursor(0, 0);
	lcd.print("#9 Held rotate  ");
	lcd.setCursor(0, 1);
	lcd.print("released        ");
}