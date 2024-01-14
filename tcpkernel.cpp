#include "tcpkernel.h"

//#include"packdef.h"

tcpkernel::tcpkernel()
{
    m_pnet = new tcpinet(this);

}
tcpkernel::~tcpkernel()
{

}
bool tcpkernel::Opensqlandnet()
{
    m_pnet->InitNetWork();



    return 1;
}
void tcpkernel::Closesqlandnet()
{
    m_pnet->UnInitNetWork();
}
void tcpkernel::dealtext(char*a)//对应不同类型发送信号
{
    switch (*a)
    {
    case _DEF_PROTOCOL_REGISTER_RS:
        emit signal_char(a);
        break;
    case _DEF_PROTOCOL_LOGIN_RS:
        emit signal__DEF_PROTOCOL_LOGIN_RS(a);
        break;
    case _DEF_PROTOCOL_GETFILELIST_RS:
        emit signal__DEF_PROTOCOL_GETFILELIST_RS(a);
        break;
    case _DEF_PROTOCOL_UPLOAD_FILEINFO_RS:
        emit signal__STRU_UPLOAD_FILEINFO_RS(a);
        break;
    case _DEF_PROTOCOL_DOWNLOAD_FILEINFO_RS:
        emit signal__DEF_DOWNLOAD_FILEINFO_RS(a);
    case _DEF_PROTOCOL_DELETE_FILE_RS:
        emit signal__DEF_DELETE_FILE_RS(a);
        break;
    case _DEF_PROTOCOL_DOWNLOAD_FILECONTENT_SEND:
        emit signal__DEF_DOWNLOAD_FILECONTENT_SEND(a);
        break;
    }
}

bool tcpkernel::SendData(char* sbuf, int nlen)
{

    m_pnet->SendData(sbuf,nlen);
    return 0;
}
