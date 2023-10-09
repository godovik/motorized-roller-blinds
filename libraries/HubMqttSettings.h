#include <GyverHub.h>
#include <RollerBlindConsts.h>

class HubMqttSettings {
  private:
    GyverHub* hub;

    char    mqtt_host[MQTT_PARAM_ZISE]  = "";
    char    mqtt_login[MQTT_PARAM_ZISE] = "";
    char    mqtt_pass[MQTT_PARAM_ZISE]  = "";
    int16_t mqtt_port                   = 0;

    EepromSync<char[MQTT_PARAM_ZISE]> mqtt_host_storage  = EepromSync<char[MQTT_PARAM_ZISE]>(mqtt_host);
    EepromSync<char[MQTT_PARAM_ZISE]> mqtt_login_storage = EepromSync<char[MQTT_PARAM_ZISE]>(mqtt_login);
    EepromSync<char[MQTT_PARAM_ZISE]> mqtt_pass_storage  = EepromSync<char[MQTT_PARAM_ZISE]>(mqtt_pass);
    EepromSync<int16_t>               mqtt_port_storage  = EepromSync<int16_t>(mqtt_port);

    GHbutton btn_connect;
    
    void saveAndConnect() {
      this->mqtt_host_storage.save();
      this->mqtt_port_storage.save();
      this->mqtt_pass_storage.save();
      this->mqtt_login_storage.save_and_commit();
      this->connect();
    }

  public:

    HubMqttSettings(GyverHub* hub) {
      this->hub = hub;
    }

    void read() {
      this->mqtt_host_storage.read();
      this->mqtt_login_storage.read();
      this->mqtt_pass_storage.read();
      this->mqtt_port_storage.read();
    }

    void draw() {
      if (this->btn_connect.changed() && this->btn_connect) {
        Serial.println("btn_connect");
        saveAndConnect();
      }

      this->hub->BeginWidgets();
      this->hub->WidgetSize(70);
      this->hub->Input_(F("mqtt_host"), &this->mqtt_host, GH_CSTR, F("MQTT host"), MQTT_PARAM_INPUT_SIZE);
      this->hub->WidgetSize(30);
      this->hub->Input_(F("mqtt_port"), &this->mqtt_port, GH_INT16, F("MQTT port"), 4);
      this->hub->WidgetSize(100);
      this->hub->Input_(F("mqtt_login"), &this->mqtt_login, GH_CSTR, F("Login"), MQTT_PARAM_INPUT_SIZE);
      this->hub->Input_(F("mqtt_pass"),  &this->mqtt_pass, GH_CSTR, F("Password"), MQTT_PARAM_INPUT_SIZE);

      this->hub->WidgetSize(25);
      this->hub->Button_(F("connect_mqtt"), &this->btn_connect, F("connect mqtt"));
      this->hub->EndWidgets();
    }

    void connect() {
      Serial.print("mqtt_host: "); Serial.println(this->mqtt_host); 
      Serial.print("mqtt_login: "); Serial.println(this->mqtt_login); 
      Serial.print("mqtt_pass: "); Serial.println(this->mqtt_pass); 
      Serial.print("mqtt_port: "); Serial.println(this->mqtt_port); 
      // todo: проверять наличие соединений и запоминать невалидные параметры
      this->hub->setupMQTT(this->mqtt_host, this->mqtt_port, this->mqtt_login, this->mqtt_pass);
    }
};
