// main.cpp
#include <Arduino.h>
#include "SDManager.h"
#include "CamManager.h" // Include your new camera manager

#include "InputManager.h"

#define BUTTON_PIN 3

// Instantiate your storage and camera objects
SDManager sdManager(39, 38, 40);
CamManager camManager; 

// Variables for file naming and button debouncing
int pictureCount = 0;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT);
    
    // Start SD card and USB MSC
    if (sdManager.begin()) {
        Serial.println("SD Manager initialized successfully.");
    }

    // Initialize Camera using the newly refactored class
    camManager.initialize();
    
    Serial.println("Ready. Press the button to take a photo!");
}

void loop() {
    int reading = digitalRead(BUTTON_PIN);

    if (reading != lastButtonState) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (reading == LOW) {
            
            if (sdManager.isMounted()) {
                char filename[32];
                snprintf(filename, sizeof(filename), "/image_%d.jpg", pictureCount);
                
                Serial.printf("Button pressed! Capturing %s...\n", filename);
                
                // Call the refactored capture function
                camManager.takeAndSavePhoto(filename);
                
                pictureCount++; 
            } else {
                Serial.println("Cannot save photo: SD Manager is not mounted.");
            }
            
            while (digitalRead(BUTTON_PIN) == LOW) {
                delay(10);
            }
        }
    }
    
    lastButtonState = reading;
}