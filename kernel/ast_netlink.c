#include <linux/list.h>
#include <linux/timer.h>
#include <linux/ioctl.h> 
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <asm/io.h>

#include <asm/uaccess.h>
#include <linux/delay.h>

#include <linux/interrupt.h>
#include <mach/irqs.h>
#include <mach/gpio.h>
#include <linux/platform_device.h>
//#include <mach/platform.h>
#include <linux/semaphore.h>
#include <linux/spinlock.h>
#include <linux/gpio.h>
#include <linux/irq.h>
#include <linux/miscdevice.h>
//for netlink use
#include <linux/netlink.h>
#include <net/netlink.h>
#include <net/net_namespace.h> 
#define NETLINK_DEVICE_NAME "netlink_test"
#define NETLINK_TEST_NUM  30

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("NETLINK DEMO");
struct sock 	*nl_sock;
static pid_t	user_pid;
static struct kobject	*this_kobj;
static struct platform_device *netlink_test_device;

static ssize_t show_netlink_userinfo(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", user_pid);
}

static ssize_t store_netlink_userinfo(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	u32 val;
	val = simple_strtoul(buf, NULL, 0);//10 base
	user_pid=val;
	return count;
}

static DEVICE_ATTR(userinfo, S_IRUGO | S_IWUSR, show_netlink_userinfo, store_netlink_userinfo);

static struct attribute *netlink_test_attributes[] = {
	&dev_attr_userinfo.attr,
	NULL
};

static const struct attribute_group netlink_attribute_group = {
	.attrs = netlink_test_attributes
};

static void recv_cbk(struct sk_buff *skb)
{
	struct nlmsghdr *nlhdr = NULL;
	void *puserdata = NULL;

	printk("skb->len:%u\n", skb->len);
	if(skb->len >= nlmsg_total_size(0))
	{
		nlhdr = nlmsg_hdr(skb);
		puserdata = NLMSG_DATA(nlhdr);
		if(puserdata)
		{
			printk("kernel receive data: %s\n", (int8_t *)puserdata);
			//send_msg(data, nlmsg_len(nlh));
		}
	}
}

int send_to_user(unsigned int data)
{
	struct sk_buff *nl_skb;
	struct nlmsghdr *nlhdr;
	unsigned int *ptr;
	uint16_t len = 0;
	int ret;
	printk("kernel send %u\n", data);
	if(!user_pid)
	{
		return -1;
	}
	len = 8;
	nl_skb = nlmsg_new(len, GFP_ATOMIC);
	if(!nl_skb)
	{
		printk("netlink_alloc_skb error\n");
		return -1;
	}

	nlhdr = nlmsg_put(nl_skb, 0, 0, NLMSG_DONE, len, 0);
	if(nlhdr == NULL)
	{
		printk("nlmsg_put() error\n");
		nlmsg_free(nl_skb);
		return -1;
	}
	NETLINK_CB(nl_skb).dst_group = 0; /* not in mcast group */
	ptr = (unsigned int *)nlmsg_data(nlhdr);
	*ptr = data;

	ret = netlink_unicast(nl_sock, nl_skb, user_pid, MSG_DONTWAIT);

	return ret;
}

static int start_netlink(void)
{
	int i = 0;
	struct netlink_kernel_cfg cfg =
	{
		.input = recv_cbk,
	};
	printk("%s(%d)\n",__FUNCTION__,__LINE__);
	nl_sock = NULL;
	nl_sock = netlink_kernel_create(&init_net, NETLINK_TEST_NUM, &cfg);
	if (!nl_sock)
	{
		printk("Error creating nl_sock.\n");
		return -1;
	}

	for (i = 0; i < 100; i++)
	{
		msleep(1000);
		send_to_user(i);
	}
	
	return 0;
}

static int __init netlink_test_init(void) 
{ 
	int ret=0;
	printk("%s(%d)\n",__FUNCTION__,__LINE__);
	do
	{
		user_pid=0;
		this_kobj = kobject_create_and_add(NETLINK_DEVICE_NAME, kernel_kobj);
		if (!this_kobj)
		{
			return -ENOMEM;
		}
		ret = sysfs_create_group(this_kobj, &netlink_attribute_group);
		if (ret) 
		{
			kobject_put(this_kobj);
		}

		netlink_test_device = platform_device_register_simple(NETLINK_DEVICE_NAME, 0, NULL, 0);
		start_netlink();
		printk("%s(%d)\n",__FUNCTION__,__LINE__);
	}while(0);
	return ret;
}
static void __exit netlink_test_exit(void)
{
	printk("%s\n",__FUNCTION__);
	netlink_kernel_release(nl_sock);
	kobject_put(this_kobj);
}

//----------------------------------------------
module_init(netlink_test_init); 
module_exit(netlink_test_exit); 

