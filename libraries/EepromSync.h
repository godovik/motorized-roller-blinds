struct NumberValidator {
  int min, max, def;
};


#include <EEPROM.h>
#include <typeinfo>

#define VARIABLE_WRITTEN   1001

// todo: переместить внутрь класса
static short next_position = 12;

template <typename T>
class EepromSync {
  private:

    T& variable;

    bool use_validator = false;
    bool variable_writen = false;

    short address;
    short check_flag_address;

    NumberValidator number_validator;

    void validate(int value) {
      if (!this->variable_writen) {
        this->variable = this->number_validator.def;
        this->save_and_commit();
      } else if (this->number_validator.min > this->variable || this->variable > this->number_validator.max) {
        this->variable = this->number_validator.def;
        this->save_and_commit();
      }
    }

    void validate(char* value) { }

    void clear(int value) {
      this->variable = 0;
    }

    void clear(char* value) { 
      strcpy(this->variable, "");
    }

    static short get_start() {
      return next_position;
    }

  public:

    void init() {
      this->check_flag_address = this->get_start();
      this->address = this->get_start() + sizeof(short);
      this->set_start(this->address + sizeof(this->variable));
    }

    void init_with_validator() {
      this->use_validator = true;
      this->init();
    }

    EepromSync(T& input) : variable(input) {
      this->init();
    }

    EepromSync(short& input, NumberValidator validator) : variable(input), number_validator(validator) {
      this->init_with_validator();
    }

    EepromSync(int8_t& input, NumberValidator validator) : variable(input), number_validator(validator) {
      this->init_with_validator();
    }

    void read() {
      short variable_writen = 0;

      EEPROM.get(this->check_flag_address, variable_writen);
      EEPROM.get(this->address, this->variable);

      this->variable_writen = variable_writen == VARIABLE_WRITTEN;

      if (!this->variable_writen) {
        this->clear(this->variable);
      }

      if (this->use_validator) {
        this->validate(this->variable);
      }
    }

    bool exist() {
      return this->variable_writen;
    }

    void save() {
      EEPROM.put(this->check_flag_address, VARIABLE_WRITTEN);
      // todo: использовать валидатор, если он передан
      EEPROM.put(this->address, this->variable);
    }

    void clear() {
      this->clear(this->variable);
    }

    static void commit() {
      EEPROM.commit();
    }

    void save_and_commit() {
      this->save();
      this->commit();
    }

    static void set_start(short new_start) {
      next_position = new_start;
    }
};

