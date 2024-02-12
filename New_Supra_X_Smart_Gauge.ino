#include <TFT_eSPI.h>

#include "./internal/screen/screen.cpp"

void setup() { initScreen(); }

void loop() { updateClock("88:88"); }
