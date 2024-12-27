#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
int queue_init(queue_t **q, int capacity, int size)
{
    queue_t *p=NULL;
    p=malloc(sizeof(queue_t));
    if(p==NULL)
    return -1;
    p->s=malloc(capacity*size);
    if(p->s==NULL)
    return -2;
    p->front=p->rear=0;
    p->capacity=capacity;
    p->size=size;
    *q=p;
    return 0;
}
/*
功能 : 判断队列是否为空
参数 : q 指向要判断的队列
返回值 : 为空返回1,不为空返回0
*/ 
int queue_is_empty(const queue_t *q)
{
    return (q->rear==q->front);
}
 
/*
功能 : 判断队列是否为满
参数 : q 指向要判断的队列
返回值 : 为满返回1,不为满返回0
*/ 
int queue_is_full(const queue_t *q)
{
    return ((q->rear+1)%q->capacity==q->front);
}
 
/*
功能 : 入队
参数 : q 指向要入队的队列
        data 指向要入队的数据
返回值 : 成功返回0,失败返回<0
*/ 
int queue_en(queue_t *q, const void *data)
{
    if(queue_is_full(q))
    return -1;
    q->rear=(q->rear+1)%(q->capacity);
    memcpy((char*)q->s+(q->rear)*(q->size),data,q->size);
    return 0; 
}
 
/*
功能 : 出队
参数 : q 指向要出队的队列
        data 指向存储空间(存储出队的数据)
返回值 : 成功返回0,失败返回<0
*/ 
int queue_de(queue_t *q, void *data)
{
    if(queue_is_empty(q))
    return -1;
    q->front=(q->front+1)%q->capacity;
    memcpy(data,(char *)(q->s)+(q->front)*(q->size),q->size);
    return 0;
}

/*
功能 : 释放队列
参数 : q 指向要释放的队列
返回值 : 空
*/ 
void queue_destroy(queue_t *q)
{
    free(q->s);
    free(q);
} 

//遍历函数由用户方实现
/*
功能：便利打印队列
参数：q 指向要打印的队列
*/
void queue_display(queue_t *q)//遍历函数由用户方实现
{
    int i=0;
    if(queue_is_empty(q))
    return ;
    if(q->rear<q->front) 
    {
        for(i=0; i<=(q->rear); i++)
        printf("%d ",*((int *)q->s+i));//此处是根据用户输入什么类型的数据进行相应的标准打印格式和相应指针类型转换
        for(i=q->front+1; i<(q->capacity); i++)
        printf("%d ",*((int *)(q->s)+i));
    }
    else
    {
        for(i=0; i<q->front; i++)
        printf("%d ",*((int *)q->s+i));//此处是根据输入什么类型的数据进行相应的标准打印格式和相应指针类型转换
        for(i=q->front+1;  i<=q->rear;i++)
        printf("%d ",*((int *)(q->s)+i));
    }
    printf("\n");

}
