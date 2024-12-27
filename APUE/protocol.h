#ifndef __NETRADIO_PROTOCOL_H
#define __NETRADIO_PROTOCOL_H

#include <stdint.h>

//[1]组播地址
#define MULTICAST_ADDR	"233.133.1.1"

//[2]本地地址                 在创建组播组时,需要本地IP地址写"0.0.0.0"的好处是将来换一台主机的时候也能使用 
#define LOCAL_IP        "0.0.0.0" 
//[3]网卡名称              通过ifconfig命令查看虚拟网卡的名字 
#define NETCARD_NAME    "ens33" 
//[4]接收端口
#define RECV_PORT_0		9520
#define RECV_PORT_1		9521
#define RECV_PORT_2		9522
#define RECV_PORT_3		9523
#define RECV_PORT_4		9524
//[5]最大支持频道个数
#define CHN_NR			200 
//[6]频道列表编号
#define CHN_LIST_ID		0
//[7]最小频道号
#define MIN_CHN_ID		1
//[8]最大频道号
#define MAX_CHN_ID		CHN_NR + MIN_CHN_ID - 1
//[9]定义一个数据包的大小
#define MSG_SIZE		740
//[10]频道描述字节数
#define DESCR_SIZE      10
//[11]频道个数
#define CHN_SIZE        5
typedef uint8_t chnid_t;//定义频道ID类型
typedef uint8_t len_t;//定义长度类型

/*
频道列表
0	频道列表编号
ID	频道描述
1	children
2	music
3	traffic
4	traditional
因为不知道要有多少个频道
所以先抽象单个频道的描述结构
再去抽象整个频道列表的结构
*/
//单个频道的描述结构
//因为不知道频道描述有多长所以使用可变长的结构体
struct list_entry
{
	chnid_t chnid;//表示频道ID
	//len_t len;//自述结构的长度[可以有可以没有]
	char descr[DESCR_SIZE];//表示频道描述
}__attribute__((packed));//跨主机要单字节对齐

//整个频道列表结构
//因为不知道有多少个频道所以使用可变长的结构体
struct chn_list_st
{
	chnid_t chnid;//表示频道列表编号(为0)
	struct list_entry entry[CHN_SIZE];//表示频道
}__attribute__((packed));//跨主机要单字节对齐

//频道数据结构
struct chn_data_st
{
	chnid_t chnid;//表示频道数据来源于哪个频道
	char msg[MSG_SIZE];//频道数据
}__attribute__((packed));//跨主机要单字节对齐
/*
因为通信双方发送端是主动的所以知道自己在发送什么结构数据
但是接收方是被动的所以不知道自己接收到当前这一包数据是什么类型的
[频道列表] [频道数据]
*/
union chn_recv_st
{
	chnid_t chnid;//区分当前这一包数据是频道列表( == 0)还是频道数据(> 0)
	struct chn_list_st list;//频道列表结构
	struct chn_data_st data;//频道数据结构
};

#endif


