#include <Arduino.h>
#include "utils.h"
#include "data.h"
#include "j1939.h"
#include "driver/twai.h"

#define CAN_TX_PIN GPIO_NUM_19
#define CAN_RX_PIN GPIO_NUM_18
#define LED_PIN 8

uint8_t transData[] = {0x20, 0x00, 0x93, 0x12, 0x1f, 0x00, 0xf0, 0x00};
twai_message_t transMessage;

void onData(twai_message_t *message);
void setupCAN();
void sendCANMessage(uint8_t id, uint8_t data[]);