

#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#include "./internal/screen/screen.cpp"

const char* ssid = "AWS Network 2.4 GHz";
const char* password = "kucingminumkopi";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, 25200L);  // 25200L is 25200 second offset for GMT+7

void setup() {
    initScreen();

    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to WiFi");

    // Initialize NTP client
    timeClient.begin();
}

void loop() {
    // Update the time client
    timeClient.update();

    // Get current time
    int hours = timeClient.getHours();
    int minutes = timeClient.getMinutes();

    // Print current time in "HH:MM" format
    char formattedTime[5];

    // Format current time in "HH:MM" format and store it in formattedTime
    sprintf(formattedTime, "%02d:%02d", hours, minutes);

    updateClock(formattedTime);
    delay(1000);  // Update every second
}
