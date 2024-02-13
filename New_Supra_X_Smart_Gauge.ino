

#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#include "./internal/screen/screen.cpp"

const char* ssid = "AWS Network 2.4 GHz";
const char* password = "kucingminumkopi";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, 25200L);  // 25200L is 25200 second offset for GMT+7

void setup() {
    Serial.begin(115200);

    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }

    Serial.println("Connected to WiFi");

    // Initialize devices
    initScreen();        // Screen
    timeClient.begin();  // NTP client
}

int temp = 0;
int alt = 0;

void loop() {
    // Get current time
    timeClient.update();
    int hours = timeClient.getHours();
    int minutes = timeClient.getMinutes();

    // Update screen
    updateRingGauge(temp, alt);
    updateClock(hours, minutes);

    temp += 1;
    alt += 50;

    if (temp >= 120) {
        temp = 0;
    }
    if (alt >= 1500) {
        alt = 0;
    }
    delay(1000);
}
