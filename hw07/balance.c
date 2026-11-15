
#include "istack.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * Part 2: Bracket Checker
 * Reads characters from standard input and checks whether
 * parentheses (), square brackets [], and curly braces {}
 * are correctly matched.
 * Uses the integer stack from Part 1 to store opening brackets.
 * Tracks line numbers, column numbers, completed bracket pairs,
 * and the maximum nesting depth.
 */

/*
 * Checks whether a character is an opening bracket.
 * Returns 1 for (, [, or {.
 * Returns 0 for any other character.
 */
static int is_opening(int ch)
{
    return ch == '(' || ch == '[' || ch == '{';
}

/*
 * Checks whether a character is a closing bracket.
 * Returns 1 for ), ], or }.
 * Returns 0 for any other character.
 */
static int is_closing(int ch)
{
    return ch == ')' || ch == ']' || ch == '}';
}

/*
 * Determines the closing bracket that matches an opening bracket.
 * Returns ')' for '('.
 * Returns ']' for '['.
 * Returns '}' for '{'.
 * Returns '\0' for an invalid opening bracket.
 */
static char closing_for(int opening)
{
    switch (opening) {
        case '(':
            return ')';

        case '[':
            return ']';

        case '{':
            return '}';

        default:
            return '\0';
    }
}

/*
 * Main function for the bracket checker.
 *
 * Reads standard input one character at a time.
 * Uses the stack to track opening brackets.
 * Reports the first bracket error or a successful result.
 */
int main(void)
{
    /* Create the stack for storing opening brackets. */
    istack_t *s = istack_create();

    /* Stop if the stack could not be created. */
    if (s == NULL) {
        fprintf(stderr, "error: could not create stack\n");
        return 1;
    }

    /* Track input position, completed pairs, and nesting depth. */
    size_t line = 1;
    size_t column = 0;
    size_t pairs = 0;
    size_t deepest = 0;

    int ch;

    /* Read one character at a time until the end of input. */
    while ((ch = getchar()) != EOF) {

        /* Move to the next line when a newline is encountered. */
        if (ch == '\n') {
            line++;
            column = 0;
            continue;
        }

        /* Count the current character's column position. */
        column++;

        /* Handle an opening bracket. */
        if (is_opening(ch)) {

            /* Push the opening bracket onto the stack. */
            if (istack_push(s, ch) != 0) {
                fprintf(stderr, "error: out of memory\n");
                istack_destroy(s);
                return 1;
            }

            /* Update the deepest nesting level if necessary. */
            size_t depth = istack_count(s);

            if (depth > deepest) {
                deepest = depth;
            }
        }

        /* Handle a closing bracket. */
        else if (is_closing(ch)) {

            int opening;

            /* Check whether there is an opening bracket available. */
            if (istack_peek(s, &opening) != 0) {
                printf(
                    "line %zu, column %zu: found '%c', nothing open\n",
                    line, column, ch
                );

                istack_destroy(s);
                return 1;
            }

            /* Find the closing bracket that should appear. */
            char expected = closing_for(opening);

            /* Report an error if the brackets do not match. */
            if (ch != expected) {
                printf(
                    "line %zu, column %zu: found '%c', expected '%c'\n",
                    line, column, ch, expected
                );

                istack_destroy(s);
                return 1;
            }

            /* Remove the successfully matched opening bracket. */
            istack_pop(s, NULL);

            /* Count one completed bracket pair. */
            pairs++;
        }
    }

    /* Check whether reading the input produced an error. */
    if (ferror(stdin)) {
        fprintf(stderr, "error: failed to read input\n");
        istack_destroy(s);
        return 1;
    }

    /* Check whether any opening brackets remain unmatched. */
    if (!istack_is_empty(s)) {

        int opening;
        istack_peek(s, &opening);

        printf(
            "end of input: %zu still open, innermost '%c'\n",
            istack_count(s), opening
        );

        istack_destroy(s);
        return 1;
    }

    /* All brackets were successfully matched. */
    printf(
        "balanced: %zu pairs, deepest %zu\n",
        pairs, deepest
    );

    /* Free all dynamically allocated stack memory. */
    istack_destroy(s);

    /* Return success. */
    return 0;
}
