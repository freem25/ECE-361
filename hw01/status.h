#ifndef STATUS_H
#define STATUS_H

#include <stdint.h>

enum {
    HEAT_POS = 0,
    HEAT_WIDTH = 1,
    COOL_POS = 1,
    COOL_WIDTH = 1,
    FAN_POS = 2,
    FAN_WIDTH = 1,
    FAULT_POS = 3,
    FAULT_WIDTH = 1,
    MODE_POS = 4,
    MODE_WIDTH = 3,
    RESERVED_POS = 7,
    RESERVED_WIDTH = 1,
    SETPOINT_POS = 8,
    SETPOINT_WIDTH = 8
};

enum {
    MODE_OFF = 0,
    MODE_HEAT = 1,
    MODE_COOL = 2,
    MODE_AUTO = 3,
    MODE_FAN_ONLY = 4,
    MODE_INVALID = -1
};

typedef struct {
    uint32_t heat;
    uint32_t cool;
    uint32_t fan;
    uint32_t fault;
    int mode;
    uint32_t reserved;
    int32_t setpoint;
} status_t;

status_t status_unpack(uint16_t word);

#endif
