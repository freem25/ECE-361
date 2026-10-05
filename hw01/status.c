#include "status.h"
#include "bits.h"

status_t status_unpack(uint16_t word)
{
    status_t status;

    status.heat = get_field(word, HEAT_POS, HEAT_WIDTH);
    status.cool = get_field(word, COOL_POS, COOL_WIDTH);
    status.fan = get_field(word, FAN_POS, FAN_WIDTH);
    status.fault = get_field(word, FAULT_POS, FAULT_WIDTH);

    uint32_t mode = get_field(word, MODE_POS, MODE_WIDTH);

    if (mode <= MODE_FAN_ONLY)
        status.mode = (int)mode;
    else
        status.mode = MODE_INVALID;

    status.reserved = get_field(word, RESERVED_POS, RESERVED_WIDTH);

    status.setpoint =
        sign_extend(get_field(word, SETPOINT_POS, SETPOINT_WIDTH),
                    SETPOINT_WIDTH);

    return status;
}
