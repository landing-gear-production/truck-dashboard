#include "spoofer.h"

twai_message_t createRpm(float rpm)
{
  float convertedRpm = rpm * 8;
  uint16_t uSpeed = convertedRpm;

  twai_message_t message;
  message.identifier = 0x0CF00400;
  message.data_length_code = 8;
  message.extd = 1;
  message.data[4] = HIGH(uSpeed);
  message.data[3] = LOW(uSpeed);
  // printf("%u -> %02x %02x\n", uSpeed, message.data[4], message.data[3]);
  return message;
}

twai_message_t createTacSpeed(float kmh)
{
  float convertedSpeed = kmh * 8;
  uint16_t uSpeed = convertedSpeed;

  twai_message_t message;
  message.identifier = 0x0000FEE8;
  message.data_length_code = 8;
  message.extd = 1;
  message.data[3] = HIGH(uSpeed);
  message.data[2] = LOW(uSpeed);
  // printf("%u -> %02x %02x\n", uSpeed, message.data[2], message.data[1]);
  return message;
}

twai_message_t createWheelSpeed(float kmh)
{
  float convertedSpeed = kmh * 256;
  uint16_t uSpeed = convertedSpeed;

  twai_message_t message;
  message.identifier = 0x18fef100;
  message.data_length_code = 8;
  message.extd = 1;
  message.data[2] = HIGH(uSpeed);
  message.data[1] = LOW(uSpeed);
  // printf("%u -> %02x %02x\n", uSpeed, message.data[2], message.data[1]);
  return message;
}

// void startUpMessages()
// {