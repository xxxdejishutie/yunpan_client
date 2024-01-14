#include "widget.h"
#include "ui_widget.h"
#include"tcpkernel.h"
#include<qfiledialog.h>
#include<qtextcodec.h>//???????????
#include<vector>

//?????????????
//HANDLE cs = CreateSemaphore(nullptr, 1, 1, NULL);
//map<int,string> map_info;




Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_dia = new Dialog;
    m_dia->show();
    m_menu = nullptr;


    ui->tableWidget_1->setColumnWidth(0,200);
    ui->tableWidget_1->setColumnWidth(1,100);
    ui->tableWidget_1->setColumnWidth(2,200);
    m_ikernel = new tcpkernel;
    map_mutex = CreateMutex(nullptr,true,L"mymutx");





    m_dia->getkernel(m_ikernel);
    // ??????????????

    //??????cpu???? CPU* 2
    SYSTEM_INFO s1;
    GetSystemInfo(&s1);
    m_threadpool.CreateThreadPool(1,s1.dwNumberOfProcessors * 2);

    QObject::connect((tcpkernel*)m_ikernel,SIGNAL(signal_char(char *)),this,SLOT(slot_char(char*)));
    QObject::connect((tcpkernel*)m_ikernel,SIGNAL(signal__DEF_PROTOCOL_LOGIN_RS(char *)),this,SLOT(slot__DEF_PROTOCOL_LOGIN_RS(char*)));
    QObject::connect((tcpkernel*)m_ikernel,SIGNAL(signal__DEF_PROTOCOL_GETFILELIST_RS(char *)),this,SLOT(slot__DEF_PROTOCOL_GETFILELIST_RS(char*)));
    QObject::connect((tcpkernel*)m_ikernel,SIGNAL(signal__STRU_UPLOAD_FILEINFO_RS(char *)),this,SLOT(slot__STRU_UPLOAD_FILEINFO_RS(char*)));
    QObject::connect(this,SIGNAL(signal_uploadfilesuccess(int)),this,SLOT(slot_uploadfilesuccess(int)));
    QObject::connect((tcpkernel*)m_ikernel,SIGNAL(signal__DEF_DELETE_FILE_RS(char*)),this,SLOT(slot__DEF_DELETE_FILE_RS(char*)));
    QObject::connect((tcpkernel*)m_ikernel,SIGNAL(signal__DEF_DOWNLOAD_FILEINFO_RS(char*)),this,SLOT(slot__DEF_DOWNLOAD_FILEINFO_RS(char*)));
    QObject::connect((tcpkernel*)m_ikernel,SIGNAL(signal__DEF_DOWNLOAD_FILECONTENT_SEND(char*)),this,SLOT(slot__DEF_DOWNLOAD_FILECONTENT_SEND(char*)));
    //QObject::connect(this,SIGNAL(signal_install_success(int)),this,SLOT(slot__DEF_DOWNLOAD_SUCCESS(int)));
    if(!m_ikernel->Opensqlandnet())
        QMessageBox::information(m_dia,"shibai","jixu");

}

Widget::~Widget()
{
    delete ui;
    m_threadpool.DestroyThreadPool();
}

void  Widget::slot_char(char*buf)//???????????????????????
{
    STRU_REGISTER_RS *rs = (STRU_REGISTER_RS*) buf;
    QString qstr;
    if(rs->m_szResult == _register_failed )
        qstr = "_register_failed";
    else
        qstr = "_register_success";
    QMessageBox::information(this,"???????",qstr);
    delete[]buf;
    buf = NULL;

}


void  Widget::slot__DEF_PROTOCOL_LOGIN_RS(char*buf)//????????????
{
    STRU_LOGIN_RS *rs = (STRU_LOGIN_RS*)buf;
    QString qstr;
    if(rs->m_szResult == _login_user_noexist)
        qstr = "¦Ä???";
    else if(rs->m_szResult == _login_password_err)
        qstr = "????????";
    else
    {
        qstr= "??????";
        //???????
        m_user_id = rs->m_luserid;
        m_dia->hide();
        m_dia->close();

        this->show();
        //???????§Ò?????
        STRU_GETFILELIST_RQ rq;
        rq.m_nType = _DEF_PROTOCOL_GETFILELIST_RQ;
        rq.m_luserid = m_user_id;

        m_ikernel->SendData((char*)&rq,sizeof(rq));//??????????????????§Ò?????
        return ;
    }

    QMessageBox::information(this,"fsd",qstr);

    delete[]buf;
    buf = NULL;
}


