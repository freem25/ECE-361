# ECE-361
Alexander Freeman
## Part 2 invalid argument behavior

For get_field, invalid arguments return 0.

For set_field, invalid arguments return the original word unchanged.

Arguments are invalid if width is less than 1 or greater than 32,
pos is less than 0 or greater than 31, or pos + width is greater than 32.
## Part 3 invalid mode behavior

For thermostat modes 5, 6, or 7, status_unpack reports the mode as MODE_INVALID (-1).
