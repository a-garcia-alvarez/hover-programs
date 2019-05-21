#############################################################################
#
# Makefile for Raspberry Pi NRF24L01/NRF24L01+ udp tunnel server
#
# Run:
#     make clean; make
#     sudo ./TxTunnelSender
#############################################################################
prefix := /usr/local

# The recommended compiler flags for the Raspberry Pi
#CCFLAGS=-Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s
CPUFLAGS=-march=armv6zk -mtune=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard
CFLAGS=-march=armv6zk -mtune=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard -Ofast -Wall -pthread

RF_LIB=rf24
RF_LIB_DIR=/usr/local/lib
RF_HEADER_DIR=/usr/local/include/RF24

# define all programs
PROGRAMS =  TxTunnelSender
SOURCES = ${PROGRAMS:=.cpp}


all: ${PROGRAMS} tests


${PROGRAMS}: ${SOURCES}
	$(CXX) $(CFLAGS) -I$(RF_HEADER_DIR) -L$(RF_LIB_DIR) -l$(RF_LIB) $@.cpp -o $@
	./doSUID.sh $@


tests: client_test

client_test: client_test.cpp
	$(CXX) $(CFLAGS) $@.cpp -o $@


.PHONY: clean
clean:
	rm -rf $(PROGRAMS) *.o client_test
