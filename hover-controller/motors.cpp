#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "motors.h"

void motor_esc_initialize(unsigned int pin) {
    //servo_mv_r(pin, 0);
    //sleep(0.5);
    servo_mv_r(pin, MAIN_PROP_STOP_PPM);
    usleep(10000);
    //sleep(1);
    //servo_mv_r(pin, MAIN_PROP_START_PPM);
}

void motor_process(char*msg){
    char servo_index = msg[4];
    char servo_mode  = msg[5];
    int lv;
    char action[5];

    static bool started=false;

    sscanf(&msg[3]," %s ", action);
    eprintf("Act: %s\n", action);

    if (strcmp(action, "START") == 0) {
        if (!started){
            eprintf("Estarting ESC\n");
            motor_esc_initialize(MAIN_PROP_GPIO);
            servo_mv_r(MAIN_PROP_GPIO, MAIN_PROP_START_PPM);
            started=true;
        }else
            eprintf("ESC already started\n");
    } else
    if (strcmp(action, "STOP") == 0) {
        servo_mv_r(MAIN_PROP_GPIO, MAIN_PROP_STOP_PPM);
        started=false;
    } else {
        sscanf(&msg[6], " %i ", &lv );
    }

    if (servo_index == 'M') {
        servo_mv_r(MAIN_PROP_GPIO, lv);
    } else
    if (servo_index == 'L') {
        servo_mv_r(LEFT_PROP_GPIO, lv);

    } else
    if (servo_index == 'R') {
        servo_mv_r(RIGHT_PROP_GPIO, lv);

    }
}
