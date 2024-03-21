#include <main.h>

VehicleState state;

void setup()
{
  Serial.begin(115200);
  setupCAN();

  transMessage.extd = 1;
  transMessage.data_length_code = 8;
}

void loop()
{
  VehicleState oldState = state;
  twai_message_t message;

  if (twai_receive(&message, portMAX_DELAY) == ESP_OK)
  {
    Serial.printf("[ID: 0x%08x, DLC: %d, Data: %02x %02x %02x %02x %02x %02x %02x %02x]\n", message.identifier, message.data_length_code, message.data[0], message.data[1], message.data[2], message.data[3], message.data[4], message.data[5], message.data[6], message.data[7]);
    neopixelWrite(LED_PIN, 0, 0, 127);
    onData(&message);

    // if (oldState != state)
    //   Serial.printf("Engine Speed: %d\n", state.engineSpeed); // state.brakePedalPosition, state.acceleratorPedalPosition, state.steeringWheelAngle, state.currentGear, state.wheelBasedSpeed); //Brake: %d, Accelerator: %d, Steering: %f, Current Gear: %d,
  }

  sendCANMessage(0x0CF00400, transData);
}

void sendCANMessage(uint8_t id, uint8_t data[])
{
  memcpy(transMessage.data, data, sizeof(data));
  twai_transmit(&transMessage, pdMS_TO_TICKS(1000));

  // Debug Sent Data
  //  Serial.printf("[Sent | ID: 0x%08x, Extd: %d, DLC: %d, Data: %02x %02x %02x %02x %02x %02x %02x %02x]\n", message.identifier, message.extd, message.data_length_code, message.data[0], message.data[1], message.data[2], message.data[3], message.data[4], message.data[5], message.data[6], message.data[7]);
}

void setupCAN()
{
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(CAN_TX_PIN, CAN_RX_PIN, TWAI_MODE_NORMAL);
  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

  // Install TWAI driver
  if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK)
  {
    // printf("Driver installed\n");
  }
  else
  {
    // printf("Failed to install driver\n");
    neopixelWrite(LED_PIN, 127, 0, 0);
    return;
  }

  // Start TWAI driver
  if (twai_start() == ESP_OK)
  {
    // printf("Driver started\n");
  }
  else
  {
    // printf("Failed to start driver\n");
    neopixelWrite(LED_PIN, 127, 0, 0);
    return;
  }

  neopixelWrite(LED_PIN, 0, 127, 0);
}

void onData(twai_message_t *message)
{
  // all J1939 frames are extended
  if (!message->extd)
    return;

  J1939Header header = parseHeader(message->identifier);
  uint8_t gear, type, rawAngle;
  uint16_t rawEngineSpeed;
  float radians;

  switch (header.pgn)
  {
  case 65265:
    state.wheelBasedSpeed = message->data[1] << 8 | message->data[0];
    break;
  case 61444:
    rawEngineSpeed = message->data[4] << 8 | message->data[3];
    state.engineSpeed = (float)rawEngineSpeed * 0.125;
    Serial.printf("[Received | ID: 0x%08x, Extd: %d, DLC: %d, Data: %02x %02x %02x %02x %02x %02x %02x %02x]\n", message->identifier, message->extd, message->data_length_code, message->data[0], message->data[1], message->data[2], message->data[3], message->data[4], message->data[5], message->data[6], message->data[7]);
    break;
  case 61441:
    // brake pedal position
    state.brakePedalPosition = message->data[1];
    break;
  case 61443:
    // accelerator pedal position
    state.acceleratorPedalPosition = message->data[1];
    break;
  case 61445:
    // extract transmission information
    gear = message->data[0];
    state.currentGear = gear - 125;
    break;
  case 61449:
    // extract steering wheel angle sensor type
    type = message->data[2];
    state.steeringWheelAngleSensorType = (SteeringWheelAngleSensorType)getBits(type, 0, 2);

    if (state.steeringWheelAngleSensorType == SteeringWheelAngleSensorType::NotAvailable)
      return;

    // todo: check if LE or BE for raw angle
    rawAngle = message->data[1] << 8 | message->data[0];
    radians = ((float)rawAngle) * 31.374;
    state.steeringWheelAngle = radians * 180 / PI;
    break;
  }
}
