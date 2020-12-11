#include "netlink_test.h"

struct sockaddr_nl  local_nl, dest_nl;
int nl_skfd;
int dest_nl_len = sizeof(struct sockaddr_nl);


int initial_netlink()
{
	int ret = 0;

	nl_skfd = socket(PF_NETLINK, SOCK_RAW, NETLINK_TEST);
	if(nl_skfd == -1)
	{
		printf("create netlink socket error...%s(%d)\n", strerror(errno),NETLINK_TEST);
		ret = -1;
		return ret;
	}

	//bind socket
	memset(&local_nl, 0, sizeof(local_nl));
	local_nl.nl_family = AF_NETLINK;
	local_nl.nl_pid = getpid();
	local_nl.nl_groups = 0;

	if(bind(nl_skfd, (struct sockaddr *)&local_nl, sizeof(local_nl)) != 0)
	{
		printf("bind() error\n");
		close(nl_skfd);
		ret = -1;
		return ret;
	}
	return ret;
}

void close_netlink()
{
	close(nl_skfd);
}

int main(int argc, char *argv[])
{
	int ret=0;
	int nl_test_fd=0;
	unsigned long userpid;
	char userinfobuf[16];
	fd_set rfds;
	struct timeval tv;
	int retval;
	struct _nl_msg irqinfo;
	unsigned int event_num = 0;
	unsigned char *ptr;

	daemon(1,1);

	nl_test_fd = open(NETLINK_USERINFO_NAME, O_RDWR);
	if (nl_test_fd == -1) {
		printf("Open nl_test_fd error!\n");
		return -1;
	}
	else {
		userpid = getpid();
		sprintf(userinfobuf,"%ld",userpid);
		write(nl_test_fd,userinfobuf,strlen(userinfobuf));
		close(nl_test_fd);
	}

	initial_netlink();

	while(1) {
		/* Watch NETLINK_TEST to see when it has input. */
		FD_ZERO(&rfds);
		FD_SET(nl_skfd, &rfds);

		tv.tv_sec = 0;
		tv.tv_usec = 1;
		retval = select(nl_skfd+1, &rfds, NULL, NULL,NULL);

		if (retval == -1)
		{
			perror("select()");
		}
		else if (retval)
		{
		   if(FD_ISSET(nl_skfd, &rfds))
		   {
				ret = recvfrom(nl_skfd, &irqinfo, sizeof(struct _nl_msg), 0, (struct sockaddr *)&dest_nl, (socklen_t*)&dest_nl_len);
				if(ret)
				{
					ptr = (void *)irqinfo.data;
					event_num = *((unsigned int *)ptr);
					printf("netlink_test: recv:%u\n", event_num);
				}
			}
		}
	}

}