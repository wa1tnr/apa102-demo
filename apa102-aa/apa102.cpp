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

void DS_START_signal(void) {
    for(int i = (4); i > 0; i--) {
        DS_clock_in_all_zeroes_8bit();
    }   
} // that's 32 bits of zeros

void blank_blinkt(void) {
    DS_START_signal(); // 32 bits
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
