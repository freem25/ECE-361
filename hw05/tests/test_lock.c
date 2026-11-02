
#include <stdio.h>
#include "../lock.h"

/* Track how many tests we run and how many fail. */
static int tests_run = 0;
static int tests_failed = 0;

/* Test one entry of the transition table.
 *
 * current:         State before the event.
 * event:           Event being tested.
 * expected_next:   State we expect afterward.
 * expected_action: Action we expect afterward.
 */
static void check_transition(state_t current,
                             event_t event,
                             state_t expected_next,
                             action_t expected_action)
{
    /* Create a variable to receive the action. */
    action_t actual_action = A_UNDEFINED;

    /* Call our FSM function to find the next state
     * and the action associated with this event.
     */
    state_t actual_next = lock_step(current,
                                    event,
                                    &actual_action);

    /* Every call to this function counts as one test. */
    tests_run++;

    /* Both the state and action must be correct. */
    if (actual_next == expected_next &&
        actual_action == expected_action)
    {
        printf("PASS: %s + %s -> %s %s\n",
               state_name(current),
               event_name(event),
               state_name(actual_next),
               action_name(actual_action));
    }
    else
    {
        /* Record a failure if either result is wrong. */
        tests_failed++;

        printf("FAIL: %s + %s\n",
               state_name(current),
               event_name(event));

        printf("  Expected: %s %s\n",
               state_name(expected_next),
               action_name(expected_action));

        printf("  Actual:   %s %s\n",
               state_name(actual_next),
               action_name(actual_action));
    }
}

/* Test how lock_event() interprets a character.
 *
 * state:    The lock's current state.
 * input:    The character entered by the user.
 * expected: The event that should be returned.
 */
static void check_event(state_t state,
                        char input,
                        int expected)
{
    /* Ask lock_event() to interpret the character. */
    int actual = lock_event(state, input);

    /* Increase the total number of tests. */
    tests_run++;

    /* Check whether the event is what we expected. */
    if (actual == expected)
    {
        printf("PASS: %s '%c' -> %s\n",
               state_name(state),
               input,
               event_name((event_t)actual));
    }
    else
    {
        /* Increase the failure count if incorrect. */
        tests_failed++;

        printf("FAIL: %s '%c'\n",
               state_name(state),
               input);

        printf("  Expected: %s\n",
               event_name((event_t)expected));

        printf("  Actual:   %s\n",
               event_name((event_t)actual));
    }
}

/* Test a complete sequence of input characters.
 *
 * input:            Characters to enter into the lock.
 * expected_final:   State expected after all characters.
 * expected_unlocks: Number of UNLOCK actions expected.
 * expected_locks:   Number of LOCK actions expected.
 * expected_beeps:   Number of BEEP actions expected.
 */
static void check_sequence(const char *input,
                           state_t expected_final,
                           int expected_unlocks,
                           int expected_locks,
                           int expected_beeps)
{
    /* Every sequence begins with the door locked. */
    state_t current = LOCKED;

    /* Count how many times each action happens. */
    int unlocks = 0;
    int locks = 0;
    int beeps = 0;
    int valid = 1;

    /* Process each character until the string ends. */
    for (int i = 0; input[i] != '\0'; i++)
    {
        /* Convert the character into an event. */
        int event = lock_event(current, input[i]);

        /* A sequence should contain only valid events. */
        if (event == -1)
        {
            valid = 0;
            break;
        }

        /* Find the next state and resulting action. */
        action_t action;
        current = lock_step(current, (event_t)event,
                            &action);

        /* Count the actions produced by the FSM. */
        if (action == A_UNLOCK)
            unlocks++;

        if (action == A_LOCK)
            locks++;

        if (action == A_BEEP)
            beeps++;
    }

    /* Count this complete sequence as one test. */
    tests_run++;

    /* Check the final state and all action counts. */
    if (valid &&
        current == expected_final &&
        unlocks == expected_unlocks &&
        locks == expected_locks &&
        beeps == expected_beeps)
    {
        printf("PASS: Sequence %s -> %s\n",
               input, state_name(current));
    }
    else
    {
        /* Record a failed sequence test. */
        tests_failed++;

        printf("FAIL: Sequence %s\n", input);
        printf("  Final state: %s\n",
               state_name(current));
        printf("  UNLOCK: %d, LOCK: %d, BEEP: %d\n",
               unlocks, locks, beeps);
    }
}

