#pragma once
#include "inet.h"
//#include"tcpkernel.h"
#include"packdef.h"
#include "ikerkel.h"
#include"getipport.h"
class tcpinet :
    public inet
{
public:
    tcpinet(Ikernel* pkernel);
    ~tcpinet();
public:
    bool InitNetWork() ;//初始化网络
    void UnInitNetWork();//关闭网络
    bool SendData(char* szbuf, int nlen);//发送数据
    void RecvData();//接收数据
    static   unsigned _stdcall ThredProc(void *);
    
public:
    SOCKET m_sock;
    HANDLE m_hThread;
    bool m_flag;
    Ikernel* m_pkernel;

public:
    getipport gip; //保存ip和端口号
    //std::string ServerIp;
    //std::string ServerPort;
};

