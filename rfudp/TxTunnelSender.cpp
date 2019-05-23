/*
TMRh20 2014

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/** General Data Transfer Rate Test
 * This example demonstrates basic data transfer functionality with the
 updated library. This example will display the transfer rates acheived using
 the slower form of high-speed transfer using blocking-writes.
 */

#include "TxTunnel.h"

using namespace std;
//
// Hardware configuration
//

/****************** Raspberry Pi ***********************/

// Radio CE Pin, CSN Pin, SPI Speed
// See http://www.airspayce.com/mikem/bcm2835/group__constants.html#ga63c029bd6500167152db4e57736d0939 and the related enumerations for pin information.

// Setup for GPIO 15 CE and CE0 CSN with SPI Speed @ 8Mhz
RF24 radio(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_32MHZ);


// Radio pipe addresses for the 2 nodes to communicate. From "TxTunnel.h"
//const uint8_t addresses[][5] = {"2RPi","0RPi"};

// loop condition
sig_atomic_t loop_on = TRUE;

void signal_handler(int sig){
  loop_on = FALSE;
}

int main(int argc, char** argv){

    // here we define a socket address to be used and a buffer to store data
    int sockfd;
    char buffer[MAX_MSG_SZ+1];
    // Here are defined both sockets structs from server and sender
    struct sockaddr_in servaddr, cliaddr;
    
    // SIGNAL handler
    struct sigaction sa;
    sa.sa_handler = &signal_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);


    /* UDP LISTENER setup */

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        //eprintf("socket creation failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // set both struct to 0s before initialization
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family      = AF_INET;     // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;  // Listen on any interface
    servaddr.sin_port        = htons(SERVER_PORT); // Port

    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        //eprintf("bind failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }


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
    radio.enableDynamicPayloads();
    radio.setRetries(2,10);                  // Optionally, increase the delay between retries & # of retries
    radio.setRetries(15,15);
    radio.setCRCLength(RF24_CRC_8);     // Use 8-bit CRC for performance


    /* Print setup details*/
    printf("================ UDP Configuration ================\n");
    printf("SERVER IP     = %s\n", inet_ntoa(servaddr.sin_addr));
    printf("SERVER PORT     = %i\n", ntohs(servaddr.sin_port));
    printf("MSG MAX SIZE     = %i\n", MAX_MSG_SZ);
    printf("ACK MODE     = %s\n", ACK_MODE? "true":"false");
    radio.printDetails();
    printf("\n");



    // This opens two pipes for these two nodes to communicate
    // back and forth.
    if ( 0 == 0 )
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
    #if !ACK_MODE
    unsigned long loop_start;
    #endif
    unsigned int cliaddr_len = sizeof(cliaddr);
    memset(buffer, 0, sizeof(buffer));

    printf("Starting listenning for messages\n");

    do{
        // GET DATA FROM CLIENT
        n = recvfrom(
            sockfd, (char *) buffer, MAX_MSG_SZ, MSG_WAITALL,
                ( struct sockaddr *) &cliaddr, &cliaddr_len
            );
        if(n == -1){ // Test for error
            //fprintf(stderr, "some error ocurred: %s\n", strerror(errno));
            perror("[UDP Listen] some error ocurred");
            continue;
        }
        buffer[n] = '\0';
        
        // Print info about client
        printf("Client addrr: %s, port: %i\n",
            inet_ntoa(cliaddr.sin_addr),    // addrress
            ntohs(cliaddr.sin_port)         // port
        );
        if ( n ==0 ) {
            printf("empty msg, discarded\n");
            continue;
        }
        printf("MSG: \'%s\'\n", buffer);


        /* START RF Part */
      #if ACK_MODE
        if ( radio.write(buffer,n) ){
            //usleep(10000);
            if(!radio.available()){
                // If nothing in the buffer, we got an ack but it is blank
                strcpy(buffer,"ACK");
            }else{
                n = radio.getDynamicPayloadSize();
                radio.read(buffer, MAX_MSG_SZ);
            }
        }else{
            // If no ack response, sending failed
            eprintf("Sending failed.\n\r");
            strcpy(buffer,"TxERR\0");
            n=strlen(buffer);
        }
      #else
        radio.write(buffer,n);

        loop_start = millis();
        radio.startListening();
        while ( !radio.available() && (millis() - loop_start) < 100) {
            // wait till receive or timeout
        }
        if (millis() - loop_start >= 100) {
            eprintf("Not Response.\n\r");
            strcpy(buffer,"TxERR\0");
            n=strlen(buffer);
        } else {
          n = radio.getDynamicPayloadSize();
          radio.read(buffer, MAX_MSG_SZ);
        }
        radio.stopListening();
      #endif
        buffer[n] = '\0';
        /* END RF Part */

        printf("ANS: \'%s\'\n", buffer);

        // Answer the client
        sendto(
            sockfd, (char *) buffer, strlen(buffer), MSG_CONFIRM,
             (const struct sockaddr *) &cliaddr, cliaddr_len
        );

        //clean buffer
        //buffer[0]='\0';
        //memset(buffer, 0, sizeof(buffer));

    }while(loop_on);
    
    // Power down the antenna
    eprintf("Powering down the antenna");
    radio.powerDown();
    eprintf("\n");
    // Close the socket
    eprintf("Closing scockets");
    close(sockfd);
    eprintf("\n");
}

