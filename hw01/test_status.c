#include <stdio.h>
#include "status.h"

int main(void)
{
    status_t s = status_unpack(0x1631);

    printf("setpoint: %d\n", s.setpoint);
    printf("mode: %d\n", s.mode);
    printf("heat: %u\n", s.heat);
    printf("cool: %u\n", s.cool);
    printf("fan: %u\n", s.fan);
    printf("fault: %u\n", s.fault);
    printf("reserved: %u\n", s.reserved);

    return 0;
}
