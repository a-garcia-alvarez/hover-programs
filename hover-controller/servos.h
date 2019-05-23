#ifndef __SERVOS_H__
#define __SERVOS_H__

#include <pigpio.h>

#include "../rfudp/TxTunnel.h"
#include "PinConfig.h"

void servo_process(char*msg);

#endif // __SERVOS_H__
