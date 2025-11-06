#pragma once
#include "esphome.h"
#include <vector>

class XYBusSniffer : public esphome::Component, public esphome::uart::UARTDevice {
 public:
  void setup() override;
  void loop() override;

  void set_bus_led(esphome::output::BinaryOutput *led) { bus_led_ = led; }
  void set_frame_out(esphome::text_sensor::TextSensor *sensor) { frame_out_ = sensor; }

  // SCHEMA
  static constexpr esphome::ConfigSchema CONFIG_SCHEMA =
      esphome::ConfigSchema()
          .add_option("uart_id", esphome::required(), esphome::uart::UARTComponent *)
          .add_option("bus_led", esphome::optional(), esphome::output::BinaryOutput *)
          .add_option("frame_out", esphome::optional(), esphome::text_sensor::TextSensor *);

  static void setup_from_config(XYBusSniffer *instance, const esphome::Config &config);

 protected:
  std::vector<uint8_t> buffer_;
  uint32_t last_activity_{0};
  esphome::output::BinaryOutput *bus_led_{nullptr};
  esphome::text_sensor::TextSensor *frame_out_{nullptr};
};