#include "cmytheadpool.h"

CMyTheadPool::CMyTheadPool()
{
   m_bFlagQuit = true;
   m_lMaxThreadNum = 0;
   m_lCreateThreadNum = 0;
   m_lRunThreadNum = 0;
   m_hMutex = CreateMutexW(0,0,0);
}


 bool  CMyTheadPool::CreateThreadPool(long lMinThreadNum,long lMaxThreadNum)
 {
     //1.校验参数
     if(lMinThreadNum <=0 || lMaxThreadNum <lMinThreadNum)
         return false;
     //线程资源
      m_hSemaphore = CreateSemaphoreW(0,0,lMaxThreadNum,0);
      
     //2.创建一些线程
     for(long i =0;i <lMinThreadNum;i++)
     {
         HANDLE hThread =(HANDLE)_beginthreadex(0,0,&CMyTheadPool::ThreadProc,this,0,0);
         if(hThread)
             m_lstThread.push_back(hThread);
     }
     m_lMaxThreadNum = lMaxThreadNum;
     m_lCreateThreadNum = lMinThreadNum;
     return true;
 }
 
bool  CMyTheadPool::Push(Itask *pItask)
{
    //1.校验参数
    if(!pItask)
        return false;
    
    //2.将任务加入队列中
    WaitForSingleObject(m_hMutex,INFINITE);
    m_qItask.push(pItask);
    ReleaseMutex(m_hMutex);
    //3.释放信号量
   // ReleaseSemaphore(m_hSemaphore,1,0);

    //1.有空闲的线程

    //2.没有空闲的线程
    if(m_lRunThreadNum ==m_lCreateThreadNum &&
            m_lCreateThreadNum <m_lMaxThreadNum )
    {
        HANDLE hThread =(HANDLE)_beginthreadex(0,0,&CMyTheadPool::ThreadProc,this,0,0);
        if(hThread)
            m_lstThread.push_back(hThread);
        m_lCreateThreadNum++;
    }
    //3.爆满


    ReleaseSemaphore(m_hSemaphore,1,0);
    return true;
}
unsigned _stdcall CMyTheadPool::ThreadProc(void * lpvoid)
{
    CMyTheadPool *pthis = (CMyTheadPool*)lpvoid;
    Itask *pItask = NULL;
    while(pthis->m_bFlagQuit)
    {
        //阻塞等信号
        WaitForSingleObject(pthis->m_hSemaphore,INFINITE);
        //阻塞--就绪--运行
        //pthis->m_lRunThreadNum++;
        InterlockedIncrement(&pthis->m_lRunThreadNum);
        //从队列中取任务

        while(!pthis->m_qItask.empty())
        {
            WaitForSingleObject(pthis->m_hMutex,INFINITE);
            if(pthis->m_qItask.empty())
            {
                ReleaseMutex(pthis->m_hMutex);
                break;
            }
            pItask = pthis->m_qItask.front();
            pthis->m_qItask.pop();
            ReleaseMutex(pthis->m_hMutex);
             //执行任务
            pItask->Run();

            delete pItask;
            pItask = NULL;
        }
        
        //pthis->m_lRunThreadNum--;
        InterlockedDecrement(&pthis->m_lRunThreadNum);
    }
    return 0;
}

void  CMyTheadPool::DestroyThreadPool()
{
      m_bFlagQuit = false;
      //检查线程是否退出，如果没有退出，则强制杀死
      auto ite = m_lstThread.begin();
      while(ite != m_lstThread.end())
      {
          if(WAIT_TIMEOUT == WaitForSingleObject(*ite,100)) 
              TerminateThread(*ite,-1);
          
          CloseHandle(*ite);
          *ite = NULL;
          ite++;
      }
      
      if(m_hSemaphore)
      {
          CloseHandle(m_hSemaphore);
          m_hSemaphore = NULL;
      }

      Itask *pItask = NULL;
      while(!m_qItask.empty())
      {
          pItask = m_qItask.front();
          m_qItask.pop();

          delete pItask;
          pItask = NULL;
      }

      if(m_hMutex)
      {
          CloseHandle(m_hMutex);
          m_hMutex = NULL;
      }
}
