
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "sauna_status.h"
#include "iom361_r4.h"
#include "sensor.h"
#include "scenario.h"
#include "fsm.h"
#include "store.h"
#include "aux.h"
#include "report.h"

int main(int argc, char *argv[])
{
    int rc;

    /* Require a scenario filename. */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s scenario.txt\n", argv[0]);
        return 1;
    }

    /* Load the scenario file into memory. */
    scenario_t scenario;

    rc = scenario_load(argv[1], &scenario);

    if (rc != 0) {
        fprintf(stderr, "Scenario load failed: %d\n", rc);
        return 1;
    }

    /* Current replay requires initial values at tick 0. */
    if (scenario.count == 0 || scenario.lines[0].tick != 0) {
        fprintf(stderr, "Scenario must begin at tick 0\n");
        return 1;
    }

    /* Initialize the sauna's three switches and LEDs. */
    uint32_t *base = iom361_initialize(3, 3, &rc);

    if (base == NULL || rc != 0) {
        fprintf(stderr, "Hardware initialization failed\n");
        return 1;
    }

    /* Prevent automatic hardware output from mixing with traces. */
    iom361_set_display(false);

    /* Initialize the existing project modules. */
    sensor_init();
    fsm_init();
    store_init();
    aux_init();
    report_init();

    /* Get the final scenario tick. */
    uint32_t last_tick = scenario.lines[scenario.count - 1].tick;

/* Start with the sauna turned OFF. */
fsm_state_t state = FSM_OFF;

/* Remember the previous switch positions. */
uint32_t previous_switches = 0;

/* Remember when the current session began. */
time_t session_start_time = 0;

/* Remember the heater's previous state for hysteresis. */
int heater_on = 0;

