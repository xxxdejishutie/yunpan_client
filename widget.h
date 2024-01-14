#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include"dialog.h"
#include"menu.h"

#include<QMessageBox>
#include"ikerkel.h"

#include<map>
#include<QFileDevice>
#include"md5.h"
#include"packdef.h"
#include<qlist.h>
#include<list>
#include <sys/stat.h>//

#include<QDebug>
#include"cmytheadpool.h"
using namespace std;



QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
     QList<uploadfileinfo*> m_lisfileinfo; //�����ϴ����ļ���Ϣ
     map<int,uploadfileinfo> m_mapfileinstall;//�洢���ص��ļ���Ϣ
public:
    Ui::Widget *ui;
    Dialog *m_dia;//��¼����
    menu *m_menu;//�˵�����
    Ikernel *m_ikernel;//����ģ��
    int m_user_id;//�û�id
    CMyTheadPool m_threadpool;//�̳߳�
    string FileDigest(const string &file);//����md5ֵ�ĺ���
    HANDLE m_han_install_pthead;

    HANDLE map_mutex;
signals:
    void signal_uploadfilesuccess(int);
public slots:
    void slot_char(char*);
    void slot__DEF_PROTOCOL_LOGIN_RS(char*);//������¼�ظ�
    void slot__DEF_PROTOCOL_GETFILELIST_RS(char*);//������ȡ�ļ��б�
    void slot__STRU_UPLOAD_FILEINFO_RS(char *);//�����ϴ��ļ��ظ�
    void slot_uploadfilesuccess(int);//û����
    void slot__DEF_DELETE_FILE_RS(char *);//����ɾ���ļ��Ļظ�
    void slot__DEF_DOWNLOAD_FILEINFO_RS(char*);//���������ļ��ظ�
    void slot__DEF_DOWNLOAD_FILECONTENT_SEND(char*);//�����������������ļ���
    //void slot__DEF_DOWNLOAD_SUCCESS(int);
private slots:
    void on_pushButton_clicked();//�ϴ��ļ���ť



    void on_pushButton_4_clicked();
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

public:
    static unsigned _stdcall ThreadProc(void *lp);
    int getFileSize1(const char *fileName);
signals:
    void signal_install_success(int);
};




class filedir : public Itask//�̳е������ṹ�壬�洢�ļ�����Ϣ
{
public:

    filedir(STRU_UPLOAD_FILEINFO_RS* rs,uploadfileinfo *pfileinfo,Widget *pwidght)//���캯��
    {
        m_rs = rs;
        m_pfileinfo = pfileinfo;
        m_pwidght = pwidght;
    }
public:
    STRU_UPLOAD_FILEINFO_RS* m_rs; //�ϴ��ļ���Ϣ�ظ���
    uploadfileinfo *m_pfileinfo;//��Ҫ�ϴ����ļ���Ϣ
    Widget *m_pwidght;

public:
    void Run();//�ϴ��ļ����ݵĺ���
};

#endif // WIDGET_H
