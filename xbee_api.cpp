#include "xbee_api.h"

using namespace xbee;


static const uint8_t START_DELIM = 0x7E;

typedef struct {
    uint8_t start_delim; // start delimeter
    uint16_t length;     // big endian order
} header_t;
