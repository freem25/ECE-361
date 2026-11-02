
#include "lock.h"

/* Transition table:
 * Each row represents the current state.
 * Each column represents an incoming event.
 * Each entry contains {next state, action}.
 *
 * Designated initializers identify each row and column.
 */
const transition_t lock_table[N_STATES][N_EVENTS] = {

    /* The lock is waiting for the first digit (3). */
    [LOCKED] = {
        [E_RIGHT]   = { GOT1,   A_NONE },
        [E_WRONG]   = { LOCKED, A_BEEP },
        [E_CLOSE]   = { LOCKED, A_NONE },
        [E_TIMEOUT] = { LOCKED, A_NONE }
    },

    /* The first digit (3) has been entered. */
    [GOT1] = {
        [E_RIGHT]   = { GOT2,   A_NONE },
        [E_WRONG]   = { LOCKED, A_BEEP },
        [E_CLOSE]   = { GOT1,   A_NONE },
        [E_TIMEOUT] = { LOCKED, A_NONE }
    },

    /* The first two digits (3, 1) were correct. */
    [GOT2] = {
        [E_RIGHT]   = { OPEN,   A_UNLOCK },
        [E_WRONG]   = { LOCKED, A_BEEP },
        [E_CLOSE]   = { GOT2,   A_NONE },
        [E_TIMEOUT] = { LOCKED, A_NONE }
    },

    /* The door is currently unlocked. */
    [OPEN] = {
        [E_RIGHT]   = { OPEN,   A_NONE },
        [E_WRONG]   = { OPEN,   A_NONE },
        [E_CLOSE]   = { LOCKED, A_LOCK },
        [E_TIMEOUT] = { LOCKED, A_LOCK }
    }
};

/* Use exactly one table lookup to find the transition.
 * s and e must be valid, and action must not be NULL.
 */
state_t lock_step(state_t s, event_t e, action_t *action)
{
    /* Retrieve the entry for the state and event. */
    transition_t result = lock_table[s][e];

    /* Store the action through the output pointer. */
    *action = result.action;

    /* Return the state that the lock moves to. */
    return result.next;
}

/* Convert a character into the corresponding event.
 * The correct digit depends on the current state.
 */
int lock_event(state_t s, char c)
{
    /* A closing door produces the CLOSE event. */
    if (c == 'c')
        return E_CLOSE;

    /* The timer expiring produces the TIMEOUT event. */
    if (c == 't')
        return E_TIMEOUT;

    /* Ignore characters that are not digits. */
    if (c < '0' || c > '9')
        return -1;

    /* Determine whether this is the next correct digit. */
    if ((s == LOCKED && c == '3') ||
        (s == GOT1   && c == '1') ||
        (s == GOT2   && c == '4'))
        return E_RIGHT;

    /* All other digits produce WRONG.
     * In OPEN, the transition table ignores digits.
     */
    return E_WRONG;
}

/* Return a readable name for a state.
 * The array is indexed using the state enum.
 */
const char *state_name(state_t s)
{
    static const char *const names[N_STATES] = {
        [LOCKED] = "LOCKED",
        [GOT1]   = "GOT1",
        [GOT2]   = "GOT2",
        [OPEN]   = "OPEN"
    };

    /* Prevent indexing outside the array. */
    if (s < LOCKED || s >= N_STATES)
        return "?";

    return names[s];
}

/* Return a readable name for an event. */
const char *event_name(event_t e)
{
    static const char *const names[N_EVENTS] = {
        [E_RIGHT]   = "RIGHT",
        [E_WRONG]   = "WRONG",
        [E_CLOSE]   = "CLOSE",
        [E_TIMEOUT] = "TIMEOUT"
    };

    if (e < E_RIGHT || e >= N_EVENTS)
        return "?";

    return names[e];
}

/* Return a readable name for an action. */
const char *action_name(action_t a)
{
    static const char *const names[N_ACTIONS] = {
        [A_UNDEFINED] = "UNDEFINED",
        [A_NONE]      = "NONE",
        [A_UNLOCK]    = "UNLOCK",
        [A_LOCK]      = "LOCK",
        [A_BEEP]      = "BEEP"
    };

    if (a < A_UNDEFINED || a >= N_ACTIONS)
        return "?";

    return names[a];
}
