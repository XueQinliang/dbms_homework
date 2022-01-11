#include<stdlib.h>
#include<iostream>
#include <sstream>
#include <fstream>
#include<stdio.h>
#include<string>
#include<string.h>
#include<netdb.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<errno.h>
#include "SQLParser.h"
#include "sql/SQLStatement.h"
#include <sys/wait.h>
#include<pthread.h>
#include<queue>
#include<map>
#include <sys/prctl.h>
#include<malloc.h>
// contains printing utilities

#include "util/sqlhelper.h"
#include "util/sqltrans.h"
using namespace std;
#define MAX_PTHREAD_NUM 20
//pthread_rwlock_t rwlock;//声明读写锁
int count = 0;
queue<int> tidqueue;
vector<int> pidvec;
void showClientInf(struct sockaddr_in client_addr) {
    printf("\nThe IP of client is:%s",inet_ntoa(client_addr.sin_addr));
    printf("\nThe Port of client is:%d\n",ntohs(client_addr.sin_port));
}
struct routearg{
    int pid;
    int new_fd;
	int sockfd;
};
void SignHandler(int iSignNo)
{
    for(auto pid : pidvec){
        kill(pid, SIGKILL); //杀死所有子进程
        printf("process %d is killed\n", pid);
    }
    exit(0);
}
void*route(void*arg)
{
    struct routearg* rarg = (struct routearg* )arg;
    int i= rarg->pid;//i是写者线程的编号
    printf("start %d %d\n", i, rarg->new_fd);
	int sockfd = rarg->sockfd;
    int numbytes = 0;
    char query[1000];
	char buf[1000];
	while(1){
		if((numbytes = recv(rarg->new_fd,query,sizeof(query),0))==-1)
		{
			perror("recv");
			exit(1);
		}

		query[numbytes]='\0';
		printf("recieved %d bytes.\n",numbytes);
		printf("the message is:%s\n",query);
		if(strncmp(query, "login", 5) == 0){
			//表明登录测试
			string userpswd = string(query);
			string::size_type pos = userpswd.find_first_of(":");
			string cuser = userpswd.substr(6, pos-6);
			string cpswd = userpswd.substr(pos+1);
			ifstream infile;
			infile.open("user.txt", ios::in);
			if (!infile.is_open())
			{
				cout << "读取文件失败" << endl;
				return 0;
			}
			//第一种读取方法，
			string user, pswd;
			bool suc = false;
			while (infile>>user>>pswd)
			{
				cout << user << " " << pswd << endl;//输出读取的文本文件数据
				cout << cuser << " " << pswd << endl <<endl;
				if(user == cuser && pswd == cpswd){
					string sig = "yes";
					suc = true;
					if(send(rarg->new_fd,sig.c_str(),sig.length(),0)==-1)
						perror("send");
					break;
				}
			}
			if (suc == false){
				string sig = "no";
				if(send(rarg->new_fd,sig.c_str(),sig.length(),0)==-1)
					perror("send");
			}
			continue;
		}
		hsql::SQLParserResult result;
		hsql::SQLParser::parse(string(query), &result);
		if (result.isValid()) {
			printf("Parsed successfully!\n");
			printf("Number of statements: %lu\n", result.size());

			for (auto i = 0u; i < result.size(); ++i) {
				// Print a statement summary.
				string likesql = hsql::TransStatementInfo(result.getStatement(i));
				// 类sql要传给spj模块，传之前要判断是否等于"Fail\n"
				// 打印一下
				string res = "";
				if((likesql[0] >= '0' && likesql[0] <= '9') || (likesql[0] == '-' && (likesql[1] >= '0' && likesql[1] <= '9'))){
					cout<<likesql<<endl;
					if(send(sockfd,likesql.c_str(),likesql.length(),0)==-1) {
						perror("send");
						exit(1);
					}
					//接受从服务器返回的信息
					// if((numbytes = recv(sockfd,buf,1000,0))==-1) {
					// 	perror("recv");
					// 	exit(1);
					// }
					numbytes = 1;
					do{
						// perror("recv");
						// exit(1);
						numbytes = recv(sockfd,buf,1000,0);
						printf("len:%d\n", numbytes);
						buf[numbytes] = '\0';
						//printf("%s",buf);
						res += string(buf);
					}while(numbytes >= 1000);
					printf("\n");
					// buf[numbytes] = '\0';
					// printf("result:%s\n",buf);
					if(res[0] == '|' && res[1] == '\n'){
						if(result.getStatement(i)->type() == hsql::StatementType::kStmtCreate || 
						result.getStatement(i)->type() == hsql::StatementType::kStmtDrop){
							// FILE *fp = fopen("table_meta", "rb");
							// int size;
							// fread(&size, 4, 1, fp);
							// map<string, vector<string>> *tablecolumns = (map<string, vector<string>> *)malloc(size);
							// fread(tablecolumns, size, 1, fp);
							// fclose(fp);
							// string showtables;
							// for(auto table:*tablecolumns){
							// 	showtables += table.first;
							// 	showtables += "\n";
							// 	for(auto columnname : table.second){
							// 		showtables += "\t";
							// 		showtables += columnname;
							// 		showtables += "\n";
							// 	}
							// }
							res = likesql;
						}
					}
				}else{
					res = likesql;
				}
				cout<<"likesql:"<<likesql<<endl;
				if(send(rarg->new_fd,res.c_str(),res.length(),0)==-1)
					perror("send");
				
			}
		} else {
			fprintf(stderr, "Given string is not a valid SQL query.\n");
			fprintf(stderr, "%s (L%d:%d)\n",
					result.errorMsg(),
					result.errorLine(),
					result.errorColumn());
			string r = "Given string is not a valid SQL query.\n";
			r += string(result.errorMsg());
			r += " ";
			r += to_string(result.errorLine());
			r += " ";
			r += to_string(result.errorColumn());
			if(send(rarg->new_fd,r.c_str(),r.length(),0)==-1)
				perror("send");
		}
	}
}
int main(int argc,char *argv[]) {
	signal(SIGINT,SignHandler); 
    //int sockfd,new_fd;
    pthread_t tid[MAX_PTHREAD_NUM];
    for(int i = 0; i < MAX_PTHREAD_NUM; i++){
        tidqueue.push(i);
    }
	int sockfd;
	//int numbytes;

	int tossockfd,new_fd;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    int sin_size;
    // char buff[1000];
    // int numbytes;
    //建立TCP套接口
    if((tossockfd = socket(AF_INET,SOCK_STREAM,0))==-1) {
        perror("socket");
        exit(1);
    }
	if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1) {
        perror("socket");
        exit(1);
    }
	//将基本名字和地址转换，用户必须输入服务器的名字为参数
	if(argc<2) {
		printf("You should input IP or Name of the server!\n");
		exit(1);
	}else if(argc<3){
		printf("You should input Port of the server!\n");
		exit(1);
	}
	struct hostent *he;
	he = gethostbyname(argv[1]);
	int port = atoi(argv[2]);
    //初始化结构体，并绑定2323端口
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(2332);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(my_addr.sin_zero),8);
    //绑定套接口
    if(bind(tossockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr))==-1)
    {
        perror("bind");
        exit(1);
    }
    //创建监听套接口
    if(listen(tossockfd,10)==-1) {
        perror("listen");
        exit(1);
    }
    printf("server is run...\n");



	//char buf[1000];
	//将基本名字和地址转换，用户必须输入服务器的名字为参数
	//建立一个TCP套接口
	// if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1) {
	// 	perror("socket");
	// 	exit(1);
	// }
	//初始化结构体，连接到服务器的2323端口
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(port);
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero),8);
	//和服务器建立连接，若连接建立失败则直接报错
	if(connect(sockfd,(struct sockaddr *)&their_addr,sizeof(struct sockaddr))==-1){
		perror("connect");
		exit(1);
	}
	printf("Hello to use BabeDB!\n");
	while(1){
		//std::string query;
		//向服务器发送字符串msg
		//cout<<"(babe)> ";
		//getline(cin, query);
		//char query[1000];
		sin_size = sizeof(struct sockaddr_in);
        //如果建立连接，将产生一个全新的套接字，their_fd存储发送方的信息
        //一个套接字与客户端保持控制连接，新套接字与客户端传递、接受信息
        if((new_fd = accept(tossockfd,(struct sockaddr *)&their_addr,(socklen_t*)&sin_size))==-1)
        {
            perror("accept");
            exit(1);
        }
		//显示客户端信息
        showClientInf(their_addr);
		struct routearg arg;
        arg.pid = int(tidqueue.front());
        arg.new_fd = new_fd;
		arg.sockfd = sockfd;
        tidqueue.pop();
		pid_t pid = fork();
		if(pid == 0) {
            /*父进程退出时，会收到SIGKILL信号*/
            prctl(PR_SET_PDEATHSIG,SIGKILL);
            bool in = false;
            while(!in && !tidqueue.empty()){
                in = true;
                pthread_create(&tid[tidqueue.front()],NULL,route,(void*)&arg);
            }
        }else{
            printf("fork the process %d\n", pid);
            pidvec.push_back(pid); 
        }
		
        // if(pid == 0) {
		// 	if((numbytes = recv(new_fd,query,sizeof(query),0))==-1)
		// 	{
		// 		perror("recv");
		// 		exit(1);
		// 	}
   
		// 	query[numbytes]='\0';
		// 	printf("recieved %d bytes.\n",numbytes);
		// 	printf("the message is:%s\n",query);
		// 	hsql::SQLParserResult result;
		// 	hsql::SQLParser::parse(string(query), &result);
		// 	if (result.isValid()) {
		// 		printf("Parsed successfully!\n");
		// 		printf("Number of statements: %lu\n", result.size());

		// 		for (auto i = 0u; i < result.size(); ++i) {
		// 			// Print a statement summary.
		// 			string likesql = hsql::TransStatementInfo(result.getStatement(i));
		// 			// 类sql要传给spj模块，传之前要判断是否等于"Fail\n"
		// 			// 打印一下
		// 			cout<<likesql<<endl;
		// 			if(send(sockfd,likesql.c_str(),likesql.length(),0)==-1) {
		// 				perror("send");
		// 				exit(1);
		// 			}
		// 			//接受从服务器返回的信息
		// 			if((numbytes = recv(sockfd,buf,1000,0))==-1) {
		// 				perror("recv");
		// 				exit(1);
		// 			}
		// 			buf[numbytes] = '\0';
		// 			printf("result:%s\n",buf);
		// 			if(send(new_fd,buf,strlen(buf),0)==-1)
		// 				perror("send");
		// 		}
		// 	} else {
		// 		fprintf(stderr, "Given string is not a valid SQL query.\n");
		// 		fprintf(stderr, "%s (L%d:%d)\n",
		// 				result.errorMsg(),
		// 				result.errorLine(),
		// 				result.errorColumn());
		// 		string r = "Given string is not a valid SQL query.\n";
		// 		r += string(result.errorMsg());
		// 		r += " ";
		// 		r += to_string(result.errorLine());
		// 		r += " ";
		// 		r += to_string(result.errorColumn());
		// 		if(send(new_fd,r.c_str(),r.length(),0)==-1)
		// 			perror("send");
		// 	}
		// }//子进程结束
	}
	close(sockfd);
	close(tossockfd);
	return 0;
}
