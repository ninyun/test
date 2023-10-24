#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include "room.hpp"
#include <iostream>
#include <string.h>



int main(void)
{
    int shmid=-1;
    void* shmaddr=nullptr;
    int ret=-1;

    //创建共享内存以0666的权限
    shmid=shmget((key_t)KEY,sizeof(room)*10,0666|IPC_CREAT);
    if(shmid==-1) 
    {
        perror("shmget");
        return -1;
    }

    //将共享内存连接到当前进程
    shmaddr=shmat(shmid,nullptr,0);
    if((shmaddr==(void*)-1)||shmaddr==nullptr)
    {
        perror("shmat");
        return -1;
    } 


    room* ptr=(room*) shmaddr;
    for(int i=0;i<10;i++)
    {
        room _temp(i,i*i);
        memcpy(ptr+i,&_temp,sizeof(room));
    }
    
    for(int i=0;i<10;i++)
    {
        std::cout<<"room:id="<<ptr[i].id<<",number="<<ptr[i].number<<std::endl;
    }

    //断开共享内存
    ret=shmdt(shmaddr);
    if(ret==-1) 
    {
        perror("shmdt");
        return -1;
    }

    
    return 0;
}