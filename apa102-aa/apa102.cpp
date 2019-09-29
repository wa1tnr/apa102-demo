#include <Arduino.h>

extern void init_dotstar_gpio(void);
extern void raise_DS_CLOCK(void);
extern void lower_DS_CLOCK(void);
extern void raise_DS_DATA(void);
extern void lower_DS_DATA(void);


void uSec(void) {
    for (volatile int i = 1; i < 2; i++) { // needs calibration
        // nothing
    }
}

void uber_short_timer(void) { // digital clock/data timescale
    uint32_t on_time  = 0; // 2140111222; // it's 2147 something ;)
    for(on_time = 3; on_time > 0; on_time--) {
        uSec();
    }
}

void DS_clock_in(void) {
    raise_DS_CLOCK();   uber_short_timer();
    lower_DS_CLOCK();   uber_short_timer();
}

void DS_clock_in_data_bit(void) {
    raise_DS_DATA();    uber_short_timer();
    DS_clock_in();
    lower_DS_DATA();    uber_short_timer();
}

void DS_clock_in_zero_data_bit(void) {
    lower_DS_DATA();    uber_short_timer();
    DS_clock_in();
    lower_DS_DATA();    uber_short_timer();
}

void DS_clock_in_all_zeroes_8bit(void) {
    for(int i = (8); i > 0; i--) {
        DS_clock_in_zero_data_bit();
    }
}

void DS_clock_in_all_ones_8bit(void) {
    for(int i = (8); i > 0; i--) { // exactly eight times
        DS_clock_in_data_bit();
    }
}

/* need an alias here for what? */
void DS_send_8_clr_bits(void) { // clock in 8 data bits, all 0's
    DS_clock_in_all_zeroes_8bit();
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

void DS_sends_demo(void) {
    init_dotstar_gpio();
    blank_blinkt();
    delay(1000); // TEMPORARY tnr 29 Sep 01:14z
    // new payload is:
    DS_send_all_eight_dotstars_in_blue();
    delay(5000);
    blank_blinkt();
    delay(800);
    DS_send_two_pairs_dotstars_in_blue_n_cyan();
    delay(7000);
    blank_blinkt();
    delay(800);
    DS_send_all_eight_dotstars_in_blue();
    delay(12000);
    while(-1);
}

void setup() {
    DS_sends_demo();
}

void loop() {
  // put your main code here, to run repeatedly:

}
