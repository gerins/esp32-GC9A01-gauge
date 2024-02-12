#include <TFT_eSPI.h>

/*
Sprite adalah feature dari library TFT_eSPI yang digunakan
untuk update layar tanpa flickr dengan cara semua update pada
component ditampung di dalam RAM sebelum ditampilkan dilayar
 */
TFT_eSPI mainScreen = TFT_eSPI();                        // Create a screen object
TFT_eSprite spriteClock = TFT_eSprite(&mainScreen);      // Create a sprite object
TFT_eSprite ringGaugeSprite = TFT_eSprite(&mainScreen);  // Create a sprite object

// Base screen size
int screenWidth = mainScreen.width();       // 240
int screenHeight = mainScreen.height();     // 240
int midScreenCoordinate = screenWidth / 2;  // Mid coordinate
int maxCircleRadius = 120;                  // Max circle radius from mid to edge

// Sprite frame size configuration
int spriteClockSize[] = {140, 50};                        // Width x Height
int spriteRingGaugeSize[] = {screenWidth, screenHeight};  // Width x Height same as main screen size

void initScreen(void) {
    // Init display
    mainScreen.init();                 // Initialize the TFT display
    mainScreen.fillScreen(TFT_BLACK);  // Fill the screen with black color
    mainScreen.setRotation(2);         // 2 = Flip vertically

    // Creating new sprite frame
    spriteClock.createSprite(spriteClockSize[0], spriteClockSize[1]);              // Clock
    ringGaugeSprite.createSprite(spriteRingGaugeSize[0], spriteRingGaugeSize[1]);  // Ring gauge
}

// updateRingGauge receive temperature value
void updateRingGauge(int temperature) {
    // Reset content on the screen
    ringGaugeSprite.fillScreen(TFT_BLUE);

    // Draw Arc (Lingkaran Cincin)
    ringGaugeSprite.drawArc(midScreenCoordinate, midScreenCoordinate, maxCircleRadius, 50, 0, temperature, TFT_WHITE, TFT_WHITE, true);

    // Display information to screen
    ringGaugeSprite.pushSprite(0, 0, TFT_TRANSPARENT);  // Display information to screen
}

// updateClock receive time with format HH:MM
void updateClock(int hours, int minutes) {
    // Format current time in "HH:MM" format and store it in formattedTime
    char formattedTime[5];
    sprintf(formattedTime, "%02d:%02d", hours, minutes);

    // Reset content on the screen
    spriteClock.fillScreen(TFT_TRANSPARENT);

    // Draw time information
    spriteClock.drawString(formattedTime, 0, 0, 7);

    // Display information to screen
    int midX = midScreenCoordinate - spriteClockSize[0] / 2;  // Middle Width
    int midY = midScreenCoordinate - spriteClockSize[1] / 2;  // Middle Height
    spriteClock.pushSprite(midX, midY, TFT_TRANSPARENT);
}

// Set text
void writeText(String input, int textSize, int color, int x, int y) {
    mainScreen.setCursor(x, y);        // Set cursor position
    mainScreen.setTextSize(textSize);  // Set text size
    mainScreen.setTextColor(color);    // Set text color to white
    mainScreen.println(input);         // Print "Hello, World!" on the TFT display
}