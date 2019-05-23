#ifndef __DISPACHER_H__
#define __DISPACHER_H__

#include "../rfudp/TxTunnel.h"
#include "PinConfig.h"
#include "sensors.h"
#include "servos.h"
#include "motors.h"

void process_msg(char*msg);

#endif // __DISPACHER_H__
