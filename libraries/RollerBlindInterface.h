#include <GyverHub.h>
#include <RollerBlind.h>
#include <RollerBlindConsts.h>

class RollerBlindInterface {
  private:
    uint8_t tab;

    GyverHub* hub;
    RollerBlind* rollerBlind;

    GHbutton btn_up;
    GHbutton btn_down;
    GHbutton btn_stop;
    GHbutton btn_reverse;
    GHbutton btn_reset;
    GHbutton btn_calibrate;
    GHbutton btn_use_hall;
    GHbutton btn_connect;
    GHbutton btn_open;
    GHbutton btn_close;

    bool opened, top_position_hall, motor_spin_hall;

    short percent_position, percent_max_position;
    short length, direction, position, target_position, reversed, calibration_status;

    String getDirection() {
      if (this->direction == DIRECTION_UP) {
        return "up";
      }
      if (this->direction == DIRECTION_DOWN) {
        return "down";
      }
      return "stop";
    }

    String getReversed() {
      if (this->reversed == REVERSED_YES) {
        return "yes";
      }
      return "no";
    }

    String getCalibrationStatus() {
      if (this->calibration_status == CALIBRATED_YES) {
        return "CALIBRATED_YES";
      }
      if (this->calibration_status == CALIBRATED_NO) {
        return "CALIBRATED_NO";
      }
      if (this->calibration_status == CALIBRATING) {
        return "CALIBRATING";
      }
      return "error";
    }

    void drawDebugInterface() {
      this->hub->BeginWidgets();
      this->hub->WidgetSize(50);
      this->hub->Label_(F("length"), String(this->length), F("length"));
      this->hub->Label_(F("direction"), this->getDirection(), F("direction"));
      this->hub->WidgetSize(50);
      this->hub->Label_(F("position"), String(this->position), F("position"));
      this->hub->Label_(F("target_position"), String(this->target_position), F("target_position"));
      this->hub->WidgetSize(20);
      this->hub->Label_(F("reversed"), this->getReversed(), F("reversed"), GH_DEFAULT, 15);
      this->hub->LED_(F("top_hall"), this->top_position_hall, F("top hall"));
      this->hub->LED_(F("spin_hall"), this->motor_spin_hall, F("spin hall"));
      this->hub->WidgetSize(40);
      this->hub->Label_(F("calibration"), this->getCalibrationStatus(), F("calibration"), GH_DEFAULT, 15);
      this->hub->EndWidgets();
      this->hub->Space(24);
    }

    void processCalibrationButtons() {
      if (this->btn_up.changed() && this->btn_up) {
        Serial.println("btn_up");
        this->rollerBlind->up();
      }

      if (this->btn_down.changed() && this->btn_down) {
        Serial.println("btn_down");
        this->rollerBlind->down();
      }

      if (this->btn_stop.changed() && this->btn_stop) {
        Serial.println("btn_stop");
        this->rollerBlind->stop();
      }

      if (this->btn_reverse.changed() && this->btn_reverse) {
        Serial.println("btn_reverse");
        this->rollerBlind->reverse();
      }

      if (this->btn_reset.changed() && this->btn_reset) {
        Serial.println("btn_reset");
        this->rollerBlind->reset();
      }

      if (this->btn_calibrate.changed() && this->btn_calibrate) {
        Serial.println("btn_calibrate");
        this->rollerBlind->calibrate();
      }

      if (this->btn_use_hall.changed() && this->btn_use_hall) {
        Serial.println("btn_use_hall");
        this->rollerBlind->toggle_use_hall();
      }
    }

    void drawCalibrationInterface() {
      drawDebugInterface();
      this->hub->BeginWidgets();
      this->hub->WidgetSize(33);
      this->hub->Button_(F("up"), &this->btn_up, F("up"));
      this->hub->Button_(F("down"), &this->btn_down, F("down"));
      this->hub->Button_(F("stop"), &this->btn_stop, F("stop"));
      this->hub->WidgetSize(25);
      this->hub->Button_(F("reverse"), &this->btn_reverse, F("reverse"));
      this->hub->Button_(F("reset"), &this->btn_reset, F("reset"), GH_RED);
      this->hub->Button_(F("calibrate"), &this->btn_calibrate, F("calibrate"), GH_GREEN);
      this->hub->Button_(F("use hall sensor"), &this->btn_use_hall, F("use_hall"), this->rollerBlind->useHallSensor() ? GH_GREEN : GH_RED);
      this->hub->EndWidgets();
      this->hub->Space(24);
    }
    
