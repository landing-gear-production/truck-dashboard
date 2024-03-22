#include "driver/twai.h"

#define LOW(x) ((x) & 0xff)
#define HIGH(x) ((x) >> 8)

// twai_message_t createSpeed(float speed);
twai_message_t createRpm(float rpm);