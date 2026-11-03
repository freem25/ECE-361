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

## 6. Current Progress

The project currently has all five module pairs created as stubs.

The main program initializes the iom361 simulator with 3 switches and 3 LEDs and prints "Sauna Controller".

The complete control logic and hardware behavior will be implemented in later assignments.


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

The Week 5 FSM table and dedicated unit tests are implemented. Actual
switch-edge detection, timestamp-based timeout detection, heater/vent
outputs, and scenario replay remain to be connected to `main.c` in
subsequent work. The sample scenarios are inputs only until that replay
is implemented; they must not be represented as passing end-to-end
scenario tests yet.
