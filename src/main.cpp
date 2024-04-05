#include <Arduino.h>
#include "main.h"

void setup() {
  Serial.begin(115200);
  
  xTaskCreatePinnedToCore(canTask, "canTask", 4096, NULL, 10, NULL, 1);
}

void loop() {
  if (Serial.available()) {
    std::string line = Serial.readString().c_str();
    auto parts = split(line, ",");
    if (parts.size() == 2) {
      speed = std::atoi(parts[0].c_str());
      rpm = std::atoi(parts[1].c_str());
    }
  }
}

void setupCAN() {
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(CAN0_TX, CAN0_RX, TWAI_MODE_NORMAL);
  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

  g_config.controller_id = 0;
  if (twai_driver_install_v2(&g_config, &t_config, &f_config, &can0) == ESP_OK) {
    printf("Driver installed\n");
  } else {
    printf("Failed to install driver\n");
    return;
  }
  //Start TWAI driver
  if (twai_start_v2(can0) == ESP_OK) {
    printf("Driver started\n");
  } else {
    printf("Failed to start driver\n");
    return;
  }

  g_config.controller_id = 1;
  g_config.tx_io = CAN1_TX;
  g_config.rx_io = CAN1_RX;
  if (twai_driver_install_v2(&g_config, &t_config, &f_config, &can1) == ESP_OK) {
    printf("Driver installed\n");
  } else {
    printf("Failed to install driver\n");
  return;
  }
  //Start TWAI driver
  if (twai_start_v2(can1) == ESP_OK) {
    printf("Driver started\n");
  } else {
    printf("Failed to start driver\n");
    return;
  }
}

void canTask(void* args) {
  setupCAN();

  uint16_t scaledRpm = engineRpm * 8;
  uint16_t scaledSpeed = speed * 256;

  for (;;) {
    // oldFeedbackData = feedbackData;
    twai_message_t message; 

    if (twai_receive_v2(can0, &message, 100) == ESP_OK) {
      receivingData = true;

      J1939Header header = parseHeader(message.identifier);
      switch (header.pgn) {
        case 61444:
          message[4] = HIGH(scaledRpm);
          message[3] = LOW(scaledRpm);
          twai_transmit_v2(can1, &message, 100);
          break;
        case 0xFEF1:
          message[2] = HIGH(scaledSpeed);
          message[1] = LOW(scaledSpeed);
          twai_transmit_v2(can1, &message, 100);
          break;
      }
    }

    delay(5);
  }

  vTaskDelete(NULL);
}