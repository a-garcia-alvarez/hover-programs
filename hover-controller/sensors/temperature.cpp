#include "temperature.h"

#include <stdio.h>
//#include <string.h>

float temp_cpu(){
    float systemp;
    float millideg;
    FILE *thermal;

    thermal = fopen("/sys/class/thermal/thermal_zone0/temp","r");
    fscanf(thermal,"%f",&millideg);
    fclose(thermal);
    systemp = millideg / 1000;

    fprintf(stderr, "[sensor:temp:cpu] CPU temperature is %f degrees C\n",systemp);

    return systemp;

}

float temp_gpu(){
    float systemp = -127;
  #ifdef __Pi__
    FILE *fp;
    fp = popen("/opt/vc/bin/vcgencmd measure_temp | cut -c6-9", "r");
    if(fp == NULL){
        perror("[sensor:temp:gpu] GPU command failed!\nAre you on a Pi?\n");
        return -127;
    }
    fscanf(fp,"%f", &systemp);
    pclose(fp);
  #else
  #endif

    return systemp;

}
