#include <Arduino.h>
#include "ScreenManager.h"
#include "SDManager.h"

// Instantiate SD Manager on 1-bit SD_MMC pins (CLK: 39, CMD: 38, D0: 40)
SDManager sdManager(39, 38, 40);

void setup() {
    // 1. Initialize Serial monitor for debugging output
    Serial.begin(115200);

    // 2. Initialize the TFT display driver & sprite canvas
    initDisplay();
    canvas.fillScreen(TFT_BLUE);
    delay(1000);
    refreshScreen();

    // 3. Mount the SD card & start USB MSC
    if (sdManager.begin()) {
        Serial.println("SD Card initialized successfully!");
        Serial.printf("SD Card Capacity: %u MB\n", sdManager.getCapacityMB());
        canvas.fillScreen(TFT_GREEN);
        delay(1000);
        refreshScreen();
    } else {
        Serial.println("SD Card initialization failed!");
        canvas.fillScreen(TFT_RED);
        delay(1000);
        refreshScreen();
    }

    // 4. (Optional) Perform an initial screen refresh
    refreshScreen();
}

void loop() {
    Serial.println("yall gay");
    delay(1000);
    // Keep loop non-blocking; USB MSC callbacks run in background tasks
}