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
int spriteRingGaugeSize[] = {screenWidth, screenHeight};  // Width x Height

void initScreen(void) {
    // Init display
    mainScreen.init();                 // Initialize the TFT display
    mainScreen.fillScreen(TFT_BLACK);  // Fill the screen with black color
    mainScreen.setRotation(2);         // 2 = Flip vertically

    // Creating new sprite frame
    // Clock
    ringGaugeSprite.setColorDepth(8);
    spriteClock.createSprite(spriteClockSize[0], spriteClockSize[1]);
    // Ring gauge
    ringGaugeSprite.setColorDepth(8);
    ringGaugeSprite.createSprite(spriteRingGaugeSize[0], spriteRingGaugeSize[1]);
}

// updateRingGauge receive temperature value
void updateRingGauge(int temperature, int altitude) {
    // Reset content on the screen
    ringGaugeSprite.fillScreen(TFT_TRANSPARENT);

    /* Attention
        1. Start radius and end radius cannot be same
        2. Ketika remap value temperature start radius di tambah 1 agar tidak sama dengan end radius ketika temperature = 0
    */
    int ringThickness = maxCircleRadius - 10;  // Ketebalan 10px

    // Draw Arc (Lingkaran Cincin Kiri)
    // 0 start di jam 6 || 180 di jam 12
    int startRadius = 45;                                                   // Jam 7:30
    int endRadius = 135;                                                    // Jam 10:30
    int gaugeValue = map(temperature, 0, 120, startRadius + 1, endRadius);  // Remap temperature value, 0 - 120C range to radius value
    ringGaugeSprite.drawSmoothArc(midScreenCoordinate, midScreenCoordinate, maxCircleRadius, ringThickness, startRadius, gaugeValue, TFT_RED, TFT_RED, false);
    Serial.printf("temperature and gauge %d %d \n", temperature, gaugeValue);

    // Draw Arc (Lingkaran Cincin Kanan)
    startRadius = 225;                                                // Jam 1:30
    endRadius = 315;                                                  // Jam 4:30
    gaugeValue = map(altitude, 1500, 0, startRadius, endRadius - 1);  // Remap altitude value 0 - 1500 MDPL range to radius value
    ringGaugeSprite.drawSmoothArc(midScreenCoordinate, midScreenCoordinate, maxCircleRadius, ringThickness, gaugeValue, endRadius, TFT_BLUE, TFT_BLUE, false);
    Serial.printf("alt and gauge %d %d \n", altitude, gaugeValue);

    // Display information to screen
    ringGaugeSprite.pushSprite(0, 0, TFT_TRANSPARENT);  // Display information to screen
}

// updateClock receive hours and minutes
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