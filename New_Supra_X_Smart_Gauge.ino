/************************************************************************************
 *                 Library
 ************************************************************************************/
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <lvgl.h>

#include "./internal/screen/screen.cpp"

/************************************************************************************
 *            LVGL Configuration
 ************************************************************************************/
#if LV_USE_TFT_ESPI
#include <TFT_eSPI.h>
#endif

#if LV_USE_LOG != 0
void my_print(lv_log_level_t level, const char *buf) {
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif

/*Set to your screen resolution and rotation*/
#define TFT_HOR_RES 240
#define TFT_VER_RES 240
#define TFT_ROTATION LV_DISPLAY_ROTATION_180

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];
lv_display_t *disp;  // Main display
lv_obj_t *timeLabel;

/*use Arduinos millis() as tick source*/
static uint32_t my_tick(void) { return millis(); }

lv_obj_t *CreateBaseArc(uint32_t indicatorColour, uint32_t backgroundColour, int arcSize, int minRange, int maxrange) {
    // Background arc, solid color
    lv_obj_t *backgroundArc = lv_arc_create(lv_screen_active());
    lv_arc_set_value(backgroundArc, 0);
    lv_arc_set_bg_angles(backgroundArc, 90, 270);                 // Arc length, 90 start at 6 o'clock
    lv_obj_center(backgroundArc);                                 // Middle position
    lv_obj_set_size(backgroundArc, arcSize + 5, arcSize + 5);     // Set width and height
    lv_obj_set_style_arc_width(backgroundArc, 20, LV_PART_MAIN);  // Ketebalan arc
    lv_obj_remove_style(backgroundArc, NULL, LV_PART_KNOB);       // Remove knob
    lv_obj_remove_flag(backgroundArc, LV_OBJ_FLAG_CLICKABLE);     // Static arc
    lv_obj_set_style_arc_color(backgroundArc, lv_color_hex(indicatorColour), LV_PART_MAIN);
    lv_obj_set_style_arc_width(backgroundArc, 20, LV_PART_MAIN);

    // Main arc
    lv_obj_t *newArc = lv_arc_create(lv_screen_active());

    // Spec
    lv_arc_set_range(newArc, minRange, maxrange);  // Value range
    lv_arc_set_bg_angles(newArc, 90, 270);         // Arc length, start at 6 o clock end at 12 o clock
    // Style
    lv_obj_center(newArc);                                                                 // Middle position
    lv_obj_set_size(newArc, arcSize, arcSize);                                             // Set width and height
    lv_obj_remove_style(newArc, NULL, LV_PART_KNOB);                                       // Remove knob
    lv_obj_remove_flag(newArc, LV_OBJ_FLAG_CLICKABLE);                                     // Static arc
    lv_obj_set_style_arc_color(newArc, lv_color_hex(backgroundColour), LV_PART_MAIN);      // LV_PART_MAIN = Glass
    lv_obj_set_style_arc_color(newArc, lv_color_hex(indicatorColour), LV_PART_INDICATOR);  // LV_PART_INDICATOR = Water
    lv_obj_set_style_arc_width(newArc, 15, LV_PART_MAIN);
    lv_obj_set_style_arc_width(newArc, 15, LV_PART_INDICATOR);

    return newArc;
}

/************************************************************************************
 *            WiFi Configuration
 ************************************************************************************/
const char *ssid = "AWS Network 2.4 GHz";
const char *password = "kucingminumkopi";
WiFiUDP ntpUDP;

// By default 'pool.ntp.org' is used with 60 seconds update interval and no offset
// Documentation https://github.com/arduino-libraries/NTPClient
// 25200L is 25200 second offset for GMT+7
NTPClient timeClient(ntpUDP, "pool.ntp.org", 25200L, 3600);

