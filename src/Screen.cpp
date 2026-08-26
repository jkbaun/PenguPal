#include "ScreenManager.h"

// Hardware driver and full-frame offscreen buffer (Sprite)
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite canvas = TFT_eSprite(&tft);

void initDisplay() {
    tft.init();
    tft.setRotation(2);
    tft.fillScreen(TFT_BLACK);

    // Allocate 16-bit canvas in RAM for double-buffering
    canvas.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT);
    canvas.setSwapBytes(true); // Correct byte ordering for RGB565 assets
}

void drawBackground() {
    // Stream base assets onto offscreen canvas
    canvas.pushImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, asset_bg_home);
    canvas.pushImage(0, 260, 240, 60, asset_bg_ui);
}

void drawIcon(int x, int y, int width, int height, const uint16_t* iconData, uint16_t transKey) {
    // Native transparent keying removes manual pixel-looping code
    canvas.pushImage(x, y, width, height, iconData, transKey);
}

void drawCursor(int x) {
    drawIcon(x, 260, 45, 55, asset_sprite_cursor, 0x0000);
}

void refreshScreen() {
    // Push completed buffer to display via fast SPI
    canvas.pushSprite(0, 0);
}