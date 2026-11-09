# Sauna Controller — Project Specification

## 1. Project Overview

The Sauna Controller is a climate control system that monitors temperature and humidity. It uses 3 switches and 3 LEDs along with an RGB LED to control and display the current operating status.

The system will use the iom361 r4 simulator to read inputs and control outputs.

## 2. System States

**OFF**
- The sauna is not heating.
- The system waits for the user to turn it on.

**PREHEAT**
- The sauna begins heating toward the target temperature.
- Temperature and humidity are monitored.

**READY**
- The sauna has reached the target temperature.
- The system is ready to begin a session.

**SESSION**
- The sauna is in active use.
- The system continues monitoring temperature and humidity.
- Heating and ventilation are adjusted as needed.

**COOLDOWN**
- Heating is turned off.
- The system allows the sauna to cool before returning to OFF.

## 3. Events

The controller responds to events such as:

- Power being turned on or off.
- Temperature reaching the target value.
- The user starting a session.
- A session ending.
- Temperature becoming unsafe.
- The cooldown process finishing.

## 4. State Transitions

Proposed normal sequence:

OFF → PREHEAT → READY → SESSION → COOLDOWN → OFF

Possible transitions:

- OFF to PREHEAT when the user turns the system on.
- PREHEAT to READY when the target temperature is reached.
- READY to SESSION when the user starts the session.
- SESSION to COOLDOWN when the session ends.
- COOLDOWN to OFF when cooling is complete.

Safety conditions may cause the controller to stop heating and move into a safe state.

## 5. Project Modules

**sensor.c / sensor.h**

Handles temperature and humidity readings.

**fsm.c / fsm.h**

Controls the state machine and transitions between operating states.

**store.c / store.h**

Stores the most recent 30 sensor readings. A binary search tree will be used where required by the assignment.

**aux.c / aux.h**

Contains helper functions used by the controller.

**report.c / report.h**

Handles messages and status information displayed to the user.


## 6. Current Integration Progress

The project has moved beyond its initial module skeleton.

The sensor module reads temperature and humidity through the
iom361 hardware simulator.

The FSM module implements the five sauna states using a
two-dimensional transition table.

The main program now loads scenario files and replays sensor
and switch inputs one second at a time.

The controller currently supports:
- State transitions and switch-edge detection.
- Heater temperature hysteresis.
- Humidity-based heater lockout during SESSION.
- Heater, vent, and ready lamp outputs.
- RGB LED indications for all five operating states.
- A packed 32-bit status word.
- Timestamped scenario traces.

Storage, circular-buffer, and reporting functionality
still require further integration and testing.


## 7. Week 5: Finite State Machine (FSM)

### Purpose

The sauna controller has five states: `OFF` (initial), `PREHEAT`, `READY`,
`SESSION`, and `COOLDOWN`. Events are produced from the switch register,
rounded temperature/humidity readings, and elapsed Unix time. The FSM
implements state changes using a two-dimensional table,
`transition_table[state][event]`. Each table cell contains a next state and
an action. `fsm_next_state()` performs the lookup; it is not a chain of
state-dependent `if` statements.

### Events

| Event | Meaning / threshold |
|---|---|
| `NONE` | No relevant change |
| `POWER_ON` | POWER level is on while the state is OFF |
| `POWER_OFF` | POWER level is off while an active state is in progress |
| `TEMP_READY` | Temperature >= 78.0 C |
| `TEMP_LOW` | Temperature < 78.0 C |
| `SESSION_START` | Rising edge of switch bit 1 |
| `SESSION_END` | Rising edge of switch bit 2 |
| `SESSION_TIMEOUT` | 900 elapsed seconds since entry into SESSION |
| `COOL_COMPLETE` | Temperature < 40.0 C |
| `HUMID_HIGH` | In SESSION, humidity > 40.0 %RH |
| `HUMID_SAFE` | In SESSION, humidity < 30.0 %RH after a lockout |

### Transition table

