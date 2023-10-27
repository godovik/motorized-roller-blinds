#include <GyverHub.h>
#include <RollerBlind.h>
#include <RollerBlindConsts.h>

class RollerBlindInterface {
  private:
    uint8_t tab;

    GyverHub* hub;
    RollerBlind* rollerBlind;

    GHbutton 
      btn_up,
      btn_down,
      btn_stop,
      btn_reverse,
      btn_reset,
      btn_calibrate,
      btn_use_hall,
      btn_connect,
      btn_open,
      btn_close;

    bool 
      opened,
      top_position_hall,
      motor_spin_hall,
      show_dedbug_interface = false;

    short 
      length,
      direction,
      position,
      reversed,
      calibration_status,
      target_position,
      percent_position,
      stop_btn_click_count = 0,
      percent_min_position,
      percent_max_position;
      
    int
      stop_btn_click_second = 0;

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
        return "calibrated";
      }
      if (this->calibration_status == CALIBRATED_NO) {
        return "not calubrated";
      }
      if (this->calibration_status == CALIBRATING) {
        return "calibrating";
      }
      return "error";
    }

    void drawDebugInterface() {
      this->hub->Space(24);
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

        int next_stop_btn_click_second = (((millis() / 1000)%3600)%60);
        if (this->stop_btn_click_second != next_stop_btn_click_second) {
          this->stop_btn_click_count = 0;
        }
        this->stop_btn_click_second = next_stop_btn_click_second;
        if (++this->stop_btn_click_count == 2) {
          this->show_dedbug_interface = !this->show_dedbug_interface;
          this->stop_btn_click_count = 0;
          this->hub->refresh();
        }
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
      this->hub->BeginWidgets();
      this->hub->WidgetSize(33);
      this->hub->Button_(F("up"), &this->btn_up, F(BTN_UP));
      this->hub->Button_(F("down"), &this->btn_down, F(BTN_DOWN));
      this->hub->Button_(F("stop"), &this->btn_stop, F(BTN_STOP));
      this->hub->WidgetSize(25);
      this->hub->Button_(F("reverse"), &this->btn_reverse, F(BTN_REVERSE));
      this->hub->Button_(F("reset"), &this->btn_reset, F(BTN_RESET), GH_RED);
      this->hub->Button_(F("calibrate"), &this->btn_calibrate, F(BTN_CALIBRATE), GH_GREEN);
      this->hub->Button_(F("use hall sensor"), &this->btn_use_hall, F(BTN_USER_HALL), this->rollerBlind->useHallSensor() ? GH_GREEN : GH_RED);
      this->hub->EndWidgets();
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
      this->hub->WidgetSize(100);

      if (this->hub->Slider_(F("percent_min_position"), &this->percent_min_position, GH_UINT16, F(SLIDER_MIN_OPEN_PERCENT), 0, 99, 1)) {
        this->rollerBlind->setPercentMinPosition(this->percent_min_position);
      }

      if (this->hub->Slider_(F("percent_position"), &this->percent_position, GH_UINT16, F(SLIDER_OPEN_PERCENT), 0, 100, 1)) {
        this->rollerBlind->setPercentPosition(this->percent_position);
      }

      if (this->hub->Slider_(F("percent_max_position"), &this->percent_max_position, GH_UINT16, F(SLIDER_MAX_OPEN_PERCENT), 1, 100, 1)) {
        this->rollerBlind->setPercentMaxPosition(this->percent_max_position);
      }

      this->hub->WidgetSize(33);
      this->hub->Button_(F("open"), &this->btn_open, F(BTN_OPEN));
      this->hub->Button_(F("stop"), &this->btn_stop, F(BTN_STOP));
      this->hub->Button_(F("close"), &this->btn_close, F(BTN_CLOSE));
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
      this->target_position = this->rollerBlind->getTarget();
      this->reversed = this->rollerBlind->getReversed();
      this->calibration_status = this->rollerBlind->getCalibrationStatus();
      this->opened = this->rollerBlind->isOpened();

      this->position = this->rollerBlind->getCurrent();
      this->percent_position = this->rollerBlind->getPercentPosition();
      this->percent_min_position = this->rollerBlind->getPercentMinPosition();
      this->percent_max_position = this->rollerBlind->getPercentMaxPosition();
    }

    void updateAll() {
      this->hub->sendUpdate("percent_position", String(this->percent_position));
      this->hub->sendUpdate("percent_min_position", String(this->percent_min_position));
      this->hub->sendUpdate("percent_max_position", String(this->percent_max_position));
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

      if (show_dedbug_interface) {
        drawDebugInterface();
      }
    }
};
