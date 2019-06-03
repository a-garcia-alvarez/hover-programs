#ifndef __TX_TUNNEL_H__
#define __TX_TUNNEL_H__

// Basic headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

// RF libs
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <RF24/RF24.h>
#include <unistd.h>

// UDP libs
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// Signal handling
#include <signal.h>

// Error handling
#include <errno.h>


// various functions and definitions
#define eprintf(...) fprintf(stderr, __VA_ARGS__);

#define FALSE   (1 != 1)
#define TRUE    (!FALSE)


// TxTunnel Congif
#define SERVER_IP   "192.168.1.149"
#define SERVER_PORT 8080
#define MAX_MSG_SZ  32
#define RF_CHANNEL  0
#define MILLIS_WAIT 200

#define ACK_MODE    FALSE

// Radio pipe addresses for the 2 nodes to communicate.
const uint8_t addresses[][5] = {"2RPi","0RPi"};


#endif // __TX_TUNNEL_H__
