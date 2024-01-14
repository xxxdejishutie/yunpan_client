#pragma once

#include<WinSock2.h>
#include<WS2tcpip.h>
#include <windows.h>
#include<iostream>
using namespace std;
class inet
{
public:
    inet()
    {}
    virtual ~inet()
    {}
public:
    virtual bool InitNetWork() = 0;
    virtual void UnInitNetWork() = 0;
    virtual bool SendData( char* szbuf, int nlen) = 0;
    virtual void RecvData() = 0;
};

