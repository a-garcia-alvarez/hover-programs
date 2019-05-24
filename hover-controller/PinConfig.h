#ifndef __PIN_CONFIG_H__
#define __PIN_CONFIG_H__


// RF pins
#define RF_CE_PIN   RPI_V2_GPIO_P1_22

// Servo pins & config
#define SERVO_MAX   2400 //2300
#define SERVO_MIN   500

#define LEFT_RUDDER_GPIO    23
#define LEFT_RUDDER_MAX     SERVO_MAX
#define LEFT_RUDDER_MIN     SERVO_MIN

#define RIGHT_RUDDER_GPIO   24
#define RIGHT_RUDDER_MAX    SERVO_MAX
#define RIGHT_RUDDER_MIN    SERVO_MIN

// ESC pins & config
#define ESC_MAX     2020
#define ESC_MIN     1000
#define ESC_REVERSE 

#define RIGHT_PROP_GPIO     RPI_V2_GPIO_P1_11
#define LEFT_PROP_GPIO      RPI_V2_GPIO_P1_15

#define MAIN_PROP_GPIO      RPI_V2_GPIO_P1_13

#define MAIN_PROP_START_PPM 1060
#define MAIN_PROP_STOP_PPM  2400


#endif // __PIN_CONFIG_H__
