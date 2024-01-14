#include "tcpinet.h"
#include<process.h>
//#include<WS2tcpip.h>
tcpinet::tcpinet(Ikernel* pkernel)
{
    m_sock = NULL;
    m_hThread = NULL;
    m_flag = true;
    m_pkernel = pkernel;
}
tcpinet::~tcpinet()
{
    if (m_sock != NULL)
        closesocket(m_sock);
    tcpinet::UnInitNetWork();

}
bool tcpinet::InitNetWork()//常规初始化tcp网络
{

    gip.getServerIpPort();
    cout << "get server ip and port " << gip.ServerIp << " " << gip.ServerPort << endl;
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {

        printf("WSAStartup failed with error: %d\n", err);
        return 0;
    }


    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        printf("Could not find a usable version of Winsock.dll\n");
        WSACleanup();
        return 0;
    }

        printf("The Winsock 2.2 dll was found okay\n");



    m_sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in sockline;
    sockline.sin_family = AF_INET;
    sockline.sin_port = htons(gip.ServerPort);//与服务器端口号相同

    //sockline.sin_addr.S_un.S_addr = inet_addr("192.168.3.238");//服务器的IP地址
    sockline.sin_addr.S_un.S_addr = inet_addr(gip.ServerIp.c_str());

    //利用得到的IP地址进行通信
//    wchar_t * wc = new wchar_t[gip.ServerIp.size()];
//    swprintf(wc,100,L"%S",gip.ServerIp.c_str());
//    InetPtonW(AF_INET,wc, &sockline.sin_addr);
    //InetPtonW(AF_INET, L"127.0.0.1", &sockline.sin_addr);
    //sockline.sin_addr.S_un.S_addr = inet_addr("192.168.137.1");
    if (SOCKET_ERROR == connect(m_sock, (sockaddr*)&sockline, sizeof(sockline)))
    {
        std::cout << "connect error " << SOCKET_ERROR << endl;
        return 0;
    }
    /*char buf[1200] = "请求";
    send(m_sock, buf, sizeof(buf), 0);*/

    m_hThread = (HANDLE)_beginthreadex(0, 0, &ThredProc, this, 0, 0);//创建线程

    return 1;
}


void tcpinet::UnInitNetWork()//断开网络
{
    m_flag = false;
    if (m_hThread)
    {
        if (WAIT_TIMEOUT == WaitForSingleObject(m_hThread, 100))
        {
            TerminateThread(m_hThread, -1);
        }

        CloseHandle(m_hThread);
        m_hThread = NULL;
    }

    if (m_sock)
    {
        closesocket(m_sock);
        m_sock = 0;
    }

    WSACleanup();
}

unsigned tcpinet::ThredProc(void*lop)//在线程里执行接收函数
{
    tcpinet* pthis = (tcpinet*)lop;
    pthis->RecvData();

    return 0;
}
bool tcpinet::SendData(char* szbuf, int nlen)//发送函数，先发文件大小再发包内容
{
    if (szbuf == NULL || nlen <= 0)
        return 0;
    if (send(m_sock, (char *)&nlen, sizeof(nlen), 0) <= 0)
        return 0;
    if (send(m_sock, szbuf, nlen, 0) <= 0)
        return 0;
    return 1;
}
void tcpinet::RecvData()
{
    int nPackSize;
    int nRecvNum;
    char* pszbuf = NULL;
    int noffset;
    while (m_flag)
    {
        nRecvNum = recv(m_sock, (char*)&nPackSize, sizeof(int), 0);
        if (nRecvNum <= 0)
        {
            //如果下线,则退出
            if (WSAGetLastError() == WSAECONNRESET)
                break;

            continue;
        }
        if (nPackSize <= 0)continue;//接收到文件大小在继续

        pszbuf = new char[nPackSize];
        noffset = 0;
        while (nPackSize)//按文件大小读取信息
        {
            nRecvNum = recv(m_sock, pszbuf + noffset, nPackSize, 0);
            noffset += nRecvNum;
            nPackSize -= nRecvNum;
        }
        //处理数据
        m_pkernel->dealtext(pszbuf);
        //cout << pszbuf << endl;

//        delete[]pszbuf;
//        pszbuf = NULL;
    }
}
