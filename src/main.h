#include <driver/twai.h>
#include <iostream>
#include <sstream>
#include <vector>
#include "j1939.h"

#define LOW(x) ((x) & 0xff)
#define HIGH(x) ((x) >> 8)

#define CAN0_TX GPIO_NUM_18
#define CAN0_RX GPIO_NUM_19
#define CAN1_TX GPIO_NUM_20
#define CAN1_RX GPIO_NUM_21

twai_handle_t can0;
twai_handle_t can1;

// for string delimiter
std::vector<std::string> split(std::string s, std::string delimiter) {
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  std::string token;
  std::vector<std::string> res;

  while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
    token = s.substr (pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back (token);
  }

  res.push_back (s.substr (pos_start));
  return res;
}

int16_t speed;
int16_t rpm;

void setupCAN();
void canTask(void *args);