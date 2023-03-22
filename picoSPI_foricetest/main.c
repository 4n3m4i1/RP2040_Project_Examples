#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

// Builtin Led GPIO 25
#define LED         25
#define GPIO_ON     1
#define GPIO_OFF    0

// Size of test send/rx arrays
#define ARRAY_SIZE  4
#define ENTER_PRESSED   13

// Pin definitions, GPIO numbering 
const uint32_t cs_pin = 5;
const uint32_t sck_pin = 2;
const uint32_t spi_rx_pin = 4;
const uint32_t spi_tx_pin = 3;

#define MODE_0

uint8_t char_to_hex(char in2cv);
bool is_valid_char(char in2chk);

int main(){
    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);
    gpio_put(LED, GPIO_OFF);

    stdio_init_all();

    sleep_ms(200);

    printf("Hello!\nWelcome to SPI Tester\nCS:\tGPIO %u\nSCK:\tGPIO %u\nMISO:\tGPIO %u\nMOSI:\tGPIO %u\n",
            cs_pin, sck_pin, spi_rx_pin, spi_tx_pin);


    // SPI Initiation!
    spi_inst_t *spi_ctrl = spi0;        // Use SPI0
    
    spi_init(spi_ctrl, 1000 * 1000);    // 1 MHz clk

    // Initialize SPI Format
    //      8 bit per frame, CPOL Polarity, CPHA Phase, Endianness
    spi_set_format(spi_ctrl, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    // Initialize SPI Port Pins
    gpio_set_function(sck_pin, GPIO_FUNC_SPI);
    gpio_set_function(spi_tx_pin, GPIO_FUNC_SPI);
    gpio_set_function(spi_rx_pin, GPIO_FUNC_SPI);

    // Initialize CS Pin as GPIO
    gpio_init(cs_pin);
    gpio_set_dir(cs_pin, GPIO_OUT);
    gpio_pull_up(cs_pin);               // Pullup just in case
    gpio_put(cs_pin, 1);                // CS is active low

    // End SPI Initiation

    printf("SPI%u Enabled at Baudrate: %u\n\n", 
            spi_get_index(spi_ctrl), spi_get_baudrate(spi_ctrl));   // USB Debug Messages


    uint8_t out_buffer[ARRAY_SIZE] = {0x00};  // Test messages
    uint8_t in_buffer[ARRAY_SIZE] = {0x00};

    sleep_ms(10);

    while(1){
        char read_val = getchar_timeout_us(0);

        switch(read_val){
            case '1':{
                gpio_put(LED, (!gpio_get(LED)));
                busy_wait_us(1);
                printf("LED %s!\n", ((gpio_get(LED)) ? "ON" : "OFF"));
            }
            break;

            case '2':
                gpio_put(LED, GPIO_OFF);
            break;

            case 'T':{       // Transmit Saved String
                printf("Send byte string\n");
                int bytes_read;
                if(spi_is_writable(spi_ctrl)){
                    gpio_put(cs_pin, GPIO_OFF);     // Pull CS low
                    bytes_read = spi_write_read_blocking(spi_ctrl, (const uint8_t *)out_buffer, in_buffer, ARRAY_SIZE);
                    gpio_put(cs_pin, GPIO_ON);     // Pull CS high

                    printf("\nSent %d bytes\n", ARRAY_SIZE);
                    printf("Read %3d bytes\n", bytes_read);

                    printf("TX Data: 0x%02X  0x%02X  0x%02X  0x%02X\n",
                            out_buffer[0], out_buffer[1], out_buffer[2], out_buffer[3]);

                    printf("RX Data: 0x%02X  0x%02X  0x%02X  0x%02X\n",
                                in_buffer[0], in_buffer[1], in_buffer[2], in_buffer[3]);
                } else {
                    printf("SPI Unavailable for write!\n");
                }
            }
            break;

            case 't':{       // Transmit 1 byte 0th
                printf("Send single byte\n");
                int bytes_read;
                if(spi_is_writable(spi_ctrl)){
                    gpio_put(cs_pin, GPIO_OFF);     // Pull CS low
                    bytes_read = spi_write_read_blocking(spi_ctrl, (const uint8_t *)out_buffer, in_buffer, 1);
                    gpio_put(cs_pin, GPIO_ON);     // Pull CS high

                    printf("\nSent %d bytes\n", 1);
                    printf("Read %3d bytes\n", bytes_read);

                    printf("TX Data: 0x%02X\n",
                            out_buffer[0]);

                    printf("RX Data: 0x%02X\n",
                                in_buffer[0]);
                } else {
                    printf("SPI Unavailable for write!\n");
                }
            }
            break;

            case 'S':{          // Hex String Capture
                printf("\nHex Value String Entry Mode\n");
                uint wr_ptr = 0;
                uint entries = 0;
                bool nibble_shift = 1;  // Entry of MSB first to follow english writing convention

                busy_wait_us(100);

                getchar_timeout_us(0);

                while(wr_ptr < (ARRAY_SIZE)){
                    char rvfromserial = getchar_timeout_us(0);
                    if(is_valid_char(rvfromserial)){
                        if(nibble_shift){
                            printf("0x%01X", char_to_hex(rvfromserial));
                            out_buffer[wr_ptr] = char_to_hex(rvfromserial) << 4;
                            nibble_shift = 0;
                        } else {
                            printf("%01X  ", char_to_hex(rvfromserial));
                            out_buffer[wr_ptr] |= char_to_hex(rvfromserial);
                            nibble_shift = 1;
                            wr_ptr += 1;
                            entries += 1;
                        }
                    } else
                    if(rvfromserial == ENTER_PRESSED){
                        while(wr_ptr < ARRAY_SIZE){
                            out_buffer[wr_ptr] = 0x00;
                            wr_ptr += 1;
                        }
                        break;
                    }
                }
                printf("Input of %d values\n", entries);
                printf("Array to Send:\n\t0x%02X  0x%02X  0x%02X  0x%02X\n",
                            out_buffer[0], out_buffer[1], out_buffer[2], out_buffer[3]);
            }
            break;

            case 'P':
                printf("Array to Send:\n\t0x%02X  0x%02X  0x%02X  0x%02X\n",
                            out_buffer[0], out_buffer[1], out_buffer[2], out_buffer[3]);
            break;
        }
        
    }    
}



uint8_t char_to_hex(char in2cv){
    uint8_t retval;
    if(in2cv < ('9' + 1) && in2cv > ('0' - 1)) retval = in2cv - '0';
    else if(in2cv < ('F' + 1) && in2cv > ('A' - 1)) retval = 10 + in2cv - 'A';
    else if(in2cv < ('f' + 1) && in2cv > ('a' - 1)) retval = 10 + in2cv - 'a';
    else retval = 0;

    return retval;
}

bool is_valid_char(char in2chk){
    bool retval = false;
    if(in2chk < ('9' + 1) && in2chk > ('0' - 1)) retval = true;
    else if(in2chk < ('Z' + 1) && in2chk > ('A' - 1)) retval = true;
    else if(in2chk < ('z' + 1) && in2chk > ('a' - 1)) retval = true;

    return retval;
}

