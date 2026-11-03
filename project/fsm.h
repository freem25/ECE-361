#ifndef FSM_H
#define FSM_H

/* The five states required by the sauna-controller specification. */
typedef enum {
    FSM_OFF,
    FSM_PREHEAT,
    FSM_READY,
    FSM_SESSION,
    FSM_COOLDOWN,
    FSM_STATE_COUNT
} fsm_state_t;

/* Events generated from switches, readings, and elapsed time.
   A press is a rising edge, not a continuously held switch. */
typedef enum {
    FSM_EVENT_NONE,
    FSM_EVENT_POWER_ON,
    FSM_EVENT_POWER_OFF,
    FSM_EVENT_TEMP_READY,      /* Temperature >= 78.0 C */
    FSM_EVENT_TEMP_LOW,        /* Temperature < 78.0 C */
    FSM_EVENT_SESSION_START,   /* Rising edge of switch bit 1 */
    FSM_EVENT_SESSION_END,     /* Rising edge of switch bit 2 */
    FSM_EVENT_SESSION_TIMEOUT, /* Session has reached 900 ticks */
    FSM_EVENT_COOL_COMPLETE,   /* Temperature < 40.0 C */
    FSM_EVENT_HUMID_HIGH,      /* Session humidity > 40 %RH */
    FSM_EVENT_HUMID_SAFE,      /* Session humidity < 30 %RH */
    FSM_EVENT_COUNT
} fsm_event_t;

/* An action describes what a transition requests. Hardware output
   decisions (such as heater hysteresis) are implemented separately. */
typedef enum {
    FSM_ACTION_NONE,
    FSM_ACTION_BEGIN_PREHEAT,
    FSM_ACTION_SET_READY,
    FSM_ACTION_BEGIN_SESSION,
    FSM_ACTION_BEGIN_COOLDOWN,
    FSM_ACTION_SWITCH_OFF,
    FSM_ACTION_BLOCK_HEATER,
    FSM_ACTION_ALLOW_HEATER
} fsm_action_t;

/* Fill the state/event transition table with default and explicit rules. */
void fsm_init(void);

/* Look up the next state and optional action in the transition table.
   The action output pointer may be NULL. */
fsm_state_t fsm_next_state(fsm_state_t current,
                           fsm_event_t event,
                           fsm_action_t *action);

/* Useful for the textual trace required later in the project. */
const char *fsm_state_name(fsm_state_t state);

#endif