int main(void)
{
    printf("Testing all 16 FSM transitions\n\n");

    /* Tests 1-4: Starting in LOCKED.
     * Correct digit 3 moves to GOT1.
     * Wrong digits cause a beep.
     * Close and timeout leave us locked.
     */
    check_transition(LOCKED, E_RIGHT, GOT1, A_NONE);
    check_transition(LOCKED, E_WRONG, LOCKED, A_BEEP);
    check_transition(LOCKED, E_CLOSE, LOCKED, A_NONE);
    check_transition(LOCKED, E_TIMEOUT, LOCKED, A_NONE);

    /* Tests 5-8: Starting in GOT1.
     * Correct digit 1 moves to GOT2.
     * Wrong digits reset the lock.
     * Close changes nothing.
     * Timeout resets silently.
     */
    check_transition(GOT1, E_RIGHT, GOT2, A_NONE);
    check_transition(GOT1, E_WRONG, LOCKED, A_BEEP);
    check_transition(GOT1, E_CLOSE, GOT1, A_NONE);
    check_transition(GOT1, E_TIMEOUT, LOCKED, A_NONE);

    /* Tests 9-12: Starting in GOT2.
     * Correct digit 4 opens the door.
     * Wrong digits reset with a beep.
     * Close changes nothing.
     * Timeout resets silently.
     */
    check_transition(GOT2, E_RIGHT, OPEN, A_UNLOCK);
    check_transition(GOT2, E_WRONG, LOCKED, A_BEEP);
    check_transition(GOT2, E_CLOSE, GOT2, A_NONE);
    check_transition(GOT2, E_TIMEOUT, LOCKED, A_NONE);

    /* Tests 13-16: Starting in OPEN.
     * Digit events leave the door open.
     * Close and timeout lock the door.
     */
    check_transition(OPEN, E_RIGHT, OPEN, A_NONE);
    check_transition(OPEN, E_WRONG, OPEN, A_NONE);
    check_transition(OPEN, E_CLOSE, LOCKED, A_LOCK);
    check_transition(OPEN, E_TIMEOUT, LOCKED, A_LOCK);

    /* Test lock_event() with digit inputs.
     *
     * Each state expects a different correct digit.
     * While OPEN, digits have no effect on the lock.
     */
    printf("\nTesting lock_event() digit recognition\n\n");

    /* LOCKED expects the first digit: 3. */
    check_event(LOCKED, '3', E_RIGHT);
    check_event(LOCKED, '1', E_WRONG);

    /* GOT1 expects the second digit: 1. */
    check_event(GOT1, '1', E_RIGHT);
    check_event(GOT1, '3', E_WRONG);

    /* GOT2 expects the third digit: 4. */
    check_event(GOT2, '4', E_RIGHT);
    check_event(GOT2, '1', E_WRONG);

    /* OPEN ignores all digits through the table. */
    check_event(OPEN, '3', E_WRONG);
    check_event(OPEN, '4', E_WRONG);

    /* Test the door closing in every state.
     * The character 'c' must always produce E_CLOSE.
     */
    printf("\nTesting door closing events\n\n");

    check_event(LOCKED, 'c', E_CLOSE);
    check_event(GOT1,   'c', E_CLOSE);
    check_event(GOT2,   'c', E_CLOSE);
    check_event(OPEN,   'c', E_CLOSE);

    /* Test timer expiration in every state.
     * The character 't' must always produce E_TIMEOUT.
     */
    printf("\nTesting timer expiration events\n\n");

    check_event(LOCKED, 't', E_TIMEOUT);
    check_event(GOT1,   't', E_TIMEOUT);
    check_event(GOT2,   't', E_TIMEOUT);
    check_event(OPEN,   't', E_TIMEOUT);

    /* Test invalid characters.
     * Characters other than digits, 'c', and 't'
     * should return -1 from lock_event().
     */
    printf("\nTesting invalid characters\n\n");

    check_event(LOCKED, 'x', -1);
    check_event(GOT1,   '?', -1);
    check_event(GOT2,   'a', -1);
    check_event(OPEN,   'z', -1);

    /* Test complete input sequences.
     * Each sequence begins in LOCKED.
     */
    printf("\nTesting complete combinations\n\n");

    /* Correct combination:
     * Door opens and produces one UNLOCK.
     */
    check_sequence("314", OPEN, 1, 0, 0);

    /* Incorrect second digit:
     * The second 3 resets the lock.
     * The following 1 and 4 are also incorrect.
     * Therefore, three BEEP actions occur.
     */
    check_sequence("3314", LOCKED, 0, 0, 3);

    /* Timer expires after entering 3 and 1:
     * Timeout resets silently.
     * The final 4 is incorrect and causes BEEP.
     */
    check_sequence("31t4", LOCKED, 0, 0, 1);

    /* Correct combination followed by closing:
     * The door unlocks, then locks again.
     */
    check_sequence("314c", LOCKED, 1, 1, 0);

    /* Print our test summary. */
    printf("\nTests run: %d\n", tests_run);
    printf("Tests failed: %d\n", tests_failed);

    /* Return nonzero if at least one test failed. */
    return tests_failed != 0;
}
