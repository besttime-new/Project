#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "protocol.h"
#include "pool.h"
#include "media_lib.h"
#include <errno.h>


// 创建组播需要用的变量
int sd[5];                         // 存储网络套接字描述符
struct sockaddr_in remote_addr[5]; // 存储对端地址
struct ip_mreqn imr;               // 组播选项的结构体

// 解析音乐目录需要的变量
struct mlib_list_entry *mylib; // 保存获取到的频道列表数组
int chn_n;                     // 频道个数

// 线程池需要的变量
pool_t *mypool; // 存储线程池地址
task_t task;

// 任务函数声明
void *send_task(void *s);

//组播端口
int port[CHN_SIZE] = {RECV_PORT_0, RECV_PORT_1, RECV_PORT_2, RECV_PORT_3, RECV_PORT_4};

int main(void)
{
    // [1] 解析媒体库获取频道列表信息 (频道号，描述信息)
    if (mlib_get_chn_list(&mylib, &chn_n))
    {
        fprintf(stderr, "mlib_get_chn_list() failed!\n");
        return -1;
    }
    // [2] 创建线程池
    if (pool_init(&mypool, CHN_NR))// 最大线程数就是最大频道的个数
    { 
        fprintf(stderr, "pool_init() failed!\n");
        return -2;
    }

    for (int i = 0, j = 0; i < CHN_SIZE; i++)//分配给对应线程分配对应频道任务
    {
        task.job = send_task; // 需要做的任务
        task.arg = (void *)i; // 任务所需参数
        pool_add_task(mypool, &task); // 添加任务到线程池
    }

    while (1);//防止主线程结束
    return 0;
}

void *send_task(void *s)
{
    chnid_t chind = (chnid_t)s;//转换频道号
    struct chn_list_st list;//存储频道列表信息
    //struct chn_data_st data;//存储频道音乐信息
    char buf[MSG_SIZE];//存储频道音乐信息
    int count = 0;
    // [3] 创建报式套接字
    sd[chind] = socket(AF_INET, SOCK_DGRAM, 0); // 创建套接字
    if (sd[chind] == -1)
    {
        perror("socket()");
        return NULL;
    }
    // [4] 配置组播信息
    inet_aton(LOCAL_IP, &imr.imr_address);          // 转换本地地址 使用0.0.0.0任何主机都能当作服务端
    imr.imr_ifindex = if_nametoindex(NETCARD_NAME); // 转换网卡设备号
    inet_aton(MULTICAST_ADDR, &imr.imr_multiaddr);  // 转换组播地址
    if (setsockopt(sd[chind], IPPROTO_IP, IP_MULTICAST_IF, &imr, sizeof(imr)) == -1)//创建组播组
    {
        perror("setsockopt()");
        close(sd[chind]);
        return NULL;
    }
    // [5] 配置对端信息
    remote_addr[chind].sin_family = AF_INET;                 // 选择IPV4协议
    inet_aton(MULTICAST_ADDR, &remote_addr[chind].sin_addr); // 填入组播组地址
    remote_addr[chind].sin_port = htons(port[chind]);        // 填入端口号
    if (chind == 0) //频道0发送频道列表
    {
        for (int i = 0; i < chn_n; i++)//读取从媒体库获取的频道列表信息
        {
            list.entry[i].chnid = (mylib + i)->chnid;//获取频道号
            strcpy(list.entry[i].descr, (mylib + i)->descr);//获取频道描述
        }
        while (1)
        {
            sendto(sd[chind], &list, sizeof(list), 0, (struct sockaddr *)(remote_addr + 0), sizeof(remote_addr[0]));//发送列表信息
            sleep(1);
        }
    }
    else//其他频道发送对应频道音乐信息
    {
        while (1)
        {
            count = mlib_read_chn_data(chind, buf, MSG_SIZE); //从媒体库读取对应频道音乐信息
            sendto(sd[chind], buf, count, 0, (struct sockaddr *)(remote_addr + chind), sizeof(*remote_addr));//发送音乐数据
            usleep(18000);
        }
    }
}
