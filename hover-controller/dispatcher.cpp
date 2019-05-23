#include <string.h>

#include "dispatcher.h"


void process_msg(char*msg) {

    char msg_cmd[3+1];

    strncpy(msg_cmd, msg, 3);
    msg_cmd[3]='\0';


    if (strcmp(msg_cmd, "ESC") == 0) {
        
    } else
    if (strcmp(msg_cmd, "SER") == 0) {
        servo_process(msg);
    } else
    if (strcmp(msg_cmd, "SEN") == 0) {
        
    } else {
        eprintf("CMD not valid");
        strcpy(msg, "CMD_NOK");
    }
}
