#include <GyverHub.h>

class CustomHub : public GyverHub {
  public:
    using GyverHub::GyverHub;

    void sendMQTT2(const String& topic, const String& msg) {
      this->sendMQTT(topic, msg);
    }
};