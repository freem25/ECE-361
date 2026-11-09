
#ifndef SAUNA_STATUS_H
#define SAUNA_STATUS_H

#include <stdint.h>
#include "fsm.h"

/* The individual fields of the sauna status word. */
typedef struct {
    fsm_state_t state;
    int heater;
    int vent;
    int ready;
    int humidity_lockout;
} sauna_status_t;

/* Pack the state, LEDs, and humidity flag into 32 bits. */
uint32_t sauna_status_pack(fsm_state_t state,
                           uint32_t leds,
                           int humidity_lockout);

/* Extract the individual fields from a status word. */
sauna_status_t sauna_status_unpack(uint32_t word);

#endif
