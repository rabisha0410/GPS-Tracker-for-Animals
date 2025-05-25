#include <SPI.h>
#include <LoRa.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

// LoRa pin definitions (for ESP32 + SX1278 LoRa module)
#define LORA_SS   5   // Chip Select
#define LORA_RST  14  // Reset
#define LORA_DIO0 2   // DIO0 - Interrupt

// GPS pin definitions (using UART2)
#define GPS_RX 16  // ESP32 RX2 (connect to GPS TX)
#define GPS_TX 17  // ESP32 TX2 (optional)

// Initialize TinyGPS++ and UART2 for GPS
TinyGPSPlus gps;
HardwareSerial gpsSerial(2); // UART2

int packetCounter = 0;

void setup() {
  // Begin Serial Monitor
  Serial.begin(115200);
  delay(1000);
  Serial.println("LoRa 433MHz + GPS Transmitter Starting...");

  // Start GPS Serial
  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
  Serial.println("GPS Serial (UART2 @ 9600 baud) started.");

  // Setup LoRa
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(433E6)) {  // <<< CHANGED TO 433 MHz
    Serial.println("LoRa init failed! Check wiring or frequency.");
    while (true);
  }

  LoRa.setTxPower(14);               // Transmit power (range: 2-20)
  LoRa.setSpreadingFactor(7);        // SF7
  LoRa.setSignalBandwidth(125E3);    // 125 kHz
  LoRa.setCodingRate4(5);            // 4/5

  Serial.println("LoRa initialized @ 433 MHz.");
}

void loop() {
  // Try reading GPS data
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      // Prepare LoRa message
      String message;
      if (gps.location.isValid()) {
        message = "Pkt#" + String(packetCounter) +
                  " Lat:" + String(gps.location.lat(), 6) +
                  " Lon:" + String(gps.location.lng(), 6) +
                  " Alt:" + String(gps.altitude.meters()) + "m";
      } else {
        message = "Pkt#" + String(packetCounter) + " GPS: No Fix";
      }

      // Debug output
      Serial.println("Sending LoRa packet:");
      Serial.println(message);

      // Send via LoRa
      LoRa.beginPacket();
      LoRa.print(message);
      LoRa.endPacket();

      Serial.println("Packet Sent! Size: " + String(message.length()) + " bytes\n");
      packetCounter++;
    }
  }

  // Check for GPS silence
  if (millis() > 10000 && gps.charsProcessed() < 10) {
    Serial.println("No GPS data received — check wiring, power, and sky visibility.");
  }

  delay(10000); // Delay between transmissions
}
