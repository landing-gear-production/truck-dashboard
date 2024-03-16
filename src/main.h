#include "data.h"
#include "utils.h"

#define CAN_TX_PIN GPIO_NUM_19
#define CAN_RX_PIN GPIO_NUM_18

bool isPeerToPeer(J1939Header *header);
void onData(CAN_FRAME *frame);