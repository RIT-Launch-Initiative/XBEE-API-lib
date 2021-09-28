#ifndef XBEE_API_H
#define XBEE_API_H

namespace xbee {

    typedef struct {
        const char* data;
        size_t size;
    } xbee_ret_t;

    typedef enum {
        vtx_on,  // video transmitter power ON
        vtx_off, // video transmitter power OFF
        transmit // transmit frame
    } packet_t;

    // generate a packet
    // leave data NULL if vtx cmd
    xbee_ret_t generate_packet(packet_t type, const char* data, size_t len);

    // TODO receiving
    // some kind of parse packet
}


#endif
