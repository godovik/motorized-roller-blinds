#define ATOMIC_FS_UPDATE // OTA обновление сжатым .gz файлом (вместо .bin)

#include <Arduino.h>
#include <EEPROM.h>
#include <GyverStepper.h>
#include <math.h>
#include <GyverHub.h>
#include <RollerBlindConsts.h>
#include <EepromSync.h>
#include <HubWiFiSettings.h>
#include <HubMqttSettings.h>
#include <RollerBlindInterface.h>

short 
  debug_enabled;

GyverHub hub(DEVICES_GROUP, NAME, ICON);
GStepper <STEPPER4WIRE> motor(2048, 13, 16, 12, 14); // для первой и второй версии печатной платы использовать "16, 12, 14, 13"

HubWiFiSettings wifiSettings(&hub);
HubMqttSettings mqttSettings(&hub);
RollerBlind     rollerBlind(&motor);

EepromSync<short> debugEnabledReader(debug_enabled, { DEBUG_ENABLED_NO, DEBUG_ENABLED_YES, DEBUG_ENABLED_NO });

RollerBlindInterface rollerBlindInterface(&hub, &rollerBlind);

void readSettings() {
  wifiSettings.read();
  mqttSettings.read();

  debugEnabledReader.read();
}

void build() {
  // todo: use MAIN_MENU define
  if (hub.Menu(F("Control panel,WiFi Settings,MQTT Settings"))) {
    hub.refresh();
  }

  switch (hub.menu) {
    case 0:
      rollerBlindInterface.draw();
      break;

    case 1:
      wifiSettings.draw();
      break;
      
    case 2:
      mqttSettings.draw();
      break;
  }
}

void onReboot(GHreason_t reason) {
  hub.turnOff();
}

void setupHub() {
  hub.onBuild(build);
  hub.onReboot(onReboot);
  hub.begin();
  hub.turnOn();
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);

  Serial.println("============================================================");
  Serial.println("============================================================");
  Serial.println("============================================================");
  Serial.println("============================================================");
  Serial.println("============================================================");
  Serial.println("============================================================");

  readSettings();
  wifiSettings.connect();
  mqttSettings.connect();
  setupHub();
  rollerBlind.setup();
  rollerBlindInterface.setup();
}

bool motor_spin_hall_active = false;
bool top_position_hall_active = false;


void loop() {
  hub.tick();
  rollerBlind.tick();
  rollerBlindInterface.updateValues();
}
