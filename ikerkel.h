#pragma once
//#include "inet.h"
//#include<WinSock2.h>
//#include<WS2tcpip.h>
//#include<iostream>


class Ikernel
{
public:
	Ikernel()
	{}
	virtual ~Ikernel()
	{}
public:
	virtual bool Opensqlandnet() = 0;//启动数据库和网络
	virtual void Closesqlandnet() = 0;//关闭
	virtual void dealtext(char*) = 0;//处理
	virtual bool SendData(char* szbuf, int nlen) = 0;
};

