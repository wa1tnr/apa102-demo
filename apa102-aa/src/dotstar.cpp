#include <Arduino.h>

#ifdef ADAFRUIT_METRO_M0_EXPRESS

  #undef DATAPIN
  #undef CLOCKPIN

  #define DATAPIN    6 // arbitrarily chosen GPIO pins
  #define CLOCKPIN   7 // can be anything available

#endif // #ifdef ADAFRUIT_METRO_M0_EXPRESS

#ifdef ADAFRUIT_ITSYBITSY_M4_EXPRESS

  #undef DATAPIN
  #undef CLOCKPIN

  #define DATAPIN    8
  #define CLOCKPIN   6

// ref program D7 is clock here in this program it's D6 instead
// ref program D6 is data  here in this program it's D8 instead

#endif // #ifdef ADAFRUIT_ITSYBITSY_M4_EXPRESS

#ifdef NEVER_DEFINED_AA
int pop(void) {
    int a = 1;
    return a;
}

// ( u1 u2 -- )
void _PINWRITE(void)  {
    digitalWrite(pop(), pop());
    // digitalWrite(13, HIGH);
}
#endif // #ifdef NEVER_DEFINED_AA

void init_dotstar_gpio(void) {
    pinMode(DATAPIN, OUTPUT);
    pinMode(CLOCKPIN, OUTPUT);
}

void raise_DS_CLOCK(void) {
    digitalWrite(CLOCKPIN, HIGH);
}

void lower_DS_CLOCK(void) {
    digitalWrite(CLOCKPIN, LOW);
}

void raise_DS_DATA(void) {
    digitalWrite(DATAPIN, HIGH);
}

void lower_DS_DATA(void) {
    digitalWrite(DATAPIN, LOW);
}
