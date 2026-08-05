#include "SDManager.h"

// Private helper to expose protected _card variable without polluting the header
class SDMMCHelper : public fs::SDMMCFS {
public:
    sdmmc_card_t* getCard() { return _card; }
};

// Initialize the static instance pointer to nullptr
SDManager* SDManager::_instance = nullptr;

SDManager::SDManager(int clkPin, int cmdPin, int d0Pin)
    : _clkPin(clkPin), _cmdPin(cmdPin), _d0Pin(d0Pin) {}

sdmmc_card_t* SDManager::getCard() {
    return ((SDMMCHelper*)&SD_MMC)->getCard();
}

bool SDManager::begin() {
    _instance = this; // Bind active instance to static pointer

    SD_MMC.setPins(_clkPin, _cmdPin, _d0Pin);

    if (!SD_MMC.begin("/sdcard", true)) {
        Serial.println("ERROR: SD Card Mount Failed!");
        _isMounted = false;
        return false;
    }

    sdmmc_card_t* card = getCard();
    if (!card) {
        Serial.println("ERROR: Failed to retrieve SD card handle!");
        _isMounted = false;
        return false;
    }

    _isMounted = true;
    _sectorCount = (uint32_t)(SD_MMC.cardSize() / _sectorSize);

    // Configure USB MSC
    _msc.vendorID("ESP32-S3");
    _msc.productID("SD Card Reader");
    _msc.productRevision("1.0");

    _msc.onRead(onReadCB);
    _msc.onWrite(onWriteCB);
    _msc.onStartStop(onStartStopCB);
    _msc.mediaPresent(true);

    _msc.begin(_sectorCount, _sectorSize);
    USB.begin();

    return true;
}

void SDManager::end() {
    _msc.end();
    SD_MMC.end();
    _isMounted = false;
    _instance = nullptr;
}

uint32_t SDManager::getCapacityMB() const {
    return (_sectorCount * _sectorSize) / (1024 * 1024);
}

// Private C++ Callback Delegates
int32_t SDManager::onReadCB(uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize) {
    return _instance ? _instance->handleRead(lba, offset, buffer, bufsize) : -1;
}

int32_t SDManager::onWriteCB(uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize) {
    return _instance ? _instance->handleWrite(lba, offset, buffer, bufsize) : -1;
}

bool SDManager::onStartStopCB(uint8_t power_condition, bool start, bool load_eject) {
    return true;
}

// Instance Logic
int32_t SDManager::handleRead(uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize) {
    sdmmc_card_t* card = getCard();
    if (!card) return -1;

    uint32_t sectors = bufsize / _sectorSize;
    esp_err_t err = sdmmc_read_sectors(card, buffer, lba, sectors);
    return (err == ESP_OK) ? bufsize : -1;
}

int32_t SDManager::handleWrite(uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize) {
    sdmmc_card_t* card = getCard();
    if (!card) return -1;

    uint32_t sectors = bufsize / _sectorSize;
    esp_err_t err = sdmmc_write_sectors(card, buffer, lba, sectors);
    return (err == ESP_OK) ? bufsize : -1;
}