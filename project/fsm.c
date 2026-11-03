#include "fsm.h"
#include <stddef.h> /* NULL */

/* Each cell of the table holds a next state and a requested action. */
typedef struct {
    fsm_state_t next_state;
    fsm_action_t action;
} fsm_transition_t;

/* Indexed exactly as required: transition_table[current_state][event]. */
static fsm_transition_t transition_table[FSM_STATE_COUNT][FSM_EVENT_COUNT];

void fsm_init(void)
{
    /* Start with a safe default: an irrelevant event leaves the state
       unchanged and does not request an action. */
    for (int state = 0; state < FSM_STATE_COUNT; state++) {
        for (int event = 0; event < FSM_EVENT_COUNT; event++) {
            transition_table[state][event].next_state = (fsm_state_t)state;
            transition_table[state][event].action = FSM_ACTION_NONE;
        }
    }

    /* OFF: a powered-on sauna begins preheating. */
    transition_table[FSM_OFF][FSM_EVENT_POWER_ON] =
        (fsm_transition_t){FSM_PREHEAT, FSM_ACTION_BEGIN_PREHEAT};

    /* PREHEAT: power-off has priority; >=78 C means READY. */
    transition_table[FSM_PREHEAT][FSM_EVENT_POWER_OFF] =
        (fsm_transition_t){FSM_COOLDOWN, FSM_ACTION_BEGIN_COOLDOWN};
    transition_table[FSM_PREHEAT][FSM_EVENT_TEMP_READY] =
        (fsm_transition_t){FSM_READY, FSM_ACTION_SET_READY};

    /* READY: a start-button press begins a session. A temperature drop
       below 78 C returns the sauna to PREHEAT. */
    transition_table[FSM_READY][FSM_EVENT_POWER_OFF] =
        (fsm_transition_t){FSM_COOLDOWN, FSM_ACTION_BEGIN_COOLDOWN};
    transition_table[FSM_READY][FSM_EVENT_TEMP_LOW] =
        (fsm_transition_t){FSM_PREHEAT, FSM_ACTION_BEGIN_PREHEAT};
    transition_table[FSM_READY][FSM_EVENT_SESSION_START] =
        (fsm_transition_t){FSM_SESSION, FSM_ACTION_BEGIN_SESSION};

    /* SESSION: the bathers leave, time runs out, or power is turned off. */
    transition_table[FSM_SESSION][FSM_EVENT_POWER_OFF] =
        (fsm_transition_t){FSM_COOLDOWN, FSM_ACTION_BEGIN_COOLDOWN};
    transition_table[FSM_SESSION][FSM_EVENT_SESSION_END] =
        (fsm_transition_t){FSM_COOLDOWN, FSM_ACTION_BEGIN_COOLDOWN};
    transition_table[FSM_SESSION][FSM_EVENT_SESSION_TIMEOUT] =
        (fsm_transition_t){FSM_COOLDOWN, FSM_ACTION_BEGIN_COOLDOWN};

    /* The humidity lockout modifies heater permission without changing
       the SESSION state. Main will retain the lockout between ticks. */
    transition_table[FSM_SESSION][FSM_EVENT_HUMID_HIGH] =
        (fsm_transition_t){FSM_SESSION, FSM_ACTION_BLOCK_HEATER};
    transition_table[FSM_SESSION][FSM_EVENT_HUMID_SAFE] =
        (fsm_transition_t){FSM_SESSION, FSM_ACTION_ALLOW_HEATER};

    /* COOLDOWN keeps the vent open until temperature is below 40 C. */
    transition_table[FSM_COOLDOWN][FSM_EVENT_COOL_COMPLETE] =
        (fsm_transition_t){FSM_OFF, FSM_ACTION_SWITCH_OFF};
}

fsm_state_t fsm_next_state(fsm_state_t current,
                           fsm_event_t event,
                           fsm_action_t *action)
{
    if (action != NULL) {
        *action = FSM_ACTION_NONE;
    }

    /* Validate indexes so a bad enum value cannot index outside the array. */
    if (current < FSM_OFF || current >= FSM_STATE_COUNT) {
        return FSM_OFF;
    }
    if (event < FSM_EVENT_NONE || event >= FSM_EVENT_COUNT) {
        return current;
    }

    /* All state changes use this two-dimensional lookup, not if-chains. */
    fsm_transition_t transition = transition_table[current][event];

    if (action != NULL) {
        *action = transition.action;
    }
    return transition.next_state;
}

const char *fsm_state_name(fsm_state_t state)
{
    switch (state) {
        case FSM_OFF:      return "OFF";
        case FSM_PREHEAT:  return "PREHEAT";
        case FSM_READY:    return "READY";
        case FSM_SESSION:  return "SESSION";
        case FSM_COOLDOWN: return "COOLDOWN";
        default:           return "INVALID";
    }
}
