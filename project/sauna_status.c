
#include "sauna_status.h"
#include "bits.h"

/* Pack the sauna's operating information into one word. */
uint32_t sauna_status_pack(fsm_state_t state,
                           uint32_t leds,
                           int humidity_lockout)
{
    uint32_t word = 0;

    /* Bits 0-2: FSM state. */
    word = set_field(word, 0, 3, (uint32_t)state);

    /* Bit 3: Heater state (LED 0). */
    word = set_field(word, 3, 1, (leds >> 0) & 1u);

    /* Bit 4: Vent state (LED 1). */
    word = set_field(word, 4, 1, (leds >> 1) & 1u);

    /* Bit 5: Ready lamp (LED 2). */
    word = set_field(word, 5, 1, (leds >> 2) & 1u);

    /* Bit 6: Humidity lockout. */
    word = set_field(word, 6, 1,
                     humidity_lockout ? 1u : 0u);

    return word;
}

/* Extract the values from the packed status word. */
sauna_status_t sauna_status_unpack(uint32_t word)
{
    sauna_status_t result;

    result.state = (fsm_state_t)get_field(word, 0, 3);
    result.heater = (int)get_field(word, 3, 1);
    result.vent = (int)get_field(word, 4, 1);
    result.ready = (int)get_field(word, 5, 1);
    result.humidity_lockout = (int)get_field(word, 6, 1);

    return result;
}
