#include <Arduino.h>
#include "can_common.h"
#include <esp32_can.h>
#include "utils.h"
#include "data.h"
#include "j1939.h"

#define CAN_TX_PIN GPIO_NUM_19
#define CAN_RX_PIN GPIO_NUM_18

void onData(CAN_FRAME *frame);