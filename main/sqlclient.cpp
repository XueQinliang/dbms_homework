#include<stdlib.h>
#include<iostream>
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
// contains printing utilities

#include "util/sqlhelper.h"
#include "util/sqltrans.h"

using namespace std;
int main(int argc,char *argv[]) {
	int sockfd,numbytes;
	char buf[1000];
	struct hostent *he;
	struct sockaddr_in their_addr;
	//将基本名字和地址转换，用户必须输入服务器的名字为参数
	if(argc<2) {
		printf("You should input IP or Name of the server!\n");
		exit(1);
	}else if(argc<3){
		printf("You should input Port of the server!\n");
		exit(1);
	}
	he = gethostbyname(argv[1]);
	int port = atoi(argv[2]);
	//建立一个TCP套接口
	if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1) {
		perror("socket");
		exit(1);
	}
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
	printf("login as: ");
	string user, pswd;
	cin >> user;
	printf("password: ");
	cin >> pswd;
	string userpswd = "login " + user + ":" + pswd;
    send(sockfd, userpswd.c_str(), strlen(userpswd.c_str()), 0);
    char res[10];
    int ret = recv(sockfd, res, 10, 0);
    res[ret] = '\0';
    if(strcmp(res, "no") == 0){
        printf("connect error !\n");
		exit(0);
    }
	getchar();
	printf("Hello to use BabeDB!\n");
	while(1){
		std::string query;
		//向服务器发送字符串msg
		cout<<"(babe)> ";
		getline(cin, query);
		if(query == "\n"){
			continue;
		}
		hsql::SQLParserResult result;
    	hsql::SQLParser::parse(query, &result);
		if (result.isValid()) {
			printf("Parsed successfully!\n");
			printf("Number of statements: %lu\n", result.size());

			for (auto i = 0u; i < result.size(); ++i) {
				// Print a statement summary.
				string likesql = hsql::TransStatementInfo(result.getStatement(i));
				// 类sql要传给spj模块，传之前要判断是否等于"Fail\n"
				// cout<<likesql<<endl;
				if((likesql[0] >= '0' && likesql[0] <= '9') || (likesql[0] == '-' && (likesql[1] >= '0' && likesql[1] <= '9'))){
					// 打印一下
					cout<<likesql<<endl;
					if(send(sockfd,likesql.c_str(),likesql.length(),0)==-1) {
						perror("send");
						exit(1);
					}
					//接受从服务器返回的信息
					numbytes = 1;
					do{
						// perror("recv");
						// exit(1);
						numbytes = recv(sockfd,buf,1000,0);
						//printf("len:%d\n", numbytes);
						buf[numbytes] = '\0';
						if(strcmp(buf, "|\n")==0){
							printf("execute successfully!");
						}else{
							printf("%s",buf);
						}
					}while(numbytes >= 1000);
					printf("\n");
				}
			}
		} else {
			fprintf(stderr, "Given string is not a valid SQL query.\n");
			fprintf(stderr, "%s (L%d:%d)\n",
					result.errorMsg(),
					result.errorLine(),
					result.errorColumn());
		}
	}
	close(sockfd);
	return 0;
}
