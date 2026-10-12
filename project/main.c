
#include <stdio.h>
#include <stdint.h>
#include "iom361_r4.h"
#include "sensor.h"
#include "fsm.h"
#include "store.h"
#include "aux.h"
#include "report.h"

int main(void) {
    int result = 0;

    uint32_t *registers = iom361_initialize(3, 3, &result);

    if (result != 0 || registers == NULL) {
        fprintf(stderr, "Sauna Controller initialization failed\n");
        return 1;
    }

    sensor_init();
    fsm_init();
    store_init();
    aux_init();
    report_init();

    printf("Sauna Controller\n");

    return 0;
}

