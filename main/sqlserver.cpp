#include<stdlib.h>
#include<iostream>
#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<netdb.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include <sys/wait.h>
#include<pthread.h>
#include<queue>
#include <sys/prctl.h>
#include<malloc.h>

#define MAX_PTHREAD_NUM 20
using namespace std;
pthread_rwlock_t rwlock;//声明读写锁
int count = 0;
queue<int> tidqueue;
vector<int> pidvec;

void showClientInf(struct sockaddr_in client_addr) {
    printf("\nThe IP of client is:%s",inet_ntoa(client_addr.sin_addr));
    printf("\nThe Port of client is:%d\n",ntohs(client_addr.sin_port));
}
//线程的入口函数
struct routearg{
    int pid;
    int new_fd;
};
void*route(void*arg)
{
    struct routearg* rarg = (struct routearg* )arg;
    int i= rarg->pid;//i是写者线程的编号
    printf("start %d %d\n", i, rarg->new_fd);
    int numbytes = 0;
    char buff[1000];
    // bool iswrite = true; //要判断是否是写操作，如果是写，则加写锁
    // bool islocking = false; //要判断是否现在有锁 (针对transaction的情况，可以执行多条语句)
    while(1){
        if((numbytes = recv(rarg->new_fd,buff,sizeof(buff),0))==-1)
        {
            perror("recv");
            exit(1);
        }
        //加锁
        // if(iswrite){
        //     printf("lock\n");
        //     printf("%p\n",&rwlock);
        //     int t = pthread_rwlock_wrlock(&rwlock); //写锁
        //     printf("t:%d\n", t);
        // }
        // else{
        //     pthread_rwlock_rdlock(&rwlock); //读锁
        // }
        // if(strcmp(buff, "transaction;")==0){
        //     printf("start a transaction\n");
        //     islocking = true; //多指令事务开始
        //     if(send(rarg->new_fd,buff,strlen(buff),0)==-1)
        //         perror("send");
        //     printf("start\n");
        //     continue;
        // }
        // if(strcmp(buff, "commit;")==0 || strcmp(buff, "rollback;")==0){
        //     islocking = false;
        //     if(send(rarg->new_fd,buff,strlen(buff),0)==-1)
        //         perror("send");
        //     pthread_rwlock_unlock(&rwlock); //解锁
        //     continue;
        // }
        buff[numbytes]='\0';
        printf("recieved %d bytes.\n",numbytes);
        printf("the message is:%s\n",buff);
        if(buff[0] == 0){ //如果收到sigint信号
            printf("get the sigint signal and close the connection.\n");
            close(rarg->new_fd);
            tidqueue.push(i);
            printf("push back %d\n", i);
            exit(0);
        }
        //将从客户端接收到的信息再发回客户端
        //执行sql，获取结果，放到buff中，这里可以考虑用一个string，然后转成c_str
        printf("execute...\n");
        // sleep(10); //假装执行10s，试试
        if(send(rarg->new_fd,buff,strlen(buff),0)==-1)
            perror("send");
        //解锁
        // if(!islocking){ //如果是多指令的事务状态，不解锁，直接接收下一次信号
        //     pthread_rwlock_unlock(&rwlock);
        // }
    }
    close(rarg->new_fd);
    tidqueue.push(i);
    printf("push back %d\n", i);
}
// //读者线程的入口函数
// void*route_read(void*arg)
// {
//     int i=*(int*)arg;//i是读者线程的编号
//     while(1){
//         //加锁
//         pthread_rwlock_rdlock(&rwlock);
//         printf("route_read:%d,%#x,count=%d\n",i,pthread_self(),count);
//         //解锁
//         pthread_rwlock_unlock(&rwlock);
//         sleep(1);
//     }
// }
void SignHandler(int iSignNo)
{
    for(auto pid : pidvec){
        kill(pid, SIGKILL); //杀死所有子进程
        printf("process %d is killed\n", pid);
    }
    exit(0);
}

int main(int argc,char *argv[]) {
    signal(SIGINT,SignHandler); 
    int sockfd,new_fd;
    pthread_rwlock_init(&rwlock,NULL);
    pthread_t tid[MAX_PTHREAD_NUM];
    for(int i = 0; i < MAX_PTHREAD_NUM; i++){
        tidqueue.push(i);
    }
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    int sin_size;
    // char buff[1000];
    // int numbytes;
    //建立TCP套接口
    if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1) {
        perror("socket");
        exit(1);
    }
    if(argc<2){
        printf("You should input Port of the server!\n");
        exit(1);
    }
    int port = atoi(argv[1]);
    //初始化结构体，并绑定2323端口
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(my_addr.sin_zero),8);
    //绑定套接口
    if(bind(sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr))==-1)
    {
        perror("bind");
        exit(1);
    }
    //创建监听套接口
    if(listen(sockfd,10)==-1) {
        perror("listen");
        exit(1);
    }
    printf("server is run...\n");
    //等待连接
    while(1) {
        sin_size = sizeof(struct sockaddr_in);
        //如果建立连接，将产生一个全新的套接字，their_fd存储发送方的信息
        //一个套接字与客户端保持控制连接，新套接字与客户端传递、接受信息
        if((new_fd = accept(sockfd,(struct sockaddr *)&their_addr,(socklen_t*)&sin_size))==-1)
        {
            perror("accept");
            exit(1);
        }
        //显示客户端信息
        showClientInf(their_addr);
        struct routearg arg;
        arg.pid = int(tidqueue.front());
        arg.new_fd = new_fd;
        tidqueue.pop();
        //生成一个子进程来完成和客户端的会话，父进程继续监听
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
        
        // if(!fork()) {
        //     //读取客户端发来的信息
        //     //只能用sizeof取buff的大小，因为buff还没初始化，用strlen很容易碰到'/0'
        //     while(1){
        //         if((numbytes = recv(new_fd,buff,sizeof(buff),0))==-1)
        //         {
        //             perror("recv");
        //             exit(1);
        //         }
        //         buff[numbytes]='\0';
        //         printf("recieved %d bytes.\n",numbytes);
        //         printf("the message is:%s\n",buff);
        //         if(buff[0] == 0){
        //             printf("get the sigint signal and close the connection.\n");
        //             close(new_fd);
        //             exit(0);
        //         }
        //         //将从客户端接收到的信息再发回客户端
        //         if(send(new_fd,buff,strlen(buff),0)==-1)
        //             perror("send");
        //     }
        // }
        // close(new_fd);
    }
    close(sockfd);
	int status;
    wait(&status);
    pthread_rwlock_destroy(&rwlock);
    return 0;
}