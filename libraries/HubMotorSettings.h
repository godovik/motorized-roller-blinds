#include <GyverHub.h>
#include <RollerBlindConsts.h>


#define VALIDATION_REGEX "^1[2346]$"

class HubMotorSettings {
  private:
    GyverHub* hub;

    int8_t pin1 = 0;
    int8_t pin2 = 0;
    int8_t pin3 = 0;
    int8_t pin4 = 0;

    EepromSync<int8_t> pin1_storage  = EepromSync<int8_t>(pin1, { 12, 16, 13});
    EepromSync<int8_t> pin2_storage  = EepromSync<int8_t>(pin2, { 12, 16, 16});
    EepromSync<int8_t> pin3_storage  = EepromSync<int8_t>(pin3, { 12, 16, 12});
    EepromSync<int8_t> pin4_storage  = EepromSync<int8_t>(pin4, { 12, 16, 14});

    GHbutton btn_save;
    
    void save() {
      this->pin1_storage.save();
      this->pin2_storage.save();
      this->pin3_storage.save();
      this->pin4_storage.save_and_commit();
    }

  public:
    HubMotorSettings(GyverHub* hub) {
      this->hub = hub;
    }

    int8_t getPin1() { return this->pin1; }
    int8_t getPin2() { return this->pin2; }
    int8_t getPin3() { return this->pin3; }
    int8_t getPin4() { return this->pin4; }

    void read() {
      this->pin1_storage.read();
      this->pin2_storage.read();
      this->pin3_storage.read();
      this->pin4_storage.read();
    }

    void draw() {
      if (this->btn_save.changed() && this->btn_save) {
        Serial.println("btn_save");
        this->save();
      }

      this->hub->BeginWidgets();
      this->hub->WidgetSize(25);
      this->hub->Input_(F("pin1"), &this->pin1, GH_INT8, F(PIN1_INPUT), 2, F(VALIDATION_REGEX));
      this->hub->Input_(F("pin2"), &this->pin2, GH_INT8, F(PIN2_INPUT), 2, F(VALIDATION_REGEX));
      this->hub->Input_(F("pin3"), &this->pin3, GH_INT8, F(PIN3_INPUT), 2, F(VALIDATION_REGEX));
      this->hub->Input_(F("pin4"), &this->pin4, GH_INT8, F(PIN4_INPUT), 2, F(VALIDATION_REGEX));
      this->hub->WidgetSize(25);
      this->hub->EndWidgets();
      this->hub->Button_(F("connect"), &this->btn_save, F(BTN_SAVE));
    }
};
