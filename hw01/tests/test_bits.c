#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "bits.h"
#include "status.h"

static int fails = 0;

#define CHECK(cond) do { \
    if (cond) \
        printf("PASS %s\n", #cond); \
    else { \
        printf("FAIL %s (line %d)\n", #cond, __LINE__); \
        fails++; \
    } \
} while (0)

static int check_print_binary(uint32_t value, int width,
                              const char *expected)
{
    FILE *tmp = tmpfile();

    if (tmp == NULL)
        return 0;

    int saved_stdout = dup(STDOUT_FILENO);

    if (saved_stdout == -1) {
        fclose(tmp);
        return 0;
    }

    fflush(stdout);

    if (dup2(fileno(tmp), STDOUT_FILENO) == -1) {
        close(saved_stdout);
        fclose(tmp);
        return 0;
    }

    print_binary(value, width);
    fflush(stdout);

    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    rewind(tmp);

    char actual[100] = {0};
    fgets(actual, sizeof(actual), tmp);

    fclose(tmp);

    return strcmp(actual, expected) == 0;
}

int main(void)
{
    printf("=== print_binary tests ===\n");

    CHECK(check_print_binary(1, 1, "1"));
    CHECK(check_print_binary(0x2C, 8, "0010 1100"));
    CHECK(check_print_binary(
        0x12345678, 32,
        "0001 0010 0011 0100 0101 0110 0111 1000"));

    printf("\n=== get_field tests ===\n");

    CHECK(get_field(0x2, 1, 1) == 1);
    CHECK(get_field(0x89ABCDEF, 0, 32) == 0x89ABCDEF);
    CHECK(get_field(0x80000000, 31, 1) == 1);
    CHECK(get_field(0xD6, 2, 3) == 5);

    /* Invalid field: pos 31 + width 2 goes past bit 31 */
    CHECK(get_field(0x12345678, 31, 2) == 0);

    printf("\n=== set_field tests ===\n");

    CHECK(set_field(0, 0, 1, 1) == 1);
    CHECK(set_field(0x12345678, 0, 32, 0xABCDEF12)
          == 0xABCDEF12);
    CHECK(set_field(0, 31, 1, 1) == 0x80000000);

    /* value 0xF is too wide for a 3-bit field.
       Only its lowest 3 bits (111) should be used. */
    CHECK(set_field(0, 4, 3, 0xF) == 0x70);

    /* Invalid field leaves original word unchanged */
    CHECK(set_field(0x12345678, 31, 2, 0xFFFFFFFF)
          == 0x12345678);

    printf("\n=== sign_extend tests ===\n");

    CHECK(sign_extend(0, 1) == 0);
    CHECK(sign_extend(1, 1) == -1);
    CHECK(sign_extend(0xF8, 8) == -8);
    CHECK(sign_extend(0x80, 8) == -128);
    CHECK(sign_extend(0x80000000, 32) == INT32_MIN);

    printf("\n=== status_unpack tests ===\n");

    status_t s1 = status_unpack(0x1631);

    CHECK(s1.setpoint == 22);
    CHECK(s1.mode == MODE_AUTO);
    CHECK(s1.heat == 1);
    CHECK(s1.cool == 0);
    CHECK(s1.fan == 0);
    CHECK(s1.fault == 0);
    CHECK(s1.reserved == 0);

    status_t s2 = status_unpack(0xF811);

    CHECK(s2.setpoint == -8);
    CHECK(s2.mode == MODE_HEAT);
    CHECK(s2.heat == 1);
    CHECK(s2.cool == 0);
    CHECK(s2.fan == 0);
    CHECK(s2.fault == 0);
    CHECK(s2.reserved == 0);

    status_t s3 = status_unpack(0x805C);

    CHECK(s3.setpoint == -128);
    CHECK(s3.mode == MODE_INVALID);
    CHECK(s3.heat == 0);
    CHECK(s3.cool == 0);
    CHECK(s3.fan == 1);
    CHECK(s3.fault == 1);
    CHECK(s3.reserved == 0);

    printf("\n=== Summary ===\n");
    printf("%d failed\n", fails);

    return fails != 0;
}
