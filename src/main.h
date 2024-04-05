#include <Arduino.h>
#include <map>
#include <vector>
#include "utils.h"
#include "data.h"
#include "j1939.h"
#include "spoofer.h"
#include "driver/twai.h"
#include <string>

#define CAN_TX_PIN GPIO_NUM_19
#define CAN_RX_PIN GPIO_NUM_18
#define LED_PIN 8

std::map<uint32_t, uint32_t> speedMessages;
unsigned long period = 100;
unsigned long lastUpdate = millis();
unsigned long now = millis();
uint32_t speedIds[] = {0x18fef10b, 0x14fef121, 0x14fef131, 0x18fef100, 0x18fef117};
void onData(twai_message_t *message);
void setupCAN();

twai_message_t rpm;
twai_message_t wheelSpeed;