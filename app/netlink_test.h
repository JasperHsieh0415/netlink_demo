#ifndef _NETLINK_TEST_H_
#define _NETLINK_TEST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <ifaddrs.h>
#include <linux/if_link.h>
#include "i2c-dev.h"
#include <pthread.h>
#include <liboled.h>
#include "json.h"
#include <camera.h>
#include "rwconf.h"
#include "ast_ipc.h"
#include <linux/netlink.h>
#include <sys/select.h>
#define NETLINK_TEST  30
#define NETLINK_USERINFO_NAME			"/sys/kernel/netlink_test/userinfo"

/*******************************************************************************
*	Structures
*******************************************************************************/
#define MAX_EVENT_BUF_SIZE				16

struct _nl_msg
{
    struct nlmsghdr hdr;
    int8_t  data[MAX_EVENT_BUF_SIZE];
};

#endif