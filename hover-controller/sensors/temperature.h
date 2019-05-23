#ifndef __SENSORS_TEMP_H__
#define __SENSORS_TEMP_H__

#ifdef __arm__
  #if __has_include("/opt/vc/include/bcm_host.h")
    #define __Pi__
  #endif
#endif

float temp_cpu();
float temp_gpu();

#endif // __SENSORS_TEMP_H__