bool cmp(FileInfo &a,FileInfo &b)
{
    string str1;
    int flag = 0;
    for(int i =0;a.m_szFileName[i] != '\0';i ++)
    {
        if(a.m_szFileName[i] == '.')
        {
            flag = 1;
        }
        if(flag == 1 && a.m_szFileName[i] != '.')
        {
            str1 += a.m_szFileName[i];
        }
    }
    string str2;
    flag = 0;
    for(int i =0;b.m_szFileName[i] != '\0';i ++)
    {
        if(b.m_szFileName[i] == '.')
        {
            flag = 1;
        }
        if(flag == 1 && b.m_szFileName[i] != '.')
        {
            str2 += b.m_szFileName[i];
        }
    }

    return str1 < str2;
}

void Widget::slot__DEF_PROTOCOL_GETFILELIST_RS(char*buf)//???????????§Ò??????
{
    STRU_GETFILELIST_RS *rs = (STRU_GETFILELIST_RS*)buf;
    int i = 0;
    QString qstr;
    this->hide();
    ui->tableWidget_1->clearContents();
    ui->tableWidget_1->removeRow(0);
    ui->tableWidget_1->removeRow(1);
    vector<FileInfo> ve;

    while(i < rs->m_nFileNum)
    {
        ve.push_back(rs->m_aryFile[i]);
//       qstr = QString::fromStdString( rs->m_aryFile[i].m_szFileName);
//       ui->tableWidget_1->setItem(i,0,new QTableWidgetItem(qstr));

//       qstr = QString::number(rs->m_aryFile[i].m_fileSize);
//       ui->tableWidget_1->setItem(i,1,new QTableWidgetItem(qstr));

//       qstr = QString::fromStdString( rs->m_aryFile[i].m_szDateTime);
//       ui->tableWidget_1->setItem(i,2,new QTableWidgetItem(qstr));

       i++;
       //this->show();
    }
    sort(ve.begin(),ve.end(),cmp);
    i = 0;
    for(auto ite = ve.begin();ite != ve.end();ite ++)
    {
        qstr = QString::fromStdString( (*ite).m_szFileName);
        ui->tableWidget_1->setItem(i,0,new QTableWidgetItem(qstr));

        qstr = QString::number((*ite).m_fileSize);
        ui->tableWidget_1->setItem(i,1,new QTableWidgetItem(qstr));

        qstr = QString::fromStdString( (*ite).m_szDateTime);
        ui->tableWidget_1->setItem(i,2,new QTableWidgetItem(qstr));
        i ++;
    }
    this->show();
    //???????
    ui->tableWidget_1->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_1->setSelectionMode(QAbstractItemView::SingleSelection);
}




string Widget::FileDigest(const string &file) //????md5??????
{

    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string name = code->fromUnicode(file.c_str()).data();

    ifstream in(name, std::ios::binary);
    //ifstream erin(name, std::ios::in);
    if (!in)
    {
        in.close();
        ifstream erin(file.c_str(), std::ios::in);
        if(!erin)
            return "00";
        return "";
    }

    MD5 md5;
    std::streamsize length;
    char buffer[1024];
    while (!in.eof()) {
        in.read(buffer, 1024);
        length = in.gcount();
        if (length > 0)
            md5.update(buffer, length);
    }
    in.close();
    return md5.toString();
}


void Widget::slot_uploadfilesuccess(int a)//?????????
{


    a = 1000;
    QMessageBox::information(this,"upload","upload successful");
    //QString qstr = fileinfo->m_szFilepath;
    //qstr.section('/',-1);
    //ui->tableWidget_1->setItem(i,0,new QTableWidgetItem(qstr));
    //qstr = fileinfo->filesize;
    //ui->tableWidget_1->setItem(i,1,new QTableWidgetItem(qstr));

    //QMessageBox::information(this,"111","successful");



}




