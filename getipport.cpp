#include "getipport.h"
#include<WS2tcpip.h>
#include<WinSock2.h>
#include <windows.h>
#include<string>
#include<iostream>

using namespace std;

getipport::getipport()
{
    ProxyIp = "127.0.0.1";
    ProxyPort = 8899;
}


bool getipport::getServerIpPort()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;


    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {

        return false;
    }

    if (LOBYTE(wsaData.wVersion) != 2 ||
        HIBYTE(wsaData.wVersion) != 2)
    {
        return false;
    }
    SOCKET Sock = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);//创建监听套接字
    //memset(&(m_stu_udp_sock.my_olp), 0, sizeof(OVERLAPPED));
    sockaddr_in sockline;
    sockline.sin_port = htons(ProxyPort);
    sockline.sin_family = AF_INET;
    wchar_t * wc = new wchar_t[ProxyIp.size()];
    swprintf(wc,100,L"%s",ProxyIp.c_str());
    InetPtonW(AF_INET,wc, &sockline.sin_addr);
    char buf[100] = "1*";

    sendto(Sock,buf,sizeof(buf), 0, (const sockaddr*)&sockline, sizeof(sockline));
    memset(buf, 0, sizeof(buf));
    recvfrom(Sock, buf, sizeof(buf), 0,0,0);
    //std::cout << buf << std::endl;
    cout << "buf is "<<buf << endl;
    ServerIp = strtok(buf,"*");
    char *ss = strtok(nullptr,"*");
    cout << ss << endl;
    ServerPort = atoi(ss);

    cout <<"Server ip and port is " <<  ServerIp << " " << ServerPort << endl;

    return true;
}
