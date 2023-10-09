#include <GyverHub.h>
#include <RollerBlindConsts.h>


class HubWiFiSettings {
  private:
    GyverHub* hub;

    char ap_ssid[AP_SSID_SIZE] = "";
    char ap_pass[AP_PASS_SIZE] = "";

    EepromSync<char[AP_SSID_SIZE]> wifi_ssid_storage = EepromSync<char[AP_SSID_SIZE]>(ap_ssid);
    EepromSync<char[AP_PASS_SIZE]> wifi_pass_storage = EepromSync<char[AP_PASS_SIZE]>(ap_pass);

    GHbutton btn_connect;
    
    void saveAndConnect() {
      this->wifi_ssid_storage.save();
      this->wifi_pass_storage.save_and_commit();
      ESP.restart();
    }

  public:

    HubWiFiSettings(GyverHub* hub) {
      this->hub = hub;
    }

    void read() {
      this->wifi_ssid_storage.read();
      this->wifi_pass_storage.read();
    }

    void draw() {
      if (this->btn_connect.changed() && this->btn_connect) {
        Serial.println("btn_connect");
        saveAndConnect();
      }

      this->hub->BeginWidgets();
      this->hub->WidgetSize(100);
      this->hub->Input_(F("wifi_ssid"), &this->ap_ssid, GH_CSTR, F("WiFi ssid"), AP_SSID_INPUT_SIZE);
      this->hub->Input_(F("wifi_pass"), &this->ap_pass, GH_CSTR, F("WiFi password"), AP_PASS_INPUT_SIZE);
      this->hub->WidgetSize(25);
      this->hub->Button_(F("connect"), &this->btn_connect, F("connect"));
      this->hub->EndWidgets();
    }

    void connect() {
      WiFi.mode(WIFI_STA);
      WiFi.begin(this->ap_ssid, this->ap_pass);
      Serial.println();
      Serial.print("Connecting: ");
      Serial.print(this->ap_ssid);
      Serial.print(" ");
      Serial.println(this->ap_pass);

      for (int i = 0; i < CONNECTION_TRY_COUNT; i++) {
        if (WiFi.status() == WL_CONNECTED) {
          break;
        }
        Serial.print(".");
        delay(500);
      }

      if (WiFi.status() != WL_CONNECTED) {
        Serial.println();
        Serial.print("Not connected");
        WiFi.disconnect();
        WiFi.mode(WIFI_AP);
        WiFi.softAP(NAME);
      } else {
        Serial.println();
        Serial.print("Connected, IP address: ");
        Serial.println(WiFi.localIP());
      }
    }
};
