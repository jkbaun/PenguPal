#include <Arduino.h>
#include "FS.h"
#include "SD_MMC.h"
#include "USB.h"
#include "USBMSC.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_defs.h"
#include "driver/sdmmc_types.h"

#define SD_MMC_CLK 39
#define SD_MMC_CMD 38
#define SD_MMC_D0  40

USBMSC msc;

// Access protected _card member in fs::SDMMCFS
class SDMMCHelper : public fs::SDMMCFS {
public:
  sdmmc_card_t* getCard() { return _card; }
};

static sdmmc_card_t* getSDCardPtr() {
  return ((SDMMCHelper*)&SD_MMC)->getCard();
}

// USB MSC Read Callback
static int32_t onRead(uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize) {
  sdmmc_card_t* card = getSDCardPtr();
  if (!card) return -1;

  uint32_t sectorCount = bufsize / 512;
  esp_err_t err = sdmmc_read_sectors(card, buffer, lba, sectorCount);
  return (err == ESP_OK) ? bufsize : -1;
}

// USB MSC Write Callback
static int32_t onWrite(uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize) {
  sdmmc_card_t* card = getSDCardPtr();
  if (!card) return -1;

  uint32_t sectorCount = bufsize / 512;
  esp_err_t err = sdmmc_write_sectors(card, buffer, lba, sectorCount);
  return (err == ESP_OK) ? bufsize : -1;
}

// USB MSC Start/Stop Callback
static bool onStartStop(uint8_t power_condition, bool start, bool load_eject) {
  return true;
}

void setup() {
  Serial.begin(115200);
  delay(1500);

  Serial.println("\n--- Starting ESP32-S3 USB MSC Setup ---");

  // 1. Initialize SD Card Pins
  SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);

  if (!SD_MMC.begin("/sdcard", true)) {
    Serial.println("ERROR: SD Card Mount Failed!");
    return;
  }
  Serial.println("SUCCESS: SD Card Mounted.");

  sdmmc_card_t* card = getSDCardPtr();
  if (!card) {
    Serial.println("ERROR: Failed to retrieve SD card handle!");
    return;
  }

  uint32_t sectorSize = 512;
  uint32_t sectorCount = (uint32_t)(SD_MMC.cardSize() / sectorSize);

  Serial.printf("Card Capacity: %u MB (%u sectors)\n", (sectorCount * sectorSize) / (1024 * 1024), sectorCount);

  // 2. Configure USB MSC Descriptors
  msc.vendorID("ESP32-S3");
  msc.productID("SD Card Reader");
  msc.productRevision("1.0");

  msc.onRead(onRead);
  msc.onWrite(onWrite);
  msc.onStartStop(onStartStop);
  msc.mediaPresent(true);

  // 3. Start TinyUSB MSC
  msc.begin(sectorCount, sectorSize);
  USB.begin();

  Serial.println("SUCCESS: USB MSC Initialized. Ready for PC connection.");
}

void loop() {
  // Main background loop
} 