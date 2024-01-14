#pragma once
#ifndef PACKDEF_H
#define PACKDEF_H

#define _DEF_PROTOCOL_BASE   10
//ע��
#define _DEF_PROTOCOL_REGISTER_RQ   _DEF_PROTOCOL_BASE +1
#define _DEF_PROTOCOL_REGISTER_RS   _DEF_PROTOCOL_BASE +2
//��¼
#define _DEF_PROTOCOL_LOGIN_RQ   _DEF_PROTOCOL_BASE +3
#define _DEF_PROTOCOL_LOGIN_RS   _DEF_PROTOCOL_BASE +4
//��ȡ�ļ��б�
#define _DEF_PROTOCOL_GETFILELIST_RQ   _DEF_PROTOCOL_BASE +5
#define _DEF_PROTOCOL_GETFILELIST_RS   _DEF_PROTOCOL_BASE +6
//�ϴ��ļ���Ϣ
#define _DEF_PROTOCOL_UPLOAD_FILEINFO_RQ   _DEF_PROTOCOL_BASE +7
#define _DEF_PROTOCOL_UPLOAD_FILEINFO_RS   _DEF_PROTOCOL_BASE +8
//�ϴ��ļ�����
#define _DEF_PROTOCOL_UPLOAD_FILECONTENT_RQ   _DEF_PROTOCOL_BASE +9
#define _DEF_PROTOCOL_UPLOAD_FILECONTENT_RS   _DEF_PROTOCOL_BASE +10

//�����ļ�����
#define _DEF_PROTOCOL_DOWNLOAD_FILEINFO_RQ   _DEF_PROTOCOL_BASE +11
#define _DEF_PROTOCOL_DOWNLOAD_FILEINFO_RS   _DEF_PROTOCOL_BASE +12
//�����ļ�����
#define _DEF_PROTOCOL_DOWNLOAD_FILECONTENT_SEND   _DEF_PROTOCOL_BASE +13
#define _DEF_PROTOCOL_DOWNLOAD_FILECONTENT_RELY   _DEF_PROTOCOL_BASE +14

//ɾ���ļ�
#define _DEF_PROTOCOL_DELETE_FILE_RQ   _DEF_PROTOCOL_BASE +15
#define _DEF_PROTOCOL_DELETE_FILE_RS   _DEF_PROTOCOL_BASE +16



#define _upload_file_exists 0 //�ļ����Լ��Ѿ��ϴ���������Ҫ�ظ��ϴ�
#define _upload_file_continue 1 //����������û�д��ꡣ�ϵ�����
#define _upload_file_success 2 // �봫
#define _upload_file_normal 3 //������



#define MAXSIZE  45
#define MAXFILENUM  500
#define MAXSQLLEN  500
#define MAXFILECONTENT 4096

//Э���
#define _register_failed   0
#define _register_success  1

#define _login_user_noexist  0
#define _login_password_err  1
#define _login_success       2


#define _delete_faild 0
#define _delete_success 1


#define _install_pause 0
#define _install_continue 1



//ע�������
struct STRU_REGISTER_RQ
{
    char m_nType;
    char m_szName[MAXSIZE];
    char m_szPassword[MAXSIZE];
    long long m_ltel;
};

//ע��ظ���
struct STRU_REGISTER_RS
{
    char m_nType;
    char m_szResult;
};

//��¼�����
struct STRU_LOGIN_RQ
{
    char m_nType;
    char m_szName[MAXSIZE];
    char m_szPassword[MAXSIZE];
};


//��¼�ظ���
struct STRU_LOGIN_RS
{
    char m_nType;
    char m_szResult;
    int m_luserid;
};


//��ȡ�ļ��б�����
struct STRU_GETFILELIST_RQ
{
    char m_nType;
    int m_luserid;
};

//�ļ���Ϣ
struct FileInfo
{
    char m_szFileName[MAXSIZE];//�ļ���
    int m_fileSize;//�ļ���С
    char m_szDateTime[MAXSIZE];//�ļ�����
};


//��ȡ�ļ��б�ظ�
struct STRU_GETFILELIST_RS
{
    char m_nType;
    FileInfo m_aryFile[MAXFILENUM];
    int m_nFileNum;
};


//�ϴ��ļ���Ϣ����
struct STRU_UPLOAD_FILEINFO_RQ
{
    char m_nType;
    int m_lUserId;
    int m_lFileSize;
    char m_szFileName[MAXSIZE];
    char m_szFileMD5[MAXSIZE];

};



//�ϴ��ļ���Ϣ�ظ�
struct STRU_UPLOAD_FILEINFO_RS
{
    char m_nType;
    int m_lUserId;
    int m_lFileId;
    int m_lPosition;
    char m_szFileMD5[MAXSIZE];
    int bresult;

};


//�ϴ��ļ���
struct STRU_UPLOAD_FILECONTENT_RQ
{
    char m_nType;
    int m_lUserId;
    int m_lFileId;
    char m_szFileContent[MAXFILECONTENT];
    int m_nNum;//��С
    int m_pos;//�ļ�����ʼλ��
};


//�����ļ�����
struct STRU_DOWNLOAD_FILEINFO_RQ
{
    char m_nType;
    int m_lUserId;
    char m_szFileName[MAXSIZE];

};

//�����ļ�����ظ�
struct STRU_DOWNLOAD_FILEINFO_RS
{
    char m_nType;
    int m_lUserId;
    int m_lFileId;
    int m_lPosition;
    int bresult;
    int m_fisize;
    char m_szFileName[MAXSIZE];
};


//�����ļ���
struct STRU_DOWNLOAD_FILECONTENT_RQ
{
    char m_nType;
    int m_lUserId;
    int m_lFileId;
    char m_szFileContent[MAXFILECONTENT];
    int m_nNum;//��С
};


//�����ļ����ݻظ���
struct STRU_DOWNLOAD_FILECONTENT_RELY
{
    char m_nType;
    int m_lUserId;
    int m_lFileId;
    int m_lPosition;
    int stage;
};



//ɾ���ļ������
struct STRU_PROTOCOL_DELETE_FILE_RQ
{
    char m_nType;
    int m_lUserId;
    char m_szFileName[MAXSIZE];
};


//ɾ���ļ��ظ���
struct STRU_PROTOCOL_DELETE_FILE_RS
{
    char m_nType;
    bool result;
};

//qt�ļ�
#include<QFile>
struct uploadfileinfo{
   char m_szFileMD5[MAXSIZE];
   char m_szFilepath[MAXSIZE];
   long long filesize;
   long long fileid;
   long long m_position;
   QFile * m_file;
   QString stage;
};

 #include<mutex>
#include<windows.h>





#endif // PACKDEF_H
