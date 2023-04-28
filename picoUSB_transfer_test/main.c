#include <stdio.h>
#include <inttypes.h>
#include "pico/stdlib.h"
#include <math.h>


#include "pico/stdio_usb.h"

#define TUD_OPT_HIGH_SPEED (1)

#define SIN_0_PERIOD    255.0f
#define SIN_1_PERIOD    127.0f
#define SIN_2_PERIOD    1024.0f

#define SINMAG          32767.0f

#define TX_BURST_CT     1024

int main(){
    uint32_t phase_accumulator = 0;;

    int16_t fake_data[4] = {0x00};

    // We're sending binary data here
    stdio_set_translate_crlf(&stdio_usb, false);
    stdio_init_all();

    while(1){
        char read_val = getchar_timeout_us(0);

        float sig0 = SINMAG * sin(phase_accumulator / SIN_0_PERIOD);
        float sig1 = SINMAG * sin(phase_accumulator / SIN_1_PERIOD);
        float sig2 = SINMAG * sin(phase_accumulator / SIN_2_PERIOD);

        phase_accumulator++;

        fake_data[0] = (int16_t) sig0;
        fake_data[1] = (int16_t) sig1;
        fake_data[2] = (int16_t) sig2;
        fake_data[3] = 0x0000;

        switch(read_val){
            case 'A':
                fwrite((void *)fake_data, 
                            sizeof(int16_t) << 2, 
                            1, 
                            stdout
                            );
                fflush(stdout);
            break;

            case 'S':
                for(int n = 0; n < TX_BURST_CT; n++){
                    fwrite((void *)fake_data, 
                            sizeof(int16_t) << 2, 
                            1, 
                            stdout
                            );
                    fflush(stdout);
                    busy_wait_us(1);
                }
            break;
        }
    }
}

