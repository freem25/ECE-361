
#include <stdio.h>
#include <stdint.h>

#include "sauna_status.h"

/* Count successful and failed tests. */
static int tests_passed = 0;
static int tests_failed = 0;

/* Print whether each test passed or failed. */
static void check(int condition, const char *description)
{
    if (condition) {
        printf("PASS: %s\n", description);
        tests_passed++;
    } else {
        printf("FAIL: %s\n", description);
        tests_failed++;
    }
}

int main(void)
{
    uint32_t word;
    sauna_status_t result;

    /* TEST 1: All outputs off. */
    word = sauna_status_pack(FSM_OFF, 0x0u, 0);

    check(word == 0x00000000u,
          "OFF state packs correctly");

    /* TEST 2: PREHEAT with heater on. */
    word = sauna_status_pack(FSM_PREHEAT, 0x1u, 0);

    check(word == 0x00000009u,
          "PREHEAT with heater packs correctly");

    /* TEST 3: READY with heater and ready lamp on. */
    word = sauna_status_pack(FSM_READY, 0x5u, 0);

    check(word == 0x0000002Au,
          "READY with LEDs packs correctly");

    /* TEST 4: SESSION with humidity lockout active. */
    word = sauna_status_pack(FSM_SESSION, 0x0u, 1);

    check(word == 0x00000043u,
          "Humidity lockout sets bit 6");

    /* TEST 5: COOLDOWN with vent on. */
    word = sauna_status_pack(FSM_COOLDOWN, 0x2u, 0);

    check(word == 0x00000014u,
          "COOLDOWN with vent packs correctly");

    /* TEST 6: Unpack the READY status word. */
    result = sauna_status_unpack(0x0000002Au);

    check(result.state == FSM_READY &&
          result.heater == 1 &&
          result.vent == 0 &&
          result.ready == 1 &&
          result.humidity_lockout == 0,
          "Unpacking READY recovers all fields");

    /* TEST 7: Unpack humidity lockout. */
    result = sauna_status_unpack(0x00000043u);

    check(result.state == FSM_SESSION &&
          result.heater == 0 &&
          result.vent == 0 &&
          result.ready == 0 &&
          result.humidity_lockout == 1,
          "Unpacking SESSION recovers humidity lockout");

    /* TEST 8: Verify all unused bits remain zero. */
    word = sauna_status_pack(FSM_READY, 0x7u, 1);

    check((word & 0xFFFFFF80u) == 0u,
          "Unused status bits remain zero");

    /* Display the final results. */
    printf("\nTests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_failed);

    return tests_failed == 0 ? 0 : 1;
}
