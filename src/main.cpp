#include <main.h>

VehicleState state;

void setup()
{
  Serial.begin(115200);
  setupCAN();
}

void loop()
{
  twai_message_t rpm = createRpm(125);
  twai_transmit(&rpm, pdMS_TO_TICKS(1000));
  // twai_message_t speed = createSpeed(125);
}

void setupCAN()
{
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(CAN_TX_PIN, CAN_RX_PIN, TWAI_MODE_NORMAL);
  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

  // Install TWAI driver
  if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK)
  {
    printf("Driver installed\n");
  }
  else
  {
    printf("Failed to install driver\n");
    neopixelWrite(LED_PIN, 127, 0, 0);
    return;
  }

  // Start TWAI driver
  if (twai_start() == ESP_OK)
  {
    printf("Driver started\n");
  }
  else
  {
    printf("Failed to start driver\n");
    neopixelWrite(LED_PIN, 127, 0, 0);
    return;
  }

  neopixelWrite(LED_PIN, 0, 127, 0);
}
