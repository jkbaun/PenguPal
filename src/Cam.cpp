// cam.cpp
#include "CamManager.h"

// Constructor implementation
CamManager::CamManager() {
    // You can initialize variables here if needed in the future
}

// Destructor implementation
CamManager::~CamManager() {
    // Clean up any dynamically allocated memory here
}

bool CamManager::initialize() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer   = LEDC_TIMER_0;
    
    // Data Pins
    config.pin_d0 = 11; // CAM_Y2
    config.pin_d1 = 9;  // CAM_Y3
    config.pin_d2 = 8;  // CAM_Y4
    config.pin_d3 = 10; // CAM_Y5
    config.pin_d4 = 12; // CAM_Y6
    config.pin_d5 = 18; // CAM_Y7
    config.pin_d6 = 17; // CAM_Y8
    config.pin_d7 = 16; // CAM_Y9
    
    // Control Pins
    config.pin_xclk  = 15; // CAM_XCLK
    config.pin_pclk  = 13; // CAM_PCLK
    config.pin_vsync = 6;  // CAM_VYSNC
    config.pin_href  = 7;  // CAM_HREF
    config.pin_sccb_sda = 4; // CAM_SIOD
    config.pin_sccb_scl = 5; // CAM_SIOC
    
    // Unused/Default Pins
    config.pin_pwdn  = -1;
    config.pin_reset = -1;
    
    // Hardware constraints and formatting
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size   = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count     = 1;

    // Initialize the camera hardware
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x\n", err);
        return false;
    }
    Serial.println("Camera successfully initialized.");
    return true;
}

void CamManager::takeAndSavePhoto(const char* path) {
    // 1. Capture the frame
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("ERROR: Camera capture failed!");
        return;
    }

    // 2. Write the frame buffer to the SD Card
    File file = SD_MMC.open(path, FILE_WRITE);
    if(!file) {
        Serial.println("ERROR: Failed to open file in writing mode!");
    } else {
        file.write(fb->buf, fb->len);
        Serial.printf("Saved image to path: %s\n", path);
    }
    
    // 3. Clean up
    file.close();
    esp_camera_fb_return(fb);
}