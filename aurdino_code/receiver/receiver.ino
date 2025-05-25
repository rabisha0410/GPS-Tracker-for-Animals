#include <SPI.h>
#include <LoRa.h>

// Define LoRa pins (raw GPIO numbers)
#define SS  5    // NSS (Chip Select)
#define RST 14   // Reset
#define DIO0 2   // DIO0 (Interrupt)

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver Starting (433 MHz)...");

  // Configure LoRa pins
  LoRa.setPins(SS, RST, DIO0);

  // Initialize LoRa at 433MHz
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa Initialization Failed! Check connections or frequency.");
    while (1); // Halt if initialization fails
  }

  // Set explicit sync word to match transmitter
  LoRa.setSyncWord(0x12);

  // Match transmitter's LoRa parameters
  LoRa.setSpreadingFactor(7); // 6 to 12
  LoRa.setSignalBandwidth(125E3); // 7.8kHz to 500kHz
  LoRa.setCodingRate4(5); // 5 to 8

  Serial.println("LoRa Receiver Initialized Successfully!");
  Serial.println("Frequency: 433 MHz");
  Serial.println("Sync Word: 0x12");
  Serial.println("Spreading Factor: 7");
  Serial.println("Bandwidth: 125 kHz");
  Serial.println("Coding Rate: 4/5");
  Serial.println("Waiting for packets...");
}

void loop() {
  // Check for incoming packets
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Received Packet (Size: ");
    Serial.print(packetSize);
    Serial.print(" bytes): ");
    // Read and display the packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }
    // Display RSSI and SNR
    Serial.println(" (RSSI: " + String(LoRa.packetRssi()) + " dBm, SNR: " + String(LoRa.packetSnr()) + " dB)");
  } else {
    // Debug: Print status every 5 seconds if no packets
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint > 5000) {
      Serial.println("No packets detected...");
      lastPrint = millis();
    }
  }
}