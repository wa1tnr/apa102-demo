// Sun Jul 29 01:36:04 UTC 2018
// main.c

// atmel start program - dotstar demo
// author: wa1tnr

// LICENSE is LGPL v2.1

#include <atmel_start.h>
#include "thisproject.h" // local include for main.c
#include "gpio_local.h"
#include "dotstar.h"

/* Many changes: by wa1tnr, July 2018 */

void uSec(void) {
    for (volatile int i = 1; i < 2; i++) { // needs calibration
        // nothing
    }
}

void short_timer(void) { // human blinkie timescale
    uint32_t on_time  = 2140111222; // it's 2147 something ;)
    for(on_time = 21401; on_time > 0; on_time--) {
        uSec();
    }
}

void vy_short_timer(void) { // human blinkie timescale
    uint32_t on_time  = 2140111222; // it's 2147 something ;)
    for(on_time = 344; on_time > 0; on_time--) {
        uSec();
    }
}

// this is what you lengthen to get a visibly slowed animation:

void uber_short_timer(void) { // digital clock/data timescale
    uint32_t on_time  = 2140111222; // it's 2147 something ;)
    // for(on_time = 23; on_time > 0; on_time--) { // worked well, but slowly
    for(on_time = 3; on_time > 0; on_time--) {
        uSec();
    }
}

void long_timer(void) {
    for(int i =  9; i > 0; i--) {
        short_timer();
    }
}

void blink_LED(void) {
    raise_D13();
    short_timer();
    lower_D13();
    short_timer();
}

void flicker_LED(void) {
    raise_D13();
    vy_short_timer();
    lower_D13();
    vy_short_timer();
}

void blinkLEDfast(void) {
    for(int i = 15; i > 0; i--) {
        blink_LED();
    }
}

void activity_LED_demo(void) {
    init_act_LED();
    blinkLEDfast();
}

void loop_vy_short_timer(void) {
    for(int i = 12; i > 0; i--) {
        vy_short_timer();
    }
}

void fleck_D7_clock_line(void) {
    raise_DS_CLOCK();
    loop_vy_short_timer();

    lower_DS_CLOCK();
    loop_vy_short_timer();
}

void fleck_D6_data_line(void) {
    raise_DS_DATA();
    loop_vy_short_timer();

    lower_DS_DATA();
    loop_vy_short_timer();
}

void DS_clock_in(void) {
    raise_DS_CLOCK();   uber_short_timer();
    lower_DS_CLOCK();   uber_short_timer();
}

void DS_clock_in_zero_data_bit(void) {
    lower_DS_DATA();    uber_short_timer();
    DS_clock_in();
    lower_DS_DATA();    uber_short_timer();
}

void DS_clock_in_data_bit(void) {
    raise_DS_DATA();    uber_short_timer();
    DS_clock_in();
    lower_DS_DATA();    uber_short_timer();
}

void DS_clock_in_all_ones_8bit(void) {
    for(int i = (8); i > 0; i--) { // exactly eight times
        DS_clock_in_data_bit();
    }
}

void DS_clock_in_all_zeroes_8bit(void) {
    for(int i = (8); i > 0; i--) {
        DS_clock_in_zero_data_bit();
    }
}

void DS_START_signal(void) {
    for(int i = (4); i > 0; i--) {
        DS_clock_in_all_zeroes_8bit();
    }
} // that's 32 bits of zeros

void DS_END_signal(void) {
    for(int i = (4); i > 0; i--) {
        DS_clock_in_all_ones_8bit();
    }
}

void DS_guard_bits(void) { // three guard bits
    for(int i = (3); i > 0; i--) {
       DS_clock_in_data_bit();
    }
}

void DS_bright_bits(void) { // haha MSB first
    DS_guard_bits(); // all 1's
    for(int i = (4); i > 0; i--) {
        DS_clock_in_zero_data_bit();
    }
    DS_clock_in_data_bit(); // not bright at all
}

void DS_send_8_clr_bits(void) { // clock in 8 data bits, all 0's
    DS_clock_in_all_zeroes_8bit();
}

void DS_send_8_set_bits(void) { // clock in 8 data bits, all 1's
    for(int i = (8); i > 0; i--) {
        DS_clock_in_data_bit();
    }
}

void DS_color_white(void) { // a 32-bit frame
    DS_bright_bits(); // 8 bits
    DS_send_8_set_bits(); // blue
    DS_send_8_set_bits(); // green
    DS_send_8_set_bits(); // red
}

void DS_send_dim_blue_byte(void) {
    // blue, in 8 bits 0b00000111:
    for(int i = (5); i > 0; i--) {
        DS_clock_in_zero_data_bit();
    }
    for(int i = (3); i > 0; i--) {
        DS_clock_in_data_bit();
    }
}

