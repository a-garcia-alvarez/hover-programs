#############################################################################
#
# Makefile for Raspberry Pi NRF24L01/NRF24L01+ hover controller
#
# Run:
#     make clean; make
#     sudo ./receiver
#############################################################################
prefix := /usr/local

# The recommended compiler flags for the Raspberry Pi
#CCFLAGS=-Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s
CPUFLAGS=-march=armv6zk -mtune=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard
CFLAGS=-march=armv6zk -mtune=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard -Ofast -Wall -pthread

RF_LIB=rf24
RF_LIB_DIR=/usr/local/lib
RF_HEADER_DIR=/usr/local/include/RF24

GPIO_LIB=pigpio

# define all programs
PROGRAMS =  hover-controller
SOURCES = ${PROGRAMS:=.cpp}

PARTS = motors servos sensors dispatcher
PARTS_O = ${PARTS:=.o}

SEN_PARTS = sensors/temperature.o


all: ${PROGRAMS} tests


#${PROGRAMS}: ${SOURCES}
#	$(CXX) $(CFLAGS) -I$(RF_HEADER_DIR) -L$(RF_LIB_DIR) -l$(RF_LIB) $@.cpp -o $@

hover-controller: $@ ${PARTS_O}
	$(CXX) $(CFLAGS) -I$(RF_HEADER_DIR) -L$(RF_LIB_DIR) -l$(RF_LIB) -l$(GPIO_LIB) -lrt $@.cpp \
	${SEN_PARTS} ${PARTS_O} -o $@

dispatcher.o: dispatcher.cpp
	$(CXX) $(CFLAGS) dispatcher.cpp -o $@ -c

servos.o: servos.cpp
	$(CXX) $(CFLAGS) servos.cpp -o $@ -c

motors.o: motors.cpp
	$(CXX) $(CFLAGS) motors.cpp -o $@ -c

sensors.o: sensors.cpp ${SEN_PARTS}
	$(CXX) $(CFLAGS) sensors.cpp -o $@ -c

sensors/%.o: sensors/%.cpp
	$(CXX) $(CFLAGS) $< -o $@ -c

tests: 


.PHONY: clean remake
clean:
	rm -rf $(PROGRAMS) $(PARTS_O) *.o sensors/*.o
remake: clean all
