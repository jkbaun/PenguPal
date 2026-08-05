#include <Arduino.h>
#include "SDManager.h"

// Instantiate your storage manager object
SDManager sdManager(39, 38, 40);

void setup() {
    sdmanager.begin();
}

void loop() {
    // Main loop remains clear for other tasks
}