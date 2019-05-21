#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "TxTunnel.h"

// loop condition
sig_atomic_t loop_on = TRUE;

void signal_handler(int sig){
  loop_on = FALSE;
}

int main(){

  int sockfd, n;
  unsigned int len;
  char buffer[MAX_MSG_SZ];
  struct sockaddr_in servaddr, direccion;

    // SIGNAL handler
    struct sigaction sa;
    sa.sa_handler = &signal_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
    perror("fallo creacion del socket");
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  memset(&direccion, 0, sizeof(direccion));

  direccion.sin_family = AF_INET;
  direccion.sin_port = htons(SERVER_PORT);
  direccion.sin_addr.s_addr = inet_addr(SERVER_IP);

do{
  printf("Introduce el mssg: ");
  fgets(buffer, MAX_MSG_SZ, stdin);

  sendto(
    sockfd, (const char *) buffer, strlen(buffer), MSG_CONFIRM,
    (const struct sockaddr *)&direccion, sizeof(direccion)
  );

  len = sizeof(servaddr);
  n = recvfrom(
    sockfd, (char *) buffer, MAX_MSG_SZ, MSG_WAITALL,
    (struct sockaddr *) &servaddr, &len
  );

  buffer[n] = '\0';
  printf("ANS: \'%s\'\n", buffer);
}while(loop_on);
}
