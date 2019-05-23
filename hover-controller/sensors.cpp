#include <stdio.h>

#include "servos.h"
#include "sensors/temperature.h"

void sensor_gyro();
void sensor_acel();
void sensor_ulson();
void sensor_laser();
void sensor_temp(char*msg, char index) {
    float temp;
    if (index == 'C') {
        temp = temp_cpu();
    } else
    if (index == 'G') {
        temp = temp_gpu();
    } else {
        eprintf("[sensor:temp] class not valid (%c)\n", index);
        strcpy(msg, "SEN_TMP_CLS_NOK");
        return;
    }
    sprintf(msg, "SEN T%c %.2f", index,  temp);
}

void sensor_process(char*msg) {
    char sensor_type = msg[4];
    char sensor_index = msg[5];
    int lv;

    sscanf(&msg[6], " %i ", &lv );

    if (sensor_type == 'G') {

    } else
    if (sensor_type == 'A') {

    } else
    if (sensor_type == 'U') {

    } else
    if (sensor_type == 'L') {

    } else
    if (sensor_type == 'T') {
        sensor_temp(msg, sensor_index);
    } else {
        eprintf("[sensor] type not valid (%c)\n", sensor_type);
        strcpy(msg, "SEN_TYP_NOK");
        return;
    }
}
