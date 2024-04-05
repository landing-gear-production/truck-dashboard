#include <main.h>

VehicleState state;

void setup()
{
  Serial.begin(115200);
  setupCAN();

  rpm = createRpm(2000);
  wheelSpeed = createWheelSpeed(130);
}

void loop()
{
  twai_message_t message;
  if (twai_receive(&message, pdMS_TO_TICKS(1000)) == ESP_OK)
  {
    neopixelWrite(LED_PIN, 0, 0, 127);
    J1939Header header = parseHeader(message.identifier);
    // if (header.pgn == 65265) {
    //   if (!speedMessages.count(message.identifier)) {
    //     speedMessages[message.identifier] = header.pgn;
    printf("%08x,", message.identifier);
    for (auto i = 0; i < message.data_length_code; i++)
      printf("%02x,", message.data[i]);
    printf("\n");
    // }
    // }
  } // else
  // {
  //   printf("no message\n");
  // }

  // now = millis();
  // if (now - lastUpdate >= period)
  // {
  //   // printf("%d\n", now - lastUpdate);
  //   rpm = createRpm(static_cast<float>(random(1000, 3000)));
  //   wheelSpeed = createWheelSpeed(static_cast<float>(random(50, 85)));
  //   lastUpdate = now;

  //   twai_transmit(&wheelSpeed, pdMS_TO_TICKS(10));
  // }

  // twai_transmit(&rpm, pdMS_TO_TICKS(10));
  // delay(5);

  // if (twai_transmit(&rpm, pdMS_TO_TICKS(10)))
  // {
  //   neopixelWrite(LED_PIN, 0, 127, 127);
  // }
  // delay(5);

  // if (twai_transmit(&rpm, pdMS_TO_TICKS(100)))
  // {
  //   neopixelWrite(LED_PIN, 0, 127, 127);
  // }

  // for (int i = 0; i < sizeof(speedIds)/sizeof(speedIds[0]); i++)
  // {
  // wheelSpeed.identifier = speedIds[i];
  // if (twai_transmit(&wheelSpeed, pdMS_TO_TICKS(100)))
  // {
  //   neopixelWrite(LED_PIN, 127, 127, 0);
  // }
  // }
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