    void processControlButtons() {
      if (this->btn_open.changed() && this->btn_open) {
        Serial.println("btn_open");
        this->rollerBlind->open();
      }

      if (this->btn_stop.changed() && this->btn_stop) {
        Serial.println("btn_stop");
        this->rollerBlind->stop();
      }

      if (this->btn_close.changed() && this->btn_close) {
        Serial.println("btn_close");
        this->rollerBlind->close();
      }
    }

    void drawControlPanel() {
      this->hub->BeginWidgets();
      if (this->hub->Slider_(F("percent_position"), &this->percent_position, GH_UINT16, F(SLIDER_OPEN_PERCENT), 0, 100, 1)) {
        this->rollerBlind->setPercentPosition(this->percent_position);
      }

      if (this->hub->Slider_(F("percent_max_position"), &this->percent_max_position, GH_UINT16, F(SLIDER_MAX_OPEN_PERCENT), 1, 100, 1)) {
        this->rollerBlind->setPercentMaxPosition(this->percent_max_position);
      }
      this->hub->WidgetSize(33);
      this->hub->Button_(F("open"), &this->btn_open, F("open"));
      this->hub->Button_(F("stop"), &this->btn_stop, F("stop"));
      this->hub->Button_(F("close"), &this->btn_close, F("close"));
      this->hub->EndWidgets();
      if (this->hub->Dummy_("opened", &this->opened, GH_INT8)) {
        if (this->opened) {
          this->rollerBlind->open();
        } else {
          this->rollerBlind->close();
        }
      }
    }

    void processHall() {
      bool next_top_position_hall = !digitalRead(TOP_POSITION_HALL);
      if (next_top_position_hall != this->top_position_hall) {
        this->top_position_hall = next_top_position_hall;
        this->hub->sendUpdate("top_hall");
        Serial.print("top_hall "); Serial.println(this->top_position_hall);
      }

      bool next_motor_spin_hall = !digitalRead(MOTOR_SPIN_HALL);
      if (next_motor_spin_hall != this->motor_spin_hall) {
        this->motor_spin_hall = next_motor_spin_hall;
        this->hub->sendUpdate("spin_hall");
        Serial.print("spin_hall "); Serial.println(this->motor_spin_hall);
      }
    }

    void readValues() {
      this->length = this->rollerBlind->getLength();
      this->direction = this->rollerBlind->getDirection();
      this->position = this->rollerBlind->getCurrent();
      this->target_position = this->rollerBlind->getTarget();
      this->reversed = this->rollerBlind->getReversed();
      this->calibration_status = this->rollerBlind->getCalibrationStatus();
      this->opened = this->rollerBlind->isOpened();
    }

    void updateAll() {
      // todo: send only when debug enabled
      this->hub->sendUpdate("position", String(this->position));
      this->hub->sendUpdate("target_position", String(this->target_position));
      this->hub->sendUpdate("direction", this->getDirection());
      this->hub->sendUpdate("length", String(this->length));
      this->hub->sendUpdate("calibration", this->getCalibrationStatus());
      this->hub->sendUpdate("reversed", this->getReversed());
      this->hub->sendUpdate("opened", String(this->opened));
    }

  public:
    RollerBlindInterface(GyverHub* hub, RollerBlind* rollerBlind) {
      this->hub = hub;
      this->rollerBlind = rollerBlind;
    }

    void setup() {
      this->percent_position = this->rollerBlind->getPercentPosition();
      this->percent_max_position = this->rollerBlind->getPercentMaxPosition();
    }

    void updateValues() {
      readValues();
      processHall();
      static GHtimer tmr(1000);
      if (tmr) {
        this->updateAll();
        this->hub->refresh();
      }
    }

    void draw() {
      processCalibrationButtons();
      processControlButtons();

      if (this->hub->Tabs(&this->tab, TABS)) {
        this->hub->refresh();
      }

      if (this->tab == 0 || this->hub->buildRead()) {
        this->drawCalibrationInterface();
      }

      if (this->tab == 1 || this->hub->buildRead()) {
        this->drawControlPanel();
      }
    }
};
