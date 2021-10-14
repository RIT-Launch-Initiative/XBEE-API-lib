#ifndef XBEE_API_H
#define XBEE_API_H

namespace xbee {

    typedef struct {
        const uint8_t* data;
        size_t size;
    } xbee_ret_t;

    typedef enum {
        vtx_on,  // video transmitter power ON
        vtx_off, // video transmitter power OFF
        transmit // transmit frame
    } command_t;

    // TODO not sure what the format of an address is yet
    typedef struct {} addr_t;

    // set the address of the XBEE to send to (transmits, remote commands)
    void set_addr(addr_t* addr);

    // generate an API frame
    // leave data NULL if 'vtx_on' or 'vtx_off'
    // if using 'transmit', send 'len' bytes at 'data'
    xbee_ret_t generate_packet(command_t cmd, const uint8_t* data, size_t len);

    // TODO receiving
    // some kind of parse packet
}


#endif
