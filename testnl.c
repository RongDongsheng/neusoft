/*this prog used to understand HOWTO use netlink to 
 *communicate between kernel mode & user mode
 *Author: Ron
 *File Name: testnl.c
 *Date:2013-6-13
 */

#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/netlink.h>
#include<linux/skbuff.h>

#include<net/sock.h>
#include<asm/types.h>

#define NETLINK_XUX 21 
#define VFW_GROUP    1
#define MSG_SIZE NLMSG_SPACE(2048)

static struct sock *xux_sock;

struct netlink_data
{
  struct nlmsghdr msg;
  char data[1024];
};

static void test_link(struct sk_buff *skb)
{
  struct nlmsghdr *nlh;
  u32 rlen;
  void *data;

  while(skb->len >= NLMSG_SPACE(0))//skb_buff中有数据
    {
      nlh = nlmsg_hdr(skb);
      if(nlh->nlmsg_len < sizeof(*nlh) || skb->len < nlh->nlmsg_len)
	return;
      rlen = NLMSG_ALIGN(nlh->nlmsg_len);
      if(rlen > skb->len)
	rlen=skb->len;
      data = NLMSG_DATA(nlh);
      printk("link: %s\n", (char *)data);
      netlink_ack(skb, nlh, 0);
      skb_pull(skb, rlen);
    }
}

int __init init_link(void)
{
  xux_sock = netlink_kernel_create(&init_net, NETLINK_XUX, 0, 
				  test_link, NULL, THIS_MODULE);
  if(!xux_sock)
    {
      printk("cannot initialize netlink socket!\n");
      return -1;
    }
  else
    {
      xux_sock->sk_sndtimeo = MAX_SCHEDULE_TIMEOUT;
      printk("init ok!\n");
      return 0;
    }
}

void __exit exit_link(void)
{
  netlink_kernel_release(xux_sock);
  printk("release ok!\n");
  return;
}





module_init(init_link);
module_exit(exit_link);

MODULE_AUTHOR("Ron");
MODULE_LICENSE("GPL");
