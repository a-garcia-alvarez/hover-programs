#include <stdio.h>
#include <string.h>

#include "PinConfig.h"
#include "servos.h"


void servo_mv_r(unsigned int pin, int lv) {
        gpioSetMode(pin, PI_OUTPUT);
        gpioServo(pin, lv);
}

int map(int input, int input_start, int input_end, int output_start, int output_end) {
    return output_start + ((output_end - output_start) / (input_end - input_start)) * (input - input_start);
}

int conver_pc_raw(unsigned pin, int lv) {
    int lv_new;
    switch(pin) {
        case LEFT_RUDDER_GPIO:
            lv_new = map(lv, 0, 100, LEFT_RUDDER_MIN, LEFT_RUDDER_MAX);
            break;
        case RIGHT_RUDDER_GPIO:
            lv_new=map(lv, 0, 100, RIGHT_RUDDER_MIN, RIGHT_RUDDER_MAX);
            break;
        default:
            return -1;
    }
    return lv_new;
}

void servo_mv(char*msg, char* mode, unsigned int pin, int *lv) {
    if (*mode == ' ')
        *mode = ( *lv < 500 ) ? '%':'R';

    if (*mode == '%') {
        *lv=conver_pc_raw(pin, *lv);
    } else
    if (*mode == 'R') {
    } else {
        eprintf("[servos] mode not valid (%c)\n", *mode);
        strcpy(msg, "SER_MODE_NOK");
        return;
    }

    servo_mv_r(pin, *lv);

}

int map_ser_per( int lv){
    int lv_r, lv_l;
    lv_r = map(lv, 0, 100, RIGHT_RUDDER_MIN, RIGHT_RUDDER_MAX);
    lv_l = map(lv, 0, 100, LEFT_RUDDER_MIN,  LEFT_RUDDER_MAX);
    
    servo_mv_r(RIGHT_RUDDER_GPIO,   &lv_r);
    servo_mv_r(LEFT_RUDDER_GPIO,    &lv_l);
}

void servo_process(char*msg) {
    char servo_index = msg[4];
    char servo_mode  = msg[5];
    int lv;

    sscanf(&msg[6], " %i ", &lv );

    if (servo_index == 'L') {
        servo_mv(msg, &servo_mode, LEFT_RUDDER_GPIO, &lv);
    } else
    if (servo_index == 'R') {
        servo_mv(msg, &servo_mode, RIGHT_RUDDER_GPIO, &lv);
    } else
    if (servo_index == 'P') {
        map_ser_per(lv);
    } else {
        eprintf("[servos] index not valid (%c)\n", servo_index);
        strcpy(msg, "SER_INDX_NOK");
        return;
    }
    sprintf(msg, "SER %c%c %i", servo_index, servo_mode, lv);
}


