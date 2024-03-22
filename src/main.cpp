#include <main.h>

VehicleState state;

void setup()
{
  Serial.begin(115200);
  setupCAN();
}

void loop()
{
  // parseWheel(125);
  // parseEngine(125);
  sendWheel();
  sendEngine();
}

void parseWheel(float fSpeed)
{
  float convertedSpeed = 1.60934 * fSpeed / 256;
  unsigned short *uSpeed;
  uSpeed = (unsigned short *)(&convertedSpeed);
  wheelBased[2] = LOW(*uSpeed);
  wheelBased[1] = HIGH(*uSpeed);
}

void parseEngine(float rpm)
{
  float convertedRpm = rpm * 0.125;
  unsigned short *uSpeed;
  uSpeed = (unsigned short *)(&rpm);
  engineBased[4] = LOW(*uSpeed);
  engineBased[3] = HIGH(*uSpeed);
}

void sendEngine()
{
  twai_message_t message;
  message.identifier = 418383102;
  message.data_length_code = 8;
  message.extd = 1;
  memcpy(message.data, engineBased, sizeof(engineBased));
  twai_transmit(&message, pdMS_TO_TICKS(1000));
}

void sendWheel()
{
  twai_message_t message;
  message.identifier = 419361278;
  message.data_length_code = 8;
  message.extd = 1;
  memcpy(message.data, wheelBased, sizeof(wheelBased));
  twai_transmit(&message, pdMS_TO_TICKS(1000));
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
