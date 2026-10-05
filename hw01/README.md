#Homework 1

#Bit manipulation library

hw 1, parts 1-4 implements a small C library for manipulating fixed-width bit fields and decoding a 16-bit thermostat status word.

The bit-manipulation library provides these functions:
- `print_binary(uint32_t x, int width)` prints the lowest `width` bits of `x` in binary, most significant bit first, with groups of four bits separated by spaces.
- `get_field(uint32_t word, int pos, int width)` extracts a bit field beginning at `pos` with the given `width` and shifts the result down to bit 0.
- `set_field(uint32_t word, int pos, int width, uint32_t value)` replaces a bit field in `word` with the lowest `width` bits of `value`.
- `sign_extend(uint32_t value, int width)` interprets the lowest `width` bits of `value` as a two's-complement integer and returns the result as an `int32_t`.

The thermostat decoder provides:
- `status_unpack(uint16_t word)`, which decodes the thermostat status word into a `status_t` structure.
