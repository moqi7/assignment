#include<stdlib.h>
#include<string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <stdio.h>

#define BUF_SIZE 200
#define PORT 1234

void registers();
void log_in();
void *re_msg(void *sockfd);
void re_or_log();

typedef struct      //存放用户名和密码
{
    char name[110];
    char pwd[20];
}users;

char re_buf[BUF_SIZE];      //接收的消息
char se_buf[BUF_SIZE];      //发送的消息
users a;        //存放正在登陆的用户信息

int main() {
    int sockfd;
    struct sockaddr_in s_addr;
    re_or_log();        //用户登陆或注册
    printf("\nCheck the number of online users,please enter \"online\"\n");
    
    //套接字创建与连接
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    
    bzero(&s_addr,sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);
    if(inet_aton("127.0.0.1",&s_addr.sin_addr) == 0) {
        perror("inet_aton error");
        exit(0);
    }
    
    if(connect(sockfd, (struct sockaddr*)&s_addr, sizeof(s_addr)) < 0) {
        perror("connect error");
        exit(1);
    }
    
    send(sockfd,a.name,(strlen(a.name)),0);     //将登陆的用户名发给服务器
    
    
    pthread_t tid;
    pthread_create(&tid, NULL, re_msg, &sockfd);    //创建接收消息线程
    //发送消息
    while (1) {
        memset(se_buf, 0, BUF_SIZE);
        scanf("%s",se_buf);
        if(strcmp(se_buf,"exit")==0){       //消息为exit时，退出聊天，关闭套接字
            memset(se_buf,0,BUF_SIZE);
            printf("You have left the chat room\n");
            close(sockfd);
            exit(0);
        }
        if(strcmp(se_buf,"online") == 0) {      //发送消息为online时准备接收在线用户信息
            send(sockfd,se_buf,sizeof(se_buf),0);
        }
        else {      //发送普通消息时发送格式为 用户名称：消息
            send(sockfd,a.name,(strlen(a.name)),0);
            send(sockfd,":",1,0);
            send(sockfd,se_buf,sizeof(se_buf),0);
        }
    }
    
    
    
     
}

void *re_msg(void *sockfd) {        //消息接收
    int fd = *(int*)sockfd;
    while(1) {
        if(recv(fd, re_buf, BUF_SIZE, 0) <= 0) {
            exit(1);
        }
        printf("%s", re_buf);
        memset(re_buf, 0, BUF_SIZE);
    }
    
}





void re_or_log() {      //s用户注册或登录
    int i;
    printf("___________________________________________\n");
    printf("register or login:\nregister：1\nlogin 2\nexit 3\n");
    printf("___________________________________________\n");
    scanf("%d",&i);
    switch (i) {
        case 1:
            registers();
            break;
        case 2:
            log_in();
            break;
        case 3:
            printf("exit");
            exit(0);
        default:
            printf("Input error,please re-enter");
            re_or_log();
    }
}


void registers() {      //用户注册，注册的信息写入File中
    
    printf("\nregister\n");
    FILE *fp;
    users b;
    fp = fopen("/Users/jokerc/Desktop/Mysocketpj/chatclient/chatclient/File","rb+");
    if(fp == NULL) {
        printf("open file error!\n");
        exit(0);
    }
    printf("username:");
    scanf("%s",a.name);     //输入用户名称
    fread(&b, sizeof(users), 1, fp);        //从文件中读取一个用户的数据
    /*
     判断输入用户名r与文件读取用户名是否相同
     相同则登录
     不相同则输入密码，把用户数据写入文件，进入登录
     */
    while(1) {
        if(strcmp(a.name,b.name)) {     
            if(!feof(fp)) {
                fread(&b, sizeof(users), 1, fp);
            }
            else {
                printf("\npassword:");
                scanf("%s",a.pwd);
                fwrite(&a,sizeof(users), 1, fp);
                fclose(fp);
                log_in();
                break;
            }
        }
        
        else {
            printf("username exists!\n");
            log_in();
            break;
            
        }
    }
    
    
}
 


void log_in() {    
    printf("\nlogin:\n");
    users b;
    FILE *fp;
    fp = fopen("/Users/jokerc/Desktop/Mysocketpj/chatclient/chatclient/File","rb+");
    if(fp == NULL) {
        printf("open file error!\n");
        exit(0);
    }
    printf("username:");
    scanf("%s",a.name);
    fread(&b, sizeof(users), 1, fp);
    /*
     从文件中读取用户数据，与输入用户名比较
     相同则输入密码
     不相同则注册
     */
    while(1){
        if(strcmp(a.name, b.name)) {
            if(!feof(fp)) {
                fread(&b, sizeof(users), 1, fp);
            }
            else {
                printf("user does not exist,please register\n");
                registers();
                
            }
        }
        else {
            break;
        }
    }
    /*输入密码与从文件中读取的密码比较
     相同则登录成功
     不同则重新h输入密码
     */
    while(1){
        printf("\npassword:");
        scanf("%s",a.pwd);
        if(strcmp(a.pwd, b.pwd)) {
            printf("Wrong password,please re-enter");
            continue;
        }
        else {
            printf("login successful!");
            break;
        }
    
    }
    fclose(fp);
}


