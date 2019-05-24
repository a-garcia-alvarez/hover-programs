#ifndef __SERVOS_H__
#define __SERVOS_H__

#include <pigpio.h>

#include "../rfudp/TxTunnel.h"
#include "PinConfig.h"

void servo_process(char*msg);
void servo_mv_r(unsigned int pin, int lv);
int map(int input, int input_start, int input_end, int output_start, int output_end);

#endif // __SERVOS_H__
