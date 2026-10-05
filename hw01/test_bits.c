#include <stdio.h>
#include "bits.h"

int main(void)
{
    print_binary(0x2C, 8);
    printf("\n");

    printf("%u\n", get_field(0xD6, 2, 3));

    printf("0x%X\n", set_field(0xF0, 2, 3, 5));

    printf("%d\n", sign_extend(0xF8, 8));
printf("invalid get_field: %u\n",
       get_field(0x12345678, 31, 2));

printf("invalid set_field: 0x%X\n",
       set_field(0x12345678, 31, 2, 0xFFFFFFFF));

    return 0;
}
