TOP_DIR := $(shell pwd)
APP = $(TOP_DIR)/jpegapp

CC = arm-linux-gcc
CFLAGS = -g 
LIBS = -ljpeg
#DEP_LIBS = -L /opt/FriendlyARM/toolschain/4.5.1/arm-none-linux-gnueabi/lib/
DEP_LIBS = -L /usr/local/jpeg8b/lib
#HEADER = -I/opt/FriendlyARM/toolschain/4.5.1/arm-none-linux-gnueabi/include/
HEADER = -I /usr/local/jpeg8b/include 
OBJS = main.o v4l2_use.o lcd.o image_process.o 

all:  $(OBJS)
	$(CC) -g -o $(APP) $(OBJS) $(LIBS) $(DEP_LIBS)  $(HEADER)

clean:
	rm -f *.o a.out $(APP) core *~
