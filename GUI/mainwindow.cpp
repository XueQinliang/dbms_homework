#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<WINSOCK2.H>
#pragma comment(lib, "ws2_32.lib")
using namespace std;
/*int Connect(SOCKET s,const struct sockaddr *name,int namelen){
    return connect(s, name, namelen);
}*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*WORD sockVersion = MAKEWORD(2, 2);
    WSADATA data;
    if(WSAStartup(sockVersion, &data)!=0)
    {
        return;
    }
    this->sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sclient == INVALID_SOCKET)
    {
        printf("invalid socket!");
        return;
    }

    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(2332);
    serAddr.sin_addr.S_un.S_addr = inet_addr("172.19.102.99");
    if(::connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
    {  //连接失败
        printf("connect error !");
        closesocket(sclient);
        return;
    }*/
    /*string aname = "A";
    vector<string> Atable;
    Atable.push_back("id");
    Atable.push_back("name");
    tablecolumns[aname] = Atable;
    vector<string> Btable;
    string bname = "B";
    Btable.push_back("id");
    Btable.push_back("name");
    tablecolumns[bname] = Btable;*/
    showtablesfunc();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_textEdit_textChanged()
{

}

void MainWindow::on_create_table_clicked()
{
    create = new Form(this);
    create->setWindowModality(Qt::WindowModal);
    connect(create, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));
    create->show();
}

void MainWindow::on_excute_clicked()
{
    this->sql = ui->sqlinput->toPlainText().toStdString();
    //char * sendData = "你好，TCP服务端，我是客户端\n";
    qDebug()<<this->sql.c_str();
    send(sclient, this->sql.c_str(), strlen(this->sql.c_str()), 0);
    //send()用来将数据由指定的socket传给对方主机
    //int send(int s, const void * msg, int len, unsigned int flags)
    //s为已建立好连接的socket，msg指向数据内容，len则为数据长度，参数flags一般设0
    //成功则返回实际传送出去的字符数，失败返回-1，错误原因存于error

    char recData[100000];
    int ret = 1;
    string r = "";
    while(ret>0){
        ret = recv(sclient, recData, 1000, 0);
        qDebug()<<"qdebug"<<ret<<endl;
        recData[ret] = 0x00;
        r += string(recData);
        if(ret<1000) break;
    }
    if(r == "|\n"){
        r = "execute successfully!";
    }
    if(r[0] == '-' && r[1] == '1'){
        istringstream cin(r);
        int n0, n1, n2;
        cin >> n0;

        if (n0 == -1) { //CREATE TABLE
            string t_name;
            cin >> t_name >> n1;
            vector<pair<string,string>> columns;
            while (n1--) {
                string var_name, var_type;
                cin >> var_name >> var_type;
                columns.push_back(make_pair(var_name, var_type));
            }
            tablecolumns[t_name] = columns;
            tableindex[t_name] = vector<string>();
        }
        r = "execute successfully!";
        showtablesfunc();
    }else if(r[0] == '-' && r[1] == '6'){
        istringstream cin(r);
        int n0;
        cin >> n0;

        if (n0 == -6) { //CREATE INDEX
            string t_name;
            string c_name;
            cin >> t_name >> c_name;
            tableindex[t_name].push_back(c_name);
        }
        r = "execute successfully!";
        showtablesfunc();
    }else if(r[0] == '-' && r[1] == '7'){
        istringstream cin(r);
        int n0;
        cin >> n0;

        if (n0 == -7) { //DROP TABLE
            string t_name;
            cin >> t_name;
            tablecolumns.erase(t_name);
        }
        r = "execute successfully!";
        showtablesfunc();
    }
    QString textPlianString = QString::fromStdString(r);
    ui->resultshow->setText(textPlianString);
    qDebug()<<recData;
}

void MainWindow::showtablesfunc()
{
    showtables = "";
    for(auto table:tablecolumns){
        showtables += table.first;
        showtables += "\n";
        for(auto columnname : table.second){
            showtables += "    ";
            showtables += (columnname.first+" "+columnname.second);
            showtables += "\n";
        }
        for(auto columnname : tableindex[table.first]){
            showtables += ("  index on " + columnname + "\n");
        }
    }
    ui->showtable->setText(QString::fromStdString(showtables));
}

void MainWindow::receiveData(QString data)
{
    //建表语句
    QString createsql = data;
    sql = createsql.toStdString();
    ui->sqlinput->setText(createsql);
    on_excute_clicked();
}

void MainWindow::on_select_button_clicked()
{
    select = new Select(this);
    select->setWindowModality(Qt::WindowModal);
    connect(select, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));
    select->settablecolumns(&tablecolumns);
    select->show();
}

/*void MainWindow::on_pushButton_clicked()
{
    log = new login(this);
    log->setWindowModality(Qt::WindowModal);
    connect(log, SIGNAL(sendSOCKET(SOCKET)), this, SLOT(receiveSOCKET(SOCKET)));
    connect(log, SIGNAL(sendCONNECTINFO(QString)), this, SLOT(receiveConnectinfo(QString)));
    log->show();
}*/

void MainWindow::receiveSOCKET(SOCKET s)
{
    sclient = s;
}

void MainWindow::receiveConnectinfo(QString data)
{
    this->ui->label_4->setText(data);
}

void MainWindow::on_connect_button_clicked()
{
    log = new login(this);
    log->setWindowModality(Qt::WindowModal);
    connect(log, SIGNAL(sendSOCKET(SOCKET)), this, SLOT(receiveSOCKET(SOCKET)));
    connect(log, SIGNAL(sendCONNECTINFO(QString)), this, SLOT(receiveConnectinfo(QString)));
    log->show();
}