void Widget::slot__STRU_UPLOAD_FILEINFO_RS(char *buf)//???????????????????
{
    STRU_UPLOAD_FILEINFO_RS* rs = (STRU_UPLOAD_FILEINFO_RS*)buf;


    auto ite = m_lisfileinfo.begin();
    QList<uploadfileinfo*>::iterator itetemp;
    uploadfileinfo *pfileinfo = NULL;
    //??????????????????
    while(ite != m_lisfileinfo.end())
    {
        if(strcmp(rs->m_szFileMD5,(*ite)->m_szFileMD5) == 0)
        {
            pfileinfo = *ite;
            itetemp = ite;
            break;
        }
        ite ++;
    }



    if(rs->bresult == _upload_file_exists)
    {
        QMessageBox::information(m_dia,"fasd","?????");

        //???????§Ò?????
        STRU_GETFILELIST_RQ rq;
        rq.m_nType = _DEF_PROTOCOL_GETFILELIST_RQ;
        rq.m_luserid = m_user_id;

        m_ikernel->SendData((char*)&rq,sizeof(rq));//??????????????????§Ò?????
       //  ??????????????????
    }
    else if(rs->bresult == _upload_file_continue ||
            rs->bresult == _upload_file_normal)
    {
        //???????????????
        Itask *pitask = new filedir(rs,pfileinfo,this);
        m_threadpool.Push(pitask);//??????????????

    }
    else if(rs->bresult == _upload_file_success)
    {
        //??
        //?¨®???????
        QString filepath = pfileinfo->m_szFilepath;
        QString filename = filepath.section('/',-1);
        QString filemd5 = pfileinfo->m_szFileMD5;
        long long filesize = pfileinfo->filesize;
        QString strfilesize = QString::number(filesize);

        //?????????,????????§Ò?
        STRU_GETFILELIST_RQ rq;
        rq.m_nType = _DEF_PROTOCOL_GETFILELIST_RQ;
        rq.m_luserid = m_user_id;

        m_ikernel->SendData((char*)&rq,sizeof(rq));//??????????????????§Ò?????
        //int nrow = 0;
        //ui->tableWidget_1->setItem(nrow,0,new QTableWidgetItem(filename));
        //ui->tableWidget_1->setItem(nrow,1,new QTableWidgetItem(strfilesize));
        //delete pfileinfo;
        m_lisfileinfo.erase(itetemp);

    }

}

// ???stat???? ?????????§³????¦Ë???
int Widget::getFileSize1(const char *fileName) {

    if (fileName == NULL) {
        return 0;
    }

    // ????????›¥???(??)???????ÈÉ???????????§³???????????????????????
    struct stat statbuf;

    // ????????????????????????????
    stat(fileName, &statbuf);

    // ????????§³
    size_t filesize = statbuf.st_size;

    return filesize;
}


void Widget::on_pushButton_clicked()//?????????????
{
    QString qstrfilepath = QFileDialog::getOpenFileName(this,"???","\\");
    //????????§Ò?

    long long filesize;
    if(!qstrfilepath.isEmpty())
    {
//      ??????????
        QString qstr = qstrfilepath.section('/',-1);//??????1??????2????

        //qt??????string
        string stdstr = qstrfilepath.toStdString();

        //???????md5????????md5????
        string md5 = FileDigest(stdstr);

        QFile file(qstrfilepath);//???¡¤??
        file.open(QIODevice::ReadOnly);
        if(file.isOpen())
        {
            filesize = file.size();
        }
        file.close();


        //ifstream ifs(stdstr,ios::in | ios::binary);
        //int a = getFileSize1(stdstr.c_str());
        //filesize = a;


        //???????????
        STRU_UPLOAD_FILEINFO_RQ rq;
        rq.m_nType = _DEF_PROTOCOL_UPLOAD_FILEINFO_RQ;
        rq.m_lUserId = m_user_id;
        rq.m_lFileSize = filesize;
        strcpy(rq.m_szFileMD5,md5.c_str());
        strcpy(rq.m_szFileName,qstr.toStdString().c_str());


        m_ikernel->SendData((char*)&rq,sizeof(rq));



        //??????????????????
        uploadfileinfo *pnew = new uploadfileinfo;
        pnew->fileid = 0;
        pnew->m_file = 0;
        pnew->filesize = filesize;
        pnew->m_position = 0;
        strcpy(pnew->m_szFileMD5,md5.c_str());
        strcpy(pnew->m_szFilepath,qstrfilepath.toStdString().c_str());
        m_lisfileinfo.push_back(pnew);


    }
}











