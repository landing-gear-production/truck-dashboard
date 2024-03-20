#include <Arduino.h>
#include "utils.h"
#include "data.h"
#include "j1939.h"
#include "driver/twai.h"

#define CAN_TX_PIN GPIO_NUM_19
#define CAN_RX_PIN GPIO_NUM_18
#define LED_PIN 8

extern uint8_t transData;
extern twai_message_t transMessage;

void onData(twai_message_t *message);
void setupCAN(uint8_t id, uint8_t data[8]);
void sendCANMessage();