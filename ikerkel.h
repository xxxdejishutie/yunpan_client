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
	virtual bool Opensqlandnet() = 0;//�������ݿ������
	virtual void Closesqlandnet() = 0;//�ر�
	virtual void dealtext(char*) = 0;//����
	virtual bool SendData(char* szbuf, int nlen) = 0;
};

