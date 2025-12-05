ARM_CROSS?=arm-ostl-linux-gnueabi-
CC=${ARM_CROSS}gcc
CXX?=${ARM_CROSS}g++
CPP?=${ARM_CROSS}g++
AR?=${ARM_CROSS}ar
LD?=${ARM_CROSS}ld
STRIP?=${ARM_CROSS}strip
#SYSROOT=/opt/stlinux_sdk_install/Developer-Package/SDK/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi

CFLAGS= -DEBUG
PREFIX=.
INCDIR=${PREFIX}/include
LIBFLAGS=-lpthread

SRC_C:=$(wildcard *.c)
SRC_CPP:=$(wildcard *.cpp)

OBJS+=$(patsubst %.cpp, %.o, $(SRC_CPP))
OBJS+=$(patsubst %.c, %.o, $(SRC_C))

path=$(abspath $(lastword ${MAKEFILE_LIST}))
dirname=$(dir ${path})

TARGET=$(shell basename ${dirname})
$(TARGET):$(OBJS)
	${CC} -o $@ $^ ${LIBFLAGS}

%.o:%.c
#       gcc -DEBUG -I../include -c -o $@ $< -MD -MF .$@.d
	${CC} -static  -c $< ${CFLAGS} -I ${INCDIR} -MD -MF $(patsubst %.o,.%.d, $@)

%.o:%.cpp
	${CPP} -static  -c $< ${CFLAGS} -I ${INCDIR} -MD -MF $(patsubst %.o,.%.d, $@)

.PHONY:clean all
clean:
	@rm -fr ${TARGET} *.o .*.d

