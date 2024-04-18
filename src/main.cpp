#include "main.h"

void delay(uint32_t ms) {
  vTaskDelay(ms / portTICK_PERIOD_MS);
}

unsigned long millis() {
  return (unsigned long) (esp_timer_get_time() / 1000ULL);
}

void setup() {  
  uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity    = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    .rx_flow_ctrl_thresh = 122,
    .source_clk = UART_SCLK_DEFAULT,
  };
  int intr_alloc_flags = 0;

#if CONFIG_UART_ISR_IN_IRAM
  intr_alloc_flags = ESP_INTR_FLAG_IRAM;
#endif

  uart_driver_install(UART_NUM_0, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags);
  uart_param_config(UART_NUM_0, &uart_config);
  uart_set_pin(UART_NUM_0, 16, 17, 0, 0);

  xTaskCreate(canTask, "canTask", 4096, NULL, 10, NULL);
}

void loop() {
  // Configure a temporary buffer for the incoming data
  int reported_length = 0;
  uart_get_buffered_data_len(UART_NUM_0, (size_t*)&reported_length);
  uint8_t data[reported_length + 1];
  int len = uart_read_bytes(UART_NUM_0, data, reported_length, 0);
  if (len > 0) {
    std::string line = (const char*)data;
    auto parts = split(line, ",");
    if (parts.size() >= 2) {
      speed = static_cast<uint16_t>(std::atoi(parts[0].c_str()));
      rpm = static_cast<uint16_t>(std::atoi(parts[1].c_str()));
      scaledRpm = rpm * 8;
      scaledSpeed = speed * 256;
    }
  }
  delay(50);
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
  STARTUP_TIME = millis();
  unsigned long now = millis();

  for (;;) {
    now = millis();
    twai_message_t message;

    if (twai_receive_v2(can0, &message, 0) == ESP_OK) {
      receivingData = true;

#ifdef PRINT_CAN_MESSAGES
    printf("[0x%08x] ", static_cast<unsigned int>(message.identifier));
    for (int i = 0; i < message.data_length_code; i++) {
      printf("%02x ", message.data[i]);
    }
    printf("\n");
#endif 

      if (now - STARTUP_TIME >= MESSAGE_SKIP_TIME_AFTER_STARTUP) {
        if (message.identifier == 0x0CF00400) {
          message.data[4] = HIGH(scaledRpm);
          message.data[3] = LOW(scaledRpm);
        } 
        else if (message.identifier == 0x18fef100) {
          message.data[2] = HIGH(scaledSpeed);
          message.data[1] = LOW(scaledSpeed);
        }

        twai_transmit_v2(can1, &message, 0);
      }
      else
        twai_transmit_v2(can1, &message, 0);
    }
    delay(1);
  }

  vTaskDelete(NULL);
}

extern "C" void app_main(void)
{
  setup();
  while(true) {
    loop();
  }

  vTaskDelete(NULL);
}