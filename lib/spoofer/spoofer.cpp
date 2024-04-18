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
  printf("%u -> %02x %02x\n", uSpeed, message.data[4], message.data[3]);
  return message;
}

// twai_message_t createSpeed(float rpm)
// {
//   float convertedRpm = rpm * 0.125;
//   unsigned short *uSpeed;
//   uSpeed = (unsigned short *)(&rpm);

//   twai_message_t message;
//   message.identifier = 0x0CF00400;
//   message.data_length_code = 8;
//   message.extd = 1;
//   message.data[4] = LOW(*uSpeed);
//   message.data[3] = HIGH(*uSpeed);

//   return message;
// }