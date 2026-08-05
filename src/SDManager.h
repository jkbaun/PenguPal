#ifndef SD_MANAGER_H
#define SD_MANAGER_H

#include <Arduino.h>
#include "FS.h"
#include "SD_MMC.h"
#include "USB.h"
#include "USBMSC.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_defs.h"
#include "driver/sdmmc_types.h"

class SDManager {
public:
    // Constructor with default pin assignments for ESP32-S3
    SDManager(int clkPin = 39, int cmdPin = 38, int d0Pin = 40);

    bool begin();
    void end();

    bool isMounted() const { return _isMounted; }
    uint32_t getSectorCount() const { return _sectorCount; }
    uint32_t getSectorSize() const { return _sectorSize; }
    uint32_t getCapacityMB() const;
    
    sdmmc_card_t* getCard();

private:
    int _clkPin;
    int _cmdPin;
    int _d0Pin;
    
    bool _isMounted = false;
    uint32_t _sectorCount = 0;
    uint32_t _sectorSize = 512;

    USBMSC _msc;

    // Instance Pointer Bridge for USB MSC Callbacks
    static SDManager* _instance;

    // Static callbacks registered with USBMSC
    static int32_t onReadCB(uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize);
    static int32_t onWriteCB(uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize);
    static bool onStartStopCB(uint8_t power_condition, bool start, bool load_eject);

    // Instance level handlers
    int32_t handleRead(uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize);
    int32_t handleWrite(uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize);
};

#endif