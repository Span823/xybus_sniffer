#include "xybus_sniffer.h"
#include <cstdio>

void XYBusSniffer::setup_from_config(XYBusSniffer *instance, const esphome::Config &config) {
  auto *uart = static_cast<esphome::uart::UARTComponent *>(config["uart_id"]);
  instance->set_uart_parent(uart);

  if (config.count("bus_led"))
    instance->set_bus_led(static_cast<esphome::output::BinaryOutput *>(config["bus_led"]));
  if (config.count("frame_out"))
    instance->set_frame_out(static_cast<esphome::text_sensor::TextSensor *>(config["frame_out"]));
}

void XYBusSniffer::setup() {
  last_activity_ = millis();
}

void XYBusSniffer::loop() {
  while (available()) {
    uint8_t b = read();
    buffer_.push_back(b);
    if (bus_led_) bus_led_->turn_on();
    last_activity_ = millis();
  }

  if (!buffer_.empty() && (millis() - last_activity_ > 5)) {
    char hex[512] = {0};
    size_t pos = 0;
    for (uint8_t b : buffer_) {
      if (pos < sizeof(hex) - 4)
        pos += snprintf(&hex[pos], 4, "%02X ", b);
    }
    if (frame_out_) frame_out_->publish_state(hex);
    if (bus_led_) bus_led_->turn_off();
    buffer_.clear();
  }
}

ESP_HOME_COMPONENT_WITH_SCHEMA(
    xybus_sniffer,
    XYBusSniffer,
    nullptr,
    XYBusSniffer::CONFIG_SCHEMA
);