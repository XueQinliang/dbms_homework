#include "login.h"
#include "ui_login.h"

using namespace std;

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    ui->lineEdit_3->setEchoMode(QLineEdit::PasswordEchoOnEdit);
}

login::~login()
{
    delete ui;
}

void login::on_pushButton_2_clicked()
{
    this->close();
}

void login::on_lineEdit_textChanged(const QString &arg1)
{
    ip = arg1;
}

void login::on_lineEdit_2_textChanged(const QString &arg1)
{
    port = arg1;
}

void login::on_lineEdit_4_textChanged(const QString &arg1)
{
    user = arg1;
}

void login::on_lineEdit_3_textChanged(const QString &arg1)
{
    pswd = arg1;
}

void login::on_pushButton_clicked()
{
    string ip = this->ip.toStdString();
    int port = this->port.toInt();
    string user = this->user.toStdString();
    string pswd = this->pswd.toStdString();
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA data;
    if(WSAStartup(sockVersion, &data)!=0)
    {
        return;
    }
    this->s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(s == INVALID_SOCKET)
    {
        printf("invalid socket!");
        return;
    }

    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(port);
    serAddr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
    if(::connect(s, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
    {  //连接失败
        printf("connect error !");
        closesocket(s);
        return;
    }
    string userpswd = "login " + user + ":" + pswd;
    send(s, userpswd.c_str(), strlen(userpswd.c_str()), 0);
    char res[10];
    int ret = recv(s, res, 10, 0);
    res[ret] = '\0';
    qDebug()<<"res:"<<res<<endl;
    if(strcmp(res, "yes") == 0){
        emit sendSOCKET(s);
        QString str = this->ip+":"+this->port+" by "+this->user;
        emit sendCONNECTINFO(str);
    }else{
        QString str = "failed "+this->ip+":"+this->port+" by "+this->user;
        emit sendCONNECTINFO(str);
        printf("connect error !");
        closesocket(s);
    }
    this->close();
    return;
}
