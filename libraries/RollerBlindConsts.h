#ifndef ROLLER_BLIND_CONSTS_H
#define ROLLER_BLIND_CONSTS_H

#ifdef ROLLER_BLIND_LANG_RU
    #define NAME "Рулонная штора"

    #define SLIDER_OPEN_PERCENT     "Положение"
    #define SLIDER_MIN_OPEN_PERCENT "Минимальное положение"
    #define SLIDER_MAX_OPEN_PERCENT "Максимальное положение"

    #define CONTROL_PANEL       "Управление"
    #define CALIBRATION_PANEL   "Настройка"
    #define WIFI_SETTINGS_PANEL "Настройки WiFi"
    #define MQTT_SETTINGS_PANEL "Настройки MQTT"

    #define BTN_OPEN      "открыть"
    #define BTN_CLOSE     "закрыть"
    #define BTN_STOP      "остановить"
    #define BTN_UP        "вверх"
    #define BTN_DOWN      "вниз"
    #define BTN_REVERSE   "развернуть"
    #define BTN_RESET     "сбросить"
    #define BTN_CALIBRATE "откалибровать"
    #define BTN_USER_HALL "use hall" // todo: придумать, короткое название
    #define BTN_CONNECT   "подключиться"

    #define WIFI_SSID_INPUT "WiFi ssid"
    #define WIFI_PASS_INPUT "WiFi пароль"

    #define MQTT_HOST_INPUT  "MQTT хост"
    #define MQTT_PORT_INPUT  "MQTT порт"
    #define MQTT_LOGIN_INPUT "Логин"
    #define MQTT_PASS_INPUT  "Пароль"

    #define BTN_CONNECT "подключиться"
#else
    #define NAME "Roller Blind"

    #define SLIDER_OPEN_PERCENT     "Position"
    #define SLIDER_MIN_OPEN_PERCENT "Min Position"
    #define SLIDER_MAX_OPEN_PERCENT "Max Position"

    #define CONTROL_PANEL       "Control Panel"
    #define WIFI_SETTINGS_PANEL "WiFi Settings"
    #define MQTT_SETTINGS_PANEL "MQTT Settings"
    #define CALIBRATION_PANEL   "Calibration"

    #define BTN_OPEN      "open"
    #define BTN_CLOSE     "close"
    #define BTN_STOP      "stop"
    #define BTN_UP        "up"
    #define BTN_DOWN      "down"
    #define BTN_REVERSE   "reverse"
    #define BTN_RESET     "reset"
    #define BTN_CALIBRATE "calibrate"
    #define BTN_USER_HALL "use hall"
    #define BTN_CONNECT   "connect"

    #define WIFI_SSID_INPUT "WiFi ssid"
    #define WIFI_PASS_INPUT "WiFi password"

    #define MQTT_HOST_INPUT  "MQTT host"
    #define MQTT_PORT_INPUT  "MQTT port"
    #define MQTT_LOGIN_INPUT "Login"
    #define MQTT_PASS_INPUT  "Password"
#endif

#define DEVICES_GROUP "MyDevices"

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
#define ICON ""

#define CONNECTION_TRY_COUNT 20
#define AP_SSID_SIZE 33
#define AP_PASS_SIZE 65

#define AP_SSID_INPUT_SIZE AP_SSID_SIZE - 1
#define AP_PASS_INPUT_SIZE AP_PASS_SIZE - 1

#define MQTT_PARAM_ZISE  33
#define MQTT_PARAM_INPUT_SIZE MQTT_PARAM_ZISE - 1

#define DELIMITER ","
#define TABS CALIBRATION_PANEL DELIMITER CONTROL_PANEL
#define MAIN_MENU CONTROL_PANEL DELIMITER WIFI_SETTINGS_PANEL DELIMITER MQTT_SETTINGS_PANEL

#define MOTOR_SPIN_HALL       4
#define TOP_POSITION_HALL     5

// #define DEFAULT_STEP              3600 // 10 оборотов 
#define DEFAULT_STEP 360 // 1 оборот

#endif