void DS_color_cyan(void) { // a 32-bit frame
// following bright_B G R
    DS_bright_bits(); // 8 bits
    DS_send_dim_blue_byte(); // blue:  0x07  in 8 bits
    DS_send_dim_blue_byte(); // green: 0x07
    DS_send_8_clr_bits();    // red:   0x00  null - all 0's
}


void DS_color_dark(void) { // a 32-bit frame
    DS_bright_bits(); // 8 bits
    DS_send_8_clr_bits(); // blue:  0x00  in 8 bits
    DS_send_8_clr_bits(); // green: 0x00
    DS_send_8_clr_bits(); // red:   0x00
}

void DS_color_blue(void) { // make this pixel a dim blue color
    DS_bright_bits();   // 8 bits
    DS_send_dim_blue_byte(); // blue:  0x07
    for(int i = (2); i > 0; i--) {
        DS_send_8_clr_bits(); // green, red both:  0x00
    }
}

void blank_blinkt(void) {
    DS_START_signal(); // 32 bits

    for(int i = (8); i > 0; i--) { // for each of eight dotstar pixels (on a strip of 8)
        DS_color_dark();           // turn off that dotstar pixel
    }
    DS_END_signal();

    // The START and END signals are not sent for each pixel,
    // but for the entire strip of 8 of them.
}

void DS_send_all_eight_dotstars_in_blue(void) {
    DS_START_signal(); // 32 bits

    for(int i = (8); i > 0; i--) {
        DS_color_blue();  // each of eight pixels in dim blue
    }
    DS_END_signal();
}

void DS_send_two_pairs_dotstars_in_blue_n_cyan(void) {
    DS_START_signal();
    for(int i = (4); i > 0; i--) { // four pairs of blue, cyan
        DS_color_blue();
        DS_color_cyan();
    }
    DS_END_signal();
}

void medium_timer(void) {
    for(int i = (7); i > 0; i--) {
        short_timer();
    }
}

void med_short_timer(void) {
    for(int i = (3); i > 0; i--) {
        short_timer();
    }
}

void DS_sends_demo(void) {
    init_dotstar_gpio();
    blank_blinkt();
    med_short_timer(); // 3x short_timer()

    DS_send_all_eight_dotstars_in_blue();

    medium_timer();

    DS_send_two_pairs_dotstars_in_blue_n_cyan();

    for(int i = (3); i > 0; i--) {
        med_short_timer();
    }
    blank_blinkt();
}

void fleck_timer(void) {
    for(int i = (2); i > 0; i--) {
        loop_vy_short_timer();
    }
}

void pulse_D7_clock_twice(void) {
    for(int i = (2); i > 0; i--) {
        fleck_D7_clock_line();
        fleck_timer();
    }
}

void pulse_D6_data_thrice(void) {
    fleck_D6_data_line();

    fleck_D6_data_line();
    fleck_timer();

    fleck_D6_data_line();
    loop_vy_short_timer();
}

void loop_D7_clock(void) {
    for(int i = 33; i > 0; i--) {
        pulse_D7_clock_twice();
        short_timer();
    }
}

void loop_D6_data(void) {
    for(int i = 23; i > 0; i--) {
        pulse_D6_data_thrice();
        for(int i = (4); i > 0; i--) {
            short_timer();
        }
    }
}

void demo_D7_clock(void) {
    init_dotstar_gpio();
    loop_D7_clock();
}

#ifdef NEED_D6_DEMO
void demo_D6_data(void) {
    init_dotstar_gpio();
    loop_D6_data();
}
#endif // #ifdef NEED_D6_DEMO

#ifdef NEED_COUNT_OF
void done_eight_times(void) {
        short_timer(); short_timer();
        short_timer(); short_timer();
        short_timer(); short_timer();
        short_timer(); short_timer();
        short_timer(); short_timer();
        short_timer(); short_timer();

    for(int i = (8); i > 0; i--) {
        blink_LED();
        short_timer(); short_timer();
    }
}
#endif // #ifdef NEED_COUNT_OF


int main(void) {
    /* Initializes MCU, drivers and middleware */
    atmel_start_init();

    /* Pimoroni Blinkt demo */
    DS_sends_demo();

    // USB_0_PORT_init(); // redundant?

    // testing of D7 logic using physical D13 (the LED)
    // usual use of D13 commented out during those tests.
    // activity_LED_demo();

    // D7 logic good - using D13
    // demo_D7_clock();
    // demo_D6_data();

    lower_D13();    // red LED D13 - OFF
    medium_timer(); // wait a bit

    // sanity check on loop counts:
#ifdef NEED_COUNT_OF
    done_eight_times();
#endif
    while (1) {
        flicker_LED(); // very short duration red LED pulse
        long_timer();  // lots of time between pulses
    }
}
