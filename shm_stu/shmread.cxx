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

    //打开共享内存，以0666的权限
    shmid=shmget((key_t)KEY,sizeof(room)*10,0666);
    if(shmid==-1) 
    {
        perror("shmget");
        return -1;
    }

    //连接共享内存
    shmaddr=shmat(shmid,nullptr,0);
    if((shmaddr==(void*)-1)||shmaddr==nullptr)
    {
        perror("shmat");
        return -1;
    } 

    
    room* ptr=(room*) shmaddr;
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
    
    //删除共享内存
    if(shmctl(shmid,IPC_RMID,0)==-1)
    {
        perror("shmctl");
        return -1;
    }
    return 0;
}