#include <GyverStepper.h>
#include <RollerBlindConsts.h>

class RollerBlind {
  private:
    GStepper<STEPPER4WIRE>* motor;

    bool opened, position_set_manually;
    short percent_position, calibration_started_from;

    short length, position, target_position, deg_since_last_spin, percent_max_position, percent_min_position;
    EepromSync<short> length_storage               = EepromSync<short>(length);
    EepromSync<short> position_storage             = EepromSync<short>(position);
    EepromSync<short> target_position_storage      = EepromSync<short>(target_position);
    EepromSync<short> deg_since_last_spin_storage  = EepromSync<short>(deg_since_last_spin);
    EepromSync<short> percent_min_position_storage = EepromSync<short>(percent_min_position, { 0, 99, 0 });
    EepromSync<short> percent_max_position_storage = EepromSync<short>(percent_max_position, { 1, 100, 100 });

    short reversed, direction, use_hall_sensor, calibration_status;
    EepromSync<short> reversed_storage           = EepromSync<short>(reversed, { REVERSED_NO, REVERSED_YES, REVERSED_NO });
    EepromSync<short> direction_storage          = EepromSync<short>(direction, { DIRECTION_DOWN, DIRECTION_UP, DIRECTION_STOP });
    EepromSync<short> use_hall_sensor_storage    = EepromSync<short>(use_hall_sensor, { USE_HALL_NO, USE_HALL_YES, USE_HALL_NO });
    EepromSync<short> calibration_status_storage = EepromSync<short>(calibration_status, { CALIBRATED_NO, CALIBRATED_YES, CALIBRATED_NO });

    void calibrated() {
      this->motor->brake();
      this->length = this->calibration_started_from - this->getCurrent();
      this->target_position = 0;
      this->position = 0;
      this->calibration_status = length == 0 ? CALIBRATED_NO : CALIBRATED_YES;
      this->motor->reset();
      this->saveAll();
      ESP.restart();
    }

    short getReversedMultiplier() {
      return this->reversed == REVERSED_NO ? 1 : -1;
    }

    void setTarget(short value) {
      this->target_position = value * this->getReversedMultiplier();
      this->motor->setTargetDeg(this->target_position);
      if (this->target_position != this->position) {
        this->setDirection(value > this->position ? DIRECTION_DOWN : DIRECTION_UP);
      }
    }

    void setDirection(short direction) {
      this->direction = direction;
    }

    void saveAll() {
      this->length_storage.save();
      this->position_storage.save();
      this->reversed_storage.save();
      this->direction_storage.save();
      this->target_position_storage.save();
      this->calibration_status_storage.save();
      this->percent_max_position_storage.save();
      this->percent_min_position_storage.save();
      this->deg_since_last_spin_storage.save_and_commit();
    }

    void tickCalibration(bool hall_sensor_active) {
      if (!hall_sensor_active) {      // еще не достигли датчика 
        if (!this->motor->tick()) {   // мотор остановился
          this->up();
        }
      } else {                        // достигли датчика
        this->calibrated();
      }
    }

    void tickNormal(bool hall_sensor_active) {
      if (this->motor->tick()) {
        this->position = this->getCurrent();
        if (this->length > 0) {
          this->percent_position = max(round((float(this->position) / float(this->length) * 100.0)), 0.0);
        }
      } else if (this->useHallSensor() && !hall_sensor_active && this->direction == DIRECTION_UP) {
        // если не достигли датчика хола, но движемся вверх
        if (-1 * this->position > this->length) {
          // кажется что-то пошло не так (отвалился магнит или заело мотор)
          // отключаем использование датчика хола
          // устанавливаем длину на 0
          this->reset();
          this->use_hall_sensor = USE_HALL_NO;
          this->saveAll();
        } else {
          this->up();
          this->position = 0;
          this->percent_position = 0;
        }
      } else if (this->direction != DIRECTION_STOP) {
        this->direction = DIRECTION_STOP;
        this->position = this->getCurrent();
        if (this->length > 0) {
          this->percent_position = round((float(this->position) / float(this->length) * 100.0));
        }
        this->opened = this->percent_position != this->percent_max_position ? OPENED_YES : OPENED_NO;
        this->saveAll();
      }
    }

    void processNewTop() {
      // штора движется вверх и достигла датчика хола
      // нужно сохранить, что мы достигли нуля и обнулить позицию мотора
      this->motor->brake();
      this->target_position = 0;
      this->percent_position = 0;
      this->position = 0;
      this->direction = DIRECTION_STOP;
      this->motor->reset();
      this->saveAll();
    }

    void setTargetPercent(short percent) {
      this->setTarget(int(float(this->length) * float(percent) / 100.0));
    }

  public:
    RollerBlind(GStepper<STEPPER4WIRE>* motor) {
      this->motor = motor;
    }

