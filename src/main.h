#include <Arduino.h>
#include "utils.h"
#include "data.h"
#include "j1939.h"
#include "driver/twai.h"

#define CAN_TX_PIN GPIO_NUM_19
#define CAN_RX_PIN GPIO_NUM_18
#define LED_PIN 8

#define LOW(x) ((x) & 0xff)
#define HIGH(x) ((x) >> 8)

uint8_t wheelBased[] = {0x00, 0x7d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t engineBased[] = {0x00, 0xa0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00};

void parseWheel(float speed);
void parseEngine(float rpm);
void sendWheel();
void sendEngine();
void onData(twai_message_t *message);
void setupCAN();