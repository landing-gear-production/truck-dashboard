#include "data.h"

J1939Header parseHeader(uint32_t id);
bool isPeerToPeer(J1939Header *header);