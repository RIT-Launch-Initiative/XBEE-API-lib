#include "xbee_api.h"

using namespace xbee;

// special bytes (need to be escaped)
static const uint8_t START_DELIM    = 0x7E;
static const uint8_t ESCAPE         = 0x7D;
static const uint8_t XON            = 0x11;
static const uint8_t XOFF           = 0x13;

// needs to be XOR'd with byte being escaped
// NOTE: checksum is calculated on NON-ESCAPED data!
static const uint8_t ESCAPE_XOR     = 0x20;

// API Frame ID's
static const uint8_t AT_CMD         = 0x08; // AT Command
static const uint8_t AT_CMD_RES     = 0x88; // AT Command Response
static const uint8_t REMOTE_AT_CMD  = 0x17; // Remote Command Request

// defines an API header
typedef struct {
    uint8_t start_delim; // start delimeter
    uint16_t length;     // big endian order
} header_t;

// all data is big endian
typedef struct {
    header_t header;
    uint8_t frame_type;
    uint8_t frame_id; // 0 for no ACK
    uint64_t dst_addr;
    uint16_t dst_net;
    uint8_t remote_cmd_opts; // currently not used, set to 0
    uint8_t at_cmd;
    uint8_t param1;
    uint8_t param2;
    uint8_t checksum;
} remote_at_frame_t;
