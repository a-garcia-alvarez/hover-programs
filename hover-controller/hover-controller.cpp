/*
TMRh20 2014
Angel garcía 2019

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/** Hover Controller
 * This program manage all the RF communications and electronic elements
 * communication.
 * The code here manage rf intereactions and pass messages to the dispatcher.
 */

#include "../rfudp/TxTunnel.h"

#include "dispatcher.h"

using namespace std;
//
// Hardware configuration
//

/****************** Raspberry Pi ***********************/

// Radio CE Pin, CSN Pin, SPI Speed
// See http://www.airspayce.com/mikem/bcm2835/group__constants.html#ga63c029bd6500167152db4e57736d0939 and the related enumerations for pin information.

// Setup for GPIO 15 CE and CE0 CSN with SPI Speed @ 8Mhz
//RF24 radio(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_32MHZ);
RF24 radio(RF_CE_PIN, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_32MHZ);


// Radio pipe addresses for the 2 nodes to communicate. From "TxTunnel.h"
//const uint8_t addresses[][5] = {"2RPi","0RPi"};

// loop condition
sig_atomic_t loop_on = TRUE;

void signal_handler(int sig) {
  loop_on = FALSE;
}

int main(int argc, char** argv) {


    char buffer[MAX_MSG_SZ+1];
    
    // SIGNAL handler
    struct sigaction sa;
    sa.sa_handler = &signal_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);


    /* RF setup */
    radio.begin();                      // Setup and configure rf radio
    radio.powerUp();
    #ifdef RF_CHANNEL
     #if RF_CHANNEL != 0
      radio.setChannel(RF_CHANNEL);     // If is definned and not 0, set channel
     #endif
    #endif
    radio.setPALevel(RF24_PA_MAX);      // Set Power Amplification to MAX
    radio.setDataRate(RF24_2MBPS);      // Set maximun datarate
    radio.setAutoAck(1);                // Ensure autoACK is enabled
    radio.enableAckPayload();               // Allow optional ack payloads
    radio.enableDynamicPayloads();
    radio.setRetries(2,10);                  // Optionally, increase the delay between retries & # of retries
    radio.setCRCLength(RF24_CRC_8);     // Use 8-bit CRC for performance

    radio.printDetails();

    /* GPIO setup */
    if(gpioInitialise()<0) {
        perror("[pigpio] gpioInitialise error");
        exit(EXIT_FAILURE);
    }
    // initialize all motors
    all_motors_zero();  // set PPM to 0
    sleep(1);           // wait 1 sec
    all_motors_stop();  // set PPM to stop


    // This opens two pipes for these two nodes to communicate
    // back and forth.
    if ( 0 == 1 )
    {
      radio.openWritingPipe(addresses[1]);
      radio.openReadingPipe(1,addresses[0]);
      radio.stopListening();
    } else {
      radio.openWritingPipe(addresses[0]);
      radio.openReadingPipe(1,addresses[1]);
      radio.startListening();
    }


    // 
    int n;
    uint8_t pipeNo;
    memset(buffer, 0, sizeof(buffer));

    printf("Starting listenning for messages\n");

    do{
        // GET DATA FROM CLIENT
        if(radio.available(&pipeNo)){
          n = radio.getDynamicPayloadSize();
          if(n < 1){
            // Corrupt payload has been flushed
            continue;
          }else
            radio.read(buffer, n);
        }else{
          continue;
        }
        buffer[n] = '\0';
        
        // Print info about msg
        eprintf("MSG: \'%s\'\n", buffer);


        /* START process Part */
        process_msg(buffer);
        n = strlen(buffer);

        /* END process Part */

        printf("ANS: \'%s\'\n", buffer);

        // Answer the client
        #if ACK_MODE        //ACK MODE
        radio.writeAckPayload(pipeNo, buffer, n );
        #else
        radio.stopListening();
        if ( radio.write(buffer,n) )
            printf("sending correct\n");
        else
            eprintf("error\n");
        radio.startListening();
        #endif

        //clean buffer
        //buffer[0]='\0';
        //memset(buffer, 0, sizeof(buffer));

    }while(loop_on);
    
    // Power down the antenna
    eprintf("Powering down the antenna");
    radio.powerDown();
    eprintf("Terminating PiGPIO");
    gpioTerminate();
    eprintf("\n");
}

