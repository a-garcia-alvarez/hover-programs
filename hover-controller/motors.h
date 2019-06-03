#ifndef __MOTORS_H__
#define __MOTORS_H__

#include "servos.h"
void motor_process(char*msg);
void all_motors_stop();
void all_motors_zero();
void all_motors_start();

#endif // __MOTORS_H__