void filedir::Run()//???§Ü???
{
    STRU_UPLOAD_FILECONTENT_RQ rq;
    rq.m_nType = _DEF_PROTOCOL_UPLOAD_FILECONTENT_RQ ;
    rq.m_lUserId = m_pwidght->m_user_id;
    rq.m_lFileId = m_rs->m_lFileId;


    ifstream ifs(m_pfileinfo->m_szFilepath, ios::in | ios::binary);


    if(m_rs->bresult == _upload_file_continue && m_rs->m_lPosition > 0)//??????????????
    {
        ifs.seekg(m_rs->m_lPosition);

    }
    rq.m_pos = m_rs->m_lPosition;

    while(1)
    {
        memset(rq.m_szFileContent,'\0',sizeof(rq.m_szFileContent));
        ifs.read(rq.m_szFileContent,4096);
        rq.m_nNum = ifs.gcount();
        qDebug() << " file pos after read  is " << ifs.tellg();

        qDebug() << "fil m_nNum is" << rq.m_nNum ;// "file read realy size "<<  ant  << endl;

        if(rq.m_nNum > 0)
            m_pwidght->m_ikernel->SendData((char*)&rq,sizeof(rq));
        else
        {
            break;
        }

        rq.m_pos += rq.m_nNum;

    }



    ifs.close();

    //???????§Ò?
    STRU_GETFILELIST_RQ getrq;
    getrq.m_nType = _DEF_PROTOCOL_GETFILELIST_RQ;
    getrq.m_luserid = m_pwidght->m_user_id;
    m_pwidght->m_ikernel->SendData((char*)&getrq,sizeof(getrq));//??????????????????§Ò?????

    emit m_pwidght->signal_uploadfilesuccess(10);

    char str[1000] ;
    sprintf(str,"upload updata successful  filesize is %d",rq.m_pos + rq.m_nNum);

    qDebug() << str;

    qDebug() << "QMessageBox create";


}

//??????
void Widget::on_pushButton_4_clicked()
{
    //??????????
    QList<QTableWidgetItem*> items = ui->tableWidget_1->selectedItems();
    if(items.size() <= 0)
        return;
    QTableWidgetItem *item = items.at(0);
    QString text = item->text();

    //char str[50] = "TCPkernel.cpp";
    STRU_PROTOCOL_DELETE_FILE_RQ rq;
    rq.m_nType = _DEF_PROTOCOL_DELETE_FILE_RQ;
    rq.m_lUserId = m_user_id;
    strcpy(rq.m_szFileName,text.toStdString().c_str());



    m_ikernel->SendData((char*)&rq,sizeof(rq));
}



//???????????
void Widget::slot__DEF_DELETE_FILE_RS(char * lp)
{
    //??????????
    STRU_PROTOCOL_DELETE_FILE_RS *rs = (STRU_PROTOCOL_DELETE_FILE_RS *)lp;
    if(rs->result == _delete_success)
    {
        //???????????????????§Ò?
        ui->tableWidget_1->removeRow(0);
        STRU_GETFILELIST_RQ getrq;
        getrq.m_nType = _DEF_PROTOCOL_GETFILELIST_RQ;
        getrq.m_luserid = m_user_id;
        m_ikernel->SendData((char*)&getrq,sizeof(getrq));//??????????????????§Ò?????
    }

}



//??????
void Widget::on_pushButton_2_clicked()
{
    //??????????
    QList<QTableWidgetItem*> items = ui->tableWidget_1->selectedItems();
    if(items.size() <= 0)
        return;
    QTableWidgetItem *item = items.at(0);
    QString text = item->text();

    //?????????????????
    STRU_DOWNLOAD_FILEINFO_RQ rq;
    rq.m_nType = _DEF_PROTOCOL_DOWNLOAD_FILEINFO_RQ;
    rq.m_lUserId = m_user_id;
    strcpy(rq.m_szFileName,text.toStdString().c_str());

    m_ikernel->SendData((char*)&rq,sizeof(rq));

}