/* Remember whether humidity has disabled the heater. */
int humidity_lockout = 0;

    /* Replay every tick from 0 through the final tick. */
    for (uint32_t tick = 0; ; tick++) {

        /* Obtain the values active at this tick. */
        const scenario_line_t *line =
            scenario_at_tick(&scenario, tick);

        if (line == NULL) {
            fprintf(stderr, "No values at tick %u\n",
                    (unsigned)tick);
            return 1;
        }

        /* Update the simulated physical environment. */
        _iom361_set_sensor1(line->temp_c, line->rh_pct);
        _iom361_set_switches(line->switches);

        /* Calculate the current Unix timestamp. */
        time_t now = scenario.start + (time_t)tick;

        /* Read temperature and humidity through the registers. */
        reading_t reading = {0};

        rc = sensor_read(base, now, &reading);

        if (rc != 0) {
            fprintf(stderr, "Sensor read failed: %d\n", rc);
            return 1;
        }

        /* Read the switch register. */
        uint32_t switches =
            iom361_readReg(base, SWITCHES_REG, &rc);

        if (rc != 0) {
            fprintf(stderr, "Switch read failed: %d\n", rc);
            return 1;
        }


        /* Detect switches that changed from OFF to ON. */
        uint32_t pressed = switches & ~previous_switches;

        /* Start with no FSM event. */
        fsm_event_t event = FSM_EVENT_NONE;

        /* Determine the highest-priority event. */

        if (state == FSM_COOLDOWN) {

            /* Cooling finishes below 40 degrees C. */
            if (reading.temperature < 40.0f) {
                event = FSM_EVENT_COOL_COMPLETE;
            }

        } else if (state != FSM_OFF &&
                   (switches & 0x1u) == 0) {

            /* Power OFF has priority in active states. */
            event = FSM_EVENT_POWER_OFF;

        } else if (state == FSM_OFF) {

            /* Power is a level switch, not a press event. */
            if (switches & 0x1u) {
                event = FSM_EVENT_POWER_ON;
            }

        } else if (state == FSM_PREHEAT) {

            /* The sauna becomes READY at 78 degrees C. */
            if (reading.temperature >= 78.0f) {
                event = FSM_EVENT_TEMP_READY;
            }

        } else if (state == FSM_READY) {

            /* Falling below 78 requires more preheating. */
            if (reading.temperature < 78.0f) {
                event = FSM_EVENT_TEMP_LOW;
            } else if (pressed & 0x2u) {
                event = FSM_EVENT_SESSION_START;
            }

        } else if (state == FSM_SESSION) {

            /* End button or 900-second timeout. */
            if (pressed & 0x4u) {
                event = FSM_EVENT_SESSION_END;
            } else if (now - session_start_time >= 900) {
                event = FSM_EVENT_SESSION_TIMEOUT;
            }
        }

        /* Consult the existing FSM transition table. */
        fsm_action_t action = FSM_ACTION_NONE;

        fsm_state_t next_state =
            fsm_next_state(state, event, &action);

        /* Record when a new session begins. */
        if (state != FSM_SESSION &&
            next_state == FSM_SESSION) {
            session_start_time = now;
        }

        /* Update the current state. */
        state = next_state;


        /* Apply humidity rules while in SESSION. */
        if (state == FSM_SESSION) {

            fsm_action_t humidity_action = FSM_ACTION_NONE;

            if (reading.humidity > 40.0f && !humidity_lockout) {

                /* Ask the FSM to block heating. */
                state = fsm_next_state(
                    state, FSM_EVENT_HUMID_HIGH, &humidity_action);

            } else if (reading.humidity < 30.0f &&
                       humidity_lockout) {

                /* Ask the FSM to allow heating again. */
                state = fsm_next_state(
                    state, FSM_EVENT_HUMID_SAFE, &humidity_action);
            }

            /* Apply the action returned by the FSM. */
            if (humidity_action == FSM_ACTION_BLOCK_HEATER) {
                humidity_lockout = 1;
            } else if (humidity_action == FSM_ACTION_ALLOW_HEATER) {
                humidity_lockout = 0;
            }

        } else {

            /* Lockout is only active during SESSION. */
            humidity_lockout = 0;
        }


        /* Determine whether the heater should operate. */
        if (state == FSM_OFF || state == FSM_COOLDOWN) {

            /* Never heat while OFF or cooling down. */
            heater_on = 0;

        } else if (humidity_lockout) {

            /* High humidity disables heating during SESSION. */
            heater_on = 0;

        } else if (reading.temperature < 78.0f) {

            /* Temperature is too low: enable heating. */
            heater_on = 1;

        } else if (reading.temperature >= 80.0f) {

            /* Temperature reached the upper limit. */
            heater_on = 0;
        }

        /* Between 78 and 80 C, preserve heater_on. */

        /* Construct the three-bit LED register value. */
        uint32_t desired_leds = 0;

        /* LED 0: HEATER */
        if (heater_on) {
            desired_leds |= 0x1u;
        }

        /* LED 1: VENT */
        if (state == FSM_COOLDOWN) {
            desired_leds |= 0x2u;
        }

        /* LED 2: READY_LAMP */
        if (state == FSM_READY) {
            desired_leds |= 0x4u;
        }

        /* Write the LED register through iom361. */
        iom361_writeReg(base, LEDS_REG, desired_leds, &rc);

        if (rc != 0) {
            fprintf(stderr, "LED write failed: %d\n", rc);
            return 1;
        }



        /* Read back the actual LED register value. */
        uint32_t leds = iom361_readReg(base, LEDS_REG, &rc);

        if (rc != 0) {
            fprintf(stderr, "LED read failed: %d\n", rc);
            return 1;
        }

/* Pack the status word after reading the LED register. */
uint32_t status = sauna_status_pack(
    state,
    leds,
    humidity_lockout
);
        /* ==========================================
           RGB LED: Display current sauna state
           ========================================== */

        uint32_t rgb_value = 0;

        /* Select a distinct RGB color for each state. */
        switch (state) {

            case FSM_OFF:
                /* Turn the RGB LED completely off. */
                rgb_value = build_rgb_reg(0, 0, 0, 0);
                break;

            case FSM_PREHEAT:
                /* Red indicates the sauna is preheating. */
                rgb_value = build_rgb_reg(1, 255, 0, 0);
                break;

            case FSM_READY:
                /* Green indicates the sauna is ready. */
                rgb_value = build_rgb_reg(1, 0, 255, 0);
                break;

            case FSM_SESSION:
                /* Blue indicates an active sauna session. */
                rgb_value = build_rgb_reg(1, 0, 0, 255);
                break;

            case FSM_COOLDOWN:
                /* Cyan indicates the sauna is cooling. */
                rgb_value = build_rgb_reg(1, 0, 255, 255);
                break;

            default:
                /* Keep the RGB LED off for an invalid state. */
                rgb_value = build_rgb_reg(0, 0, 0, 0);
                break;
        }

        /* Write the selected color to the RGB register. */
        iom361_writeReg(base, RGB_LED_REG, rgb_value, &rc);

        if (rc != 0) {
            fprintf(stderr, "RGB LED write failed: %d\n", rc);
            return 1;
        }


        /* Save switches for the next tick. */
        previous_switches = switches;


        /* Format the timestamp in UTC. */
        struct tm *utc = gmtime(&reading.timestamp);
        char time_string[32];

        if (utc == NULL ||
            strftime(time_string, sizeof(time_string),
                     "%Y-%m-%d %H:%M:%S", utc) == 0) {
            fprintf(stderr, "Time formatting failed\n");
            return 1;
        }



        /* Print the required sauna controller trace. */
        printf("%s state=%s temp=%.1f rh=%.1f "
               "leds=0x%X status=0x%08X\n",
               time_string,
               fsm_state_name(state),
               reading.temperature,
               reading.humidity,
               (unsigned)leds,
               (unsigned)status);


        if (tick == last_tick) {
            break;
        }
    }

    return 0;
}
