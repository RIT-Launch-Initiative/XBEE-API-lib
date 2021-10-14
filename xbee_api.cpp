#include "xbee_api.h"

using namespace xbee;

// special bytes (need to be escaped)
static const uint8_t START_DELIM = 0x7E;
static const uint8_t ESCAPE      = 0x7D;
static const uint8_t XON         = 0x11;
static const uint8_t XOFF        = 0x13;

// needs to be XOR'd with byte being escaped
static const uint8_t ESCAPE_XOR  = 0x20;

// API Frame ID's
static const uint8_t AT_CMD      = 0x08; // AT Command
static const uint8_t AT_CMD_RES  = 0x88; // AT Command Response

// defines an API header
typedef struct {
    uint8_t start_delim; // start delimeter
    uint16_t length;     // big endian order
} header_t;
