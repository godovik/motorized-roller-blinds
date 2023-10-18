#define ATOMIC_FS_UPDATE // OTA обновление сжатым .gz файлом (вместо .bin)
#define ROLLER_BLIND_LANG_RU

#include <Arduino.h>
#include <EEPROM.h>
#include <GyverStepper.h>
#include <GyverHub.h>
#include <RollerBlindConsts.h>
#include <EepromSync.h>
#include <HubWiFiSettings.h>
#include <HubMqttSettings.h>
#include <HubMotorSettings.h>
#include <RollerBlindInterface.h>

// todo: allow ability to rename 
GyverHub hub(DEVICES_GROUP, NAME, ICON);
GStepper <STEPPER4WIRE, STEPPER_VIRTUAL> motor(2048);

HubWiFiSettings       wifiSettings(&hub);
HubMqttSettings       mqttSettings(&hub);
HubMotorSettings      motorSettings(&hub);
RollerBlind           rollerBlind(&motor, &motorSettings);
RollerBlindInterface  rollerBlindInterface(&hub, &rollerBlind);


void readSettings() {
  wifiSettings.read();
  mqttSettings.read();
  motorSettings.read();
}

void build() {
  if (hub.Menu(F(MAIN_MENU))) {
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
      
    case 3:
      motorSettings.draw();
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

void loop() {
  hub.tick();
  rollerBlind.tick();
  rollerBlindInterface.updateValues();
}
