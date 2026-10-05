

#include <stdint.h>
#include <stdio.h>
#include "bits.h"

void print_binary(uint32_t x, int width)
{
    if (width < 1 || width > 32)
        return;

    for (int i = width - 1; i >= 0; i--) {
        if (i != width - 1 && (i + 1) % 4 == 0)
            putchar(' ');

        putchar(((x >> i) & 1u) ? '1' : '0');
    }
}

uint32_t get_field(uint32_t word, int pos, int width)
{
    if (width < 1 || width > 32 ||
        pos < 0 || pos > 31 ||
        width > 32 - pos)
        return 0;

    uint32_t mask;

    if (width == 32)
        mask = UINT32_MAX;
    else
        mask = (1u << width) - 1u;

    return (word >> pos) & mask;
}


uint32_t set_field(uint32_t word, int pos, int width, uint32_t value)
{
    if (width < 1 || width > 32 ||
        pos < 0 || pos > 31 ||
        width > 32 - pos)
        return word;

    if (width == 32)
        return value;

    uint32_t mask = (1u << width) - 1u;

    word &= ~(mask << pos);
    word |= (value & mask) << pos;

    return word;
}


int32_t sign_extend(uint32_t value, int width)
{
    if (width < 1 || width > 32)
        return 0;

    if (width == 32)
        return (int32_t)value;

    uint32_t mask = (1u << width) - 1u;
    uint32_t sign_bit = 1u << (width - 1);

    value &= mask;

    if (value & sign_bit)
        value |= ~mask;

    return (int32_t)value;
}
