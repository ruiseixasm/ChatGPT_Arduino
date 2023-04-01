#include <Arduino.h>

uint16_t crc16(uint8_t *data, size_t length) {
  const uint16_t polynomial = 0x1021;
  uint16_t crc = 0xFFFF;

  for (size_t i = 0; i < length; i++) {
    crc ^= (uint16_t)data[i] << 8;
    for (uint8_t j = 0; j < 8; j++) {
      crc = (crc & 0x8000) ? (crc << 1) ^ polynomial : (crc << 1);
    }
  }

  return crc;
}

void setup() {
  Serial.begin(9600);

  char str[] = "hello world";
  uint16_t checksum = crc16((uint8_t*)str, sizeof str);

  Serial.print("CRC16 of '");
  Serial.print(str);
  Serial.print("' is 0x");
  Serial.println(checksum, HEX);
}

void loop() {
  // Do nothing
}