| Current state | Event | Next state | Action |
|---|---|---|---|
| OFF | POWER_ON | PREHEAT | BEGIN_PREHEAT |
| PREHEAT | POWER_OFF | COOLDOWN | BEGIN_COOLDOWN |
| PREHEAT | TEMP_READY | READY | SET_READY |
| READY | POWER_OFF | COOLDOWN | BEGIN_COOLDOWN |
| READY | TEMP_LOW | PREHEAT | BEGIN_PREHEAT |
| READY | SESSION_START | SESSION | BEGIN_SESSION |
| SESSION | POWER_OFF | COOLDOWN | BEGIN_COOLDOWN |
| SESSION | SESSION_END | COOLDOWN | BEGIN_COOLDOWN |
| SESSION | SESSION_TIMEOUT | COOLDOWN | BEGIN_COOLDOWN |
| SESSION | HUMID_HIGH | SESSION | BLOCK_HEATER |
| SESSION | HUMID_SAFE | SESSION | ALLOW_HEATER |
| COOLDOWN | COOL_COMPLETE | OFF | SWITCH_OFF |
| Any state | Any other event | Same state | NONE |

`OFF` with POWER off stays `OFF`; the POWER-off-to-COOLDOWN requirement
applies to active operation. Otherwise a powered-off, cool sauna could
oscillate between `OFF` and `COOLDOWN` every tick. POWER on during
COOLDOWN is ignored until cooling finishes; if it stays on, the sauna
may start PREHEAT on the following tick.

### State diagram (normal sequence)

```text
                  POWER on
    +-----+ --------------------> +---------+
    | OFF |                      | PREHEAT |
    +-----+ <------+             +---------+
       ^             |                  |
       |             |             temp >= 78 C
       |             |                  v
       |             |              +-------+
       |             |              | READY | -- temp < 78 C --> PREHEAT
       |             |              +-------+
       |             |                  |
       |             |             START press
       |             |                  v
       |             |             +---------+
       |             |             | SESSION |
       |             |             +---------+
       |             |                  |
       |             |          END / 900 ticks
       |             |                  v
       |             +--------------+----------+
       +----- temp < 40 C ----------| COOLDOWN |
                                   +----------+

 POWER off from PREHEAT, READY, or SESSION also enters COOLDOWN.
 High humidity in SESSION changes heater permission, not the state.
```

### Actions versus heater/vent control

State-transition actions communicate entry into PREHEAT, READY,
SESSION, COOLDOWN, or OFF, and turn humidity lockout on/off. They do not
by themselves enforce heater temperature hysteresis. The actuator
logic, to be added in main.c, must turn the heater on below 78.0 C,
turn it off at 80.0 C, and retain the previous heater setting between
78.0 and 80.0 C. Entry into READY at 78.0 C must **not** force the
heater off early. COOLDOWN always turns the heater off and opens the
vent. During SESSION, humidity > 40.0 %RH blocks heating until
humidity < 30.0 %RH, independently of temperature. The READY_LAMP
identifies the READY state, and the RGB LED uses a distinct color per
state (colors will be selected before hardware output integration).

### Decisions where the rules were silent

- `SESSION_START` before READY is ignored; the switch must be released
  and pressed again in READY.
- A drop below 78.0 C while waiting in READY returns to PREHEAT.
  During SESSION, the session continues and temperature is controlled
  through heater hysteresis.
- Power off during PREHEAT, READY, or SESSION starts COOLDOWN.
  POWER off while already OFF does nothing.
- Power on during COOLDOWN does not bypass the cooling requirement.
- Humidity is not an automatic stop condition in PREHEAT; the specified
  humidity heater lockout applies only during SESSION.
- The main replay code will generate events, and fsm.c will only map
  state/event pairs to next-state/action pairs. This separates input
  interpretation from the table-driven state machine.
- If multiple conditions occur in one tick, the later event selector
  must prioritize POWER_OFF while in an active state, then
  SESSION_END or SESSION_TIMEOUT, and then other relevant events.
  During COOLDOWN, COOL_COMPLETE must still be processed even if
  the POWER switch remains off. It must ensure the humidity lockout
  affects heater output immediately when humidity is above 40 %RH,
  including on a tick that starts a session.
- No independent overtemperature emergency threshold was provided in
  the assignment; no arbitrary limit is invented here.


### Implementation and testing status

The Week 5 FSM transition table and its 16 dedicated tests
are implemented and passing.