    void setup() {
      this->length_storage.read();
      this->position_storage.read();
      this->reversed_storage.read();
      this->direction_storage.read();
      this->target_position_storage.read();
      this->calibration_status_storage.read();
      this->deg_since_last_spin_storage.read();
      this->percent_max_position_storage.read();
      this->percent_min_position_storage.read();

      this->percent_position = max(round((float(this->position) / float(this->length) * 100.0)), 0.0);

      this->motor->setSpeedDeg(800);
      this->motor->autoPower(true);
      this->motor->setAccelerationDeg(720);
      this->motor->setCurrentDeg(this->position * this->getReversedMultiplier());
      this->motor->setTargetDeg(this->position * this->getReversedMultiplier());
    }

    bool useHallSensor() {
      return this->use_hall_sensor == USE_HALL_YES;
    }

    void tick() {
      // todo: rewrite logic
      bool hall_sensor_active = !digitalRead(TOP_POSITION_HALL);
      if (this->calibration_status == CALIBRATING) {
        this->tickCalibration(hall_sensor_active);
      } else {
        if (!hall_sensor_active || direction == DIRECTION_DOWN) {
          this->tickNormal(hall_sensor_active);
        } else if (hall_sensor_active && direction == DIRECTION_UP && this->useHallSensor()) {
          this->processNewTop();
        }
      }
    }

    void up() {
      this->setTarget(this->getTarget() - DEFAULT_STEP);
    }

    void down() {
      Serial.println("down");
      this->setTarget(this->getTarget() + DEFAULT_STEP);
    }

    void stop() {
      Serial.println("stop");
      this->motor->brake();
      if (this->calibration_status == CALIBRATING) {
        this->calibration_status = CALIBRATED_NO;
      }
    }

    void reverse() {
      Serial.println("reverse");
      this->stop();
      this->reversed = this->reversed == REVERSED_NO ? REVERSED_YES : REVERSED_NO;
      this->motor->setCurrentDeg(-this->motor->getCurrentDeg());
      this->target_position = this->motor->getCurrentDeg();
      this->saveAll();
    }

    void reset() {
      Serial.println("reset");
      this->stop();
      this->length = 0;
      this->position = 0;
      this->target_position = 0;
      this->motor->setTargetDeg(0);
      this->motor->setCurrentDeg(0);
      this->direction = DIRECTION_STOP;
      this->calibration_status = CALIBRATED_NO;
      this->saveAll();
    }

    void calibrate() {
      Serial.println("calibrate");
      this->calibration_status = CALIBRATING;
      this->calibration_started_from = this->getCurrent();
      this->up();
    }

    void toggle_use_hall() {
      this->use_hall_sensor = this->use_hall_sensor == USE_HALL_YES ? USE_HALL_NO : USE_HALL_YES;
      this->use_hall_sensor_storage.save_and_commit();
    }

    short getLength() {
      return this->length; 
    }

    short getDirection() {
      return this->direction; 
    }

    short getReversed() {
      return this->reversed;
    }

    short getCurrent() {
      return this->motor->getCurrentDeg() * this->getReversedMultiplier();
    }

    short getTarget() {
      return this->target_position * this->getReversedMultiplier();
    }

    short getCalibrationStatus() {
      return this->calibration_status;
    }

    short getPercentPosition() {
      return this->percent_position;
    }

    short getPercentMaxPosition() {
      return this->percent_max_position;
    }

    short getPercentMinPosition() {
      return this->percent_min_position;
    }

    void setPercentPosition(short percent_position) {
      if (percent_position > this->percent_max_position) {
        this->percent_max_position = percent_position;
      }
      if (percent_position < this->percent_min_position) {
        this->percent_min_position = percent_position;
      }
      this->position_set_manually = true;
      this->setTargetPercent(percent_position);
      this->saveAll();
    }

    void setPercentMaxPosition(short percent_max_position) {
      this->percent_max_position = percent_max_position;
      if (this->percent_max_position <= this->percent_min_position) {
        this->percent_min_position = this->percent_max_position - 1;
      }
      if (this->percent_max_position < this->percent_position) {
        this->setPercentPosition(this->percent_max_position);
      }
      this->saveAll();
    }

    void setPercentMinPosition(short percent_min_position) {
      this->percent_min_position = percent_min_position;
      if (this->percent_min_position >= this->percent_max_position) {
        this->percent_max_position = this->percent_min_position + 1;
      }
      if (this->percent_min_position > this->percent_position) {
        this->setPercentPosition(this->percent_min_position);
      }
      this->saveAll();
    }

    void open() {
      this->position_set_manually = false;
      this->setPercentPosition(this->percent_min_position);
      this->saveAll();
    }

    void close() {
      this->position_set_manually = false;
      this->setTargetPercent(this->percent_max_position);
      this->saveAll();
    }

    bool isOpened() {
      return this->opened;
    }
};
