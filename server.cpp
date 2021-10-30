#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<bits/socket.h>
#include<netdb.h>
#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>
using namespace std;

int main(int argc,char *argv[])
{
if(argc!=2)
{
	printf("Using:./server port\nExample:./server 5005\n\n");
	return -1;
}
//第一步：创建服务端socket；
int listenfd;
int port=atoi(argv[1]);//指定端口，把指定端口字符串转换成整形；
if((listenfd=socket(AF_INET,SOCK_STREAM,0))==-1)
{
	perror("socket");
	return -1;
}
//第二步：绑定（命名）socket
struct sockaddr_in myserveraddr;
memset(&myserveraddr,0,sizeof(myserveraddr));
myserveraddr.sin_family=AF_INET;//IPv4地址协议族；
myserveraddr.sin_addr.s_addr=htonl(INADDR_ANY);//INADDR_ANY任意IP地址；
myserveraddr.sin_port=htons(atoi(argv[1]));//指定端口号；
if((bind(listenfd,(struct sockaddr*)&myserveraddr,sizeof(myserveraddr)))!=0)
{
	perror("bind");
	close(listenfd);
	return -1;
}
//第三步：监听socket；
if(listen(listenfd,5)!=0)
{
	perror("listen");
	close(listenfd);
	return -1;
}
//第四步：接受客户端的连接
int clientfd;//客户端的socket；
struct sockaddr_in clientaddr;//客户端的地址信息
socklen_t len=sizeof(clientaddr);
if((clientfd=accept(listenfd,(struct sockaddr *)&clientaddr,&len))==-1)
{
	perror("accept");
//	close(listenfd);
	return -1;
}
//accept成功时返回一个新的socket；
//第五步：与客户端通信，接收客户端发过来的报文
printf("客户端（%s）已连接。\n",inet_ntoa(clientaddr.sin_addr));
printf("%d",clientfd);
char buffer[1024];//读缓冲区
while(1)
{
	int iret;
	bzero(&buffer,sizeof(buffer));
	if((iret=recv(clientfd,buffer,sizeof(buffer),0))<=0)
	{
		printf("iret=%d\n",iret);
	break;
	}
	printf("接收：%s\n",buffer);
	if((iret=send(clientfd,buffer,strlen(buffer),0))<=0)// 向客户端发送响应结果
	{perror("send");
	break;
	}
	printf("发送：%s\n",buffer);
}
//第六步：关闭socket，释放资源。
close(listenfd);
close(clientfd);
}
