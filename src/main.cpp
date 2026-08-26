#include <Arduino.h>
#include "ScreenManager.h"[cite: 3, 5]

void setup() {
    Serial.begin(115200);
    delay(2000); // Allow USB CDC Serial terminal to attach
    Serial.println("--- Starting Direct TFT Hardware Test ---");

    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    
    Serial.println("TFT Initialized successfully!");
}

void loop() {
    Serial.println("Displaying RED...");
    tft.fillScreen(TFT_RED);
    delay(1000);

    Serial.println("Displaying GREEN...");
    tft.fillScreen(TFT_GREEN);
    delay(1000);

    Serial.println("Displaying BLUE...");
    tft.fillScreen(TFT_BLUE);
    delay(1000);
}