CROSS := ${CROSS_COMPILE}

ifeq ($(obj),)
obj = .
endif

#################################################
# configuration

ifeq ($(FORCE_A1_BOARD),1)
  EXTRA_CFLAGS += -DFORCE_A1_BOARD
endif

MDIR		:= ast_netlink

# drivers objects
list-multi	:= ast_netlink.o

# what to build
obj-m		:= ast_netlink.o

tardest		:= .

#################################################
# compile modules
KDIR := $(OUTPUT_CFG_DIR)/kernel
PWD		:= $(shell pwd)

# which files to install?
inst-m		:= $(wildcard *.ko)
ifeq ($(inst-m),)
  inst-m	:= $(obj-m)
endif

all:
	$(MAKE) ARCH=arm -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
	-rm -f .version