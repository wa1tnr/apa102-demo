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

void DS_color_dark(void) { // a 32-bit frame
    DS_bright_bits(); // 8 bits
    DS_send_8_clr_bits(); // blue:  0x00  in 8 bits
    DS_send_8_clr_bits(); // green: 0x00
    DS_send_8_clr_bits(); // red:   0x00
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

void DS_sends_demo(void) {
    init_dotstar_gpio();
    blank_blinkt();
}

void setup() {
    DS_sends_demo();
}

void loop() {
  // put your main code here, to run repeatedly:

}
