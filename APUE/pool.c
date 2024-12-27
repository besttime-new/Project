#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include "pool.h"

static void *admin_job(void *arg);//管理者线程要做的事情
static void *worker_job(void *arg);//工作线程要做的事情

int pool_init(pool_t **mypool, int capacity)
{
    pool_t *me = NULL;//存储开辟的线程池地址
    int err;//存储返回值
    int i = 0;//循环变量

    me = malloc(sizeof(pool_t));//开辟线程池空间
    if(me == NULL)//判断开辟线程池空间是否失败
        return -errno;//由于开辟线程池空间失败,结束函数,并且返回-错误码
    /*将来调用者在调用的时候
    int ret = pool_init(); if(ret < 0) strerror(-ret);
    调用者可以直接打印错误信息了
    */

    //动态开辟工作线程结构
    me->workers = calloc(capacity, sizeof(pthread_t));
    if(me->workers == NULL)//判断动态开辟工作线程结构是否失败
    {
        free(me);//释放线程池结构
        return -errno;//由于动态开辟工作线程结构失败,结束函数,返回-错误码
    }
    //任务队列初始化
    queue_init(&me->task_queue, MAXJOB, sizeof(task_t));
    me->max_threads = capacity;//存储客户指定的最多线程并发数
    me->min_free_threads = MIN_FREE_NR;//指定最小空闲线程个数
    me->max_free_threads = MAX_FREE_NR;//指定最大空闲线程个数
    me->busy_threads = 0;//指定busy线程数为0
    me->live_threads = me->min_free_threads;//指定live线程为最小空闲线程数
    me->exit_threads = 0;//指定需要终止的线程数为0
    me->shutdown = 0;//指定线程池是否工作(0表示正常工作)

    //互斥量以及条件变量初始化
    pthread_mutex_init(&me->mut_pool, NULL);//初始化整个线程的互斥量
    pthread_mutex_init(&me->mut_busy, NULL);//初始化busy线程的互斥量
    pthread_cond_init(&me->queue_not_empty, NULL);//初始化任务队列不为空的条件变量
    pthread_cond_init(&me->queue_not_full, NULL);//初始化任务队列不为满的条件变量

    //需要构建管理者线程,填入标识符
    err = pthread_create(&me->admin_tid, NULL, admin_job, me);//创建管理者线程
    if(err != 0)//判断创建管理者线程是否失败
    {
        free(me->workers);//释放工作线程结构
        free(me);//释放线程池
        return -err;//由于创建管理者线程失败,结束程序,并且返回-错误码
    }

    //创建线程池中最小空闲的线程
    for(i = 0; i < me->min_free_threads; i++)
    {
        err = pthread_create(me->workers + i, NULL, worker_job, me);//创建空闲线程
        if(err != 0)//判断创建空闲线程是否失败
        {
            free(me->workers);//释放工作线程结构
            free(me);//释放线程池
            return -err;//由于创建管理者线程失败,结束程序,并且返回-错误码
        }
        //线程进行分离(将来可以不用收尸了)
        pthread_detach((me->workers)[i]);
    }

    *mypool = me;//把开辟成功的线程池地址,回填

    return 0;
}   

//找到工作线程数组中空闲的位置
static int __get_free_pos(pthread_t *jobs, int n)
{
    int i = 0;//循环变量

    for(i = 0; i < n; i++)
    {//检测线程是否存在
        if(pthread_kill(jobs[i], 0) == 0)//判断当前的线程是否存在
            return i;//如果存在返回当前位置的下标
    }
    return -1;//如果没有找到,返回-1
}


static void *admin_job(void *arg)//管理者线程要做的事情
{
    /*周期性(1s)查看线程池中busy的线程个数和free的线程个数,适当的增减线程数量
        while(1)
        {
            抢整个线程池的互斥量
            获取busy数和free数
            free > MAX_FREE_NR 减少2个
            free == 0 增加2个
            sleep(1);
        }
        问题 : 如何减少线程?
            方式1 : 取消线程
            方式2 : 自杀机制
                exit_threads 增加数值,发送queue_not_empty通知
    */
    pool_t *mypool = arg;//把形参转换成线程池类型
    int busy_cnt = 0;//用来存储获取的busy线程个数
    int free_cnt = 0;//用来存储获取的free线程个数
    int i = 0;//循环变量
    int pos = 0;//标记位置

    while(1)//使用轮询法
    {
        pthread_mutex_lock(&mypool->mut_pool);//抢整个线程池的互斥锁
        if(mypool->shutdown)//判断线程池是否为关闭状态
        {
            pthread_mutex_unlock(&mypool->mut_pool);//解整个线程池的互斥锁
            break;//跳出循环
        }
        pthread_mutex_lock(&mypool->mut_busy);//抢busy线程的互斥锁
        busy_cnt = mypool->busy_threads;//获取有多少个busy的线程
        pthread_mutex_unlock(&mypool->mut_busy);//解busy线程的互斥锁
        free_cnt = mypool->live_threads - busy_cnt;//获取有多少个free的线程

        //如果空闲的线程数大于最大空闲线程 + 预置线程 需要较少线程
        if(free_cnt >= mypool->max_free_threads + STEP)//判断是否需要减少线程
        {
            mypool->exit_threads = STEP;//每次减少STEP个线程
            for(i = 0; i < STEP; i++)//发送STEP次通知
            {
                pthread_cond_signal(&mypool->queue_not_empty);//发送工作队列不为空的通知
                //主要是为了唤醒线程
            }
        }

        //如果现在busy线程和live线程一样多,而且没有达到线程的上限->增加线程
        if(busy_cnt == mypool->live_threads && \
            mypool->live_threads < mypool->max_threads)//判断是否需要增加线程
        {
            for(i = 0; i < STEP; i++)
            {//需要把新创建的线程放到工作线程的合适位置,所以需要查找合适的位置
                pos = __get_free_pos(mypool->workers, mypool->max_threads);//查找空闲位置
                if(pos == -1)//判断是否没有空闲位置
                {
                    fprintf(stderr, "[%d] __get_free_pos() failed\n", __LINE__);
                    //打印出错函数信息
                }
                pthread_create(mypool->workers + pos, NULL, worker_job, mypool);//创建线程
                pthread_detach((mypool->workers)[pos]);//线程分离(将来不用收尸)
                mypool->live_threads++;//创建成功后增加live线程数
            }
        }
        //测试语句-->打印线程中线程的个数
        //打印有多少个busy线程
        for(i = 0; i < mypool->busy_threads; i++)
            write(1, "O", 1);
        //打印有多少个free线程
        for(i = 0; i < mypool->live_threads - mypool->busy_threads; i++)
            write(1, "X", 1);
        write(1, "\n", 1);

        pthread_mutex_unlock(&mypool->mut_pool);//解整个线程池的互斥锁
        sleep(1);//管理者线程1s看一次线程池
    }
    pthread_exit(0);//终止线程
}

