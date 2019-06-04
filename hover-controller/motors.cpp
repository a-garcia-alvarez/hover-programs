#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "motors.h"

void motor_esc_initialize(unsigned int pin) {
    //servo_mv_r(pin, 0);
    //sleep(0.5);
    servo_mv_r(pin, MAIN_PROP_STOP_PPM);
    //sleep(1);
    //servo_mv_r(pin, MAIN_PROP_START_PPM);
}

void all_motors_stop() {
    servo_mv_r(MAIN_PROP_GPIO,  MAIN_PROP_STOP_PPM);
    servo_mv_r(LEFT_PROP_GPIO,  LEFT_PROP_STOP_PPM);
    servo_mv_r(RIGHT_PROP_GPIO, RIGHT_PROP_STOP_PPM);
}
void all_motors_zero() {
    servo_mv_r(MAIN_PROP_GPIO,  0);
    servo_mv_r(LEFT_PROP_GPIO,  0);
    servo_mv_r(RIGHT_PROP_GPIO, 0);
}
void all_motors_start() {
    servo_mv_r(MAIN_PROP_GPIO,  MAIN_PROP_START_PPM);
    servo_mv_r(LEFT_PROP_GPIO,  LEFT_PROP_START_PPM);
    servo_mv_r(RIGHT_PROP_GPIO, RIGHT_PROP_START_PPM);
}

int map_esc_per(unsigned int pin, int lv) {
    int lv_new;
    switch(pin) {
        case MAIN_PROP_GPIO:
            lv_new = map(lv, 0, 100, MAIN_PROP_IDLE_PPM, MAIN_PROP_MAX_PPM);
            break;
        case LEFT_PROP_GPIO:
            lv_new = map(lv, 0, 100, LEFT_PROP_IDLE_PPM, LEFT_PROP_MAX_PPM);
            break;
        case RIGHT_PROP_GPIO:
            lv_new = map(lv, 0, 100, RIGHT_PROP_IDLE_PPM, RIGHT_PROP_MAX_PPM);
            break;
        default:
            lv_new = map(lv, 0, 100, ESC_MIN, ESC_MAX);
    }
    return lv_new;
}
void advance_both(char*msg, int *lv, int offset){
    int lv_r, lv_l;
    int offset_base = *lv/4;    //get offset to ajust 

    lv_r = lv_l = *lv;      // asign base level
    eprintf("lv: %i,\tlv_r:%i\t,lv_l:%i\n", *lv, lv_r, lv_l);

    // offset values if necesary
    if( offset < 50)
        lv_r = lv_r - offset_base;
    if( offset < 50)
        lv_l = lv_l - offset_base;
    eprintf("lv: %i,\tlv_r:%i\t,lv_l:%i\n", *lv, lv_r, lv_l);

    // map to real values
    lv_r = map_esc_per(RIGHT_PROP_GPIO, lv_r);
    lv_l = map_esc_per(LEFT_PROP_GPIO,  lv_l);

    // ajust motor speed
    servo_mv_r(RIGHT_PROP_GPIO, lv_r);
    servo_mv_r(LEFT_PROP_GPIO,  lv_l);
}

void motor_process(char*msg){
    char servo_index = msg[4];
    char servo_mode  = msg[5];
    int lv, offset;
    char action[5];
    unsigned int pin;

    static bool started=false;

    sscanf(&msg[3]," %s ", action);
    eprintf("Act: %s\n", action);

    if (strcmp(action, "START") == 0) {
        if (!started) {
            eprintf("Estarting ESCs\n");
            all_motors_stop();  // set all motors to stop position
            usleep(10000);      // wait just a bit (10000 µs or 0,01 secs)
            all_motors_start(); // set all to start speed
            //servo_mv_r(MAIN_PROP_GPIO, MAIN_PROP_START_PPM);
            started=true;

            strcpy(msg, "ESC_STARTED");
        } else {
            eprintf("ESC already started\n");

            strcpy(msg, "ESC_IS_STARTED");
        }
        return;
    } else
    if (strcmp(action, "STOP") == 0) {
        //servo_mv_r(MAIN_PROP_GPIO, MAIN_PROP_STOP_PPM);
        all_motors_stop();  // Stops all ESCs
        started=false;

        strcpy(msg, "ESC_STOPPED");
        return;
    } else {
        sscanf(&msg[6], " %i %i", &lv, &offset );
    }

    if (servo_index == 'M')
        pin = MAIN_PROP_GPIO;
    else
    if (servo_index == 'L')
        pin = LEFT_PROP_GPIO;
    else
    if (servo_index == 'R')
        pin = RIGHT_PROP_GPIO;
    else
    if (servo_index == 'P') {
        advance_both(msg, &lv, offset);
        return;
    }
    else {
        eprintf("[motors] index not valid (%c)\n", servo_index);
        strcpy(msg, "ESC_INDX_NOK");
        return;
    }
    
    switch (servo_mode) {
        case ' ':
            if (lv < 500)
                servo_mode = '%';
            else {
                servo_mode = 'R';
                break;
            }
        case '%':
            lv = map_esc_per(pin, lv);
            break;
        case 'R':
            break;
        default:
            eprintf("[motors] mode not valid (%c)\n", servo_mode);
            strcpy(msg, "ESC_MODE_NOK");
            return;
    }

    servo_mv_r(pin, lv);
    sprintf(msg, "ESC %c%c %i", servo_index, servo_mode, lv);
}
