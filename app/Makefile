VERSION = 1.0
LIBPATH =-L$(PREFIX_DIR)/lib
INCPATH =-I$(PREFIX_DIR)/include/
Dest_BIN = netlink_test
OBJS	= netlink_test.o
CFILES= netlink_test.c $(LIBPATH)

LD	= $(CROSS_COMPILE)ld
CC	= $(CROSS_COMPILE)gcc
INSTALL	= install

CFLAGS	?=-O2 -Wall $(INCPATH)
LDFLAGS	?=

ifdef INST
ifeq ("$(origin INST)", "command line")
INSTALL_DIR := $(INST)
endif
endif

EXTRA_CFLAGS += -DCAM_NUM=$(CAM_NUM)

all:
	@echo "$(LIBPATH)"
	@echo "$(INCPATH)"
	$(CC) $(CFLAGS) $(CFILES) $(EXTRA_CFLAGS)  -o $(Dest_BIN)

clean:
	@echo "CLEAN"
	@rm -f $(OBJS) $(Dest_BIN)