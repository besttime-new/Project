#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include "protocol.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
// 映射地址大小
#define MAP_SIZE 1024

static void kill_child(pid_t pid1, pid_t pid2)
{
    kill(pid1, SIGTERM);
    kill(pid2, SIGTERM);
}

int main(void)
{
    char buffer[MSG_SIZE];
    union chn_recv_st recv; // 使用共用体接收判断信息
    void *ptr = NULL;       // 存储映射存储空间的地址
    int sd;                 // 存储socket套接字描述符
    // struct data_st buf;         // 存储发送的内容
    struct sockaddr_in my_addr; // 存储本地地址
    struct sockaddr_in remote_addr; // 存储远程地址
    struct ip_mreqn imr;        // 组播选项的结构
    ssize_t count = 0;          // 存储接收到数据的字节数
    int pfd[2];                 // 存储pipe创建之后的读端和写端的文件描述符
    pid_t pid[2];               // 存储进程号
    int ret;                    // 存储返回值
    // 存储返回值
    char buf[MSG_SIZE] = {0};                                                               // 存储接收的数据
    int port[CHN_SIZE] = {RECV_PORT_0, RECV_PORT_1, RECV_PORT_2, RECV_PORT_3, RECV_PORT_4}; // 组播端口
    int chn_cur = 0;                                                                        // 存储当前频道
    int chn_new = 1;                                                                        // 切换后的频道
    int fd;                                                                                 // 存储空洞文件描述符
    int i = 0;
ret_make:                // 循环变量
    if (pipe(pfd) == -1) // 判断创建匿名管道是否失败
    {
        perror("pipe()"); // 打印错误信息
        ret = -1;         // 给返回值赋值-1
        goto ERR_1;       // 跳转到ERR_1的标识符
    }

    ptr = mmap(NULL, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); // 映射共享内存
    if (ptr == MAP_FAILED)                                                                 // 判断映射是否失败
    {
        perror("mmap()"); // 打印错误信息
        ret = -2;         // 给返回值赋值-2
        goto ERR_2;       // 跳转到ERR_2的标识符
    }

    for (i = 0; i < 2; i++) // 创建两个子进程
    {
        pid[i] = fork(); // 创建子进程并且父进程默认等待子进程结束
        if (pid[i] < 0)
        {
            perror("fork()");
            ret = -3;
            goto ERR_3; // 跳转到ERR_3的标识符
        }
        if (pid[i] == 0) // 让子进程退出循环
            break;
    }
    if (pid[0] == 0) // 子进程1
    {
        close(pfd[1]);                  // 子进程关闭写端的文件描述符
        munmap(ptr, MAP_SIZE);          // 解除映射关系
        fd = open("/dev/null", O_RDWR); // 打开空洞文件
        if (fd < 0)                     // 判断打开文件是否失败
        {
            perror("open()");
            exit(1);
        }
        dup2(pfd[0], 0); // 将读端的文件描述符复制到标准输入
        dup2(fd, 1);     // 将标准输出重定向到空洞文件
        dup2(fd, 2);
        execl("/usr/bin/mplayer", "mplayer", "-cache", "8888","-cache-min", "2","-", NULL); // 将子进程1替换为mplayer
        close(pfd[0]);                                                   // 子进程关闭读端的文件描述符
        exit(0);                                                         // 终止子进程
    }
    if (pid[1] == 0) // 子进程2
    {
        close(pfd[0]); // 父进程关闭读端的文件描述符
        while (1)
        {
            memcpy(&chn_cur, ptr, sizeof(chn_cur)); //
            if (chn_cur)                            // 判断是不是零号判断
            {
                sd = socket(AF_INET, SOCK_DGRAM, 0); // 创建报式套接字
                if (sd == -1)                        // 判断创建报式套接字是否失败
                {
                    perror("socket()"); // 打印错误信息
                    exit(1);
                }
                //[2]本地地址与套接字绑定
                my_addr.sin_family = AF_INET;            // 指定IPV4协议
                inet_aton(LOCAL_IP, &my_addr.sin_addr);  // 转换本地IP
                my_addr.sin_port = htons(port[chn_cur]); // 填入接收端端口
                if (bind(sd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
                {
                    perror("bind()"); // 打印错误信息
                    continue;
                    ;
                }
                //[3]加入到组播组中
                inet_aton(MULTICAST_ADDR, &imr.imr_multiaddr); // 转换组播地址
                inet_aton(LOCAL_IP, &imr.imr_address);         // 转换本地地址
                imr.imr_ifindex = if_nametoindex(NETCARD_NAME);
                if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &imr, sizeof(imr)) == -1) // 加入组播
                {
                    perror("setsockopt()"); // 打印错误信息
                    close(sd);
                    continue;
                }
                // 接受频道信息，并且写入管道
                while (1)
                {
                    memset(buf, 0, MSG_SIZE);                           // 清空buf
                    count = recvfrom(sd, buf, MSG_SIZE, 0, NULL, NULL); // 接收频道音乐信息
                    write(pfd[1], buf, count);                          // 写入管道发送给子进程1
                    memcpy(&chn_new, ptr, sizeof(chn_new));             // 获取新的频道号
                    if (chn_cur != chn_new)                             // 判断频道号是不是改变了
                    {
                        close(sd);
                        break;
                    }
                }
            }
        }
        close(pfd[1]); // 子进程关闭读端的文件描述符
        exit(0);
    }
    if (chn_cur == 0)
    {
        //[1]创建套接字
        sd = socket(AF_INET, SOCK_DGRAM, 0); // 创建报式套接字
        if (sd == -1)                        // 判断创建报式套接字是否失败
        {
            perror("socket()"); // 打印错误信息
            ret = -4;
            goto ERR_4;
        }

        //[2]本地地址与套接字绑定
        my_addr.sin_family = AF_INET;            // 指定IPV4协议
        inet_aton(LOCAL_IP, &my_addr.sin_addr);  // 转换本地IP
        my_addr.sin_port = htons(port[chn_cur]); // 填入接收端端口
        if (bind(sd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
        {
            perror("bind()"); // 打印错误信息
            ret = -5;         // 关闭套接字
            goto ERR_4;
        }
        //[3]加入到组播组中
        inet_aton(MULTICAST_ADDR, &imr.imr_multiaddr);                              // 转换组播地址
        inet_aton(LOCAL_IP, &imr.imr_address);                                      // 转换本地地址
        imr.imr_ifindex = if_nametoindex(NETCARD_NAME);                             // 通过网卡名称转换成网卡设备号
        if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &imr, sizeof(imr)) == -1) // 加入组播
        {
            perror("setsockopt()"); // 打印错误信息
            ret = -6;               // 关闭套接字文件
            goto ERR_4;             // 由于加入失败,结束程序,并且返回-3
        }
        count = recvfrom(sd, &recv.list, sizeof(recv.list), 0,  NULL, 0); // 接受一份表单信息存储起来
        close(sd);                                                          // 父进程无需使用套接字，所以关闭套接字
    } // 父进程关闭读端的文件描述符
    close(pfd[1]); // 父进程关闭写端的文件描述符
    while (1)
    {
        memcpy(ptr, &chn_cur, sizeof(chn_cur)); // 将频道号存储到共享内存中
        // 显示频道列表信息
        printf("频道号        描述\n");
        for (int i = 0; i < CHN_SIZE - 1; i++)
        {
            printf("%d\t\t", recv.list.entry[i].chnid);
            printf("%s\n", recv.list.entry[i].descr);
        }
        printf("选择需要收听的频道(1-4):");
        scanf("%d", &chn_cur);
        if (chn_cur < 1 || chn_cur > 4)
        {
            printf("输入错误，请重新输入\n");
            continue;
        }
        kill_child(pid[0], pid[1]);
        waitpid(pid[0], NULL, 0);
        waitpid(pid[1], NULL, 0);
        while (read(pfd[0], buffer, MSG_SIZE) > 0) ;
        close(pfd[0]); // 关闭读端
        goto ret_make;
    }
    //[5]关闭套接字文件
    wait(NULL); // 等待子进程结束(收尸)
    wait(NULL); // 等待子进程结束(收尸)

ERR_4:
    close(sd); // 关闭套接字文件
ERR_3:
    munmap(ptr, MAP_SIZE); // 解除映射关系
ERR_2:
    close(pfd[0]); // 父进程关闭读端的文件描述符
    close(pfd[1]); // 父进程关闭写端的文件描述符
ERR_1:
    return ret;
}
