#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include "net.h"
#include "spinlock.h"
#include "xbee.h"

#define TEST_START_DELIMETER      0x7E
#define TEST_RX_FRAME_TYPE        0x90
#define TEST_ADDR                 0xCF
#define TEST_RESERVED_B1          0xFF
#define TEST_RESERVED_B2          0xFE
#define TEST_RCV_OPTION           0x80
#define TEST_LENGTH_B1            0x00
#define TEST_LENGTH_B2            0x10 // 16


void rx_print_data(uint8_t *data, size_t length) {
    printf("Received data: ");
    for(size_t i = 0; i < length; i++) {
        printf("0x%02X ", data[i]);
    }
    printf("\n");
}

static uint8_t* tx_data;
static size_t tx_len;

int fake_write(uint8_t* data, size_t len) {
    printf("transmitted: ");
    for(size_t i = 0; i < len; i++) {
        printf("0x%02x ", data[i]);
    }
    printf("\n");

    tx_data = data;
    tx_len = len;

    return len;
}

void fake_delay(uint32_t ms) {
    usleep(ms * 1000);
}

int main() {
    printf("putting XBee into API mode\n");
    xb_init(fake_write, fake_delay);
    printf("\n");

    xb_attach_rx_callback(&rx_print_data);

    uint8_t TEST_RX_PACKET[] = {0x67, 0x61, 0x72, 0x62, 0x61, 0x67, 0x65, // Garbage data
                                TEST_START_DELIMETER, // start delim
                                TEST_LENGTH_B1, TEST_LENGTH_B2, // length (16)
                                TEST_RX_FRAME_TYPE, // frame type
                                TEST_ADDR, TEST_ADDR, TEST_ADDR, TEST_ADDR, // addr high
                                TEST_ADDR, TEST_ADDR, TEST_ADDR, TEST_ADDR, // addr low
                                TEST_RESERVED_B1, TEST_RESERVED_B2, // reserved
                                0x00, // receive options
                                0x69, 0x69, 0x69, 0x69, // payload
                                0x00 // checksum to calculate
    };

    // calculate checksum (b/w length and checksum fields)
    uint8_t check = 0;
    for(size_t i = 10; i < sizeof(TEST_RX_PACKET) - 1; i++) {
        check += TEST_RX_PACKET[i];
    }

    check = 0xFF - check;
    TEST_RX_PACKET[sizeof(TEST_RX_PACKET) - 1] = check;

    // test with one chunk
    printf("should print once\n");
    xb_raw_recv(TEST_RX_PACKET, sizeof(TEST_RX_PACKET));
    printf("\n");

    // test with two chunks
    printf("should print once\n");
    xb_raw_recv(TEST_RX_PACKET, sizeof(TEST_RX_PACKET) - 15);
    xb_raw_recv(TEST_RX_PACKET + sizeof(TEST_RX_PACKET) - 15, 15);
    printf("\n");

    // test with single bytes received at a time
    printf("should print once\n");
    for(size_t i = 0; i < sizeof(TEST_RX_PACKET); i++) {
        xb_raw_recv(TEST_RX_PACKET + i, 1);
    }
    printf("\n");

    // change the frame id to not an rx frame
    TEST_RX_PACKET[10] = 0x91;
    // shouldn't print
    xb_raw_recv(TEST_RX_PACKET, sizeof(TEST_RX_PACKET));

    // try sending a sample packet
    uint8_t payload[4] = {0x69, 0x69, 0x69, 0x69};
    uint8_t expected_packet[22] = {
                                0x7E, // start delim
                                0x00, 0x12, // length (18)
                                0x10, // frame type
                                0x00, // frame id
                                0x00, 0x00, 0x00, 0x00, // dst addr high
                                0x00, 0x00, 0xFF, 0xFF, // dst addr low
                                0xFF, 0xFE, // reserved
                                0x0, // broadcast radius
                                0x80, // transmit options
                                0x69, 0x69, 0x69, 0x69, // payload
                                0x00 // checksum to calculate
                            };

    check = 0;
    for(size_t i = 3; i < sizeof(expected_packet) - 1; i++) {
        check += expected_packet[i];
    }
    expected_packet[sizeof(expected_packet) - 1] = 0xFF - check;

    xb_send(payload, 4);

    if(tx_len != sizeof(expected_packet)) {
        printf("tx packet wrong size!\n");
        printf("expected %li, actual: %li\n", sizeof(expected_packet), tx_len);

        return 0;
    }

    uint8_t bad = 0;
    for(size_t i = 0; i < sizeof(expected_packet); i++) {
        if(expected_packet[i] != tx_data[i]) {
            printf("incorrect tx data at index: %li\n", i);
            bad = 1;
        }
    }

    if(!bad) {
        printf("all transmit data matched expected data\n");
    }

    printf("\n\ntest complete\n");

    return 0;
}
