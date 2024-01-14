#pragma once

#include"tcpinet.h"
//          : public qobject , public Ikernel
#include<QObject>
#include<QWidget>

class tcpkernel : public QObject, public Ikernel
{
    Q_OBJECT

public:
	//tcpkernel(Qobject *parent) : qobject parent
	tcpkernel();
	~tcpkernel();
public:
    bool Opensqlandnet();//�������ݿ������,�ͻ��˽�������
    void Closesqlandnet();//�ر�
	void dealtext(char*); // ����
	 bool SendData(char* sbuf, int nlen);
public:
	inet* m_pnet;

signals:
    void signal_char(char *);
    void signal__DEF_PROTOCOL_LOGIN_RS(char *);
    void signal__DEF_PROTOCOL_GETFILELIST_RS(char *);
    void signal__STRU_UPLOAD_FILEINFO_RS(char *);
    void signal__DEF_DELETE_FILE_RS(char*);
    void signal__DEF_DOWNLOAD_FILEINFO_RS(char*);
    void signal__DEF_DOWNLOAD_FILECONTENT_SEND(char*);
};

