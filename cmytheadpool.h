#ifndef CMYTHEADPOOL_H
#define CMYTHEADPOOL_H
#include <windows.h>
#include <list>
#include <queue>
#include <process.h>
#include <stdio.h>
class Itask{
public:
    Itask()
    {}
    virtual ~ Itask()
    {}
public:
    virtual void Run() = 0;
};
class CMyTheadPool
{
public:
    CMyTheadPool();
public:
    //1.创建线程
    bool  CreateThreadPool(long lMinThreadNum,long lMaxThreadNum);
    void  DestroyThreadPool();//2.销毁线程池
    bool  Push(Itask *); //3.投递任务
    static unsigned _stdcall ThreadProc(void *); //4.线程执行函数
private:
    std::list<HANDLE> m_lstThread;//线程存放的链表
    std::queue<Itask*> m_qItask;//任务数组
    bool  m_bFlagQuit;//推出标志
    HANDLE m_hSemaphore;//信号
    long  m_lMaxThreadNum;  //最多的线程个数   5
    long  m_lCreateThreadNum; //当下存在的线程个数   2
    long  m_lRunThreadNum; // 正在工作的线程数 1
    HANDLE m_hMutex;//信号量
};

#endif // CMYTHEADPOOL_H