void Widget::slot__DEF_DOWNLOAD_FILEINFO_RS(char*lp)
{
    //??????????????????,???????
    STRU_DOWNLOAD_FILEINFO_RS *rs = (STRU_DOWNLOAD_FILEINFO_RS*)lp;
    uploadfileinfo info;
    info.fileid = rs->m_lUserId;
    info.m_position = rs->m_lPosition;
    info.filesize = rs->m_fisize;


    //??????
    const char* PATH = "C:/clientdestory";
    QDir dir;
    if( !dir.exists(PATH) )  // ?????????????
    {
       //qDebug() <<  PATH;
       bool flag = dir.mkdir(PATH);
       //qDebug() << flag;
    }
    char m_path[MAXFILENUM] = {0};
    strcpy_s(m_path, "C:/clientdestory/install");
    //CreateDirectoryA(".\\install", 0);


    if (!dir.exists(m_path))
    {
        dir.mkdir(m_path);
    }

    char m_filepath[MAXFILENUM] = {0};
    sprintf(m_filepath,"%s\\%s",m_path,rs->m_szFileName);
    strcpy_s(info.m_szFilepath,m_filepath);


    //§Õ?????
    QFile fil(m_filepath);
    bool b = fil.open(QIODevice::WriteOnly | QIODevice::Append);
    info.m_file = &fil;

    //??????????????????????
    info.stage = "installing";

    WaitForSingleObject(map_mutex,INFINITE);
    m_mapfileinstall[rs->m_lFileId] = info;
    ReleaseMutex(map_mutex);


    //????????????¦Ë????????,?????????
    STRU_DOWNLOAD_FILECONTENT_RELY rely;
    rely.m_nType = _DEF_PROTOCOL_DOWNLOAD_FILECONTENT_RELY;
    rely.m_lFileId = rs->m_lFileId;
    rely.m_lUserId = rs->m_lUserId;
    rely.m_lPosition = info.m_position;
    rely.stage = _install_continue;


    //???????????
    m_ikernel->SendData((char*)&rely,sizeof(rely));


}


void Widget::slot__DEF_DOWNLOAD_FILECONTENT_SEND(char* lp)
{
    //?????????¦Ë??

    auto rq = (STRU_DOWNLOAD_FILECONTENT_RQ*)lp;

    WaitForSingleObject(map_mutex,INFINITE);
    QFile fil(m_mapfileinstall[rq->m_lFileId].m_szFilepath);
    ReleaseMutex(map_mutex);


    bool b = fil.open(QIODevice::WriteOnly | QIODevice::Append);
    int num = fil.write(rq->m_szFileContent,rq->m_nNum);


    int nowposition = num + m_mapfileinstall[rq->m_lFileId].m_position;


    WaitForSingleObject(map_mutex,INFINITE);
    m_mapfileinstall[rq->m_lFileId].m_position = nowposition;
    int size = m_mapfileinstall[rq->m_lFileId].filesize;
    ReleaseMutex(map_mutex);

    if(nowposition == size)
    {
        fil.close();
        QMessageBox::information(this,"fasd","install successful");
    }


    //????????????¦Ë?????????
    STRU_DOWNLOAD_FILECONTENT_RELY rely;
    rely.m_nType = _DEF_PROTOCOL_DOWNLOAD_FILECONTENT_RELY;
    rely.m_lFileId = rq->m_lFileId;
    rely.m_lUserId = rq->m_lUserId;
    rely.m_lPosition = nowposition;




     WaitForSingleObject(map_mutex,INFINITE);
     QString str = m_mapfileinstall[rq->m_lFileId].stage;
     ReleaseMutex(map_mutex);


    if(str == "installing")
        rely.stage = _install_continue;
    else
    {
        rely.stage = _install_pause;
    }

    //HANDLE han1 =(HANDLE)_beginthreadex(0,0,&ThreadProc,&sf,0,0);

    //???????????
    m_ikernel->SendData((char*)&rely,sizeof(rely));
}




unsigned _stdcall Widget::ThreadProc(void *lp)
{

}




void Widget::on_pushButton_3_clicked()
{
    if(m_menu == nullptr)
        m_menu = new menu;
    m_menu->show();

}

