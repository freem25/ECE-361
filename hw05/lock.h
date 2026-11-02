
#ifndef LOCK_H
#define LOCK_H

/* The four possible states of our combination lock.
 * N_STATES tells us how many actual states exist.
 */
typedef enum {
    LOCKED,     /* Waiting for the first digit: 3 */
    GOT1,       /* Got 3; waiting for 1 */
    GOT2,       /* Got 3, 1; waiting for 4 */
    OPEN,       /* Correct code entered; door unlocked */
    N_STATES    /* Number of states: 4 */
} state_t;

/* Events that can happen to our lock. */
typedef enum {
    E_RIGHT,    /* Correct next digit */
    E_WRONG,    /* Incorrect digit */
    E_CLOSE,    /* Door closing */
    E_TIMEOUT,  /* Timer expired */
    N_EVENTS    /* Number of events: 4 */
} event_t;

/* Actions the lock can perform. */
typedef enum {
    A_UNDEFINED, /* Indicates a missing table entry */
    A_NONE,      /* No action */
    A_UNLOCK,    /* Unlock the door */
    A_LOCK,      /* Lock the door */
    A_BEEP,      /* Beep for an incorrect digit */
    N_ACTIONS    /* Number of action values: 5 */
} action_t;

/* One transition contains:
 * - next: the state we move to
 * - action: what the lock does
 */
typedef struct {
    state_t next;
    action_t action;
} transition_t;

/* The transition table is defined in lock.c.
 * It has 4 rows (states) and 4 columns (events).
 */
extern const transition_t lock_table[N_STATES][N_EVENTS];

/* Performs one table lookup and returns the next state.
 * Writes the action through the action pointer.
 */
state_t lock_step(state_t s, event_t e, action_t *action);

/* Converts an input character into an event.
 * Returns -1 if the character is not recognized.
 */
int lock_event(state_t s, char c);

/* Returns the state's readable name, or "?" if invalid. */
const char *state_name(state_t s);

/* Returns the event's readable name, or "?" if invalid. */
const char *event_name(event_t e);

/* Returns the action's readable name, or "?" if invalid. */
const char *action_name(action_t a);

#endif
