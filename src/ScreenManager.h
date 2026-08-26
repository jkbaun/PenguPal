#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <TFT_eSPI.h>

// Screen Dimensions
#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 320

// Core Drivers
extern TFT_eSPI tft;
extern TFT_eSprite canvas;

// Core Lifecycle
void initDisplay();
void refreshScreen();

// UI & Drawing Methods
void drawBackground();
void drawCursor(int x);
void drawIcon(int x, int y, int width, int height, const uint16_t* iconData, uint16_t transKey = 0x0000);

#endif