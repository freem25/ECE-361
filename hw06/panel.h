
#ifndef PANEL_H
#define PANEL_H

#include <stdint.h>

/* Returns bits for switches that changed from OFF to ON. */
uint32_t panel_pressed(uint32_t now, uint32_t before);

/* Returns bits for switches that changed from ON to OFF. */
uint32_t panel_released(uint32_t now, uint32_t before);

/* Turns LED n on or off without changing other LEDs.
   Returns -1 if n is outside the range 0 to 31. */
int panel_led(uint32_t *base, int n, int on);

/* Toggles LED n without changing other LEDs.
   Returns -1 if n is outside the range 0 to 31. */
int panel_toggle(uint32_t *base, int n);

#endif
