
#include <stdio.h>
#include "lock.h"

/* Main program for our combination lock.
 *
 * Reads characters from standard input.
 * Converts each valid character into an event.
 * Uses the transition table to update the state.
 * Prints each transition and the final state.
 */
int main(void)
{
    /* The lock always begins in the LOCKED state. */
    state_t current = LOCKED;

    /* getchar() returns an int so it can represent
     * both characters and the special EOF value.
     */
    int ch;

    /* Read one character at a time until EOF.
     * EOF means there is no more input available.
     */
    while ((ch = getchar()) != EOF)
    {
        /* Convert the character into an FSM event.
         * For example, '3' is RIGHT when LOCKED.
         */
        int event = lock_event(current, (char)ch);

        /* Ignore characters that are not valid events.
         * This also skips spaces and newlines.
         */
        if (event == -1)
        {
            continue;
        }

        /* Store the action returned by lock_step(). */
        action_t action;

        /* Look up the transition in our FSM table.
         * The function also writes the action.
         */
        state_t next = lock_step(current,
                                 (event_t)event,
                                 &action);

        /* Print the complete transition:
         * current state, character, event,
         * next state, and action.
         */
        printf("%s %c %s -> %s %s\n",
               state_name(current),
               (char)ch,
               event_name((event_t)event),
               state_name(next),
               action_name(action));

        /* Update the current state so the next
         * character uses our new FSM state.
         */
        current = next;
    }

    /* Print the final state after all input is read. */
    printf("final: %s\n", state_name(current));

    /* Return zero to indicate normal completion. */
    return 0;
}