void setup() {
    Serial.begin(115200);

    /************************************************************************************
     *            LVGL Initialization
     ************************************************************************************/
    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
    Serial.println(LVGL_Arduino);
    lv_init();

    /*Set a tick source so that LVGL will know how much time elapsed. */
    lv_tick_set_cb(my_tick);

#if LV_USE_LOG != 0
    lv_log_register_print_cb(my_print);
#endif

    /*TFT_eSPI can be enabled lv_conf.h to initialize the display in a simple way*/
    disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, sizeof(draw_buf));
    lv_display_set_rotation(disp, TFT_ROTATION);

    // ARC 1
    lv_obj_t *rpmArc = CreateBaseArc(0xFF34A5F1, 0xFF183C52, 220, 0, 7000);
    lv_arc_set_value(rpmArc, 3000);
    lv_obj_t *rpmLabel = lv_label_create(lv_screen_active());            // Create text
    lv_obj_align(rpmLabel, LV_ALIGN_TOP_MID, 29, 9);                     // Position
    lv_obj_set_style_text_color(rpmLabel, lv_color_hex(0xFF34A5F1), 0);  // Change text color
    lv_label_set_text(rpmLabel, "max");

    // ARC 2
    lv_obj_t *engineTempArc = CreateBaseArc(0xFF7D1716, 0xFF311410, 170, 0, 120);
    lv_arc_set_value(engineTempArc, 90);
    lv_obj_t *tempLabel = lv_label_create(lv_screen_active());            // Create text
    lv_obj_align(tempLabel, LV_ALIGN_TOP_MID, 30, 35);                    // Position
    lv_obj_set_style_text_color(tempLabel, lv_color_hex(0xFF7D1716), 0);  // Change text color
    lv_label_set_text(tempLabel, "90 c");
    // lv_obj_t *tempSymbol = lv_label_create(lv_scr_act());                 //
    // lv_obj_align(tempSymbol, LV_ALIGN_CENTER, 0, -108);                   // Center position
    // lv_obj_set_style_text_font(tempSymbol, &custom_symbol_16, 0);         // Change font style
    // lv_obj_set_style_text_color(tempSymbol, lv_color_hex(0xFFD72749), 0); // Change text color
    // lv_label_set_text(tempSymbol, tempLogo);                              // Set logo symbol

    // ARC 3
    lv_obj_t *voltArc = CreateBaseArc(0xFFFFC848, 0xFF524C20, 120, 0, 16);
    lv_arc_set_value(voltArc, 12);
    lv_obj_t *voltLabel = lv_label_create(lv_screen_active());            // Create text
    lv_obj_align(voltLabel, LV_ALIGN_TOP_MID, 30, 60);                    // Position
    lv_obj_set_style_text_color(voltLabel, lv_color_hex(0xFFFFC848), 0);  // Change text color
    lv_label_set_text(voltLabel, "13.2v");

    timeLabel = lv_label_create(lv_screen_active());                      // Create text
    lv_obj_align(timeLabel, LV_ALIGN_CENTER, 0, 0);                       // Center
    lv_obj_set_style_text_color(timeLabel, lv_color_hex(0xFF1E2021), 0);  // Change text color
    lv_label_set_text(timeLabel, "3000");

    /************************************************************************************
     *            WiFi Initialization
     ************************************************************************************/
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }

    Serial.println("Connected to WiFi");

    // Initialize devices
    // initScreen();        // Screen
    timeClient.begin();  // NTP real time clock client
}

void loop() {
    // Get current time
    timeClient.update();

    // int hours = timeClient.getHours();
    // int minutes = timeClient.getMinutes();

    lv_timer_handler(); /* let the LVGL GUI do its work */
    lv_label_set_text(timeLabel, timeClient.getFormattedTime().c_str());

    // // Update screen
    // updateRingGauge(temp, alt);
    // updateClock(hours, minutes);

    // temp += 1;
    // alt += 50;

    // if (temp >= 120) {
    //     temp = 0;
    // }
    // if (alt >= 1500) {
    //     alt = 0;
    // }

    delay(5);
}