static void *worker_job(void *arg)//工作线程要做的事情
{
    /*
        循环
            抢锁--->判断任务队列是否为空
            空
                解锁等待queue_not_empty通知
                通知来了
                    情况1:线程池关闭了
                    情况2:exit_threads > 0 终止
                    情况3:任务队列真的来任务了,出队,busy个数增加,做任务
                    任务做完了,循环
    */
    pool_t *mypool = arg;//把形参转换为线程池类型
    task_t mytask;//定义任务结构变量,存储出队的任务

    while(1)
    {
        pthread_mutex_lock(&mypool->mut_pool);//抢整个线程池的互斥锁
        if(queue_is_empty(mypool->task_queue))//判断任务队列是否为空
        {
            pthread_cond_wait(&mypool->queue_not_empty, &mypool->mut_pool);
            //原子操作,等待队列不为空的条件并且解整个线程池的互斥锁
        }
        if(mypool->shutdown)//判断线程池是否已关闭
        {
            pthread_mutex_unlock(&mypool->mut_pool);//解整个线程池的互斥锁
            break;//跳出循环
        }
        if(mypool->exit_threads > 0)//判断是否有线程需要终止
        {
            mypool->exit_threads--;//较少需要终止线程的数量
            pthread_mutex_unlock(&mypool->mut_pool);//解整个线程池的互斥锁
            break;//跳出循环
        }
        //任务来了
        queue_de(mypool->task_queue, &mytask);//出队任务
        pthread_cond_signal(&mypool->queue_not_full);//发送队列不为满的通知
        pthread_mutex_unlock(&mypool->mut_pool);//解整个线程池的互斥锁
        //做任务(free线程要变为busy线程)
        pthread_mutex_lock(&mypool->mut_busy);//抢busy线程的互斥锁
        mypool->busy_threads++;//增加busy线程的数量
        pthread_mutex_unlock(&mypool->mut_busy);//解busy线程的互斥锁

        (mytask.job)(mytask.arg);//执行任务

        pthread_mutex_lock(&mypool->mut_busy);//抢busy线程的互斥锁
        mypool->busy_threads--;//减少busy线程的数量
        pthread_mutex_unlock(&mypool->mut_busy);//解busy线程的互斥锁
    }
    pthread_exit(0);//线程终止
}

int pool_add_task(pool_t *mypool, task_t *t)
{
    pthread_mutex_lock(&mypool->mut_pool);//抢整个线程池的互斥锁
    while(queue_is_full(mypool->task_queue))//判断任务队列是否已满
    {
        pthread_cond_wait(&mypool->queue_not_full, &mypool->mut_pool);
        //原子操作,等待队列不为满的条件并且解整个线程池的互斥锁
    }
    queue_en(mypool->task_queue, t);//任务入队
    pthread_cond_signal(&mypool->queue_not_empty);//发送任务队列不为空的通知
    pthread_mutex_unlock(&mypool->mut_pool);//解整个线程池的互斥锁

    return 0;
}

void pool_destroy(pool_t *mypool)
{
    pthread_mutex_lock(&mypool->mut_pool);//抢整个线程池的互斥锁
    mypool->shutdown = 1;//把线程池设置为关闭状态
    pthread_cond_broadcast(&mypool->queue_not_empty);//骗线程(唤醒线程)
    pthread_mutex_unlock(&mypool->mut_pool);//解整个线程池的互斥锁

    sleep(1);

    free(mypool->workers);//销毁工作线程结构
    queue_destroy(mypool->task_queue);//销毁任务队列
    pthread_mutex_destroy(&mypool->mut_pool);//销毁整个线程池的互斥量
    pthread_mutex_destroy(&mypool->mut_busy);//销毁busy的互斥量
    pthread_cond_destroy(&mypool->queue_not_empty);//销毁任务队列不为空的条件变量
    pthread_cond_destroy(&mypool->queue_not_full);//销毁任务队列不为满的条件变量
    free(mypool);//销毁整个线程池
}


