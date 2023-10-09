#ifndef ROLLER_BLIND_CONSTS_H
#define ROLLER_BLIND_CONSTS_H

#define EEPROM_SIZE 512

#define CALIBRATED_NO   1000
#define CALIBRATING     1001
#define CALIBRATED_YES  1002

#define DIRECTION_DOWN    -1
#define DIRECTION_STOP     0
#define DIRECTION_UP       1

#define REVERSED_NO     1001
#define REVERSED_YES    1002

#define OPENED_NO          0
#define OPENED_YES         1

#define WIFI_SAVED_NO   1000
#define WIFI_SAVED_YES  1001

#define DEBUG_ENABLED_NO   0
#define DEBUG_ENABLED_YES  1

#define USE_HALL_NO  1000
#define USE_HALL_YES 1002

#define DEVICES_GROUP "MyDevices"
#define NAME "Roller Blind"
#define ICON ""

#define CONNECTION_TRY_COUNT 20
#define AP_SSID_SIZE 33
#define AP_PASS_SIZE 65

#define AP_SSID_INPUT_SIZE AP_SSID_SIZE - 1
#define AP_PASS_INPUT_SIZE AP_PASS_SIZE - 1

#define MQTT_PARAM_ZISE  33
#define MQTT_PARAM_INPUT_SIZE MQTT_PARAM_ZISE - 1

#define SLIDER_OPEN_PERCENT "Position"
#define SLIDER_MAX_OPEN_PERCENT "Max Position"

#define CONTROL_PANEL "Control Panel"
#define WIFI_SETTINGS_PANEL "WiFi Settings"
#define MQTT_SETTINGS_PANEL "MQTT Settings"
#define CALIBRATION_PANEL "Calibration"
#define TABS String(CALIBRATION_PANEL) + ',' + String(CONTROL_PANEL) 
#define MAIN_MENU String(CONTROL_PANEL) + ',' + String(WIFI_SETTINGS_PANEL) + ',' + String(MQTT_SETTINGS_PANEL)

#define MOTOR_SPIN_HALL       4
#define TOP_POSITION_HALL     5

// #define DEFAULT_STEP              3600 // 10 оборотов 
#define DEFAULT_STEP 360 // 1 оборот

#endif
