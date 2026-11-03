#include <stdio.h>
#include "../fsm.h"

static int passed = 0;
static int failed = 0;

static void check(int condition, const char *description)
{
    if (condition) {
        printf("PASS: %s\n", description);
        passed++;
    } else {
        printf("FAIL: %s\n", description);
        failed++;
    }
}

int main(void)
{
    fsm_action_t action = FSM_ACTION_NONE;
    fsm_state_t state = FSM_OFF;

    fsm_init();

    state = fsm_next_state(state, FSM_EVENT_POWER_ON, &action);
    check(state == FSM_PREHEAT && action == FSM_ACTION_BEGIN_PREHEAT,
          "power on starts preheat");

    state = fsm_next_state(state, FSM_EVENT_SESSION_START, &action);
    check(state == FSM_PREHEAT && action == FSM_ACTION_NONE,
          "session start before ready is ignored");

    state = fsm_next_state(state, FSM_EVENT_TEMP_READY, &action);
    check(state == FSM_READY && action == FSM_ACTION_SET_READY,
          "preheat reaches ready temperature");

    state = fsm_next_state(state, FSM_EVENT_TEMP_LOW, &action);
    check(state == FSM_PREHEAT && action == FSM_ACTION_BEGIN_PREHEAT,
          "ready falls back to preheat when too cool");

    state = fsm_next_state(state, FSM_EVENT_TEMP_READY, &action);
    state = fsm_next_state(state, FSM_EVENT_SESSION_START, &action);
    check(state == FSM_SESSION && action == FSM_ACTION_BEGIN_SESSION,
          "start press begins session from ready");

    state = fsm_next_state(state, FSM_EVENT_HUMID_HIGH, &action);
    check(state == FSM_SESSION && action == FSM_ACTION_BLOCK_HEATER,
          "high humidity blocks heater during session");

    state = fsm_next_state(state, FSM_EVENT_HUMID_SAFE, &action);
    check(state == FSM_SESSION && action == FSM_ACTION_ALLOW_HEATER,
          "low humidity releases heater lockout");

    state = fsm_next_state(state, FSM_EVENT_SESSION_TIMEOUT, &action);
    check(state == FSM_COOLDOWN && action == FSM_ACTION_BEGIN_COOLDOWN,
          "900-tick timeout starts cooldown");

    state = fsm_next_state(state, FSM_EVENT_POWER_ON, &action);
    check(state == FSM_COOLDOWN && action == FSM_ACTION_NONE,
          "power on during cooldown does not skip cooling");

    state = fsm_next_state(state, FSM_EVENT_COOL_COMPLETE, &action);
    check(state == FSM_OFF && action == FSM_ACTION_SWITCH_OFF,
          "cooling below 40 C returns off");

    state = fsm_next_state(state, FSM_EVENT_POWER_OFF, &action);
    check(state == FSM_OFF && action == FSM_ACTION_NONE,
          "power off while already off stays off");

    state = fsm_next_state(FSM_SESSION, FSM_EVENT_POWER_OFF, &action);
    check(state == FSM_COOLDOWN && action == FSM_ACTION_BEGIN_COOLDOWN,
          "power off in session begins cooldown");

    state = fsm_next_state(FSM_SESSION, FSM_EVENT_SESSION_END, &action);
    check(state == FSM_COOLDOWN && action == FSM_ACTION_BEGIN_COOLDOWN,
          "session end starts cooldown");

    state = fsm_next_state(FSM_PREHEAT, FSM_EVENT_NONE, &action);
    check(state == FSM_PREHEAT && action == FSM_ACTION_NONE,
          "no event keeps state unchanged");

    state = fsm_next_state(FSM_PREHEAT, FSM_EVENT_COUNT, &action);
    check(state == FSM_PREHEAT && action == FSM_ACTION_NONE,
          "invalid event cannot index outside table");

    check(fsm_state_name(FSM_READY)[0] == 'R',
          "state name supports future trace");

    printf("FSM tests: %d passed, %d failed\n", passed, failed);
    return failed != 0;
}
