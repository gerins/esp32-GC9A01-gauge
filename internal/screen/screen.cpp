#include <TFT_eSPI.h>

/*
Sprite adalah feature dari library TFT_eSPI yang digunakan
untuk update layar tanpa flickr dengan cara semua update pada
component ditampung di dalam RAM sebelum ditampilkan dilayar
 */
TFT_eSPI mainScreen = TFT_eSPI();                    // Create a screen object
TFT_eSprite spriteClock = TFT_eSprite(&mainScreen);  // Create a sprite object

// Base screen size
int screenWidth = mainScreen.width();    // 240
int screenHeight = mainScreen.height();  // 240
int midScreenCoordinate = screenWidth / 2;

// Sprite configuration
int spriteClockSize[] = {140, 50};  // Width x Height

void initScreen(void) {
    // Init display
    mainScreen.init();                 // Initialize the TFT display
    mainScreen.fillScreen(TFT_BLACK);  // Fill the screen with black color
    mainScreen.setRotation(2);         // 2 = Flip vertically

    // Creating new sprite frame for clock
    spriteClock.createSprite(spriteClockSize[0], spriteClockSize[1]);  // Create frame
}

void updateClock(String timeNow) {
    spriteClock.fillScreen(TFT_TRANSPARENT);  // Reset content on the screen

    // Draw time information
    spriteClock.drawString(timeNow, 0, 0, 7);

    int midX = midScreenCoordinate - spriteClockSize[0] / 2;  // Middle Width
    int midY = midScreenCoordinate - spriteClockSize[1] / 2;  // Middle Height
    spriteClock.pushSprite(midX, midY, TFT_TRANSPARENT);      // Display information to screen
}

// Set text
void writeText(String input, int textSize, int color, int x, int y) {
    mainScreen.setCursor(x, y);        // Set cursor position
    mainScreen.setTextSize(textSize);  // Set text size
    mainScreen.setTextColor(color);    // Set text color to white
    mainScreen.println(input);         // Print "Hello, World!" on the TFT display
}