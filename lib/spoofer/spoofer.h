#include "driver/twai.h"

#define LOW(x) ((x) & 0xff)
#define HIGH(x) ((x) >> 8)

// twai_message_t createSpeed(float speed);
twai_message_t createRpm(float rpm);
twai_message_t createTacSpeed(float kmh);
twai_message_t createWheelSpeed(float kmh);
void startUpMessages();

/*
352252193
419361035
419361047
352252209
419361024

18fef10b X
14fef121 X
14fef131 X
18fef100
18fef117
*/