The FSM has now been connected to main.c. The replay loop
generates events from switch positions, temperature,
humidity, and elapsed time.

Scenario replay, heater hysteresis, humidity protection,
standard LED outputs, and RGB LED outputs have been
implemented and manually verified.

The normal scenario, temperature threshold scenario, and
corrected humidity scenario have produced the expected
state transitions and outputs.

The 900-second timeout has been checked in the FSM unit
tests but still requires end-to-end scenario testing.

Automated scenario trace comparisons and full integration
of the storage, auxiliary, and reporting modules remain
future work.



## 8. Week 6: Scenario Replay and Hardware Integration

### Overview

The Sauna Controller now integrates the Homework 6 scenario loader and sensor-reading implementation with the existing table-driven FSM.

The program loads scenario files, advances the simulated clock by one second per tick, reads the hardware registers, determines FSM events, controls the heater and LEDs, and prints a timestamped trace.

### Scenario Replay

The scenario loader accepts files containing a Unix starting timestamp followed by temperature, humidity, and switch values at specified ticks.

Values remain unchanged between scenario entries. The loader supports up to 1,000 data entries.

The current replay implementation requires the first scenario entry to occur at tick 0.

### FSM Integration

The program uses the existing `fsm_next_state()` transition table to determine the next operating state.

The normal operating sequence is:

OFF → PREHEAT → READY → SESSION → COOLDOWN → OFF

The POWER switch is interpreted as a level input. SESSION_START and SESSION_END are detected as rising-edge events.

The program also tracks the elapsed session time to enforce the 900-second session timeout.

### Temperature Hysteresis

The heater follows these rules during active operation:

- Below 78.0°C: heater ON.
- At or above 80.0°C: heater OFF.
- From 78.0°C to below 80.0°C: preserve the previous heater state.
- OFF and COOLDOWN: heater OFF.

This hysteresis prevents rapid heater switching near the temperature thresholds.

### Humidity Protection

During SESSION, humidity above 40.0% activates heater lockout.

The lockout remains active until humidity falls strictly below 30.0%.

When lockout is active, the heater stays OFF regardless of temperature.

### LED Outputs

The three standard LEDs have these assignments:

| Bit | LED | Meaning |
|---|---|---|
| 0 | HEATER | Heater enabled |
| 1 | VENT | Vent open during COOLDOWN |
| 2 | READY_LAMP | Controller in READY |

### RGB LED State Colors

| State | Color | RGB register |
|---|---|---|
| OFF | Disabled | `0x00000000` |
| PREHEAT | Red | `0x80FF0000` |
| READY | Green | `0x8000FF00` |
| SESSION | Blue | `0x800000FF` |
| COOLDOWN | Cyan | `0x8000FFFF` |

The RGB register is written through the `iom361` hardware interface.

### Packed 32-Bit Status Word

The status word is implemented in `sauna_status.c` using the Homework 1 `get_field()` and `set_field()` functions.

| Bit positions | Field |
|---|---|
| 0–2 | FSM state |
| 3 | HEATER |
| 4 | VENT |
| 5 | READY_LAMP |
| 6 | Humidity lockout |
| 7–31 | Unused; zero |

State encodings are OFF = 0, PREHEAT = 1, READY = 2, SESSION = 3, and COOLDOWN = 4.

The functions `sauna_status_pack()` and `sauna_status_unpack()` convert between individual fields and the packed 32-bit representation.

### Testing and Verification

The following functionality has been verified:

- Sensor readings and switch values replay correctly.
- The normal scenario transitions through all five states.
- Heater hysteresis works at 78.0°C and 80.0°C.
- Humidity lockout activates above 40.0% and clears below 30.0%.
- The vent and ready lamp respond to the FSM state.
- The RGB LED register matches the selected state colors.
- Status-word packing and unpacking recover the expected fields.

The current test suite includes 3 passing storage checks, 16 passing FSM tests, and 8 passing status-word tests.

### Remaining Project Work

The replay loop currently initializes the storage, auxiliary, and reporting modules, but does not yet fully integrate record insertion, circular-buffer updates, or report generation.

Additional scenario regression tests and the remaining project-specific data-structure requirements will be completed in later development.
