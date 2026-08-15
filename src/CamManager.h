// cammanager.h
#pragma once

#include <Arduino.h>
#include "esp_camera.h"
#include "FS.h"
#include "SD_MMC.h"

class CamManager {
public:
    // Constructor & Destructor
    CamManager(); 
    ~CamManager();

    // Initializes the camera hardware and pins
    bool initialize();
    
    // Captures a frame and saves it to the SD card
    void takeAndSavePhoto(const char* path);
};