#include <Arduino.h>
#include "FS.h"
#include "SD_MMC.h"

// Hardware pin mapping from Freenove pinout
#define SD_MMC_CLK 39
#define SD_MMC_CMD 38
#define SD_MMC_D0  40

void listDirectory(fs::FS &fs, const char *dirname) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.printf("  DIR  : %s\n", file.name());
    } else {
      Serial.printf("  FILE : %-20s  SIZE: %d bytes\n", file.name(), file.size());
    }
    file = root.openNextFile();
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000); 

  Serial.println("\n--- SD Card Initialization Test ---");

  // Assign custom pins for ESP32-S3 MMC interface
  SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);

  // Mount SD Card in 1-bit mode (second parameter true = mode1bit)
  if (!SD_MMC.begin("/sdcard", true)) {
    Serial.println("Error: SD Card Mount Failed! Check card formatting (FAT32/exFAT).");
    return;
  }

  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("Error: No SD card attached");
    return;
  }

  // Print SD card details
  Serial.print("Card Type: ");
  if (cardType == CARD_MMC) Serial.println("MMC");
  else if (cardType == CARD_SD) Serial.println("SDSC");
  else if (cardType == CARD_SDHC) Serial.println("SDHC/SDXC");
  else Serial.println("UNKNOWN");

  uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
  uint64_t totalBytes = SD_MMC.totalBytes() / (1024 * 1024);
  uint64_t usedBytes = SD_MMC.usedBytes() / (1024 * 1024);

  Serial.printf("Card Size:  %llu MB\n", cardSize);
  Serial.printf("Total Space: %llu MB\n", totalBytes);
  Serial.printf("Used Space:  %llu MB\n", usedBytes);

  Serial.println("\n--- Directory Contents ---");
  listDirectory(SD_MMC, "/");
  Serial.println("\n--- Test Finished ---");
}

void loop() {
  // Idle loop
}