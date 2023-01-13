#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<errno.h>
#include<pthread.h>

#define MAX 10
       
#define BUF_SIZE 200

#define PORT 1234


int fd[MAX] = {0};  //存放用户套接字标识符

typedef struct  {
    char name[BUF_SIZE];    // 存放用户名字
}name;

name u_name[10];





void SendMsg(int tmp,int new_fd,char* buf) ;

char msg[1024];

void *thread_func(void* co_fd) {        //线程函数
    int new_fd = *(int*)co_fd;
    int i,j,tmp,flag;
    
    while(1) {
        memset(msg, 0, BUF_SIZE);
        tmp= 0;
        flag = 0;
        tmp = recv(new_fd, msg, sizeof(msg), 0);    //接受消息
        if(tmp <= 0) {      //用户退出
            for(i = 0; i < MAX; i++) {
                if(new_fd == fd[i]) {
                    fd[i] = 0;
                    bzero(u_name[i].name,sizeof(u_name[i].name));
                    break;
                }
            }
            printf("fd = %d exit!\n",new_fd);
            close(new_fd);
            break;
        }
        else if(strcmp(msg,"online") == 0) {        //接收消息为online时发送在线用户姓名
            for(j = 0; j< MAX; j++){
                if(fd[j] != 0) {
                    send(new_fd,u_name[j].name,sizeof(u_name[j].name),0);
                    send(new_fd, "\n", 1, 0);
                }
            }
            printf("send online name to %d\n",new_fd);
            bzero(msg,BUF_SIZE);
        }
        else {      //普通消息时到发送消息函数
            
            printf("receive message from %d,buf %d\n",new_fd,tmp);
            SendMsg(tmp, new_fd, msg);
            bzero(msg,BUF_SIZE);
        }
        
    }
    return 0;
}

void SendMsg(int tmp,int new_fd,char* buf) {        //消息发送函数
    int i;
    
    for (i = 0; i < MAX; i++) {
        if(fd[i] != 0 && fd[i] != new_fd) {  //发送消息给其他用户
            send(fd[i], buf, tmp, 0);
            printf("send message to %d\n",fd[i]);
        }
    }
    
}

int main() {
    int sockfd,connectfd;
    int size,num = 0;
    
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    size = sizeof(struct sockaddr_in);
    
    //套接字的创建
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }
    int opt = SO_REUSEADDR;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    if(listen(sockfd, MAX) == -1)
    {
        perror("listen");
        exit(1);
    }
    
    printf("listen port %d \n", PORT);
    
   //连接客户端
    while(1) {
        if ((connectfd = accept(sockfd, (struct  sockaddr*)&client_addr, &size)) == -1) {
            perror("accept() error\n");
            exit(1);
        }
        
        //最多j连接MAX个用户
        if(num >= MAX) {
            printf("no more client is allowed\n");
            close(connectfd);
        }
        int i = 0;
                
        for(; i < MAX; i ++) {
           
                if(fd[i] == 0) {
                    fd[i] = connectfd;
                    recv(connectfd, u_name[i].name, BUF_SIZE, 0);
                    break;
                }
        }
        
        //创建线程
        pthread_t tid;
        pthread_create(&tid,NULL,(void*)thread_func,&connectfd);
        num++;
    }
        
}


