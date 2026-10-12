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
